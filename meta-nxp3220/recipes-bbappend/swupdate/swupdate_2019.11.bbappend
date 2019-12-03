### For SWUpdate
FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI_append = " \
	file://0001-2019.11-adds-android-sparse-image-and-disk-partition.patch \
	file://0004-2019.11-add-reset-mode-option-for-progress.patch \
	"
