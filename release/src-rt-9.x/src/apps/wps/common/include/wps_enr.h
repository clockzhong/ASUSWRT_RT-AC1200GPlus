/*
 * WPS ENROLL header file
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
 * $Id: wps_enr.h 525052 2015-01-08 20:18:35Z $
 */

#ifndef __WPS_ENR_H__
#define __WPS_ENR_H__

#define WPS_ENR_EAP_DATA_MAX_LENGTH			2048
#define WPS_ENR_EAP_READ_DATA_TIMEOUT		1

/* OS dependent APIs */
void wpsenr_osl_proc_states(int state);
int wpsenr_osl_set_wsec(int ess_id, void *credential, int mode);
int wpsenr_osl_clear_wsec(void);
int wpsenr_osl_restore_wsec(void);

#endif /* __WPS_ENR_H__ */
