/*
 * TuTrace
 *
 * Broadcom Proprietary and Confidential. Copyright (C) 2016,
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom.
 *
 *
 * <<Broadcom-WL-IPTag/Proprietary:>>
 *
 * $Id: tutrace.h 525052 2015-01-08 20:18:35Z $
 */

#ifndef _TUTRACE_H
#define _TUTRACE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*WPS_TRACEMSG_OUTPUT_FN)(int is_err, char *traceMsg);
void wps_set_traceMsg_output_fn(WPS_TRACEMSG_OUTPUT_FN fn);
void wps_tutrace_set_msglevel(unsigned int level);
unsigned int wps_tutrace_get_msglevel();
int WPS_HexDumpAscii(unsigned int level, char *title, unsigned char *buf, unsigned int len);

/* Default trace level */
#define TUTRACELEVEL    (TUERR | TUINFO)

/* trace levels */
#define TUERR		0x0001
#define TUINFO		0x0002
#define TUNFC		0x0004
#define TUDUMP_MSG	0x0008
#define TUDUMP_IE	0x0010
#define TUDUMP_PROBE	0x0020
#define TUDUMP_KEY	0x0040
#define TUDUMP_NFC	0x0080
#define TUTIME		0x8000


#define TUTRACE_ERR        TUERR, __FUNCTION__, __LINE__
#define TUTRACE_INFO       TUINFO, __FUNCTION__, __LINE__
#define TUTRACE_NFC        TUNFC, __FUNCTION__, __LINE__

#ifdef _TUDEBUGTRACE

#define TUTRACE(VARGLST)   print_traceMsg VARGLST

void print_traceMsg(int level, const char *lpszFile,
	int nLine, char *lpszFormat, ...);

#else

#define TUTRACE(VARGLST)    ((void)0)

#endif /* _TUDEBUGTRACE */

#ifdef __cplusplus
}
#endif

#endif /* _TUTRACE_H */
