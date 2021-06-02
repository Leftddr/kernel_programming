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

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xe89b6b83, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x5b2f4bfc, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x542de5a3, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0xfad9ee14, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xb90a1470, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x8f2d819b, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0xa45fe9e1, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xa202a8e5, __VMLINUX_SYMBOL_STR(kmalloc_order_trace) },
	{ 0xd518655a, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x1ffe4976, __VMLINUX_SYMBOL_STR(cdev_alloc) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x4f6b400b, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x1bb31047, __VMLINUX_SYMBOL_STR(add_timer) },
	{ 0x9580deb, __VMLINUX_SYMBOL_STR(init_timer_key) },
	{ 0xb14f552d, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xe44e3f36, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x24f36661, __VMLINUX_SYMBOL_STR(module_refcount) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x8b81418e, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x7d11c268, __VMLINUX_SYMBOL_STR(jiffies) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "2E6D732B0C91C10EFD6B3FA");
