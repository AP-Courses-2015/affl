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

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("firewall");

//+++++++++++++++++++++++++++++++++++++++++++++++
//===============================================

static int changeSysCalls(void);
static int returnSysCalls(void);
static int startTimer(void);
static int stopTimer(void);

//================================================

static int __init mod_init(void)
{
  initBlackList();
  changeSysCalls();
  startTimer();
  
  return 0;
}

//================================================

static void __exit mod_exit(void)
{
  releaseBlackList();
  returnSysCalls();
  stopTimer();  
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
static void changeMemMode(unsigned long **table, TMemMode mode);

static asmlinkage long fakeExecve(const char __user *filename,
				  const char __user *const __user *argv,
				  const char __user *const __user *envp);

static asmlinkage long (*sysExecve)(const char __user *filename,
				    const char __user *const __user *argv,
				    const char __user *const __user *envp);

static void timerFunc(unsigned long data);

//=================================================

void changeSysCalls(void)
{
  sys_call_table = findSysCallTable();
  
  changeMemMode(sys_call_table, MODE_RW);
    sysExecve = sys_call_table[__NR_execve];
    sys_call_table[__NR_execve] = fakeExecve;
  changeMemMode(sys_call_table, MODE_RO);
}

//=================================================

void returnSysCalls(void)
{
  changeMemMode(sys_call_table, MODE_RW);
    sys_call_table[__NR_execve] = sysExecve;
  changeMemMode(sys_call_table, MODE_RO);
}

//================================================

void startTimer(void)
{
  setup_timer(&timer, timerFunc, 0);
  mod_timer(&timer, jiffies + msec_to_jiffies(1000));
}

//=================================================

void stopTimer(void)
{
  del_timer(&timer);
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

void changeMemMode(unsigned long **table, TMemMode mode)
{
  unsigned int l;
  pte_t *pte;
  pte = lookup_address((long unsigned int)table, &l);
  if (mode == MODE_RW)
    pte->pte |= _PAGE_RW;
  else
    pte->pte &= ~_PAGE_RW;
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
  refreshBlackList();
  mod_timer(&timer, jiffies + msec_to_jiffies(1000));
}

//------------------------------------------------------