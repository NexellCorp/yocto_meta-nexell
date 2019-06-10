DESCRIPTION = "Nexell Make User Data Filesystem Image For NXP3220"
SECTION = "kernel"
LICENSE = "GPLv2+"

inherit nexell-mkimage

do_deploy() {
	make_image_dataimg ${DEPLOY_DIR_IMAGE}
}
addtask deploy after do_package

# not execute tasks
do_packagedata[noexec] = "1"
do_package_qa[noexec] = "1"
do_package_write_rpm[noexec] = "1"
do_prepare_recipe_sysroot[noexec] = "1"
do_populate_sysroot[noexec] = "1"

# always run task
do_deploy[nostamp] = "1"
