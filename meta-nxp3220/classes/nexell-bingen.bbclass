DEPENDS += "openssl-native"

# BIN -> BIN.raw (Binary + NSIH)
do_bingen_raw () {
        local index=$1 binary=$2 nsih=$3
	local bootkey=$4 userkey=$5 loadaddr=$6
	local opt=$7

	if [ ! -f $binary ]; then
		echo "WARNING: NOT exist '$binary' for raw"
		return
	fi

        ${TOOL_BINGEN} -k $index -n $nsih -i $binary \
		-b $bootkey -u $userkey -l $loadaddr -s $loadaddr -t $opt;
}

# BIN -> BIN.enc (Encrypted Binary)
do_bingen_enc () {
	local binary=$1 enckey=$2 ivector=$3

	if [ ! -f $binary ]; then
		echo "WARNING: NOT exist '$binary' for encrypt"
		return
	fi

	${TOOL_BINENC} enc -e -nosalt -aes-128-cbc -in ${binary} -out ${binary}.enc \
                -K $(cat ${enckey}) -iv $(cat $ivector)
}

# BIN -> BIN.ecc (Binary + ECC per PAGE_SIZE))
do_bingen_ecc () {
	local binary=$1 page_size=$2
	local psize=$page_size

	if [ ! -f $binary ]; then
		echo "WARNING: NOT exist '$binary' for ECC"
		return
	fi

	if [ $page_size -gt 512 ]; then
		psize=1024
	fi

        ${TOOL_BINECC} -p $psize -i $binary;
}
