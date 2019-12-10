#!/bin/bash
#
# make swu cpio image

SWU_IMAGE_PREFIX="nxp3220"
SWU_IMAGE_VERSION="1.0"
SWU_IMAGE_NAME=${SWU_IMAGE_PREFIX}-${SWU_IMAGE_VERSION}.swu
SWU_SW_DESCRIPTION="sw-description"
SWU_RESULT_DIR="."

# for encryption
SWU_SIGN_PRIVATE="swu.private.key"
SWU_SIGN_PUBLIC="swu.public.key"
SWU_SIGN_DESCRIPTION="sw-description.sig"
SWU_SIGN_HASH_TOOL="swu_hash.py"

SWU_UPDATE_TARGET=()
SWU_SIGN_UPDATE_TARGET=()

function usage()
{
	echo "usage: `basename $0` [OPTION....]"
	echo ""
	echo " refer to : http://sbabic.github.io/swupdate/"
	echo " [General Option]"
	echo "  -f $SWU_SW_DESCRIPTION file"
	echo "  -d result directory for images"
	echo "  -o swupdate out image name (default: $SWU_IMAGE_NAME)"
	echo "  -e open file with vim with option '-f'"
	echo "  -h help"
        echo ""
        echo " [Signing Option]"
	echo "  -s enable signing if not set '-p' use sha256"
	echo "  -p password for singing (aes256)"
	echo "  -k use private key file to encrypt '$SWU_SW_DESCRIPTION -> $SWU_SW_DESCRIPTION.sig'"
	echo "   note> if not set private key, Generate new private and public key files"
	echo "         ($SWU_SIGN_PRIVATE/$SWU_SIGN_PUBLIC)"
	echo "  -m singing mode (pkcs/pss/cms) default pkcs"
	echo "  -g Generate new private and public key files ($SWU_SIGN_PRIVATE/$SWU_SIGN_PUBLIC)"
	echo ""
	echo " SWU Archive contents (cpio):"
	echo "     #> cpio -it -F $SWU_IMAGE_NAME"
	echo " SWU Extract (cpio):"
	echo "     #> cpio -idmv -F $SWU_IMAGE_NAME"
	exit 1;
}

function swu_parse_elements () {
	local swdesc=$1

	if [[ ! -f $swdesc ]]; then
		echo -e "\033[47;31m Not found '$SWU_SW_DESCRIPTION : '$swdesc'\033[0m"
		exit 1;
	fi

	# copy to sw-description
	[ -f $SWU_SW_DESCRIPTION ] && rm $SWU_SW_DESCRIPTION;

	cp -f $swdesc $SWU_SW_DESCRIPTION

	# first image must be sw-description file
	SWU_UPDATE_TARGET=( $(basename $SWU_SW_DESCRIPTION) )

	echo -e "\033[0;33m------------------------------------------------------------------ \033[0m"
	echo -e "\033[0;33m IMAGES: $SWU_RESULT_DIR \033[0m"

	echo " ${SWU_UPDATE_TARGET[0]}"

	while IFS='{', read -ra i;
	do
		[[ $i != *filename* ]] && continue;
		file=$(echo $i | grep "filename" | awk -F'"' '{print $2}')
		if [[ ! -f $SWU_RESULT_DIR/$file ]]; then
			echo -e "\033[47;31m Not found: '$SWU_RESULT_DIR/$file'\033[0m"
			exit 1;
		fi

		SWU_UPDATE_TARGET+=("$file")
		echo " $file"
	done < $swdesc
}

function swu_make_hash () {
	local swdesc=$1

	[ $SWU_OPT_SIGN_ENABLE == false ] && return;
        [[ -z $swdesc ]] && return;

	echo -e "\033[0;33m------------------------------------------------------------------ \033[0m"
	echo -e "\033[0;33m HASH: $swdesc \033[0m"

	while IFS='{', read -ra i;
	do
		[[ $i != *filename* ]] && continue;
		file=$(echo $i | grep "filename" | awk -F'"' '{print $2}')
		file=$SWU_RESULT_DIR/$file
		if [[ ! -f $file ]]; then
			echo -e "\033[47;31m Not found: '$file'\033[0m"
			exit 1;
		fi

		search="sha256"
		value=${i#*$search}
		value="@"$(echo $value | cut -d'"' -f 2)

		# get hash value
		# sha256="$($SWU_RESULT_DIR/$SWU_SIGN_HASH_TOOL $file 2>&1 >/dev/null)"
		sha256="$(openssl dgst -sha256 $file | grep -o '[[:xdigit:]][[:xdigit:]]*$')"

		# replace hash: sha256 = "@file" to sha256 = "<sha256 value>"
		sed -i "s,$value,$sha256," $swdesc
		echo " $value -> $sha256"
	done < $swdesc
}

function swu_make_sign_key () {
	if [ ! -z $SWU_OPT_SIGN_PASSWD ]; then
	        openssl genrsa -passout pass:$SWU_OPT_SIGN_PASSWD -aes256 -out $SWU_SIGN_PRIVATE
        	[ $? -ne 0 ] && exit 1;
		echo -e "\033[0;33m AES256 PRIVATE: $SWU_SIGN_PRIVATE \033[0m"

        	# export public key
	        openssl rsa -passin pass:$SWU_OPT_SIGN_PASSWD -in $SWU_SIGN_PRIVATE -out $SWU_SIGN_PUBLIC -outform PEM -pubout
	        [ $? -ne 0 ] && exit 1;
		echo -e "\033[0;33m AES256 PUBLIC : $SWU_SIGN_PUBLIC \033[0m"
	else
	        openssl genrsa -out $SWU_SIGN_PRIVATE
        	[ $? -ne 0 ] && exit 1;
		echo -e "\033[0;33m SHA256 PRIVATE: $SWU_SIGN_PRIVATE \033[0m"

        	# export public key
	        openssl rsa -in $SWU_SIGN_PRIVATE -out $SWU_SIGN_PUBLIC -outform PEM -pubout
	        [ $? -ne 0 ] && exit 1;
		echo -e "\033[0;33m SHA256 PUBLIC : $SWU_SIGN_PUBLIC \033[0m"
	fi
}

function swu_make_sign_rsa () {
	local swdesc=$1 mode=$2 privkey=$3
	local keygen=false

        [[ -z $privkey ]] && keygen=true;

        # generate private key
	[[ $keygen == true ]] && swu_make_sign_key;
        [[ -n $privkey ]] && SWU_SIGN_PRIVATE=$privkey;

	if [[ ! -f $SWU_SIGN_PRIVATE ]]; then
		echo -e "\033[47;31m Not found signing private key: '$SWU_SIGN_PRIVATE\033[0m"
		exit 1;
	fi

        # signing sw-description
        if [[ $mode == "pkcs" ]]; then
		if [ ! -z $SWU_OPT_SIGN_PASSWD ]; then
			echo "PKCS: aes256"
                	openssl dgst -passin pass:$SWU_OPT_SIGN_PASSWD \
				-sha256 -sign $SWU_SIGN_PRIVATE $swdesc > $SWU_SIGN_DESCRIPTION
		else
			echo "PKCS: sha256"
                	openssl dgst \
				-sha256 -sign $SWU_SIGN_PRIVATE $swdesc > $SWU_SIGN_DESCRIPTION
		fi
        elif [[ $mode == "pss" ]]; then
		if [ ! -z $SWU_OPT_SIGN_PASSWD ]; then
			echo "PSS : aes256"
                	openssl dgst -passin pass:$SWU_OPT_SIGN_PASSWD \
				-sha256 -sign $SWU_SIGN_PRIVATE -sigopt rsa_padding_mode:pss \
                        	-sigopt rsa_pss_saltlen:-2 $swdesc > $SWU_SIGN_DESCRIPTION
		else
			echo "PSS : sha256"
	                openssl dgst \
				-sha256 -sign $SWU_SIGN_PRIVATE -sigopt rsa_padding_mode:pss \
        	                -sigopt rsa_pss_saltlen:-2 $swdesc > $SWU_SIGN_DESCRIPTION
		fi
        fi
        [ $? -ne 0 ] && exit 1;

	echo "$SWU_SIGN_PRIVATE"
	echo "$SWU_SIGN_PUBLIC"
}

function swu_make_sign () {
	local swdesc=$1 mode=$2
	local privkey=$3
	local count=0

	[ $SWU_OPT_SIGN_ENABLE == false ] && return;
        [[ -z $swdesc ]] && return;

	echo -e "\033[0;33m------------------------------------------------------------------ \033[0m"
	echo -e "\033[0;33m SIGNING: $swdesc -> $SWU_SIGN_DESCRIPTION \033[0m"

        case "$mode" in
        "pkcs") swu_make_sign_rsa "$swdesc" "pkcs" "$privkey";;
        "pss" ) swu_make_sign_rsa "$swdesc" "pss"  "$privkey";;
        "cms" | *)
		echo -e "\033[47;31m Not support CMS singing !!!\033[0m"
                exit 1;;
        esac

	SWU_SIGN_UPDATE_TARGET=( $(basename $SWU_SW_DESCRIPTION)  $(basename $SWU_SIGN_DESCRIPTION) )
	for i in ${SWU_UPDATE_TARGET[@]}; do
		((count++))
		# skip sw-description, sw-description.sig
		[ $count -lt 2 ] && continue;
		SWU_SIGN_UPDATE_TARGET+=("$i")
	done
}

function swu_make_archive () {
	local output=$1;
	local target=()

	cd $SWU_RESULT_DIR
	echo -e "\033[0;33m================================================================== \033[0m"
	echo -e "\033[0;33m ARCHIVE: $(pwd) \033[0m"

	if [ $SWU_OPT_SIGN_ENABLE == false ]; then
		target=(${SWU_UPDATE_TARGET[@]})
	else
		target=(${SWU_SIGN_UPDATE_TARGET[@]})
	fi

	for i in ${target[@]}; do
		echo $i;
	done | cpio -ov -H crc > $output

	echo -e "\033[0;33m------------------------------------------------------------------ \033[0m"
	echo -e "\033[0;33m DONE: $(pwd)/$output \033[0m"
	echo -e "\033[0;33m================================================================== \033[0m"
}

SWU_OPT_SW_DESCRIPTION=""
SWU_OPT_IMAGE_NAME=""
SWU_OPT_RESULT_DIR=""
SWU_OPT_FILE_EDIT=false
SWU_OPT_SIGN_ENABLE=false
SWU_OPT_SIGN_PASSWD=""
SWU_OPT_SIGN_PRIV_KEY=""
SWU_OPT_SIGN_MODE="pkcs"
SWU_OPT_SIGN_KEY_GEN=false

[ $# -eq 0 ] && usage;

while getopts 'hf:o:d:sp:k:m:ge' opt
do
        case $opt in
	f ) SWU_OPT_SW_DESCRIPTION=$OPTARG;;
	o ) SWU_OPT_IMAGE_NAME=$OPTARG;;
	d ) SWU_OPT_RESULT_DIR=$OPTARG;;
	s ) SWU_OPT_SIGN_ENABLE=true;;
	p ) SWU_OPT_SIGN_ENABLE=true; SWU_OPT_SIGN_PASSWD=$OPTARG;;
	k ) SWU_OPT_SIGN_ENABLE=true; SWU_OPT_SIGN_PRIV_KEY=$OPTARG;;
	m ) SWU_OPT_SIGN_MODE=$OPTARG;;
	g ) SWU_OPT_SIGN_KEY_GEN=true;;
	e ) SWU_OPT_FILE_EDIT=true;;
	h | *)
		usage;;
	esac
done

if [ $SWU_OPT_SIGN_KEY_GEN == true ]; then
	swu_make_sign_key
	exit 0;
fi

[[ -n $SWU_OPT_IMAGE_NAME ]] && SWU_IMAGE_NAME=$SWU_OPT_IMAGE_NAME;
if [[ -n $SWU_OPT_RESULT_DIR ]]; then
	SWU_RESULT_DIR=$SWU_OPT_RESULT_DIR;
	SWU_SIGN_PRIVATE=$SWU_RESULT_DIR/$SWU_SIGN_PRIVATE
	SWU_SIGN_PUBLIC=$SWU_RESULT_DIR/$SWU_SIGN_PUBLIC
	SWU_SIGN_DESCRIPTION=$SWU_RESULT_DIR/$SWU_SIGN_DESCRIPTION
fi

if [[ -z $SWU_OPT_SW_DESCRIPTION ]]; then
	echo -e "\033[47;31m No input $SWU_SW_DESCRIPTION file !!!\033[0m"
	usage;
fi

if [ ! -f $SWU_OPT_SW_DESCRIPTION ]; then
	echo -e "\033[47;31m Not such $SWU_SW_DESCRIPTION: $SWU_OPT_SW_DESCRIPTION !!! \033[0m"
	exit 1
fi

if [ $SWU_OPT_FILE_EDIT == true ]; then
	vim $SWU_OPT_SW_DESCRIPTION
	exit 0;
fi

SWU_SW_DESCRIPTION=$SWU_RESULT_DIR/$SWU_SW_DESCRIPTION

swu_parse_elements $SWU_OPT_SW_DESCRIPTION
swu_make_hash $SWU_SW_DESCRIPTION
swu_make_sign "$SWU_SW_DESCRIPTION" "$SWU_OPT_SIGN_MODE" "$SWU_OPT_SIGN_PRIV_KEY"
swu_make_archive $SWU_IMAGE_NAME
