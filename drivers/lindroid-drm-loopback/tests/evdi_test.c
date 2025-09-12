// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2024 DisplayLink (UK) Ltd.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License v2. See the file COPYING in the main directory of this archive for
 * more details.
 */

#include <linux/jiffies.h>
#include <kunit/test.h>
#include <kunit/test-bug.h>
#include <kunit/device.h>
#include <linux/mman.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include "evdi_drm_drv.h"

void __user *evdi_kunit_alloc_usermem(struct kunit *test, unsigned int size)
{
	void *kmem = kunit_kzalloc(test, size, GFP_KERNEL);
	unsigned long user_addr;

	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, kmem);

	user_addr = kunit_vm_mmap(test, NULL, 0, size,
			    PROT_READ | PROT_WRITE | PROT_EXEC,
			    MAP_ANONYMOUS | MAP_PRIVATE, 0);
	KUNIT_ASSERT_NE_MSG(test, user_addr, 0,
		"Could not create userspace mm");
	KUNIT_ASSERT_LT_MSG(test, user_addr, (unsigned long)TASK_SIZE,
		"Failed to allocate user memory");

	return (void __user *)user_addr;
}

void evdi_testhook_painter_vt_register(struct notifier_block *vt_notifier)
{
	struct kunit *test = kunit_get_current_test();
	struct evdi_test_data *base = (struct evdi_test_data *)test->priv;

	if (base && base->hooks.painter_vt_register)
		base->hooks.painter_vt_register(vt_notifier);
}

void evdi_testhook_painter_send_dpms(int mode)
{
	struct kunit *test = kunit_get_current_test();
	struct evdi_test_data *base = (struct evdi_test_data *)test->priv;

	if (base && base->hooks.painter_send_dpms)
		base->hooks.painter_send_dpms(mode);
}

void evdi_testhook_drm_device_destroyed(void)
{
	struct kunit *test = kunit_get_current_test();
	struct evdi_test_data *base = (struct evdi_test_data *)test->priv;

	if (base && base->hooks.drm_device_destroyed)
		base->hooks.drm_device_destroyed();
}

static void testhook_drm_device_destroyed(void)
{
	struct kunit *test = kunit_get_current_test();
	struct evdi_test_data *base = (struct evdi_test_data *)test->priv;

	complete(base->dev_destroyed);
}

void evdi_test_data_init(struct kunit *test, struct evdi_test_data *data)
{
	data->parent = kunit_device_register(test, "/dev/card1");
	data->dev_destroyed = kunit_kzalloc(test, sizeof(struct completion), GFP_KERNEL);
	init_completion(data->dev_destroyed);

	data->hooks.drm_device_destroyed = testhook_drm_device_destroyed;
	test->priv = (void *)data;
}

void evdi_test_data_exit(struct kunit *test, struct evdi_test_data *data)
{
	if (!wait_for_completion_timeout(data->dev_destroyed, msecs_to_jiffies(1000)))
		KUNIT_FAIL(test, "Failed to wait for drm_device removal\n");

	kunit_device_unregister(test, data->parent);
}

static void test_evdi_create_drm_device(struct kunit *test)
{
	struct evdi_test_data *data = kunit_kzalloc(test, sizeof(struct evdi_test_data), GFP_KERNEL);
	struct drm_device *dev;

	evdi_test_data_init(test, data);

	dev = evdi_drm_device_create(data->parent);

	KUNIT_EXPECT_NOT_NULL(test, dev);

	evdi_drm_device_remove(dev);
	evdi_test_data_exit(test, data);
	kunit_kfree(test, test->priv);
}

static struct kunit_case evdi_test_cases[] = {
	KUNIT_CASE(test_evdi_create_drm_device),
	{}
};

static struct kunit_suite evdi_test_suite = {
	.name = "drm_evdi_tests",
	.test_cases = evdi_test_cases,
};

kunit_test_suite(evdi_test_suite);

MODULE_LICENSE("GPL");
