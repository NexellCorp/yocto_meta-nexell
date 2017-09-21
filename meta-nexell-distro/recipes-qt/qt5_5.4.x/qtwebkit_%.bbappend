require qt5-nexell.inc

SRC_URI[md5sum] = "f84f6c672b09633310c9f198b70b951a"
SRC_URI[sha256sum] = "56dd27d308434ca4d854aa3f7c5cbb54c0d10621dd035985e0ae3708566fed2f"

SRC_URI += " \
    file://0001-qtwebkit-fix-QA-issue-bad-RPATH.patch \
    file://0002-Remove-TEXTREL-tag-in-x86.patch \
"
