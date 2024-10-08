// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrey Misyurov");
MODULE_DESCRIPTION("A kernel module for interacting with debugfs, providing jiffies and data interfaces.");

static ssize_t jiffies_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos);
static ssize_t data_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos);
static ssize_t data_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos);

static struct dentry *kernel_care_dir = NULL;
static struct mutex data_mutex;
static const struct file_operations jiffies_fops = {
    .read = jiffies_read,
};
static const struct file_operations data_fops = {
    .read = data_read,
    .write = data_write,
};

static char global_data[PAGE_SIZE];

static ssize_t jiffies_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[64] = {0};
    int len = 0;

    len = snprintf(buf, sizeof(buf), "%lu\n", jiffies);
    return simple_read_from_buffer(user_buf, count, ppos, buf, len);
}

static ssize_t data_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    ssize_t result = 0;
    mutex_lock(&data_mutex);
    pr_info("Data mutex locked\n");
    result = simple_read_from_buffer(user_buf, count, ppos, global_data, strlen(global_data));
    mutex_unlock(&data_mutex);
    pr_info("Data mutex unlocked\n");
    return result;
}

static ssize_t data_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{

    if (count > PAGE_SIZE - 1)
        count = PAGE_SIZE - 1;

    mutex_lock(&data_mutex);
    pr_info("Data mutex locked\n");
    if (copy_from_user(global_data, user_buf, count)) {
        mutex_unlock(&data_mutex);
        pr_info("Data mutex unlocked\n");
        return -EFAULT;
    }

    global_data[count] = '\0';

    pr_info("Written to data: %s\n", global_data);

    mutex_unlock(&data_mutex);
    pr_info("Data mutex unlocked\n");
    return (ssize_t)count;
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

    if (!debugfs_create_file("data", 0644, kernel_care_dir, NULL, &data_fops)) {
        pr_alert("Could not create data file in debugfs\n");
        return -ENOMEM;
    }
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("Goodbye, KernelCare!\n");
    debugfs_remove_recursive(kernel_care_dir);
}

module_init(hello_init);
module_exit(hello_exit);
