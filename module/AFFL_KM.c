#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("firewall");

static int __init mod_init(void)
{
  
}

static void __exit mod_exit(void)
{
  
}

module_init(mod_init);
modult_exit(mod_exit);