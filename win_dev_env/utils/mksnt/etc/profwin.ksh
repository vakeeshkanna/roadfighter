#print -r Running $ROOTDIR/etc/profwin.ksh

# MKS Toolkit sample $ROOTDIR/etc/profwin.ksh -- Modify as required.

# This file is executed by Windows KornShells started via "sh -L".
# It contains global initialization commands common to all
# Windows NT/Windows 95 KornShell users.

# SHELL identifies the location of the MKS KornShell.
export SHELL=${SHELL:=$(getconf _CS_SHELL)}

# LOGNAME identifies the current user name.
export LOGNAME=${USERNAME}	# USERNAME is set by Windows NT
if [ ! $LOGNAME ] ; then
	# If still not set try
	export LOGNAME=$(logname)
	# If still not set try
	export LOGNAME=${LOGNAME:=mks}	# in case logname.exe was not installed
fi

if [ $(uname) = Windows_NT ] ; then
	# The following is necessary because Windows NT sets ComSpeC and Path,
	# and we document COMSPEC and PATH.
	COMSPEC=${COMSPEC:=$ComSpec}
	# Define a Windows NT-specific PATH setting.
#	rootdir_mksnt=$(getconf _CS_PATH) ; rootdir_mksnt=${rootdir_mksnt%%;*}
#	PATH="$rootdir_mksnt;$Path;"
#	unset rootdir_mksnt

	# HOME identifies the pathname of the current user's home directory.
	if [ ! $HOME ];then
		# HOMEDRIVE and HOMEPATH are set by Windows NT
		export HOME=$HOMEDRIVE$HOMEPATH
		# If still not set try
		export HOME=${HOME:=c:/users/$USERNAME}
	fi
fi

# COMSPEC identifies the location of the Standard Windows Command Interpreter.
# PATH identifies the series of directories that the KornShell will search
# when it tries to execute a command.
export COMSPEC
export	PATH="$PATH"
#export	PATH="$HOME;$Path;$PATH"
while true ; do
	case $PATH in
	*\\*)	PATH=${PATH%%\\*}/${PATH#*\\};;
	*)	break;;
	esac
done

if [ -d ${temp:=c:/temp} ] ; then
	export TMPDIR=${TMPDIR:=$temp}
fi
