#!/bin/sh

function usage() {
	echo "usage: `basename $0` [-l byte [K|M|G]][-c repeat][-p parall] "
	echo ""
}

LENGTH=512K
REPEAT=10
PARALL=1

while getopts 'hl:c:p:' opt
do
        case $opt in
        l )
		LENGTH=$OPTARG
		;;
        c )
		REPEAT=$OPTARG
		;;
        p )
		PARALL=$OPTARG
		;;
        h | *)
        	usage
		exit 1;;
		esac
done

function convert_hn_to_byte() {
	local val=$1
	local ret=$2 # store calculated byte
	local delmi="" mulitple=0

	case "$val" in
	*K* ) delmi='K'; mulitple=1024;;
	*k* ) delmi='k'; mulitple=1024;;
	*M* ) delmi='M'; mulitple=1048576;;
	*m* ) delmi='m'; mulitple=1048576;;
	*G* ) delmi='G'; mulitple=1073741824;;
	*g* ) delmi='g'; mulitple=1073741824;;
	-- ) ;;
	esac

	if [ ! -z $delmi ]; then
		val=$(echo $val| cut -d$delmi -f 1)
		val=`expr $val \* $mulitple`
		eval "$ret=\"${val}\""
	fi
}

if [ ! -x $BW_MEM_EXEC ]; then
	echo "File '$BW_MEM_EXEC' is not executable or found"
	exit 1
fi

echo "bw_mem   :"
echo "- Length : $LENGTH"
echo "- Repeat : $REPEAT"
echo "- Parall : $PARALL"

LM_LISTS="rd wr rdwr cp fwr frd bzero bcopy";

convert_hn_to_byte $LENGTH Length 
Repeat=$REPEAT
Parall=$PARALL
BW_MEM_EXEC="/usr/bin/bw_mem"

for i in $LM_LISTS;
	do echo -en "$i\t";  done;
echo;

for i in $LM_LISTS;
	do res=$($BW_MEM_EXEC -P $Parall -N $Repeat $Length $i 2>&1 | awk "{print \$2}");
	echo -en "$res\t"; done;
echo MB/Sec
