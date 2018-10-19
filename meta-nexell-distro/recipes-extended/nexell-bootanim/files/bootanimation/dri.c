#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "dri.h"

/**
 * fatal:
 */
#define fatal(format, ...) \
	do { \
    		fprintf(stderr, "Fatal: "); \
		fprintf(stderr, format, ##__VA_ARGS__); \
    		fprintf(stderr, "\n"); \
    		exit(127); \
  	} while (0)

/**
 * error:
 */
#define error(format, ...) \
	do { \
    		fprintf(stderr, "Error: [%s:%d]", __FUNCTION__, __LINE__); \
		fprintf(stderr, format, ##__VA_ARGS__); \
    		fprintf(stderr, "\n"); \
  	} while (0)

/**
 * debug:
 */
#define debug(format, ...) \
	do { \
    		fprintf(stderr, "Debug: "); \
		fprintf(stderr, format, ##__VA_ARGS__); \
    		fprintf(stderr, "\n"); \
  	} while (0)


struct type_name {
	unsigned int type;
	const char *name;
} type_name_t;

static const struct type_name connector_type_names[] = {
	{ DRM_MODE_CONNECTOR_Unknown, "unknown" },
	{ DRM_MODE_CONNECTOR_VGA, "VGA" },
	{ DRM_MODE_CONNECTOR_DVII, "DVI-I" },
	{ DRM_MODE_CONNECTOR_DVID, "DVI-D" },
	{ DRM_MODE_CONNECTOR_DVIA, "DVI-A" },
	{ DRM_MODE_CONNECTOR_Composite, "composite" },
	{ DRM_MODE_CONNECTOR_SVIDEO, "s-video" },
	{ DRM_MODE_CONNECTOR_LVDS, "LVDS" },
	{ DRM_MODE_CONNECTOR_Component, "component" },
	{ DRM_MODE_CONNECTOR_9PinDIN, "9-pin DIN" },
	{ DRM_MODE_CONNECTOR_DisplayPort, "DP" },
	{ DRM_MODE_CONNECTOR_HDMIA, "HDMI-A" },
	{ DRM_MODE_CONNECTOR_HDMIB, "HDMI-B" },
	{ DRM_MODE_CONNECTOR_TV, "TV" },
	{ DRM_MODE_CONNECTOR_eDP, "eDP" },
	{ DRM_MODE_CONNECTOR_VIRTUAL, "Virtual" },
	{ DRM_MODE_CONNECTOR_DSI, "DSI" },
};

static const char *const connector_names[] = {
	[DRM_MODE_CONNECTOR_Unknown] = "unknown",
	[DRM_MODE_CONNECTOR_VGA] = "VGA",
	[DRM_MODE_CONNECTOR_DVII] = "DVI-I",
	[DRM_MODE_CONNECTOR_DVID] = "DVI-D",
	[DRM_MODE_CONNECTOR_DVIA] = "DVI-A",
	[DRM_MODE_CONNECTOR_Composite] = "composite",
	[DRM_MODE_CONNECTOR_SVIDEO] = "s-video",
	[DRM_MODE_CONNECTOR_LVDS] = "LVDS",
	[DRM_MODE_CONNECTOR_Component] = "component",
	[DRM_MODE_CONNECTOR_9PinDIN] = "9-pin DIN",
	[DRM_MODE_CONNECTOR_DisplayPort] = "DP",
	[DRM_MODE_CONNECTOR_HDMIA] = "HDMI-A",
	[DRM_MODE_CONNECTOR_HDMIB] = "HDMI-B",
	[DRM_MODE_CONNECTOR_TV] = "TV",
	[DRM_MODE_CONNECTOR_eDP] = "eDP",
	[DRM_MODE_CONNECTOR_VIRTUAL] = "Virtual",
	[DRM_MODE_CONNECTOR_DSI] = "DSI",
};

static const char *util_lookup_type_name(unsigned int type,
			const struct type_name *table,
			int count)
{
	int i;

	for (i = 0; i < count; i++) {
		debug("[%d.%d] type %d:%d %s",
			count, i, type, table[i].type, table[i].name);
		if (table[i].type == type)
			return table[i].name;
	}

	return NULL;
}

/**
 * dri_lookup_connector_type_name:
 */
const char *dri_lookup_connector_type_name(unsigned int type)
{
	return util_lookup_type_name(type, connector_type_names,
			ARRAY_SIZE(connector_type_names));
}

static int connector_probe(dri_connector_t *connector)
{
	drmModeConnectorPtr con;
	dri_t *dri = connector->dri;
	const char *type;
	int len;

	con = drmModeGetConnector(dri->fd, connector->id);
	if (!con) {
		error("drmModeGetConnector");
		return -ENOENT;
	}
	connector->type = con->connector_type;

	if (con->connection == DRM_MODE_CONNECTED)
		connector->connected = true;
	else
		connector->connected = false;

	debug("connector %u (%s-%u)\n", con->connector_id,
		dri_lookup_connector_type_name(con->connector_type),
		con->connector_type_id);
#if 0
	if (false == connector->connected) {
		error("ignoring unused connector %u",
			con->connector_id);
		return -ENOENT;
	}

	if (con->count_modes == 0) {
		error("no valid mode for connector %u",
			con->connector_id);
		return -EFAULT;
	}
#endif

	/* name */
	type = connector_names[connector->type];
	len = snprintf(NULL, 0, "%s-%u", type, con->connector_type_id);
	connector->name = malloc(len + 1);
	if (!connector->name)
		goto err_connector_probe;

	snprintf(connector->name, len + 1, "%s-%u", type, con->connector_type_id);

	/* copy */
	if (con->count_modes)
		memcpy(&connector->mode, &con->modes[0], sizeof(drmModeModeInfo));

	memcpy(&connector->connector, con, sizeof(drmModeConnector));

	connector->width = connector->mode.hdisplay;
	connector->height = connector->mode.vdisplay;

err_connector_probe:
	drmModeFreeConnector(con);

	return 0;
}

/**
 * dri_connector_create:
 */
dri_connector_t *dri_connector_create(dri_t *dri, uint32_t id)
{
	dri_connector_t *connector;
	int err;

	connector = calloc(1, sizeof(*connector));
	if (!connector){
		error("calloc for connector create");
		return NULL;
	}

	connector->dri = dri;
	connector->id = id;

	err = connector_probe(connector);
	if (0 > err) {
		free(connector);
		return NULL;
	}

	return connector;
}

/**
 * dri_connector_free:
 */
void dri_connector_free(dri_connector_t *connector)
{
	free(connector->name);
	free(connector);
}

/**
 * dri_connectors_probe:
 */
int dri_connectors_probe(dri_t *dri)
{
	unsigned int counts[ARRAY_SIZE(connector_names)];
	dri_connector_t *connector;
	drmModeRes *res;
	int i;

	memset(counts, 0, sizeof(counts));

	res = drmModeGetResources(dri->fd);
	if (!res) {
		error("drmModeGetResources");
		return -ENOENT;
	}

	dri->connectors = calloc(res->count_connectors, sizeof(connector));
	if (!dri->connectors){
		error("calloc for connectors probe %d",
			res->count_connectors);
		return -ENOMEM;
	}

	debug("connectors: %d", res->count_connectors);

	for (i = 0; i < res->count_connectors; i++) {
		connector = dri_connector_create(dri, res->connectors[i]);
		if (!connector)
			continue;

		dri->connectors[i] = connector;
		dri->num_connectors++;
	}

	drmModeFreeResources(res);

	return 0;
}

/**
 * dri_connectors_free:
 */
void dri_connectors_free(dri_t *dri)
{
	int i;

	assert(dri);

	for (i = 0; i < dri->num_connectors; i++)
		dri_connector_free(dri->connectors[i]);

	free(dri->connectors);
}

static const struct type_name encoder_type_names[] = {
	{ DRM_MODE_ENCODER_NONE, "none" },
	{ DRM_MODE_ENCODER_DAC, "DAC" },
	{ DRM_MODE_ENCODER_TMDS, "TMDS" },
	{ DRM_MODE_ENCODER_LVDS, "LVDS" },
	{ DRM_MODE_ENCODER_TVDAC, "TVDAC" },
	{ DRM_MODE_ENCODER_VIRTUAL, "Virtual" },
	{ DRM_MODE_ENCODER_DSI, "DSI" },
	{ DRM_MODE_ENCODER_DPMST, "DPMST" },
};

static const char *const encoder_names[] = {
	[DRM_MODE_ENCODER_NONE] 	= "none",
	[DRM_MODE_ENCODER_DAC] 		= "DAC",
	[DRM_MODE_ENCODER_TMDS] 	= "TMDS",
	[DRM_MODE_ENCODER_LVDS] 	= "LVDS",
	[DRM_MODE_ENCODER_TVDAC] 	= "TVDAC",
	[DRM_MODE_ENCODER_VIRTUAL] 	= "Virtual",
	[DRM_MODE_ENCODER_DSI] 		= "DSI",
	[DRM_MODE_ENCODER_DPMST] 	= "DPMST",
};

/**
 * dri_lookup_encoder_type_name:
 */
const char *dri_lookup_encoder_type_name(unsigned int type)
{
	return util_lookup_type_name(type, encoder_type_names,
			ARRAY_SIZE(encoder_type_names));
}

static int encoder_probe(dri_encoder_t *encoder)
{
	drmModeEncoderPtr enc;
	dri_t *dri = encoder->dri;
	const char *type;
	int len;

	enc = drmModeGetEncoder(dri->fd, encoder->id);
	if (!enc) {
		error("drmModeGetEncoder");
		return -ENOENT;
	}
	encoder->type = enc->encoder_type;

	debug("encoder %u (%s) crtc %u possible crtcs 0x%x clones 0x%x",
		enc->encoder_id, dri_lookup_encoder_type_name(enc->encoder_type),
		enc->crtc_id, enc->possible_crtcs, enc->possible_clones);

	/* name */
	type = encoder_names[encoder->type];
	len = snprintf(NULL, 0, "%s", type);
	encoder->name = malloc(len + 1);
	if (!encoder->name)
		goto err_encoder_probe;

	snprintf(encoder->name, len + 1, "%s", type);

	/* copy */
	memcpy(&encoder->encoder, enc, sizeof(drmModeEncoder));

	encoder->crtc_id = enc->crtc_id;
	encoder->possible_crtcs = enc->possible_crtcs;
	encoder->possible_clones = enc->possible_clones;

err_encoder_probe:

	drmModeFreeEncoder(enc);

	return 0;
}

/**
 * dri_encoder_create:
 */
dri_encoder_t *dri_encoder_create(dri_t *dri, uint32_t id)
{
	dri_encoder_t *encoder;
	int err;

	encoder = calloc(1, sizeof(*encoder));
	if (!encoder){
		error("calloc for encoder create");
		return NULL;
	}

	encoder->dri = dri;
	encoder->id = id;

	err = encoder_probe(encoder);
	if (0 > err) {
		free(encoder);
		return NULL;
	}

	return encoder;
}

/**
 * dri_encoder_free:
 */
void dri_encoder_free(dri_encoder_t *encoder)
{
	free(encoder->name);
	free(encoder);
}

/**
 * dri_encoders_probe:
 */
int dri_encoders_probe(dri_t *dri)
{
	unsigned int counts[ARRAY_SIZE(encoder_names)];
	dri_encoder_t *encoder;
	drmModeRes *res;
	int i;

	memset(counts, 0, sizeof(counts));

	res = drmModeGetResources(dri->fd);
	if (!res) {
		error("drmModeGetResources");
		return -ENOENT;
	}

	dri->encoders = calloc(res->count_encoders, sizeof(encoder));
	if (!dri->encoders){
		error("calloc for count_encoders %d",
			res->count_encoders);
		return -ENOMEM;
	}
	debug("encoders %d", res->count_encoders);

	for (i = 0; i < res->count_encoders; i++) {
		encoder = dri_encoder_create(dri, res->encoders[i]);
		if (!encoder)
			continue;

		dri->encoders[i] = encoder;
		dri->num_encoders++;
	}

	drmModeFreeResources(res);

	return 0;
}

void dri_encoders_free(dri_t *dri)
{
	int i;

	assert(dri);

	for (i = 0; i < dri->num_encoders; i++)
		dri_encoder_free(dri->encoders[i]);

	free(dri->encoders);
}

/**
 * dri_crtc_create:
 */
dri_crtc_t *dri_crtc_create(dri_t *dri, uint32_t id)
{
	dri_crtc_t *crtc;

	crtc = calloc(1, sizeof(*crtc));
	if (!crtc)
		return NULL;

	crtc->dri = dri;
	crtc->id = id;

	return crtc;
}

/**
 * dri_crtc_free:
 */
void dri_crtc_free(dri_crtc_t *crtc)
{
	free(crtc);
}

int dri_crtcs_probe(dri_t *dri)
{
	dri_crtc_t *crtc;
	drmModeRes *res;
	int i;

	res = drmModeGetResources(dri->fd);
	if (!res) {
		error("drmModeGetResources");
		return -ENOENT;
	}

	dri->crtcs = calloc(res->count_crtcs, sizeof(crtc));
	if (!dri->crtcs) {
		error("calloc for count_crtcs %d\n", res->count_crtcs);
		return -ENOMEM;
	}

	debug("crtcs %d", res->count_crtcs);

	for (i = 0; i < res->count_crtcs; i++) {
		crtc = dri_crtc_create(dri, res->crtcs[i]);
		if (!crtc)
			continue;

		dri->crtcs[i] = crtc;
		dri->num_crtcs++;
	}

	drmModeFreeResources(res);

	return 0;
}

void dri_crtcs_free(dri_t *dri)
{
	int i;

	assert(dri);

	for (i = 0; i < dri->num_crtcs; i++)
		dri_crtc_free(dri->crtcs[i]);

	free(dri->crtcs);
}

static int plane_probe(dri_plane_t *plane)
{
	dri_t *dri = plane->dri;
	drmModeObjectPropertiesPtr props;
	drmModePlane *p;
	unsigned int i;

	p = drmModeGetPlane(dri->fd, plane->id);
	if (!p)
		return -ENODEV;

	if (p->crtc_id == 0) {
		for (i = 0; i < dri->num_crtcs; i++) {
			if (p->possible_crtcs & (1 << i)) {
				p->crtc_id = dri->crtcs[i]->id;
				break;
			}
		}
	}

	for (i = 0; i < dri->num_crtcs; i++) {
		if (dri->crtcs[i]->id == p->crtc_id) {
			plane->crtc = dri->crtcs[i];
			break;
		}
	}

	plane->formats = calloc(p->count_formats, sizeof(uint32_t));
	if (!plane->formats)
		return -ENOMEM;

	for (i = 0; i < p->count_formats; i++)
		plane->formats[i] = p->formats[i];

	plane->num_formats = p->count_formats;

	drmModeFreePlane(p);

	props = drmModeObjectGetProperties(dri->fd, plane->id,
					   DRM_MODE_OBJECT_PLANE);
	if (!props)
		return -ENODEV;

	for (i = 0; i < props->count_props; i++) {
		drmModePropertyPtr prop;

		prop = drmModeGetProperty(dri->fd, props->props[i]);
		if (prop) {
			strcpy(plane->prop[i].name, (const char*)prop->name);
			plane->prop[i].value = *prop->values;
			plane->prop[i].prop_id = prop->prop_id;
			plane->num_props = i+1;

			if (strcmp(prop->name, "type") == 0)
				plane->type = props->prop_values[i];

			drmModeFreeProperty(prop);
		}
	}

	drmModeFreeObjectProperties(props);

	return 0;
}

/**
 * dri_plane_create:
 */
dri_plane_t *dri_plane_create(dri_t *dri, uint32_t id)
{
	dri_plane_t *plane;

	plane = calloc(1, sizeof(*plane));
	if (!plane)
		return NULL;

	plane->dri = dri;
	plane->id = id;

	plane_probe(plane);

	return plane;
}

/**
 * dri_plane_free:
 */
void dri_plane_free(dri_plane_t *plane)
{
	free(plane->formats);
	free(plane);
}

static int drm_set_client_cap(int fd, uint64_t capability, uint64_t value)
{
        struct drm_set_client_cap cap = { 0, };

        cap.capability = capability;
        cap.value = value;

        return drmIoctl(fd, DRM_IOCTL_SET_CLIENT_CAP, &cap);
}

/**
 * dri_planes_probe:
 */
int dri_planes_probe(dri_t *dri)
{
	dri_plane_t *plane;
	drmModePlaneRes *res;
	unsigned int i;

	/*
	 * set universal plane to get total planes info
	 * if not set, get only overlay planes info
	 */
	drm_set_client_cap(dri->fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);

	res = drmModeGetPlaneResources(dri->fd);
	if (!res) {
		error("drmModeGetPlaneResources");
		return -ENOENT;
	}

	dri->planes = calloc(res->count_planes, sizeof(plane));
	if (!dri->planes) {
		error("calloc for count_planes %d", res->count_planes);
		return -ENOMEM;
	}

	debug("planes %d", res->count_planes);

	for (i = 0; i < res->count_planes; i++) {
		plane = dri_plane_create(dri, res->planes[i]);
		if (!plane)
			continue;

		dri->planes[i] = plane;
		dri->num_planes++;
	}

	drmModeFreePlaneResources(res);

	return 0;
}

void dri_planes_free(dri_t *dri)
{
	int i;

	assert(dri);

	for (i = 0; i < dri->num_planes; i++)
		dri_plane_free(dri->planes[i]);

	free(dri->planes);
}

static int dri_probe(dri_t *dri)
{
	int ret;

	if ((ret = dri_connectors_probe(dri)) < 0)
		return ret;

	if ((ret = dri_encoders_probe(dri)) < 0)
		return ret;

	if ((ret = dri_crtcs_probe(dri)) < 0)
		return ret;

	if ((ret = dri_planes_probe(dri)) < 0)
		return ret;

	return ret;
}

/**
 * dri_initialize:
 */
dri_t *dri_initialize(const char *devname)
{
	dri_t *dri;
	int file, ret;

	assert(devname);

	file = open(devname, O_RDWR);
  	if (file < 0) {
    		error("Unable to open dri %s", devname);
		return NULL;
	}

	dri = (dri_t *)malloc(sizeof(dri_t));
    	if (!dri) {
        	error("dri malloc %d", sizeof(dri_t));
        	goto cleanup_dri;
	}

	dri->fd = file;

	ret = dri_probe(dri);
	if (ret) {
		error("dri probe");
		goto cleanup_dri;
	}

	return dri;

cleanup_dri:
	close(dri->fd);
	return NULL;
}

/**
 * dri_release:
 */
void dri_release(dri_t *dri)
{
	dri_planes_free(dri);
	dri_crtcs_free(dri);
	dri_encoders_free(dri);
	dri_connectors_free(dri);

	free(dri);
	close(dri->fd);
}
