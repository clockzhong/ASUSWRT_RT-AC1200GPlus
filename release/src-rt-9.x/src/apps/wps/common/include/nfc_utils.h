/*
 * WPS NFC share utility header file
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
 * $Id: $
 */

#ifndef _WPS_NFC_UTILS_H_
#define _WPS_NFC_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <wps_devinfo.h>

/*
 * implemented in nfc_utils.c
 */
uint32 nfc_utils_build_cfg(DevInfo *info, uint8 *buf, uint *buflen);
uint32 nfc_utils_build_pw(DevInfo *info, uint8 *buf, uint *buflen);
uint32 nfc_utils_build_cho(DevInfo *info, uint8 *buf, uint *buflen,
#ifdef WFA_WPS_20_NFC_TESTBED
	bool b_fake_pkh,
#endif
	bool b_cho_r);
uint32 nfc_utils_parse_cfg(WpsEnrCred *cred, uint8 *buf, uint buflen);
uint32 nfc_utils_parse_pw(WpsOobDevPw *devpw, uint8 *buf, uint buflen);
uint32 nfc_utils_parse_cho(WpsCho *wpscho, WPS_SCSTATE type, uint8 *buf, uint buflen);

#ifdef __cplusplus
}
#endif

#endif /* _WPS_NFC_UTILS_H_ */
