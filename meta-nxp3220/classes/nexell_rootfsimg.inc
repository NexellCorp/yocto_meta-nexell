inherit classes/nexell-mkimage.bbclass

do_rootfs_image() {
	make_ext4_rootfsimg ${DEPLOY_DIR_IMAGE}/${PN}-${MACHINE}
}
addtask rootfs_image after do_image_complete before do_build

# always run task
do_rootfs_image[nostamp] = "1"
