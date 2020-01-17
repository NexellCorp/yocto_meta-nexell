### For SWUpdate
FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI_append = " \
	file://0001-2019.11-adds-android-sparse-image-and-disk-partition.patch \
	file://0004-2019.11-add-reset-mode-option-for-progress.patch \
	file://0005-2019.11-add-mtd-handler.patch \
	file://0006-2019.11-add-ubi-image-handler.patch \
	file://0007-2019.11-done-progress-status-a-already-installed-images.patch \
	"
