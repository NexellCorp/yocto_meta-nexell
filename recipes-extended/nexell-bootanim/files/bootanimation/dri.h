#ifndef _DRI_H_
#define _DRI_H_

#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <drm/drm.h>
#include <drm/drm_mode.h>
#include <xf86drmMode.h>

typedef struct dri dri_t;
typedef struct dri_crtc dri_crtc_t;
typedef struct dri_plane_prop dri_plane_prop_t;
typedef struct dri_plane dri_plane_t;
typedef struct dri_framebuffer dri_framebuffer_t;
typedef struct dri_encoder dri_encoder_t;
typedef struct dri_connector dri_connector_t;

/**
 * struct dri:
 */
struct dri {
	int fd;
	dri_connector_t **connectors;
	int num_connectors;
	struct dri_encoder **encoders;
	int num_encoders;
	struct dri_crtc **crtcs;
	int num_crtcs;
	struct dri_plane **planes;
	int num_planes;
};

/**
 * struct dri_crtc:
 */
struct dri_crtc {
	dri_t *dri;
	uint32_t id;
};

/**
 * struct dri_plane_prop:
 */
struct dri_plane_prop {
	uint32_t prop_id;
	char name[32];
	uint64_t value;
};

/**
 * struct dri_plane:
 */
struct dri_plane {
	dri_t *dri;
	dri_crtc_t *crtc;
	unsigned int type;
	uint32_t id;
	uint32_t *formats;
	unsigned int num_formats;
	dri_plane_prop_t prop[64];
	unsigned int num_props;
};

/**
 * struct dri_framebuffer:
 */
struct dri_framebuffer {
	dri_t *dri;
	uint32_t id;
	int width;
	int height;
	uint32_t format;
	bool seperated;
	int planes;
	uint32_t handles[4];
	int pitches[4];
	uint32_t offsets[4];
	size_t sizes[4];
	void *ptrs[4];
};

/**
 * struct dri_encoder:
 */
struct dri_encoder {
	dri_t *dri;
	char *name;
	uint32_t id;
	uint32_t type;
	uint32_t crtc_id;
	uint32_t possible_crtcs;
	uint32_t possible_clones;
	/* save drmModeEncoder */
	drmModeEncoder encoder;
};

/**
 * struct dri_connector:
 */
struct dri_connector {
	dri_t *dri;
	bool connected;
	uint32_t type;
	uint32_t id;
	int width;
	int height;
	char *name;
	drmModeModeInfo mode;
	/* save drmModeConnector */
	drmModeConnector connector;
};

#define DRM_MODE_OBJECT_PLANE 0xeeeeeeee

/**
 * ARRAY_SIZE:
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

/**
 * dri extern
 */
const char *dri_lookup_connector_type_name(unsigned int type);
dri_connector_t *dri_connector_create(dri_t *dri, uint32_t id);
void dri_connector_free(dri_connector_t *connector);
int dri_connectors_probe(dri_t *dri);
void dri_connectors_free(dri_t *dri);

const char *dri_lookup_encoder_type_name(unsigned int type);
dri_encoder_t *dri_encoder_create(dri_t *dri, uint32_t id);
void dri_encoder_free(dri_encoder_t *encoder);
int dri_encoders_probe(dri_t *dri);
void dri_free_encoders(dri_t *dri);

dri_crtc_t *dri_crtc_create(dri_t *dri, uint32_t id);
void dri_crtc_free(dri_crtc_t *crtc);
int dri_crtcs_probe(dri_t *dri);
void dri_crtcs_free(dri_t *dri);

dri_plane_t *dri_plane_create(dri_t *dri, uint32_t id);
void dri_plane_free(dri_plane_t *plane);
int dri_planes_probe(dri_t *dri);
void dri_planes_free(dri_t *dri);

dri_t *dri_initialize(const char *devname);
void dri_release(dri_t *dri);
#endif