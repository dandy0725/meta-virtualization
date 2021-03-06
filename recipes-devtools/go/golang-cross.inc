DESCRIPTION = "Go Programming Language Cross Compiler."
HOMEPAGE = "golang.org"
# DEPENDS = "virtual/${TARGET_PREFIX}gcc libgcc"
DEPENDS = "libgcc"
PROVIDES = "virtual/${TARGET_PREFIX}golang"
SRC_URI = "\
	http://golang.org/dl/go${PV}.src.tar.gz \
	file://bsd_svid_source.patch \
	"

S="${WORKDIR}/go"

do_compile () {
	export CC_FOR_TARGET="${CC}"
	export CXX_FOR_TARGET="${CXX}"
	# the above should set the right sysroot option, else...:
	# export CGO_CFLAGS="--sysroot=${STAGING_DIR_HOST}"
	# export CGO_LDFLAGS="--sysroot=${STAGING_DIR_HOST}"

	export CC="${BUILD_CC}"
	export GOROOT_FINAL="${libdir}/go"
	export GOARCH="${TARGET_ARCH}"

	# supported amd64, 386, arm
	if [ "${TARGET_ARCH}" = "x86_64" ]; then
		export GOARCH="amd64"
	fi

	if [ "${TARGET_ARCH}" = "arm" ]
	then
		if [ `echo ${TUNE_PKGARCH} | cut -c 1-7` = "cortexa" ]
		then
			echo GOARM 7
			export GOARM="7"
		fi
	fi
	export GOOS="linux"
	export GO_GCFLAGS=""
	export GO_LDFLAGS=""
	export GO_CCFLAGS=""
	export CGO_ENABLED="1"
	#export CC_FOR_TARGET="${TARGET_SYS}-gcc"
	#export CXX_FOR_TARGET="${TARGET_SYS}-g++"
	echo GOARCH ${GOARCH}
	echo CC_FOR_TARGET ${CC_FOR_TARGET}

	cd src
	sh -x ./make.bash
}

do_install () {
	# Install the executables into build system
	mkdir -p ${D}${bindir}
	cp -a bin/go ${D}${bindir}/
	mkdir -p ${D}${libdir}/go
	cp -a pkg ${D}${libdir}/go/
	cp -a include ${D}${libdir}/go/
	cp -a api ${D}${libdir}/go/
	cp -a src ${D}${libdir}/go/
}
