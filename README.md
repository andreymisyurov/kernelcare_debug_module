# kernelcare_debug_module
The repository hosts a Linux kernel module that demonstrates debugfs usage. It logs messages on load/unload and allows jiffies monitoring and secure file interactions.

## Checking CONFIG_DEBUG_FS
call:
</br>`cat /boot/config-$(uname -r) | grep CONFIG_DEBUG_FS`
</br>It should be:
</br>`CONFIG_DEBUG_FS=y`

## Checking mounting debugfs
To verify that debugfs is mounted, run:
</br>`mount | grep debugfs`

If nothing is returned, you can mount it manually using:
</br>`sudo mount -t debugfs none /sys/kernel/debug`


## How to use the module
please call:
</br>`sudo ls -la /sys/kernel/debug/kernelcare` - Verify the directory and files creation
</br>`sudo cat /sys/kernel/debug/kernelcare/jiffies` - Read the jiffies value
</br>`echo "Hello, KernelCare!" | sudo tee /sys/kernel/debug/kernelcare/data` - Write to the data file
</br>`sudo cat /sys/kernel/debug/kernelcare/data` - Read the contents of the data file


## Usful command

`git format-patch OLD_CAMMIT_HASH..LAST_CAMMIT_HASH -o ../patches` - for creating patch