cmd_/home/kernel/Desktop/hw71/vms.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /home/kernel/Desktop/hw71/vms.ko /home/kernel/Desktop/hw71/vms.o /home/kernel/Desktop/hw71/vms.mod.o