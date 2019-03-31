#!/bin/bash
#
# Broadcom Proprietary and Confidential. Copyright (C) 2016,
# All Rights Reserved.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom.
#  
# Check all files in a GPL license tree for Broadcom header comment
#
# $Id: validate_gpl.sh 666639 2016-10-24 04:57:46Z $
#

TMP=/tmp/gplcheck$$

trap 'rm -f $TMP' INT EXIT

if [ "$1" = "" ]; then
    echo >&2 "$0: Missing directory argument"
    exit 1
fi

grep --exclude-dir=opensource -l -r "UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom" "$1/" > $TMP
nongpl=$?

if [ $nongpl -eq 0 ]; then
    echo "ERROR: Found files in the GPL tarball with proprietary Broadcom header comment:"
    sed -e 's/^/    /' $TMP
    exit 1;
else
    echo "GPL Header Check Done" 
fi
