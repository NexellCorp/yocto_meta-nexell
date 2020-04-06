/**************************************************************************
 *
 * Copyright © 2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/


#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <direct/types.h>
#include <misc/conf.h>
#include <xf86drm.h>
#include <libkms/libkms.h>
#include <core/core.h>

#include "libdrm_macros.h"

enum dumb_bo_type {
	/*
	 * DMA continuous memory
	 * user   : non-cacheable
	 * kernel : non-cacheable
	 */
	DUMB_BO_DMA,

	/*
	 * DMA continuous memory, allocate from DMA,
	 * user   : cacheable
	 * kernel : non-cacheable
	 */
	DUMB_BO_DMA_CACHEABLE,

	/*
	 * System continuous memory, allocate from system
	 * user   : non-cacheable
	 * kernel : non-cacheable
	 */
	DUMB_BO_SYSTEM,

	/*
	 * System continuous memory, allocate from system
	 * user   : cacheable
	 * kernel : cacheable
	 */
	DUMB_BO_SYSTEM_CACHEABLE,

	/*
	 * System non-continuous memory, allocate from system
	 * user   : non-cacheable
	 * kernel : non-cacheable
	 */
	DUMB_BO_SYSTEM_NONCONTIG,

	/*
	 * System non-continuous memory, allocate from system
	 * user   : cacheable
	 * kernel : cacheable
	 */
	DUMB_BO_SYSTEM_NONCONTIG_CACHEABLE,

	DUMB_BO_MAX,
};

static const char * const dumb_bo_type_name[] = {
	[DUMB_BO_DMA] = "DMA,nocachae",
        [DUMB_BO_DMA_CACHEABLE] = "DMA,cache",
        [DUMB_BO_SYSTEM] = "SYS,nocache",
        [DUMB_BO_SYSTEM_CACHEABLE] = "SYS,cache",
        [DUMB_BO_SYSTEM_NONCONTIG] = "SYS,nocache,nocontig",
        [DUMB_BO_SYSTEM_NONCONTIG_CACHEABLE] = "SYS,cache,nocontig",
};

struct drm_mode_user_dumb {
	uint64_t size;
	unsigned int flags;
	unsigned int handle;
	void *priv_data;
};

#define DRM_IOCTL_USER_DUMB_SYNC \
	DRM_IOWR(DRM_COMMAND_BASE + 0x05, struct drm_mode_user_dumb)

int kms_bo_sync(struct kms_bo *bo, unsigned int flags);

struct kms_driver
{
	int (*get_prop)(struct kms_driver *kms, const unsigned key,
			unsigned *out);
	int (*destroy)(struct kms_driver *kms);

	int (*bo_create)(struct kms_driver *kms,
			 unsigned width,
			 unsigned height,
			 enum kms_bo_type type,
			 const unsigned *attr,
			 struct kms_bo **out);
	int (*bo_get_prop)(struct kms_bo *bo, const unsigned key,
			   unsigned *out);
	int (*bo_map)(struct kms_bo *bo, void **out);
	int (*bo_unmap)(struct kms_bo *bo);
	int (*bo_sync)(struct kms_bo *bo, unsigned int flags);
	int (*bo_destroy)(struct kms_bo *bo);

	int fd;
};

struct kms_bo
{
	struct kms_driver *kms;
	void *ptr;
	size_t size;
	size_t offset;
	size_t pitch;
	unsigned handle;
	unsigned surface_type;
};

struct dumb_bo
{
	struct kms_bo base;
	unsigned map_count;
};

static enum dumb_bo_type dumb_bo_system = DUMB_BO_DMA;
static enum dumb_bo_type dumb_bo_primary = DUMB_BO_DMA;

static inline bool dumb_bo_is_cachead(enum dumb_bo_type type)
{
	if (type == DUMB_BO_DMA_CACHEABLE ||
	    type == DUMB_BO_SYSTEM_CACHEABLE ||
	    type == DUMB_BO_SYSTEM_NONCONTIG_CACHEABLE)
		return true;

	return false;
}

static unsigned int dumb_bo_get_type(const char *option)
{
	if (strcmp( option, "video" ) == 0)
		return DUMB_BO_DMA;
	else
	if (strcmp( option, "video-cache" ) == 0)
		return DUMB_BO_DMA_CACHEABLE;
	else
	if (strcmp( option, "system" ) == 0)
		return DUMB_BO_SYSTEM;
	else
	if (strcmp( option, "system-cache" ) == 0)
		return DUMB_BO_SYSTEM_CACHEABLE;

	return DUMB_BO_DMA;
}

static int
dumb_get_prop(struct kms_driver *kms, unsigned key, unsigned *out)
{
	switch (key) {
	case KMS_BO_TYPE:
		*out = KMS_BO_TYPE_SCANOUT_X8R8G8B8 | KMS_BO_TYPE_CURSOR_64X64_A8R8G8B8;
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int
dumb_destroy(struct kms_driver *kms)
{
	free(kms);
	return 0;
}

static int
dumb_bo_create(struct kms_driver *kms,
		 const unsigned width, const unsigned height,
		 const enum kms_bo_type type, const unsigned *attr,
		 struct kms_bo **out)
{
	struct drm_mode_create_dumb arg;
	struct dumb_bo *bo;
	int i, ret;

	for (i = 0; attr[i]; i += 2) {
		switch (attr[i]) {
		case KMS_WIDTH:
		case KMS_HEIGHT:
			break;
		case KMS_BO_TYPE:
			break;
		default:
			return -EINVAL;
		}
	}

	bo = calloc(1, sizeof(*bo));
	if (!bo)
		return -ENOMEM;

	memset(&arg, 0, sizeof(arg));

	/* All BO_TYPE currently are 32bpp formats */
	arg.bpp = 32;
	arg.width = width;
	arg.height = height;
	arg.flags = type &  CSTF_EXTERNAL ? dumb_bo_primary : dumb_bo_system;

	ret = drmIoctl(kms->fd, DRM_IOCTL_MODE_CREATE_DUMB, &arg);
	if (ret)
		goto err_free;

	bo->base.kms = kms;
	bo->base.handle = arg.handle;
	bo->base.size = arg.size;
	bo->base.pitch = arg.pitch;
	bo->base.surface_type = arg.flags;

	*out = &bo->base;

	return 0;

err_free:
	free(bo);
	return ret;
}

static int
dumb_bo_get_prop(struct kms_bo *bo, unsigned key, unsigned *out)
{
	switch (key) {
	default:
		return -EINVAL;
	}
}

static int
dumb_bo_map(struct kms_bo *_bo, void **out)
{
	struct dumb_bo *bo = (struct dumb_bo *)_bo;
	struct drm_mode_map_dumb arg;
	void *map = NULL;
	int ret;

	if (bo->base.ptr) {
		bo->map_count++;
		*out = bo->base.ptr;
		return 0;
	}

	memset(&arg, 0, sizeof(arg));
	arg.handle = bo->base.handle;

	ret = drmIoctl(bo->base.kms->fd, DRM_IOCTL_MODE_MAP_DUMB, &arg);
	if (ret)
		return ret;

	map = drm_mmap(0, bo->base.size, PROT_READ | PROT_WRITE, MAP_SHARED, bo->base.kms->fd, arg.offset);
	if (map == MAP_FAILED)
		return -errno;

	bo->base.ptr = map;
	bo->map_count++;
	*out = bo->base.ptr;

	return 0;
}

static int
dumb_bo_unmap(struct kms_bo *_bo)
{
	struct dumb_bo *bo = (struct dumb_bo *)_bo;
	bo->map_count--;

	return 0;
}

static int
dumb_bo_sync(struct kms_bo *_bo, unsigned int flags)
{
	struct dumb_bo *bo = (struct dumb_bo *)_bo;
	struct drm_mode_user_dumb arg;

	if (!dumb_bo_is_cachead(bo->base.surface_type))
		return 0;
/*
	if (flags != CSAID_CPU)
		return 0;
*/
	arg.handle = bo->base.handle;

	return drmIoctl(bo->base.kms->fd, DRM_IOCTL_USER_DUMB_SYNC, &arg);
}

static int
dumb_bo_destroy(struct kms_bo *_bo)
{
	struct dumb_bo *bo = (struct dumb_bo *)_bo;
	struct drm_mode_destroy_dumb arg;
	int ret;

	if (bo->base.ptr) {
		/* XXX Sanity check map_count */
		drm_munmap(bo->base.ptr, bo->base.size);
		bo->base.ptr = NULL;
	}

	memset(&arg, 0, sizeof(arg));
	arg.handle = bo->base.handle;

	ret = drmIoctl(bo->base.kms->fd, DRM_IOCTL_MODE_DESTROY_DUMB, &arg);
	if (ret)
		return -errno;

	free(bo);
	return 0;
}

static int
dumb_create(int fd, struct kms_driver **out)
{
	struct kms_driver *kms;
     	char *option = NULL;
	int ret;
	u64 cap = 0;

	ret = drmGetCap(fd, DRM_CAP_DUMB_BUFFER, &cap);
	if (ret || cap == 0)
		return -EINVAL;

	kms = calloc(1, sizeof(*kms));
	if (!kms)
		return -ENOMEM;

	kms->fd = fd;

	kms->bo_create = dumb_bo_create;
	kms->bo_map = dumb_bo_map;
	kms->bo_unmap = dumb_bo_unmap;
	kms->bo_sync = dumb_bo_sync;
	kms->bo_get_prop = dumb_bo_get_prop;
	kms->bo_destroy = dumb_bo_destroy;
	kms->get_prop = dumb_get_prop;
	kms->destroy = dumb_destroy;
	*out = kms;

	if (direct_config_get("drmkms-buffer-system", &option, 1, &ret) == DR_OK)
		dumb_bo_system = dumb_bo_get_type(option);

	if (direct_config_get("drmkms-buffer-primary", &option, 1, &ret) == DR_OK)
		dumb_bo_primary = dumb_bo_get_type(option);

	D_INFO("DRMKMS/Dumb: primary:%s, system:%s\n",
		dumb_bo_type_name[dumb_bo_primary], dumb_bo_type_name[dumb_bo_system]);

	return 0;
}

drm_public int kms_create(int fd, struct kms_driver **out)
{
	return dumb_create(fd, out);
}

drm_public int kms_get_prop(struct kms_driver *kms, unsigned key, unsigned *out)
{
	switch (key) {
	case KMS_BO_TYPE:
		break;
	default:
		return -EINVAL;
	}
	return kms->get_prop(kms, key, out);
}

drm_public int kms_destroy(struct kms_driver **kms)
{
	if (!(*kms))
		return 0;

	free(*kms);
	*kms = NULL;
	return 0;
}

drm_public int kms_bo_create(struct kms_driver *kms, const unsigned *attr, struct kms_bo **out)
{
	unsigned width = 0;
	unsigned height = 0;
	enum kms_bo_type type = KMS_BO_TYPE_SCANOUT_X8R8G8B8;
	int i;

	for (i = 0; attr[i];) {
		unsigned key = attr[i++];
		unsigned value = attr[i++];

		switch (key) {
		case KMS_WIDTH:
			width = value;
			break;
		case KMS_HEIGHT:
			height = value;
			break;
		case KMS_BO_TYPE:
			type = value;
			break;
		default:
			return -EINVAL;
		}
	}

	if (width == 0 || height == 0)
		return -EINVAL;

	return kms->bo_create(kms, width, height, type, attr, out);
}

drm_public int kms_bo_get_prop(struct kms_bo *bo, unsigned key, unsigned *out)
{
	switch (key) {
	case KMS_PITCH:
		*out = bo->pitch;
		break;
	case KMS_HANDLE:
		*out = bo->handle;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

drm_public int kms_bo_map(struct kms_bo *bo, void **out)
{
	return bo->kms->bo_map(bo, out);
}

drm_public int kms_bo_unmap(struct kms_bo *bo)
{
	return bo->kms->bo_unmap(bo);
}

drm_public int kms_bo_sync(struct kms_bo *bo, unsigned int flags)
{
	return bo->kms->bo_sync(bo, flags);
}

drm_public int kms_bo_destroy(struct kms_bo **bo)
{
	int ret;

	if (!(*bo))
		return 0;

	ret = (*bo)->kms->bo_destroy(*bo);
	if (ret)
		return ret;

	*bo = NULL;
	return 0;
}
