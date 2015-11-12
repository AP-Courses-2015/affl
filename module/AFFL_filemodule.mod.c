#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xe90ce097, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xec4c80d, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x4c4fef19, __VMLINUX_SYMBOL_STR(kernel_stack) },
	{ 0x7e756b15, __VMLINUX_SYMBOL_STR(single_open) },
	{ 0xa05f372e, __VMLINUX_SYMBOL_STR(seq_printf) },
	{ 0xf84f67e5, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{        0, __VMLINUX_SYMBOL_STR(filp_close) },
	{ 0xc26f91eb, __VMLINUX_SYMBOL_STR(seq_read) },
	{ 0xab379e22, __VMLINUX_SYMBOL_STR(vfs_read) },
	{ 0x206276fd, __VMLINUX_SYMBOL_STR(proc_mkdir) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x5a921311, __VMLINUX_SYMBOL_STR(strncmp) },
	{ 0xf0fdf6cb, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0xfcd1feec, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x41a42ce0, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x6bbd5c88, __VMLINUX_SYMBOL_STR(vfs_write) },
	{ 0x79578f83, __VMLINUX_SYMBOL_STR(filp_open) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "27FC822F9B6254F7C7B7798");
