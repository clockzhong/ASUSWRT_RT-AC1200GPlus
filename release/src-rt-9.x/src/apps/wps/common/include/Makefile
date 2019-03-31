#
# include/Makefile
#
# Broadcom Proprietary and Confidential. Copyright (C) 2016,
# All Rights Reserved.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom.
#
#
# <<Broadcom-WL-IPTag/Proprietary:>>
#
# $Id: Makefile 525052 2015-01-08 20:18:35Z $
#

SRCBASE	= ..

TARGETS	= wpsvers.h


all release:
	bash wpsvers.sh
	mv wpsvers.h wps_version.h

clean:
	rm -rf ${TARGETS} *.prev


.PHONY: all release clean
