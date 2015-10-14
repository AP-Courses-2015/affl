#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include "AFFL_list.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("firewall");

static int __init mod_init(void)
{
  initBlackList();
}

static void __exit mod_exit(void)
{
  releaseBlackList();
}

module_init(mod_init);
modult_exit(mod_exit);