/*
 * Object Registry API Implementation
 * Broadcom 802.11abg Networking Device Driver
 *
 * Copyright (C) 2015, Broadcom Corporation
 * All Rights Reserved.
 * 
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 *
 * $Id: bcm_objregistry.c 526842 2015-01-15 05:22:32Z $
 */

/**
 * @file
 * @brief
 * With the rise of RSDB (Real Simultaneous Dual Band), the need arose to support two 'wlc'
 * structures, with the requirement to share data between the two. To meet that requirement, a
 * simple 'key=value' mechanism is introduced.
 *
 * WLC Object Registry provides mechanisms to share data across WLC instances in RSDB
 * The key-value pairs (enums/void *ptrs) to be stored in the "registry" are decided at design time.
 * Even the Non_RSDB (single instance) goes thru the Registry calls to have a unified interface.
 * But the Non_RSDB functions call have dummy/place-holder implementation managed using MACROS.
 *
 * The registry stores key=value in a simple array which is index-ed by 'key'
 * The registry also maintains a reference counter, which helps the caller in freeing the
 * 'value' associated with a 'key'
 * The registry stores objects as pointers represented by "void *" and hence a NULL value
 * indicates unused key
 *
 */


#include <typedefs.h>
#include <bcmdefs.h>
#include <osl.h>
#include <bcmutils.h>
#include <d11.h>
#include <siutils.h>
#include <bcm_objregistry.h>
#ifdef EVENT_LOG_COMPILE
#include <event_log.h>
#endif

#ifdef BCMDBG
#define BCM_OBJR_DBG(x) printf x
#else
#define BCM_OBJR_DBG(x)
#endif

#if defined(BCMDBG_ERR) && defined(ERR_USE_EVENT_LOG)

#if defined(ERR_USE_EVENT_LOG_RA)
#define	BCM_OBJR_ERROR(args)	EVENT_LOG_RA(EVENT_LOG_TAG_OBJR_ERROR, args)
#else
#define	BCM_OBJR_ERROR(args)	EVENT_LOG_COMPACT_CAST_PAREN_ARGS(EVENT_LOG_TAG_OBJR_ERROR, args)
#endif /* ERR_USE_EVENT_LOG_RA */

#elif defined(BCMDBG_ERR) || defined(BCMDBG)
#define BCM_OBJR_ERROR(args)	printf args
#else	/* BCMDBG */
#define BCM_OBJR_ERROR(args)
#endif	/* BCMDBG */

struct obj_registry {
	int count;
	void **value;
	uint8 *ref;
};


/* Object Registry Public APIs */
obj_registry_t*
BCMATTACHFN(bcm_obj_registry_alloc)(osl_t *osh, int count)
{
	obj_registry_t *objr = NULL;
	if ((objr = MALLOCZ(osh, sizeof(obj_registry_t))) == NULL) {
			BCM_OBJR_ERROR(("bcm obj registry %s: out of memory, malloced %d bytes\n",
				__FUNCTION__, MALLOCED(osh)));
	} else {
		objr->count = count;

		if ((objr->value =
			MALLOCZ(osh, sizeof(void*) * count)) == NULL) {
			BCM_OBJR_ERROR(("bcm obj registry %s: out of memory, malloced %d bytes\n",
				__FUNCTION__, MALLOCED(osh)));
			bcm_obj_registry_free(objr, osh);
			return NULL;
		}

		if ((objr->ref =
			MALLOCZ(osh, sizeof(uint8) * count)) == NULL) {
			BCM_OBJR_ERROR(("bcm obj registry %s: out of memory, malloced %d bytes\n",
				__FUNCTION__, MALLOCED(osh)));
			bcm_obj_registry_free(objr, osh);
			return NULL;
		}
	}
	BCM_OBJR_DBG(("BCM_OBJR CREATED : %p\n", objr));
	return objr;
}

void
BCMATTACHFN(bcm_obj_registry_free)(obj_registry_t *objr, osl_t *osh)
{
	if (objr) {
#ifdef BCMDBG
		if (objr->value && objr->ref) {
			/* Check if some stale refs are still present */
			int i = 0;
			for (i = 0; i < objr->count; i++) {
				if (objr->ref[i] || objr->value[i]) {
					BCM_OBJR_ERROR(("key:%d ref:%d value:%p\n",
						i, objr->ref[i], objr->value[i]));
					ASSERT(0);
				}
			}
		}
#endif /* BCMDBG */
		if (objr->value)
			MFREE(osh, objr->value, sizeof(void*) * objr->count);
		if (objr->ref)
			MFREE(osh, objr->ref, sizeof(uint8) * objr->count);
		MFREE(osh, objr, sizeof(obj_registry_t));
	}
}

int
BCMATTACHFN(bcm_obj_registry_set)(obj_registry_t *objr, int key, void *value)
{
	if (objr) {
		BCM_OBJR_DBG(("%s:%d:key[%d]=value[%p]\n",
			__FUNCTION__, __LINE__, (int)key, value));
		if ((key < 0) || (key >= objr->count)) return BCME_RANGE;
		objr->value[(int)key] = value;
		return BCME_OK;
	}
	return BCME_ERROR;
}

void*
BCMATTACHFN(bcm_obj_registry_get)(obj_registry_t *objr, int key)
{
	void *value = NULL;
	if (objr) {
		if ((key < 0) || (key >= objr->count)) return NULL;
		value = objr->value[(int)key];
		BCM_OBJR_DBG(("%s:%d:key[%d]=value[%p]\n",
			__FUNCTION__, __LINE__, (int)key, value));
	}
	return value;
}

int
BCMATTACHFN(bcm_obj_registry_ref)(obj_registry_t *objr, int key)
{
	int ref = 0;
	if (objr && (key >= 0) && (key < objr->count)) {
#ifdef BCMDBG
		void *value = NULL;
		value = objr->value[(int)key];
#endif
		ref = ++(objr->ref[(int)key]);
		BCM_OBJR_DBG(("%s:%d: key[%d]=value[%p]/REF[%d]\n",
			__FUNCTION__, __LINE__, (int)key, value, ref));
	}
	return ref;
}

int
BCMATTACHFN(bcm_obj_registry_unref)(obj_registry_t *objr, int key)
{
	int ref = 0;
	if (objr && (key >= 0) && (key < objr->count)) {
#ifdef BCMDBG
		void *value = NULL;
		value = objr->value[(int)key];
#endif
		ref = (objr->ref[(int)key]);
		if (ref > 0) {
			ref = --(objr->ref[(int)key]);
			BCM_OBJR_DBG(("%s:%d: key[%d]=value[%p]/REF[%d]\n",
				__FUNCTION__, __LINE__, (int)key, value, ref));
		}
	}
	return ref;
}


/* A special helper function to identify if we are cleaning up for the finale obj reg */
int
BCMATTACHFN(bcm_obj_registry_islast)(obj_registry_t *objr)
{
	/* NOTE: Index 0 corresponds to OBJ_SELF always */
	if (objr && (objr->value[0])) {
		return ((objr->ref[0] <= 1) ? 1 : 0);
	}
	return 1;
}

#if defined(BCMDBG)
int
BCMATTACHFN(bcm_dump_objr)(obj_registry_t *objr, struct bcmstrbuf *b)
{
	int i = 0;
	BCM_OBJR_DBG(("%s: %d \n", __FUNCTION__, __LINE__));
	if (objr) {
		if (b) {
			bcm_bprintf(b, "\nDumping Object Registry\n");
			bcm_bprintf(b, "Key\tValue\t\tRef\n");
			for (i = 0; i < objr->count; i++) {
				bcm_bprintf(b, "%d\t%p\t%d\n",
					i, objr->value[(int)i], objr->ref[(int)i]);
			}
		} else {
			BCM_OBJR_DBG(("\nkey\tvalue\tref\n"));
			for (i = 0; i < objr->count; i++) {
				BCM_OBJR_DBG(("%d\t%p\t%d\n",
					i, objr->value[(int)i], objr->ref[(int)i]));
			}
		}
	}
	else {
		bcm_bprintf(b, "\nObject Registry is not present\n");
	}
	return BCME_OK;
}
#endif /* BCMDBG */
