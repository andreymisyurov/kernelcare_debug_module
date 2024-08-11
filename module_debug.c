// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrey Misyurov");
MODULE_DESCRIPTION("A simple Hello, KernelCare! module");

static ssize_t jiffies_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos);

static struct dentry *kernel_care_dir = NULL;
static const struct file_operations jiffies_fops = {
    .read = jiffies_read,
};

static ssize_t jiffies_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[64] = {0};
    int len = 0;

    len = snprintf(buf, sizeof(buf), "%lu\n", jiffies);
    return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static int __init hello_init(void)
{
    pr_info("Hello, KernelCare!\n");
    kernel_care_dir = debugfs_create_dir("kernelcare", NULL);
    if (!kernel_care_dir) {
        pr_alert("Could not create kernelcare directory in debugfs\n");
        return -ENOMEM;
    }
    pr_info("kernelcare directory was created!\n");

    if (!debugfs_create_file("jiffies", 0444, kernel_care_dir, NULL, &jiffies_fops)) {
        pr_alert("Could not create jiffies file in debugfs\n");
        return -ENOMEM;
    }
    pr_info("Jiffies was created!\n");
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("Goodbye, KernelCare!\n");
    debugfs_remove_recursive(kernel_care_dir);
}

module_init(hello_init);
module_exit(hello_exit);
