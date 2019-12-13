# BIN -> BIN.raw (Binary + NSIH)
do_bingen_raw () {
        local index=$1 binary=$2 nsih=$3
	local bootkey=$4 userkey=$5 loadaddr=$6

	if [ ! -f $binary ]; then
		echo "WARNING: NOT exist '$binary' for raw"
		return
	fi

        ${TOOL_BINGEN} -k $index -n $nsih -i $binary \
		-b $bootkey -u $userkey -l $loadaddr -s $loadaddr -t;
}

# BIN -> BIN.enc (Encrypted Binary)
do_bingen_enc () {
	local binary=$1 aeskey=$2 aesvector=$3 block_bit=$4

	if [ ! -f $binary ]; then
		echo "WARNING: NOT exist '$binary' for encrypt"
		return
	fi

        ${TOOL_BINENC} -n $binary -k $(cat $aeskey) -v $(cat $aesvector) \
		-m enc -b $block_bit;
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
