### For SWUpdate
FILESEXTRAPATHS_append := ":${THISDIR}/files"

SRC_URI += " \
    file://0001-git-adds-android-sparse-image-and-disk-partition.patch \
    file://0002-git-skip-check-hash-for-disk-partition.patch \
    file://0003-git-remove-depend-hash-verify-and-singed-image.patch \
    file://0004-git-add-remove-mode-option-for-progress.patch \
    "
