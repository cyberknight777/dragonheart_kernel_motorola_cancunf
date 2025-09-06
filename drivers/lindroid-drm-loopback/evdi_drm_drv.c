// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2012 Red Hat
 * Copyright (c) 2015 - 2020 DisplayLink (UK) Ltd.
 *
 * Based on parts on udlfb.c:
 * Copyright (C) 2009 its respective authors
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License v2. See the file COPYING in the main directory of this archive for
 * more details.
 */

#include <linux/version.h>
#if KERNEL_VERSION(5, 16, 0) <= LINUX_VERSION_CODE || defined(EL8) || defined(EL9)
#include <drm/drm_ioctl.h>
#include <drm/drm_file.h>
#include <drm/drm_drv.h>
#include <drm/drm_vblank.h>
#elif KERNEL_VERSION(5, 5, 0) <= LINUX_VERSION_CODE
#else
#include <drm/drmP.h>
#endif
#if KERNEL_VERSION(5, 1, 0) <= LINUX_VERSION_CODE || defined(EL8)
#include <drm/drm_probe_helper.h>
#endif
#if KERNEL_VERSION(5, 8, 0) <= LINUX_VERSION_CODE || defined(EL8)
#include <drm/drm_managed.h>
#endif
#include <drm/drm_atomic_helper.h>
#include "evdi_drm_drv.h"
#include "evdi_platform_drv.h"
#include "evdi_cursor.h"
#include "evdi_debug.h"
#include "evdi_drm.h"

#if KERNEL_VERSION(6, 8, 0) <= LINUX_VERSION_CODE || defined(EL8)
#define EVDI_DRM_UNLOCKED 0
#else
#define EVDI_DRM_UNLOCKED DRM_UNLOCKED
#endif

static struct drm_driver driver;
int evdi_swap_callback_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file);
int evdi_add_buff_callback_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file);

int evdi_destroy_buff_callback_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file);

int evdi_gbm_add_buf_ioctl(
					struct drm_device *dev,
					void *data,
					struct drm_file *file);

int evdi_get_buff_callback_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file);

int evdi_gbm_get_buf_ioctl(struct drm_device *dev, void *data,
					struct drm_file *file);

int evdi_gbm_del_buf_ioctl(struct drm_device *dev, void *data,
					struct drm_file *file);

int evdi_gbm_create_buff(struct drm_device *dev, void *data,
					struct drm_file *file);

int evdi_create_buff_callback_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file);

struct drm_ioctl_desc evdi_painter_ioctls[] = {
	DRM_IOCTL_DEF_DRV(EVDI_CONNECT, evdi_painter_connect_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_REQUEST_UPDATE, evdi_painter_request_update_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_GRABPIX, evdi_painter_grabpix_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_ENABLE_CURSOR_EVENTS, evdi_painter_enable_cursor_events_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_POLL, evdi_poll_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_SWAP_CALLBACK, evdi_swap_callback_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_ADD_BUFF_CALLBACK, evdi_add_buff_callback_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_GET_BUFF_CALLBACK, evdi_get_buff_callback_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_DESTROY_BUFF_CALLBACK, evdi_destroy_buff_callback_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_GBM_ADD_BUFF, evdi_gbm_add_buf_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_GBM_GET_BUFF, evdi_gbm_get_buf_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_GBM_DEL_BUFF, evdi_gbm_del_buf_ioctl, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_GBM_CREATE_BUFF, evdi_gbm_create_buff, EVDI_DRM_UNLOCKED),
	DRM_IOCTL_DEF_DRV(EVDI_GBM_CREATE_BUFF_CALLBACK, evdi_create_buff_callback_ioctl, EVDI_DRM_UNLOCKED),
};

#if KERNEL_VERSION(5, 11, 0) <= LINUX_VERSION_CODE || defined(EL8)
#else
static const struct vm_operations_struct evdi_gem_vm_ops = {
	.fault = evdi_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};
#endif

static const struct file_operations evdi_driver_fops = {
	.owner = THIS_MODULE,
	.open = drm_open,
	.mmap = evdi_drm_gem_mmap,
	.poll = drm_poll,
	.read = drm_read,
	.unlocked_ioctl = drm_ioctl,
	.release = drm_release,

#ifdef CONFIG_COMPAT
	.compat_ioctl = evdi_compat_ioctl,
#endif

	.llseek = noop_llseek,

#if defined(FOP_UNSIGNED_OFFSET)
	.fop_flags = FOP_UNSIGNED_OFFSET,
#endif
};

#if KERNEL_VERSION(5, 11, 0) <= LINUX_VERSION_CODE || defined(EL8)
#else
static int evdi_enable_vblank(__always_unused struct drm_device *dev,
			      __always_unused unsigned int pipe)
{
	return 1;
}

static void evdi_disable_vblank(__always_unused struct drm_device *dev,
				__always_unused unsigned int pipe)
{
}
#endif

static struct drm_driver driver = {
#if KERNEL_VERSION(5, 4, 0) <= LINUX_VERSION_CODE || defined(EL8)
	.driver_features = DRIVER_MODESET | DRIVER_GEM | DRIVER_ATOMIC,
#else
	.driver_features = DRIVER_MODESET | DRIVER_GEM | DRIVER_PRIME
			 | DRIVER_ATOMIC,
#endif

	.open = evdi_driver_open,
	.postclose = evdi_driver_postclose,

	/* gem hooks */
#if KERNEL_VERSION(5, 11, 0) <= LINUX_VERSION_CODE || defined(EL8)
#elif KERNEL_VERSION(5, 9, 0) <= LINUX_VERSION_CODE
	.gem_free_object_unlocked = evdi_gem_free_object,
#else
	.gem_free_object = evdi_gem_free_object,
#endif

#if KERNEL_VERSION(5, 11, 0) <= LINUX_VERSION_CODE || defined(EL8)
#else
	.gem_vm_ops = &evdi_gem_vm_ops,
#endif

	.dumb_create = evdi_dumb_create,
	.dumb_map_offset = evdi_gem_mmap,
#if KERNEL_VERSION(5, 12, 0) <= LINUX_VERSION_CODE || defined(EL8)
#else
	.dumb_destroy = drm_gem_dumb_destroy,
#endif

	.ioctls = evdi_painter_ioctls,
	.num_ioctls = ARRAY_SIZE(evdi_painter_ioctls),

	.fops = &evdi_driver_fops,

	.gem_prime_import = drm_gem_prime_import,
#if KERNEL_VERSION(6, 6, 0) <= LINUX_VERSION_CODE
#else
	.prime_fd_to_handle = drm_gem_prime_fd_to_handle,
	.prime_handle_to_fd = drm_gem_prime_handle_to_fd,
#endif
#if KERNEL_VERSION(5, 11, 0) <= LINUX_VERSION_CODE || defined(EL8)
#else
	.preclose = evdi_driver_preclose,
	.gem_prime_export = drm_gem_prime_export,
	.gem_prime_get_sg_table = evdi_prime_get_sg_table,
	.enable_vblank = evdi_enable_vblank,
	.disable_vblank = evdi_disable_vblank,
#endif
	.gem_prime_import_sg_table = evdi_prime_import_sg_table,

	.name = DRIVER_NAME,
	.desc = DRIVER_DESC,
	.date = DRIVER_DATE,
	.major = DRIVER_MAJOR,
	.minor = DRIVER_MINOR,
	.patchlevel = DRIVER_PATCH,
};

struct evdi_event *evdi_create_event(struct evdi_device *evdi, enum poll_event_type type, void *data)
{
	struct evdi_event *event = kzalloc(sizeof(*event), GFP_KERNEL);
	if (!event)
		return NULL;

	event->type = type;
	event->data = data;
	init_waitqueue_head(&event->wait);
	event->completed = false;
	event->evdi = evdi;

	mutex_lock(&evdi->event_lock);

	event->poll_id = atomic_fetch_inc(&evdi->next_event_id);
	idr_alloc(&evdi->event_idr, event, event->poll_id, event->poll_id + 1, GFP_KERNEL);
	list_add_tail(&event->list, &evdi->event_queue);

	mutex_unlock(&evdi->event_lock);
	return event;
}


int evdi_swap_callback_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file)
{
	struct evdi_device *evdi = drm_dev->dev_private;
	struct drm_evdi_add_buff_callabck *cmd = data;
	struct evdi_event *event;

	mutex_lock(&evdi->event_lock);
	event = idr_find(&evdi->event_idr, cmd->poll_id);
	mutex_unlock(&evdi->event_lock);

	if (!event)
		return -EINVAL;

	event->result = 0;
	event->completed = true;
	wake_up(&event->wait);
	return 0;
}

int evdi_add_buff_callback_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file)
{
	struct evdi_device *evdi = drm_dev->dev_private;
	struct drm_evdi_add_buff_callabck *cmd = data;
	struct evdi_event *event;

	mutex_lock(&evdi->event_lock);
	event = idr_find(&evdi->event_idr, cmd->poll_id);
	mutex_unlock(&evdi->event_lock);

	if (!event)
		return -EINVAL;

	int *buff_id_ptr = kzalloc(sizeof(int), GFP_KERNEL);
	*buff_id_ptr = cmd->buff_id;
	event->reply_data = buff_id_ptr;
	event->result = 0;
	event->completed = true;
	wake_up(&event->wait);
	return 0;
}

int evdi_get_buff_callback_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file)
{
	struct evdi_device *evdi = drm_dev->dev_private;
	struct drm_evdi_get_buff_callabck *cmd = data;
	struct evdi_event *event;

	mutex_lock(&evdi->event_lock);
	event = idr_find(&evdi->event_idr, cmd->poll_id);
	mutex_unlock(&evdi->event_lock);

	if (!event)
		return -EINVAL;

	struct evdi_gralloc_buf *gralloc_buf = kzalloc(sizeof(struct evdi_gralloc_buf), GFP_KERNEL);
	gralloc_buf->version = cmd->version;
	gralloc_buf->numFds = cmd->numFds;
	gralloc_buf->numInts = cmd->numInts;
	gralloc_buf->data_ints = kzalloc(sizeof(int)*cmd->numInts, GFP_KERNEL);
	gralloc_buf->data_files = kzalloc(sizeof(struct file*)*cmd->numFds, GFP_KERNEL);

	copy_from_user(gralloc_buf->data_ints, cmd->data_ints, sizeof(int) * cmd->numInts);
	int *fd_ints = kzalloc(sizeof(int)*cmd->numFds, GFP_KERNEL);
	copy_from_user(fd_ints, cmd->fd_ints, sizeof(int) * cmd->numFds);
	for(int i = 0; i < cmd->numFds; i++) {
		gralloc_buf->data_files[i] = fget(fd_ints[i]);
		if (!gralloc_buf->data_files[i]) {
			printk("evdi_get_buff_callback_ioctl: Failed to open fake fb %d\n", cmd->fd_ints[i]);
			return -EINVAL;
		}
	}
	event->reply_data = gralloc_buf;
	event->result = 0;
	event->completed = true;
	wake_up(&event->wait);
	return 0;
}

int evdi_destroy_buff_callback_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file)
{
	struct evdi_device *evdi = drm_dev->dev_private;
	struct drm_evdi_add_buff_callabck *cmd = data;
	struct evdi_event *event;
	mutex_lock(&evdi->event_lock);
	event = idr_find(&evdi->event_idr, cmd->poll_id);
	mutex_unlock(&evdi->event_lock);
	if (!event) {
		printk("evdi_destroy_buff_callback_ioctl: event is null\n");
		return -EINVAL;
	}

	event->result = 0;
	event->completed = true;
	wake_up(&event->wait);
	return 0;
}

int evdi_create_buff_callback_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file)
{
	struct evdi_device *evdi = drm_dev->dev_private;
	struct drm_evdi_create_buff_callabck *cmd = data;
	struct evdi_event *event;
	struct drm_evdi_create_buff_callabck *buf = kzalloc(sizeof(struct drm_evdi_create_buff_callabck), GFP_KERNEL);
	memcpy(buf, data, sizeof(struct drm_evdi_create_buff_callabck));
	mutex_lock(&evdi->event_lock);
	event = idr_find(&evdi->event_idr, cmd->poll_id);
	mutex_unlock(&evdi->event_lock);

	if (!event)
		return -EINVAL;

	event->result = 0;
	event->completed = true;
	event->reply_data = buf;
	wake_up(&event->wait);
	return 0;
}

int evdi_gbm_add_buf_ioctl(struct drm_device *dev, void *data,
					struct drm_file *file)
{
	struct file *memfd_file;
	struct file *fd_file;
	int ret;
	uint32_t handle;
	int version, numFds, numInts, fd;
	ssize_t bytes_read;
	struct evdi_gralloc_buf *add_gralloc_buf;
	struct evdi_device *evdi = dev->dev_private;
	struct drm_evdi_gbm_add_buf *cmd = data;

	memfd_file = fget(cmd->fd);
	if (!memfd_file) {
		printk("Failed to open fake fb: %d\n", cmd->fd);
		return -EINVAL;
	}

	loff_t pos = 0; // Initialize offset
	bytes_read = kernel_read(memfd_file, &version, sizeof(version), &pos);
	if (bytes_read != sizeof(version)) {
		printk("Failed to read version from memfd, bytes_read=%zd\n", bytes_read);
		return -EIO;
	}

	bytes_read = kernel_read(memfd_file, &numFds, sizeof(numFds), &pos);
	if (bytes_read != sizeof(numFds)) {
		printk("Failed to read numFds from memfd, bytes_read=%zd\n", bytes_read);
		return -EIO;
	}

	bytes_read = kernel_read(memfd_file, &numInts, sizeof(numInts), &pos);
	if (bytes_read != sizeof(numInts)) {
		printk("Failed to read numInts from memfd, bytes_read=%zd\n", bytes_read);
		return -EIO;
	}
	add_gralloc_buf = kzalloc(sizeof(struct evdi_gralloc_buf), GFP_KERNEL);
	add_gralloc_buf->numFds = numFds;
	add_gralloc_buf->numInts = numInts;
	add_gralloc_buf->data_ints = kzalloc(sizeof(int)*numInts, GFP_KERNEL);
	add_gralloc_buf->data_files = kzalloc(sizeof(struct file*)*numFds, GFP_KERNEL);
	add_gralloc_buf->memfd_file = memfd_file;

	for(int i = 0; i < numFds; i++) {
		bytes_read = kernel_read(memfd_file, &fd, sizeof(fd), &pos);
		if (bytes_read != sizeof(fd)) {
			printk("Failed to read fd from memfd, bytes_read=%zd\n", bytes_read);
			return -EIO;
		}
		fd_file = fget(fd);
		if (!fd_file) {
			printk("Failed to open fake fb's %d fd file: %d\n", cmd->fd, fd);
			return -EINVAL;
		}
		add_gralloc_buf->data_files[i] = fd_file;

	}

	bytes_read = kernel_read(memfd_file, add_gralloc_buf->data_ints, sizeof(int) *numInts, &pos);
	if (bytes_read != sizeof(int) *numInts) {
		printk("Failed to read ints from memfd, bytes_read=%zd\n", bytes_read);
		return -EIO;
	}

	struct evdi_event *event = evdi_create_event(evdi, add_buf, add_gralloc_buf);
	if (!event)
		return -ENOMEM;

	wake_up(&evdi->poll_ioct_wq);
	ret = wait_event_interruptible(event->wait, event->completed);
	if (ret < 0){
		printk("evdi_gbm_add_buf_ioctl: wait_event_interruptible interrupted: %d\n", ret);
		return ret;
	}

	ret = event->result;
	if (ret < 0) {
		pr_err("evdi_gbm_add_buf_ioctl: user ioctl failled\n");
		return ret;
	}

	if (ret)
		goto err_inval;
	cmd->id = *((int *)event->reply_data);
	mutex_lock(&evdi->event_lock);
	idr_remove(&evdi->event_idr, event->poll_id);
	mutex_unlock(&evdi->event_lock);
	kfree(event);
	return 0;

 err_no_mem:
	return -ENOMEM;
 err_inval:
	return -EINVAL;
}

int evdi_gbm_get_buf_ioctl(struct drm_device *dev, void *data,
					struct drm_file *file)
{
	struct drm_evdi_gbm_get_buff *cmd = data;
	struct evdi_gralloc_buf_user *gralloc_buf = kzalloc(sizeof(struct evdi_gralloc_buf_user), GFP_KERNEL);
	struct evdi_gralloc_buf *gralloc_buf_tmp;
	struct evdi_device *evdi = dev->dev_private;
	int fd_tmp, ret;

	struct evdi_event *event = evdi_create_event(evdi, get_buf, &cmd->id);
	if (!event)
		return -ENOMEM;

	wake_up(&evdi->poll_ioct_wq);
	ret = wait_event_interruptible(event->wait, event->completed);
	if (ret < 0) {
		printk("evdi_gbm_get_buf_ioctl: wait_event_interruptible interrupted: %d\n", ret);
		return ret;
	}

	ret = event->result;
	if (ret < 0) {
		pr_err("evdi_gbm_get_buf_ioctl: user ioctl failled\n");
		return ret;
	}

	gralloc_buf_tmp = event->reply_data;
	gralloc_buf->version = gralloc_buf_tmp->version;
	gralloc_buf->numFds = gralloc_buf_tmp->numFds;
	gralloc_buf->numInts = gralloc_buf_tmp->numInts;
	memcpy(&gralloc_buf->data[gralloc_buf->numFds], gralloc_buf_tmp->data_ints, sizeof(int)*gralloc_buf->numInts);

	for(int i = 0; i < gralloc_buf->numFds; i++) {
		fd_tmp = get_unused_fd_flags(O_RDWR);
		fd_install(fd_tmp, gralloc_buf_tmp->data_files[i]);
		gralloc_buf->data[i] = fd_tmp;
	}

	if (copy_to_user(cmd->native_handle, gralloc_buf, sizeof(int)*(3 + gralloc_buf->numFds + gralloc_buf->numInts))) {
		pr_err("Failed to copy file descriptor to userspace\n");
		kfree(gralloc_buf);
		return -EFAULT;
	}

	kfree(gralloc_buf);
	mutex_lock(&evdi->event_lock);
	idr_remove(&evdi->event_idr, event->poll_id);
	mutex_unlock(&evdi->event_lock);
	kfree(event);

	return 0;
}

int evdi_gbm_del_buf_ioctl(struct drm_device *dev, void *data,
					struct drm_file *file)
{
	struct drm_evdi_gbm_del_buff *cmd = data;
	struct evdi_device *evdi = dev->dev_private;
	int ret;

	struct evdi_event *event = evdi_create_event(evdi, destroy_buf, &cmd->id);
	if (!event)
		return -ENOMEM;

	wake_up(&evdi->poll_ioct_wq);
	ret = wait_event_interruptible(event->wait, event->completed);
	if (ret < 0) {
		printk("evdi_gbm_get_buf_ioctl: wait_event_interruptible interrupted: %d\n", ret);
		return ret;
	}

	ret = event->result;
	if (ret < 0) {
		pr_err("evdi_gbm_get_buf_ioctl: user ioctl failled\n");
		return ret;
	}

	mutex_lock(&evdi->event_lock);
	idr_remove(&evdi->event_idr, event->poll_id);
	mutex_unlock(&evdi->event_lock);
	kfree(event);

	return 0;
}

int evdi_gbm_create_buff (struct drm_device *dev, void *data,
					struct drm_file *file)
{
	struct drm_evdi_gbm_create_buff *cmd = data;
	struct evdi_device *evdi = dev->dev_private;
	int ret;
	struct evdi_event *event = evdi_create_event(evdi, create_buf, cmd);
	if (!event)
		return -ENOMEM;

	wake_up(&evdi->poll_ioct_wq);
	ret = wait_event_interruptible(event->wait, event->completed);
	if (ret < 0) {
		printk("evdi_gbm_create_buff: wait_event_interruptible interrupted: %d\n", ret);
		return ret;
	}

	ret = event->result;
	if (ret < 0) {
		pr_err("evdi_gbm_create_buff: user ioctl failled\n");
		return ret;
	}

	struct drm_evdi_create_buff_callabck *cb_cmd = (struct drm_evdi_create_buff_callabck *)event->reply_data;
	copy_to_user(cmd->id, &cb_cmd->id, sizeof(int));
	copy_to_user(cmd->stride, &cb_cmd->stride, sizeof(int));
	mutex_lock(&evdi->event_lock);
	idr_remove(&evdi->event_idr, event->poll_id);
	mutex_unlock(&evdi->event_lock);
	kfree(event);

	return 0;
}

int evdi_poll_ioctl(struct drm_device *drm_dev, void *data,
                    struct drm_file *file)
{
	struct evdi_device *evdi = drm_dev->dev_private;
	struct drm_evdi_poll *cmd = data;
	struct evdi_event *event;
	int fd, fd_tmp, ret;
	ssize_t bytes_write;
	loff_t pos;

	EVDI_CHECKPT();

	if (!evdi) {
		pr_err("evdi is null\n");
		return -ENODEV;
	}

	ret = wait_event_interruptible(evdi->poll_ioct_wq,
		!list_empty(&evdi->event_queue));

	if (ret < 0) {
		pr_err("evdi_poll_ioctl: Wait interrupted by signal\n");
		return ret;
	}

	mutex_lock(&evdi->event_lock);

	if (list_empty(&evdi->event_queue)) {
		mutex_unlock(&evdi->event_lock);
		return -EAGAIN;
	}

	event = list_first_entry(&evdi->event_queue, struct evdi_event, list);
	list_del(&event->list);

	mutex_unlock(&evdi->event_lock);

	cmd->event = event->type;
	cmd->poll_id = event->poll_id;

	switch(cmd->event) {
		case add_buf:
			{
			struct evdi_gralloc_buf *add_gralloc_buf = event->data;
			fd = get_unused_fd_flags(O_RDWR);
			if (fd < 0) {
				pr_err("Failed to allocate file descriptor\n");
				return fd;
			}

			fd_install(fd, add_gralloc_buf->memfd_file);

			for(int i = 0; i < add_gralloc_buf->numFds; i++) {
				fd_tmp = get_unused_fd_flags(O_RDWR);
				fd_install(fd_tmp, add_gralloc_buf->data_files[i]);
				pos = sizeof(int) * (3 + i);
				bytes_write = kernel_write(add_gralloc_buf->memfd_file, &fd_tmp, sizeof(fd_tmp), &pos);
				if (bytes_write != sizeof(fd_tmp)) {
					pr_err("Failed to write fd\n");
					put_unused_fd(fd);
					return -EFAULT;
				}
			}

			if (copy_to_user(cmd->data, &fd, sizeof(fd))) {
				pr_err("Failed to copy file descriptor to userspace\n");
				put_unused_fd(fd);
				return -EFAULT;
			}
			break;
			}
		case create_buf:
			copy_to_user(cmd->data, event->data, sizeof(struct drm_evdi_gbm_create_buff));
			break;
		case get_buf:
		case swap_to:
		case destroy_buf:
			copy_to_user(cmd->data, event->data, sizeof(int));
			break;
		default:
			pr_err("unknown event: %d\n", cmd->event);
	}

	return 0;
}

static void evdi_drm_device_release_cb(__always_unused struct drm_device *dev,
				       __always_unused void *ptr)
{
	struct evdi_device *evdi = dev->dev_private;

	evdi_cursor_free(evdi->cursor);
	evdi_painter_cleanup(evdi->painter);
	kfree(evdi);
	dev->dev_private = NULL;
	EVDI_INFO("Evdi drm_device removed.\n");

	EVDI_TEST_HOOK(evdi_testhook_drm_device_destroyed());
}

static int evdi_drm_device_init(struct drm_device *dev)
{
	struct evdi_device *evdi;
	int ret;

	EVDI_CHECKPT();
	evdi = kzalloc(sizeof(struct evdi_device), GFP_KERNEL);
	if (!evdi)
		return -ENOMEM;

	evdi->ddev = dev;
	evdi->dev_index = dev->primary->index;
	evdi->cursor_events_enabled = false;
	dev->dev_private = evdi;
	evdi->poll_event = none;
	init_waitqueue_head (&evdi->poll_ioct_wq);
	init_waitqueue_head (&evdi->poll_response_ioct_wq);
	mutex_init(&evdi->poll_lock);
	init_completion(&evdi->poll_completion);
	evdi->poll_data_size = -1;

	mutex_init(&evdi->event_lock);
	INIT_LIST_HEAD(&evdi->event_queue);
	idr_init(&evdi->event_idr);
	atomic_set(&evdi->next_event_id, 1);

	ret = evdi_painter_init(evdi);
	if (ret)
		goto err_free;
	ret =  evdi_cursor_init(&evdi->cursor);
	if (ret)
		goto err_free;

	evdi_modeset_init(dev);

	ret = drm_vblank_init(dev, 1);
	if (ret)
		goto err_init;
	drm_kms_helper_poll_init(dev);

#if KERNEL_VERSION(5, 8, 0) <= LINUX_VERSION_CODE || defined(EL8)
	ret = drmm_add_action_or_reset(dev, evdi_drm_device_release_cb, NULL);
	if (ret)
		goto err_init;
#endif

	return 0;

err_init:
err_free:
	EVDI_ERROR("Failed to setup drm device %d\n", ret);
	evdi_cursor_free(evdi->cursor);
	kfree(evdi->painter);
	kfree(evdi);
	dev->dev_private = NULL;
	return ret;
}

int evdi_driver_open(struct drm_device *dev, __always_unused struct drm_file *file)
{
	char buf[100];

	evdi_log_process(buf, sizeof(buf));
	EVDI_INFO("(card%d) Opened by %s\n", dev->primary->index, buf);
	return 0;
}

static void evdi_driver_close(struct drm_device *drm_dev, struct drm_file *file)
{
	struct evdi_device *evdi = drm_dev->dev_private;

	EVDI_CHECKPT();
	if (evdi)
		evdi_painter_close(evdi, file);
}

void evdi_driver_preclose(struct drm_device *drm_dev, struct drm_file *file)
{
	evdi_driver_close(drm_dev, file);
}

void evdi_driver_postclose(struct drm_device *dev, struct drm_file *file)
{
	char buf[100];

	evdi_log_process(buf, sizeof(buf));
	evdi_driver_close(dev, file);
	EVDI_INFO("(card%d) Closed by %s\n", dev->primary->index, buf);
}

struct drm_device *evdi_drm_device_create(struct device *parent)
{
	struct drm_device *dev = NULL;
	int ret;

	dev = drm_dev_alloc(&driver, parent);
	if (IS_ERR(dev))
		return dev;

	ret = evdi_drm_device_init(dev);
	if (ret)
		goto err_free;

	ret = drm_dev_register(dev, 0);
	if (ret)
		goto err_free;

	return dev;

err_free:
	drm_dev_put(dev);
	return ERR_PTR(ret);
}

static void evdi_drm_device_deinit(struct drm_device *dev)
{
	drm_kms_helper_poll_fini(dev);
	evdi_modeset_cleanup(dev);
	drm_atomic_helper_shutdown(dev);
}

int evdi_drm_device_remove(struct drm_device *dev)
{
	drm_dev_unplug(dev);
	evdi_drm_device_deinit(dev);
#if KERNEL_VERSION(5, 8, 0) <= LINUX_VERSION_CODE || defined(EL8)
#else
	evdi_drm_device_release_cb(dev, NULL);
#endif
	drm_dev_put(dev);
	return 0;
}

