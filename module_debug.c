// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrey Misyurov");
MODULE_DESCRIPTION("A simple Hello, KernelCare! module");

static int __init hello_init(void)
{
    pr_info("Hello, KernelCare!\n");
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("Goodbye, KernelCare!\n");
}

module_init(hello_init);
module_exit(hello_exit);
