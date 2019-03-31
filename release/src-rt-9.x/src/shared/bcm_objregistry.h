/*
 * BCM Object Registry API definition
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
 * $Id:.$
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


#ifndef _bcm_objregistry_h_
#define _bcm_objregistry_h_

typedef struct obj_registry obj_registry_t;

/* === Object Registry API === */

/* Create the registry only once in wl_xxx.c per port layer and pass it to wlc_attach()
 * Each wlc_attach() creates a new WLC instance that shares the same objr instance
 */
obj_registry_t* bcm_obj_registry_alloc(osl_t* osh, int entries);

/* Destroy the registry at the end, after all instances of WLC are freed-up */
void bcm_obj_registry_free(obj_registry_t* objr, osl_t *osh);

/* obj_registry_set() is used to setup the value for key.
 * It simply overwrites the existing value if any
 * returns, BCME_OK on success
 * returns, BCME_RANGE if key exceeds, max limit
 */

int bcm_obj_registry_set(obj_registry_t *objr, int key, void *value);

/* obj_registry_get() is used to get the value for key.
 * return of NULL, indicates key is unused / invalid
 *
 */
void * bcm_obj_registry_get(obj_registry_t *objr, int key);

/* Ref counting on registry objects is provided for users to keep track of ref counts */

/*
 * Typical call sequence will be as follows:
 * 	Step (1). check if the registry has a value for key 'KEY_X'
 * 	Step (2). if it has value, go to Step (4)
 * 	Step (3). registry has no value, so allocate and store value for 'KEY_X'
 *	Step (4). reference the stored value for 'KEY_X'
 */

/* obj_registy_ref() is used to increment ref_cnt associated with 'key'
 * If there is no value stored, reference is not incremented.
 */
int bcm_obj_registry_ref(obj_registry_t *objr, int key);

/* obj_registry_unref() is used to decrement ref_cnt associated with 'key'
 * Decrements the reference count for each call.
 * If there is no value stored, reference is not decremented.
 */
int bcm_obj_registry_unref(obj_registry_t *objr, int key);

/* A special helper function to identify if we are cleaning up for the final obj reg */
int bcm_obj_registry_islast(obj_registry_t *objr);

#if defined(BCMDBG)
/* Debug function to dump out all contents of the registry */
int bcm_dump_objr(obj_registry_t *objr, struct bcmstrbuf *b);
#endif


#endif /* _bcm_objregistry_h_ */
