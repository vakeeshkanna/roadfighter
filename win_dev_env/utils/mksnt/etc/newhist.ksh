set -o nolog
function mktemp {
	typeset path="${1:-${TMPDIR:-/tmp}}"
	typeset path="${path%/}/tmp$$"
	typeset -i36 ext=36#aaa

	until
		(set -o noclobber; : >"$path.${ext#???}") 2>|nul
	do ext=ext+1
	done

	print -r "$path.${ext#???}"
}
if [ ! -d $TMPDIR ] ; then
	echo "$ROOTDIR/etc/newhist.ksh: TMPDIR not properly set" 1>&2
else
	export HISTFILE=$(mktemp)
	trap 'rm $HISTFILE; exit' EXIT
fi
