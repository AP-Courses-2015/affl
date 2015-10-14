#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include "AFFL_list.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("firewall");

//===============================================

static int changeSysCalls();
static int returnSysCalls();

//================================================

static int __init mod_init(void)
{
  initBlackList();
  changeSysCalls();
  
  return 0;
}

//================================================

static void __exit mod_exit(void)
{
  releaseBlackList();
  returnSysCalls();
}

//================================================

module_init(mod_init);
modult_exit(mod_exit);

//+++++++++++++++++++++++++++++++++++++++++++++++++

typedef enum
{
  MODE_RW,
  MODE_RO
}TMemMode;

//+++++++++++++++++++++++++++++++++++++++++++++++++

static unsigned long **sys_call_table;

//+++++++++++++++++++++++++++++++++++++++++++++++++

static unsigned long **findSysCallTable(void);
static void changeMemMode(unsigned long **table, TMemMode mode);

static asmlinkage long fakeExecve(const char __user *filename,
				  const char __user *const __user *argv,
				  const char __user *const __user *envp);

static asmlinkage long (*sysExecve)(const char __user *filename,
				    const char __user *const __user *argv,
				    const char __user *const __user *envp);

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

//=================================================

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