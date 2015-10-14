/*Модуль ядра для блокирования запуска процессов из черного списка.
 *Подменяется системный вызов sys_execve(), который порождает процессы.
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
#include "AFFL_errors.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("firewall");

//+++++++++++++++++++++++++++++++++++++++++++++++
//===============================================

static int err = ENO_ERROR;

//===============================================

static int changeSysCalls(void);
static int returnSysCalls(void);
static int startTimer(void);
static int stopTimer(void);

//================================================

static int __init mod_init(void)
{  
  if (err = initBlackList())
  {
    printk(KERN_ALERT "error (%i): can't init blacklist\n", err);
    return err; 
  }
  
  if (err = changeSysCalls())
  {
    printk(KERN_ALERT "error (%i): can't change system call\n", err);
    return err;
  }
  
  if (err = startTimer())
  {
    printk(KERN_ALERT "error (%i): can't startup timer\n", err);
    return err;
  }
  
  return ENO_ERROR;
}

//================================================

static void __exit mod_exit(void)
{
  if (err = releaseBlackList())
  {
    printk(KERN_ALERT "error (%i): can't release blacklist\n", err);
  }
    
  if (err = returnSysCalls())
  {
    printk(KERN_ALERT "error (%i): can't return system call back\n", err);
  }
  
  while(err = stopTimer())
  {
    printk(KERN_ALERT "error (%i): can't stop timer\n", err);
  }
}

//================================================

module_init(mod_init);
modult_exit(mod_exit);

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

//================================================

static unsigned long **findSysCallTable(void);
static int changeMemMode(unsigned long **table, TMemMode mode);

static asmlinkage long fakeExecve(const char __user *filename,
				  const char __user *const __user *argv,
				  const char __user *const __user *envp);

static asmlinkage long (*sysExecve)(const char __user *filename,
				    const char __user *const __user *argv,
				    const char __user *const __user *envp);

static void timerFunc(unsigned long data);

//=================================================

int changeSysCalls(void)
{
  if ((sys_call_table = findSysCallTable()) == NULL)
  {
    err = -ESYS_CALLS_TABLE_FIND;
    printk(KERN_ALERT "error (%i): can't find system calls table\n", err);
    
    return err;
  }
  
  if (err = changeMemMode(sys_call_table, MODE_RW))
  {
    printk(KERN_ALERT "error (%i): can't set memory RW\n", err);
    
    return err;
  }
   
    sysExecve = sys_call_table[__NR_execve];
    sys_call_table[__NR_execve] = fakeExecve;
    
  if (err = changeMemMode(sys_call_table, MODE_RO))
  {
    printk(KERN_ALERT "error (%i): can't set memory RO\n", err);
    
    return err;
  }
  
  return ENO_ERROR;
}

//=================================================

int returnSysCalls(void)
{
  if (err = changeMemMode(sys_call_table, MODE_RW))
  {
    printk(KERN_ALERT "error (%i): can't set memory RW. Restart your computer\n", err);
    
    return err;
  }
  
    sys_call_table[__NR_execve] = sysExecve;
    
  if (err = changeMemMode(sys_call_table, MODE_RO))
  {
    printk(KERN_ALERT "error (%i): can't set memory RO\n", err);
    
    return err;
  }
  
  return ENO_ERROR;
}

//================================================

int startTimer(void)
{
  setup_timer(&timer, timerFunc, 0);
  if (err = mod_timer(&timer, jiffies + msec_to_jiffies(1000)))
  {
    printk(KERN_ALERT "error (%i): can't mod timer\n", err);
    
    err = -ETIMER_MOD;
    
    return err;
  }
  
  return ENO_ERROR;
}

//=================================================

int stopTimer(void)
{
  if (err = del_timer(&timer))
  {
    printk(KERN_ALERT "error (%i): can't delete timer\n", err);
 
    err = -ETIMER_DEL;
    
    return err;
  }
  
  return ENO_ERROR;
}

//---------------------------------------------------
//+++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned long **findSysCallTable(void)
{
    unsigned long ptr;
    unsigned long *p;
    
    for (ptr = (unsigned long)sys_close;
	 prt < (unsigned long)&loops_per_jiffy;
	 prt += sizeof(unsigned long))
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
    err = -ENO_PTE;
    printk(KERN_ALERT "error (%i): can't take pte from system calls table\n", err);
    
    return err;
  }
  
  if (mode == MODE_RW)
    pte->pte |= _PAGE_RW;
  else
    pte->pte &= ~_PAGE_RW;
  
  return ENO_ERROR;
}

//====================================================

asmlinkage long fakeExecve(const char __user *filename,
			   const char __user *const __user *argv,
			   const char __user *const __user *envp)
{
  if (findProcInBlackList(filename))
    return 0;
  
  return sysExecve(filename, argv, envp);
}

//=====================================================

void timerFunc(unsigned long data)
{
  if (err = refreshBlackList())
  {
    printk(KERN_ALERT "error (%i): can't refresh blacklist\n", err);
  }
  
  if (err = mod_timer(&timer, jiffies + msec_to_jiffies(1000)))
  {
    printk(KERN_ALERT "error (%i): can't mod timer\n", err);
    
    err = -ETIMER_MOD;
  }
}

//------------------------------------------------------