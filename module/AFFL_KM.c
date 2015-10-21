/*Модуль ядра для блокирования запуска процессов из черного списка.
 *Подменяется системный вызов sys_execve(), который порождает процессы.
 *Т.к. sys_call_table[__NR_execve] указывает не на sys_execve(), а на stub_execve(),
 *подменяется агумент инструкции call в stub_execve(), которая вызывает sys_execve().
 * 
 *Функции работы с черным списком:
 * initBlackList()		(инициализирует список)
 * refreshBlackList()		(обновляет список по таймеру (раз в 1 секунду)
 * findProcInBlackList()	(ищет имя процесса в черном списке.
 * 				 Если процесс не найден - возвращает 0)
 * releaseBlackList()		(освобождает список)

/*Ряд символов "+" - начало блока, ряд символов "-" - конец блока.
 *Ряд символов "=" - простой разделитель.
 * 
 *Глобальные для блока объявления располагаются в начале,
 * после объявлений идет определение функций,
 * которые были объявлены в предыдущем блоке.
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include "AFFL_list.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("firewall");

//+++++++++++++++++++++++++++++++++++++++++++++++
//===============================================

static int changeSysCall(void);
static int returnSysCall(void);
static int startTimer(void);
static int stopTimer(void);

//================================================

static int __init mod_init(void)
{  
  if (initBlackList())
  {
    printk(KERN_WARNING "AFFL error: can't init blacklist\n");
    return -1; 
  }
  
  if (changeSysCall())
  {
    printk(KERN_WARNING "AFFL error: can't change system call\n");
    return -1;
  }
  
  if (startTimer())
  {
    printk(KERN_WARNING "AFFL error: can't startup timer\n");
    return -1;
  }
  
  return 0;
}

//================================================

static void __exit mod_exit(void)
{
  if (releaseBlackList())
  {
    printk(KERN_ERR "AFFL error: can't release blacklist\n");
  }
    
  if (returnSysCall())
  {
    printk(KERN_CRIT "AFFL error: can't return system call back\n");
  }
  
  while(stopTimer())
  {
    printk(KERN_WARNING "AFFL error: can't stop timer\n");
  }
}

//================================================

module_init(mod_init);
module_exit(mod_exit);

//------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++

typedef enum
{
  MODE_RW,
  MODE_RO
}TMemMode;

//================================================

static unsigned long **sys_call_table;
static struct timer_list timer;

//Адрес агрумента call (адреса sys_execve), который находится в stub_execve
static unsigned long addr_call_arg;

//================================================

static unsigned long **findSysCallTable(void);
static int changeMemMode(unsigned long **table, TMemMode mode);
static int patchStubExecve(void);
static void unpatchStubExecve(void);

static asmlinkage long fakeExecve(const char __user *filename,
				  const char __user *const __user *argv,
				  const char __user *const __user *envp);

static asmlinkage long (*sysExecve)(const char __user *filename,
				    const char __user *const __user *argv,
				    const char __user *const __user *envp);

static void timerFunc(unsigned long data);

//=================================================

int changeSysCall(void)
{
  if ((sys_call_table = findSysCallTable()) == NULL)    
    return -1;
  
  if (changeMemMode(sys_call_table[__NR_execve], MODE_RW))
  {
    printk(KERN_WARNING "AFFL error: can't set memory RW\n");
    
    return -1;
  }
   
    patchStubExecve();
    
  if (changeMemMode(sys_call_table[__NR_execve], MODE_RO))
  {
    printk(KERN_ERR "AFFL error: can't set memory RO\n");
    
    return -1;
  }
  
  return 0;
}

//=================================================

int returnSysCall(void)
{
  if (changeMemMode(sys_call_table[__NR_execve], MODE_RW))
  {
    printk(KERN_CRIT "AFFL error: can't set memory RW. Restart your computer\n");
    
    return -1;
  }
  
    unpatchStubExecve();
    
  if (changeMemMode(sys_call_table[__NR_execve], MODE_RO))
  {
    printk(KERN_ERR "AFFL error: can't set memory RO\n");
    
    return -1;
  }
  
  return 0;
}

//================================================

int startTimer(void)
{
  setup_timer(&timer, timerFunc, 0);
  if (mod_timer(&timer, jiffies + msec_to_jiffies(1000)))
  {
    printk(KERN_WARNING "AFFL error: can't mod timer\n");
    
    return -1;
  }
  
  return 0;
}

//=================================================

int stopTimer(void)
{
  if (del_timer(&timer))
  {
    printk(KERN_WARNING "AFFL error: can't delete timer\n");
 
    return -1;
  }
  
  return 0;
}

//---------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned long **findSysCallTable(void)
{
    unsigned long ptr;
    unsigned long *p;
    
    for (ptr = (unsigned long)sys_close;
	 ptr < (unsigned long)&loops_per_jiffy;
	 ptr += sizeof(unsigned long))
    {
      p = (unsigned long *)ptr;
      
      if (p[__NR_close] == (unsigned long)sys_close)
      {
	return (unsigned long **)p;
      }
    }
    
    return NULL;
}

//====================================================

int changeMemMode(unsigned long **table, TMemMode mode)
{
  unsigned int l;
  pte_t *pte;
  if (!(pte = lookup_address((long unsigned int)table, &l)))
  {
    return -1;
  }
  
  if (mode == MODE_RW)
    pte->pte |= _PAGE_RW;
  else
    pte->pte &= ~_PAGE_RW;
  
  return 0;
}

//====================================================

int patchStubExecve()
{
  uint8_t *ptr = memchr(sys_call_table[__NR_execve], 0xE8, 200);
  if (!ptr++)
    return -1;
  
  addr_call_arg = (unsigned long)ptr;
    
  sysExecve = (void *)((void *)addr_call_arg + 4 + *(int32_t *)addr_call_arg);
  *((int32_t *)addr_call_arg) = (int32_t)((unsigned long)fakeExecve - addr_call_arg - 4);
  
  return 0;
}

//====================================================

void unpatchStubExecve()
{
  *((int32_t *)addr_call_arg) = (int32_t)((unsigned long)sysExecve - addr_call_arg - 4);
}


//====================================================

asmlinkage long fakeExecve(const char __user *filename,
			   const char __user *const __user *argv,
			   const char __user *const __user *envp)
{
  if (findProcInBlackList(filename))
  {
    printk(KERN_NOTICE "AFFL notice: process %s blocked\n", filename);
    
    return 0;
  }
  
  return sysExecve(filename, argv, envp);
}

//=====================================================

void timerFunc(unsigned long data)
{
  if (refreshBlackList())
  {
    printk(KERN_ERR "AFFL error: can't refresh blacklist\n");
  }
  
  if (mod_timer(&timer, jiffies + msec_to_jiffies(1000)))
  {
    printk(KERN_WARNING "AFFL error: can't mod timer\n");
  }
}

//------------------------------------------------------