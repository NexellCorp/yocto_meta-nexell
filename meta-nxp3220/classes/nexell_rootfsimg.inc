inherit nexell-mkimage

do_rootfs_image() {
	make_image_rootfsimg ${DEPLOY_DIR_IMAGE}/${PN}-${MACHINE}
}
addtask rootfs_image after do_image_complete before do_build

# always run task
do_rootfs_image[nostamp] = "1"

# run do_image
IMAGE_PREPROCESS_COMMAND += "make_extra_rootfs; "

# always run task
do_image[nostamp] = "1"
