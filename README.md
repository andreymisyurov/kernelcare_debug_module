# kernelcare_debug_module
The repository hosts a Linux kernel module that demonstrates debugfs usage. It logs messages on load/unload and allows jiffies monitoring and secure file interactions.

## Checking CONFIG_DEBUG_FS
call:
</br>`cat /boot/config-$(uname -r) | grep CONFIG_DEBUG_FS`
</br>It should be:
</br>`CONFIG_DEBUG_FS=y`


mount | grep debugfs

cat /sys/kernel/debug/kernelcare/jiffies