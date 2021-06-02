#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x637135ec, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xea0fb81, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0xf7b2a335, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x99563ee2, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x3b7fdfe1, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x198bbba8, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x91a57697, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xa202a8e5, __VMLINUX_SYMBOL_STR(kmalloc_order_trace) },
	{ 0x33472876, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xef428fed, __VMLINUX_SYMBOL_STR(cdev_alloc) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xfaef0ed, __VMLINUX_SYMBOL_STR(__tasklet_schedule) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xf01fb7e0, __VMLINUX_SYMBOL_STR(module_refcount) },
	{ 0xb5fe70d, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x15ba50a6, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "29F288AAF8BEA346E0A345C");
