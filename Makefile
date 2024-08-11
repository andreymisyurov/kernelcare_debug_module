obj-m += module_hello.o
CC = $(shell cat /proc/version | grep -oP 'gcc-\d+')

all:
	make -C /lib/modules/$(shell uname -r)/build CC=$(CC) M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	sudo insmod module_hello.ko
	@echo "Checking kernel log:"
	@sudo dmesg -c
	@echo "Checking our module in kernel:"
	@lsmod | grep module_hello

unload:
	sudo rmmod module_hello.ko
	@echo "Checking kernel log:"
	@sudo dmesg -c
	@echo "Checking our module in kernel:"
	@if lsmod | grep -q module_hello; then \
		echo "Module is still loaded in the system."; \
	else \
		echo "Module is not found in the system."; \
	fi