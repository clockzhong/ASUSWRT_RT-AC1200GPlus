#include <bmd_config.h>
#if CDK_CONFIG_INCLUDE_BCM53115_A0 == 1
/*
 * $Id: bcm53115_a0_bmd_cosq_port_mapping_set.c,v 1.2 Broadcom SDK $
 * 
 * $Copyright: Copyright 2010 Broadcom Corporation.
 * This program is the proprietary software of Broadcom Corporation
 * and/or its licensors, and may only be used, duplicated, modified
 * or distributed pursuant to the terms and conditions of a separate,
 * written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized
 * License, Broadcom grants no license (express or implied), right
 * to use, or waiver of any kind with respect to the Software, and
 * Broadcom expressly reserves all rights in and to the Software
 * and all intellectual property rights therein.  IF YOU HAVE
 * NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE
 * IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE
 * ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use
 * all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of
 * Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS
 * PROVIDED "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 * REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 * OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 * DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 * NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 * ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 * CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 * OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
 * BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL,
 * INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER
 * ARISING OUT OF OR IN ANY WAY RELATING TO YOUR USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF
 * THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR USD 1.00,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING
 * ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.$
 */

#include <bmd/bmd.h>
#include <bmd/bmd_device.h>

#include <cdk/chip/bcm53115_a0_defs.h>
#include <cdk/arch/robo_chip.h>
#include <cdk/cdk_debug.h>

#include "bcm53115_a0_bmd.h"
#include "bcm53115_a0_internal.h"

int 
bcm53115_a0_bmd_cosq_port_mapping_set(
    int unit, 
    int port, 
    int priority, 
    int egressq)
{
    int ioerr = 0, queue_bits;
    TC2COS_MAPr_t ctrl;

    BMD_CHECK_UNIT(unit);
    BMD_CHECK_PORT(unit, port);
    BMD_CHECK_PRIORITY(unit, port);
    if ((egressq < 0) || (egressq > NUM_EGRESS_QUEUES))
        return CDK_E_PARAM;

    /* No need of floor(log2()) as the num queues is either 4 or 8 */
        queue_bits = 2;

    ioerr += READ_TC2COS_MAPr(unit, &ctrl);
    ctrl.tc2cos_map[0] &= ~(0x3 << (priority * queue_bits) );
    ctrl.tc2cos_map[0] |= (egressq << (priority * queue_bits) );
    ioerr += WRITE_TC2COS_MAPr(unit, ctrl);

    return ioerr ? CDK_E_IO : CDK_E_NONE;
}
#endif /* CDK_CONFIG_INCLUDE_BCM53115_A0 */

