/*
 * drivers/staging/zte_ftm/ftm.c
 *
 * Copyright (C) 2012-2013 ZTE, Corp.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  Date         Author           Comment
 *  -----------  --------------   -------------------------------------
 *  2012-08-03	 Jia   	          The kernel module for FTM,
 *                                created by ZTE_BOO_JIA_20120803 jia.jia
 *  -------------------------------------------------------------------
 *
*/

#include <linux/module.h>
#include <linux/sysdev.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/platform_device.h>

/*
 * Macro Definition
 */
#define FTM_VERSION "1.0"
#define FTM_CLASS_NAME "zte_ftm"

/*
 * Type Definition
 */
typedef struct {
  int32_t fusion;
  int32_t simlocking;
} ftm_sysdev_attrs_t;

/*
 * Global Variables Definition
 */
static struct sys_device ftm_sysdev;
static ftm_sysdev_attrs_t ftm_sysdev_attrs;
static spinlock_t ftm_spinlock;

/*
 * Function Declaration
 */
static int32_t ftm_register_sysdev(struct sys_device *sysdev);

/*
 * Function Definition
 */
static ssize_t show_fusion(struct sys_device *dev, struct sysdev_attribute *attr, char *buf)
{
  return snprintf(buf, PAGE_SIZE, "%d\n", ftm_sysdev_attrs.fusion);
}

static ssize_t store_fusion(struct sys_device *dev, struct sysdev_attribute *attr, const char *buf, size_t buf_sz)
{
  return buf_sz;
}

static SYSDEV_ATTR(fusion, S_IRUGO | S_IWUSR, show_fusion, store_fusion);

static ssize_t show_simlocking(struct sys_device *dev, struct sysdev_attribute *attr, char *buf)
{
  return snprintf(buf, PAGE_SIZE, "%d\n", ftm_sysdev_attrs.simlocking);
}

static ssize_t store_simlocking(struct sys_device *dev, struct sysdev_attribute *attr, const char *buf, size_t buf_sz)
{
  return buf_sz;
}

static SYSDEV_ATTR(simlocking, S_IRUGO | S_IWUSR, show_simlocking, store_simlocking);

static struct sysdev_attribute *ftm_attrs[] = {
  &attr_fusion,
  &attr_simlocking
};

static struct sysdev_class ftm_sysdev_class = {
  .name = FTM_CLASS_NAME
};

/*
 * Sys device register
 *
 * sysdev file:
 *
 * /sys/devices/system/zte_ftm/zte_ftm0/fusion
 * /sys/devices/system/zte_ftm/zte_ftm0/simlocking
 */
static int32_t ftm_register_sysdev(struct sys_device *sysdev)
{
  int32_t ret;
  int32_t i;

  ret = sysdev_class_register(&ftm_sysdev_class);
  if (ret)
  {
    return ret;
  }

  sysdev->id = 0;
  sysdev->cls = &ftm_sysdev_class;

  ret = sysdev_register(sysdev);
  if (ret)
  {
    sysdev_class_unregister(&ftm_sysdev_class);
    return ret;
  }

  for (i = 0; i < ARRAY_SIZE(ftm_attrs); i++)
  {
    ret = sysdev_create_file(sysdev, ftm_attrs[i]);
    if (ret)
    {
      goto ftm_fail;
    }
  }

  return 0;

ftm_fail:

  while (--i >= 0) sysdev_remove_file(sysdev, ftm_attrs[i]);

  sysdev_unregister(sysdev);
  sysdev_class_unregister(&ftm_sysdev_class);

  return ret;
}

static int32_t __init ftm_probe(struct platform_device *pdev)
{
  pr_info("%s: e\n", __func__);

  pr_info("%s: x\n", __func__);

  return 0;
}

static int32_t ftm_remove(struct platform_device *pdev)
{
  pr_info("%s: e\n", __func__);

  pr_info("%s: x\n", __func__);

  return 0;
}

static struct platform_driver ftm_driver = {
  .remove = ftm_remove,
  .driver = {
    .name = "zte_ftm",
    .owner = THIS_MODULE,
  },
};

/*
 * Initializes the module.
 */
static int32_t __init ftm_init(void)
{
  int ret;

  pr_info("%s: e\n", __func__);

  ret = platform_driver_probe(&ftm_driver, ftm_probe);
  if (ret)
  {
    return ret;
  }

  /*
   * Initialize spinlock
   */
  spin_lock_init(&ftm_spinlock);

  /*
   * Initialize sys device
   */
  ftm_register_sysdev(&ftm_sysdev);

  pr_info("%s: x\n", __func__);

  return 0;
}

/*
 * Cleans up the module.
 */
static void __exit ftm_exit(void)
{
  platform_driver_unregister(&ftm_driver);
}

module_init(ftm_init);
module_exit(ftm_exit);

MODULE_DESCRIPTION("ZTE FTM Driver Ver %s" FTM_VERSION);
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif
