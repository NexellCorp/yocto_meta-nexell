# Generate Nexell Images
#

inherit nexell-mkimage

#
# Generate rootfs image
#
do_rootfs_image() {
	make_rootfs_image ${DEPLOY_DIR_IMAGE}/${PN}-${MACHINE} ${PART_ROOTFS_IMAGE} ${PART_ROOTFS_SIZE}
}
addtask rootfs_image after do_image_complete before do_build

# always run task
do_rootfs_image[nostamp] = "1"

# run do_image
IMAGE_PREPROCESS_COMMAND += "make_rootfs_extra; "

# always run task
do_image[nostamp] = "1"

#
# Generate rootfs image
#
do_data_image() {
	make_data_image
}
addtask data_image after do_image_complete before do_build

# always run task
do_data_image[nostamp] = "1"

#
# Generate misc image
#
do_misc_image() {
	make_misc_image
}
addtask misc_image after do_image_complete before do_build

# always run task
do_misc_image[nostamp] = "1"
