#print -r Running d:/etc/profile.ksh

# MKS Toolkit sample $ROOTDIR/etc/profile.ksh -- Modify as required.

# This file is executed by KornShells started either from login, or via
# "sh -L".  It contains global initialization commands common to all
# KornShell users and all supported operating systems.
# Operating system specific initializations for DOS, OS/2 and Windows NT
# are performed by profdos.ksh, profos2.ksh and profnt.ksh respectively.

# Turn on vi editting mode by default.
set -o vi

# Required by the default ENVIRON file provided.
export SWITCHAR=/

# LOGNAME identifies the current user name.
export LOGNAME=${LOGNAME:=mks}

# ROOTDIR identifies the directory in which the Toolkit is installed.
export ROOTDIR=${ROOTDIR:=d:}; ROOTDIR=${ROOTDIR%/}

# Execute the appropriate OS-specific profile.
case $(uname) in
DOS)		. $ROOTDIR/etc/profdos.ksh;;
OS/2)		. $ROOTDIR/etc/profos2.ksh;;
Windows*)	. $ROOTDIR/etc/profwin.ksh;;
*)		;;	# what to do if uname fails
esac

# Only define the following if they aren't defined in the OS-specific profile.

# TZ identifies the local time zone. See the timezone(5) manual page.
export TZ=${TZ:=EST0}

# TMPDIR identifies the directory in which most Toolkit utilities store
# their temporary files.
export TMPDIR=${TMPDIR:=$ROOTDIR/tmp}

# HOME identifies the pathname of the current user's home directory.
export HOME=${HOME:=$ROOTDIR}
if [ x${HOME%?:} = x ]; then HOME=$HOME/; fi
# Change each '\' in the HOME environment variable into '/'.
while true ; do
	case $HOME in
	*\\*)	HOME=${HOME%%\\*}/${HOME#*\\};;
	*)	break;;
	esac
done

