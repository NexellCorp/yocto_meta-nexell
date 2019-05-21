# BIN -> BIN.raw (Binary + NSIH)
do_bingen_raw () {
        local INDEX=$1 BIN=$2 NSIH=$3
	local BOOTKEY=$4 USERKEY=$5 LOADADDR=$6

	if [ ! -f $BIN ]; then
		echo "WARNING: NOT exist '$BIN' for raw"
		return
	fi

        ${TOOL_BINGEN} -k $INDEX -n $NSIH -i $BIN \
		-b $BOOTKEY -u $USERKEY -l $LOADADDR -s $LOADADDR -t;
}

# BIN -> BIN.enc (Encrypted Binary)
do_bingen_enc () {
	local BIN=$1 AESKEY=$2 AESVECTOR=$3 BLOCK_BIT=$4

	if [ ! -f $BIN ]; then
		echo "WARNING: NOT exist '$BIN' for encrypt"
		return
	fi

        ${TOOL_BINENC} -n $BIN -k $(cat $AESKEY) -v $(cat $AESVECTOR) \
		-m enc -b $BLOCK_BIT;
}

# BIN -> BIN.ecc (Binary + ECC per PAGE_SIZE))
do_bingen_ecc () {
	local BIN=$1 PAGE_SIZE=$2

	if [ ! -f $BIN ]; then
		echo "WARNING: NOT exist '$BIN' for ECC"
		return
	fi

        ${TOOL_BINECC} -p $PAGE_SIZE -i $BIN;
}
