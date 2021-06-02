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
	{ 0x89c51293, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xc1d5297f, __VMLINUX_SYMBOL_STR(platform_device_unregister) },
	{ 0x8a1400c9, __VMLINUX_SYMBOL_STR(sysfs_remove_group) },
	{ 0xb65bb8b, __VMLINUX_SYMBOL_STR(input_unregister_device) },
	{ 0x8f310124, __VMLINUX_SYMBOL_STR(sysfs_create_group) },
	{ 0x9d267bfc, __VMLINUX_SYMBOL_STR(platform_device_register_full) },
	{ 0xc688d89b, __VMLINUX_SYMBOL_STR(input_register_device) },
	{ 0x772e118b, __VMLINUX_SYMBOL_STR(input_allocate_device) },
	{ 0xeae3dfd6, __VMLINUX_SYMBOL_STR(__const_udelay) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x53411306, __VMLINUX_SYMBOL_STR(input_event) },
	{ 0x20c55ae0, __VMLINUX_SYMBOL_STR(sscanf) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "B0973E4E31D40DDB99B5EB7");
