# BIN -> BIN.raw (Binary + NSIH)
do_bingen_raw () {
        local INDEX=$1 BIN=$2 NSIH=$3
	local BOOTKEY=$4 USERKEY=$5 LOADADDR=$6

        ${TOOL_BINGEN} -k ${INDEX} -n ${NSIH} -i ${BIN} \
		-b ${BOOTKEY} -u ${USERKEY} -l ${LOADADDR} -s ${LOADADDR} -t;
}

# BIN -> BIN.enc (Encrypted Binary)
do_bingen_enc () {
	local BIN=$1 AESKEY=$2 AESVECTOR=$3 BLOCK_BIT=$4

        ${TOOL_BINENC} -n ${BIN} -k $(cat ${AESKEY}) -v $(cat ${AESVECTOR}) \
		-m enc -b ${BLOCK_BIT};
}

# BIN -> BIN.ecc (Binary + ECC per PAGE_SIZE))
do_bingen_ecc () {
	local BIN=$1 PAGE_SIZE=$2

        ${TOOL_BINECC} -p ${PAGE_SIZE} -i ${BIN};
}
