SRC_URI = "http://www.openssl.org/source/openssl-${PV}.tar.gz \
			file://run-ptest \
			file://openssl-c_rehash.sh \
			file://0001-skip-test_symbol_presence.patch \
			file://0001-buildinfo-strip-sysroot-and-debug-prefix-map-from-co.patch \
			file://afalg.patch \
			file://CVE-2019-1543.patch \
			"
