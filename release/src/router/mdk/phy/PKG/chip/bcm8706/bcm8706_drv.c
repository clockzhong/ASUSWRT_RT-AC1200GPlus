/*
 * $Id: bcm8706_drv.c,v 1.10 Broadcom SDK $
 * $Copyright: Copyright 2013 Broadcom Corporation.
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
 *
 * PHY driver for BCM8706/BCM8726.
 *
 */

#include <phy/phy.h>
#include <phy/ge_phy.h>

/* Custom configuration definitions */
#define PHY_CONFIG_BCM8706_REVA_SUPPORT 0

#define PHY_RESET_POLL_MAX              10
#define PHY_ROM_LOAD_POLL_MAX           500
#define PHY_LANES_POLL_MAX              500

#define BCM8706_PMA_PMD_ID0             0x0020
#define BCM8706_PMA_PMD_ID1             0x6035
#define BCM8706_PMA_PMD_ID1_REVA        0x6034

#define C45_DEVAD(_a)                   LSHIFT32((_a),16)
#define DEVAD_PMA_PMD                   C45_DEVAD(MII_C45_DEV_PMA_PMD)
#define DEVAD_PCS                       C45_DEVAD(MII_C45_DEV_PCS)
#define DEVAD_PHY_XS                    C45_DEVAD(MII_C45_DEV_PHY_XS)
#define DEVAD_AN                        C45_DEVAD(MII_C45_DEV_AN)

/* PMA/PMD registers */
#define PMA_PMD_CTRL_REG                (DEVAD_PMA_PMD + MII_CTRL_REG)
#define PMA_PMD_STAT_REG                (DEVAD_PMA_PMD + MII_STAT_REG)
#define PMA_PMD_ID0_REG                 (DEVAD_PMA_PMD + MII_PHY_ID0_REG)
#define PMA_PMD_ID1_REG                 (DEVAD_PMA_PMD + MII_PHY_ID1_REG)
#define PMA_PMD_SPEED_ABIL              (DEVAD_PMA_PMD + 0x0005)
#define PMA_PMD_DEV_IN_PKG              (DEVAD_PMA_PMD + 0x0006)
#define PMA_PMD_CTRL2_REG               (DEVAD_PMA_PMD + 0x0007)
#define PMA_PMD_EXT_BOOTROM_REG_1       (DEVAD_PMA_PMD + 0xca85)
#define PMA_PMD_EXT_BOOTROM_REG_2       (DEVAD_PMA_PMD + 0xca10)
#define PMA_PMD_GEN_REG_1               (DEVAD_PMA_PMD + 0xca19)

/* PCS registers */
#define PCS_CTRL_REG                    (DEVAD_PCS + MII_CTRL_REG)
#define PCS_STAT_REG                    (DEVAD_PCS + MII_STAT_REG)
#define PCS_ID0_REG                     (DEVAD_PCS + MII_PHY_ID0_REG)
#define PCS_ID1_REG                     (DEVAD_PCS + MII_PHY_ID1_REG)
#define PCS_SPEED_ABIL                  (DEVAD_PCS + 0x0005)
#define PCS_DEV_IN_PKG                  (DEVAD_PCS + 0x0006)
#define PCS_POLARITY                    (DEVAD_PCS + 0xc808)

/* PHY XS registers */
#define PHY_XS_CTRL_REG                 (DEVAD_PHY_XS + MII_CTRL_REG)
#define PHY_XS_STAT_REG                 (DEVAD_PHY_XS + MII_STAT_REG)
#define PHY_XS_ID0_REG                  (DEVAD_PHY_XS + MII_PHY_ID0_REG)
#define PHY_XS_ID1_REG                  (DEVAD_PHY_XS + MII_PHY_ID1_REG)
#define PHY_XS_SPEED_ABIL               (DEVAD_PHY_XS + 0x0005)
#define PHY_XS_DEV_IN_PKG               (DEVAD_PHY_XS + 0x0006)
#define PHY_XS_XGXS_LANE_STAT           (DEVAD_PHY_XS + 0x0018)
#define PHY_XS_XGXS_TX_POLARITY         (DEVAD_PHY_XS + 0x80a1)
#define PHY_XS_XGXS_RX_POLARITY         (DEVAD_PHY_XS + 0x80fa)
#define PHY_XS_XGXS_RX_LANE_SWAP        (DEVAD_PHY_XS + 0x8100)
#define PHY_XS_XGXS_TX_LANE_SWAP        (DEVAD_PHY_XS + 0x8101)

/* AN registers */
#define AN_CTRL_REG                     (DEVAD_AN + MII_CTRL_REG)
#define AN_STAT_REG                     (DEVAD_AN + MII_STAT_REG)
#define AN_CLAUSE_73_ADVERT_REG         (DEVAD_AN + 0x11)
#define AN_LINK_STAT_REG                (DEVAD_AN + 0x8304)
#define AN_CLAUSE_37_73_ALLOW_REG       (DEVAD_AN + 0x8370)
#define AN_CLAUSE_37_ENABLE_REG         (DEVAD_AN + 0xffe0)
#define AN_CLAUSE_37_ADVERT_REG         (DEVAD_AN + 0xffe4)

/* PMA/PMD control register */
#define PMA_PMD_CTRL_RESET              (1L << 15)
#define PMA_PMD_CTRL_LO_PWR             (1L << 6)
#define PMA_PMD_CTRL_LE                 (1L << 0)

/* PMA/PMD control2 register */
#define PMA_PMD_CTRL2r_PMA_TYPE_MASK    0xF
#define PMA_PMD_CTRL2r_PMA_TYPE_1G      0xD
#define PMA_PMD_CTRL2r_PMA_TYPE_10G     0x8

/* XS polarity registers */
#define XS_TX_POLARITY_INVERT           (1L << 5)
#define XS_RX_POLARITY_INVERT           ((1L << 3) | (1L << 2))

/* XS lane swap registers */
#define XS_TX_LANE_SWAP                 (1L << 15)
#define XS_RX_LANE_SWAP                 ((1L << 15) | (1L << 14))

/* Devices in package register 1 */
#define DEV_IN_PKG_AN                   (1L << 7)
#define DEV_IN_PKG_DTE_XS               (1L << 5)
#define DEV_IN_PKG_PHY_XS               (1L << 4)
#define DEV_IN_PKG_PCS                  (1L << 3)
#define DEV_IN_PKG_WIS                  (1L << 2)
#define DEV_IN_PKG_PMA_PMD              (1L << 1)
#define DEV_IN_PKG_C22                  (1L << 0)

/*PCS polarity registers */
#define PCS_TX_POLARITY_INVERT          (1L << 10)
#define PCS_RX_POLARITY_INVERT          (1L << 9)

/* Misc. control register */
#define MISC_CTRL_LOL_OPT_LOS_EN        (1L << 9)
#define MISC_CTRL_P_IN_MUXSEL           (1L << 7)
#define MISC_CTRL_X_IN_MUXSEL           (1L << 6)
#define MISC_CTRL_CLUPLL_EN             (1L << 5)
#define MISC_CTRL_XCLKMODE_OVRD         (1L << 4)
#define MISC_CTRL_XFP_CLK_EN            (1L << 3)
#define MISC_CTRL_REFOUTFREQ            (7L << 0)

/* Gen Reg 1 values */
#define GEN_REG_1_LANES                 0x1234
#define GEN_REG_1_LANES_REV             0x4321

/* AN Control register */
#define AN_CTRL_EXT_NXT_PAGE            (1L << 13)
#define AN_CTRL_ENABLE                  (1L << 12)
#define AN_CTRL_RESTART                 (1L << 9)

/* AN Status register */
#define AN_STAT_AN_DONE                 (1L << 5)

/* AN Link Status register */
#define AN_LINK_STATUS_1G               (1L << 1)

/* AN Clause 37-73 allow register */
#define AN_CLAUSE_37_73_VALUE           0x040c

/* AN Clause 37 enable register */
#define AN_CLAUSE_37_ENABLE_VALUE       0x1000

/* AN Clause 37 & 73 advert registers */
#define AN_CLAUSE_37_73_ADVERT_FULL_DUPLEX  (1L << 5)

/* Low level debugging (off by default) */
#ifdef BCM8706_DEBUG_ENABLE
#define BCM8706_DBG(_pc, _str) \
    CDK_WARN(("bcm8706[%d.%d]: " _str "\n", \
               PHY_CTRL_UNIT(_pc), PHY_CTRL_PORT(_pc)));
#else
#define BCM8706_DBG(_pc, _str)
#endif

/***********************************************************************
 *
 * PHY DRIVER FUNCTIONS
 *
 ***********************************************************************/

/*
 * Function:
 *      bcm8706_phy_probe
 * Purpose:     
 *      Probe for 8706 PHY
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_probe(phy_ctrl_t *pc)
{
    uint32_t phyid0, phyid1;
    int ioerr = 0;

    PHY_CTRL_CHECK(pc);

    ioerr += PHY_BUS_READ(pc, PMA_PMD_ID0_REG, &phyid0);
    ioerr += PHY_BUS_READ(pc, PMA_PMD_ID1_REG, &phyid1);

    if (phyid0 == BCM8706_PMA_PMD_ID0 && 
        phyid1 == BCM8706_PMA_PMD_ID1) {
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#if PHY_CONFIG_BCM8706_REVA_SUPPORT
    if (phyid0 == BCM8706_PMA_PMD_ID0 && 
        phyid1 == BCM8706_PMA_PMD_ID1_REVA) {
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
    return CDK_E_NOT_FOUND;
}

/*
 * Function:
 *      bcm8706_phy_notify
 * Purpose:     
 *      Handle PHY notifications
 * Parameters:
 *      pc - PHY control structure
 *      event - PHY event
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_notify(phy_ctrl_t *pc, phy_event_t event)
{
    int rv = CDK_E_NONE;

    PHY_CTRL_CHECK(pc);

    /* Call up the PHY chain */
    if (CDK_SUCCESS(rv)) {
        rv = PHY_NOTIFY(PHY_CTRL_NEXT(pc), event);
    }

    return rv;
}

/*
 * Function:
 *      bcm8706_phy_reset
 * Purpose:     
 *      Reset 8706 PHY
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_reset(phy_ctrl_t *pc)
{
    uint32_t pma_pmd_ctrl, pcs_ctrl, xs_ctrl;
    int cnt;
    int ioerr = 0;
    int rv = CDK_E_NONE;

    PHY_CTRL_CHECK(pc);

    /* Reset all internal devices */
    ioerr += PHY_BUS_READ(pc, PMA_PMD_CTRL_REG, &pma_pmd_ctrl);
    pma_pmd_ctrl |= MII_CTRL_RESET;
    ioerr += PHY_BUS_WRITE(pc, PMA_PMD_CTRL_REG, pma_pmd_ctrl);

    ioerr += PHY_BUS_READ(pc, PCS_CTRL_REG, &pcs_ctrl);
    pcs_ctrl |= MII_CTRL_RESET;
    ioerr += PHY_BUS_WRITE(pc, PCS_CTRL_REG, pcs_ctrl);

    ioerr += PHY_BUS_READ(pc, PHY_XS_CTRL_REG, &xs_ctrl);
    xs_ctrl |= MII_CTRL_RESET;
    ioerr += PHY_BUS_WRITE(pc, PHY_XS_CTRL_REG, xs_ctrl);

    /* Wait for reset completion */
    for (cnt = 0; cnt < PHY_RESET_POLL_MAX; cnt++) {
        ioerr += PHY_BUS_READ(pc, PHY_XS_CTRL_REG, &xs_ctrl);
        if ((xs_ctrl & MII_CTRL_RESET) == 0) {
            break;
        }
    }
    if (cnt >= PHY_RESET_POLL_MAX) {
        rv = CDK_E_TIMEOUT;
    }

    /* Call up the PHY chain */
    if (CDK_SUCCESS(rv)) {
        rv = PHY_RESET(PHY_CTRL_NEXT(pc));
    }

    return ioerr ? CDK_E_IO : rv;
}

/*
 * Function:
 *      bcm8706_phy_init
 * Purpose:     
 *      Initialize 8706 PHY driver
 * Parameters:
 *      pc - PHY control structure
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_init(phy_ctrl_t *pc)
{
    uint32_t lane_stat;
    int sleep_time = 1000;
    int cnt;
    int ioerr = 0;
    int rv = CDK_E_NONE;

    PHY_CTRL_CHECK(pc);

    /* Reset PHY */
    if (CDK_SUCCESS(rv)) {
        rv = PHY_RESET(pc);
    }

    PHY_CTRL_FLAGS(pc) |= PHY_F_FIBER_MODE;

#if PHY_CONFIG_BCM8706_REVA_SUPPORT
    if (CDK_SUCCESS(rv)) {
        uint32_t phyid1;
        ioerr += PHY_BUS_READ(pc, PMA_PMD_ID1_REG, &phyid1);

        /* For REV A, force these settings to the default values */
        if (phyid1 == BCM8706_PMA_PMD_ID1_REVA) {
#if PHY_CONFIG_INCLUDE_XAUI_TX_LANE_MAP_SET
            if (CDK_SUCCESS(rv)) {
                rv = PHY_CONFIG_SET(pc, PhyConfig_XauiTxLaneRemap, 0x0123);
            }
#endif

#if PHY_CONFIG_INCLUDE_XAUI_RX_LANE_MAP_SET
            if (CDK_SUCCESS(rv)) {
                rv = PHY_CONFIG_SET(pc, PhyConfig_XauiRxLaneRemap, 0x0123);
            }
#endif

#if PHY_CONFIG_INCLUDE_XAUI_TX_POLARITY_SET
            if (CDK_SUCCESS(rv)) {
                rv = PHY_CONFIG_SET(pc, PhyConfig_XauiTxPolInvert, 0);
            }
#endif

#if PHY_CONFIG_INCLUDE_XAUI_RX_POLARITY_SET
            if (CDK_SUCCESS(rv)) {
                rv = PHY_CONFIG_SET(pc, PhyConfig_XauiRxPolInvert, 0);
            }
#endif

#if PHY_CONFIG_INCLUDE_PCS_TX_POLARITY_SET
            if (CDK_SUCCESS(rv)) {
                rv = PHY_CONFIG_SET(pc, PhyConfig_PcsTxPolInvert, 0);
            }
#endif

#if PHY_CONFIG_INCLUDE_PCS_RX_POLARITY_SET
            if (CDK_SUCCESS(rv)) {
                rv = PHY_CONFIG_SET(pc, PhyConfig_PcsRxPolInvert, 0);
            }
#endif
        }
    }
#endif

#if PHY_CONFIG_EXTERNAL_BOOT_ROM
    if (CDK_SUCCESS(rv)) {
        uint32_t gen_reg1;

        ioerr += PHY_BUS_WRITE(pc, PMA_PMD_EXT_BOOTROM_REG_1, 0x0001);
        PHY_SYS_USLEEP(sleep_time);
        ioerr += PHY_BUS_WRITE(pc, PMA_PMD_EXT_BOOTROM_REG_2, 0x0188);
        PHY_SYS_USLEEP(sleep_time);
        ioerr += PHY_BUS_WRITE(pc, PMA_PMD_EXT_BOOTROM_REG_2, 0x018a);
        PHY_SYS_USLEEP(sleep_time);
        ioerr += PHY_BUS_WRITE(pc, PMA_PMD_EXT_BOOTROM_REG_2, 0x0188);

        /* Wait for at least 100ms for code to download via SPI port */
        PHY_SYS_USLEEP(200000);
        ioerr += PHY_BUS_WRITE(pc, PMA_PMD_EXT_BOOTROM_REG_1, 0x0000);
        PHY_SYS_USLEEP(sleep_time);

        /* Make sure SPI-ROM load is complete */
        for (cnt = 0; cnt < PHY_ROM_LOAD_POLL_MAX; cnt++) {
            ioerr += PHY_BUS_READ(pc, PMA_PMD_GEN_REG_1, &gen_reg1);
            if ((gen_reg1 == GEN_REG_1_LANES) ||
                (gen_reg1 == GEN_REG_1_LANES_REV)) {
                BCM8706_DBG(pc, "rom ok");
                break;
            }
            PHY_SYS_USLEEP(sleep_time);
        }
        if (cnt >= PHY_ROM_LOAD_POLL_MAX) {
            BCM8706_DBG(pc, "rom load timeout");
            rv = CDK_E_TIMEOUT;
        }
    }
#endif

    if (CDK_SUCCESS(rv)) {
        /* Make sure 8706 XAUI lanes are synchronized with the SOC XAUI */
        for (cnt = 0; cnt < PHY_LANES_POLL_MAX; cnt++) {
            ioerr += PHY_BUS_READ(pc, PHY_XS_XGXS_LANE_STAT, &lane_stat);
            if ((lane_stat & 0xf) == 0xf) {
                BCM8706_DBG(pc, "lane sync ok");
                break;
            }
            PHY_SYS_USLEEP(sleep_time);
        }
        if (cnt >= PHY_LANES_POLL_MAX) {
            BCM8706_DBG(pc, "lane sync timeout");
            rv = CDK_E_TIMEOUT;
        }
    }

    /* Call up the PHY chain */
    if (CDK_SUCCESS(rv)) {
        rv = PHY_INIT(PHY_CTRL_NEXT(pc));
    }

    return ioerr ? CDK_E_IO : rv;
}

/*
 * Function:    
 *      bcm8706_phy_link_get
 * Purpose:     
 *      Determine the current link up/down status
 * Parameters:
 *      pc - PHY control structure
 *      link - (OUT) non-zero indicates link established.
 * Returns:
 *      CDK_E_xxx
 * Notes:
 *      MII_STATUS bit 2 reflects link state.
 */
static int
bcm8706_phy_link_get(phy_ctrl_t *pc, int *link, int *autoneg_done)
{
    uint32_t pma_pmd_stat, pcs_stat, phy_xs_stat;
    uint32_t link_stat, stat;
    uint32_t cur_speed;
    int ioerr = 0;
    int rv;

    PHY_CTRL_CHECK(pc);

    *link = 0;

    rv = PHY_SPEED_GET(pc, &cur_speed);
    if (CDK_FAILURE(rv)) {
        return rv;
    }

    if (cur_speed == 10000) {
        /* Link must be up in all devices */
        ioerr += PHY_BUS_READ(pc, PMA_PMD_STAT_REG, &pma_pmd_stat);
        ioerr += PHY_BUS_READ(pc, PCS_STAT_REG, &pcs_stat);
        ioerr += PHY_BUS_READ(pc, PHY_XS_STAT_REG, &phy_xs_stat);
        link_stat = (pma_pmd_stat & pcs_stat & phy_xs_stat);
        if (link_stat & MII_STAT_LA) {
            *link = 1;
        }
    } else {
        ioerr += PHY_BUS_READ(pc, AN_LINK_STAT_REG, &link_stat);
        if (link_stat & AN_LINK_STATUS_1G) {
            *link = 1;
        }
    }

    if (autoneg_done) {
        ioerr += PHY_BUS_READ(pc, AN_STAT_REG, &stat);
        *autoneg_done = (stat & AN_STAT_AN_DONE);
    }

    return CDK_E_NONE;
}

/*
 * Function:    
 *      bcm8706_phy_duplex_set
 * Purpose:     
 *      Set the current duplex mode (forced).
 * Parameters:
 *      pc - PHY control structure
 *      duplex - non-zero indicates full duplex, zero indicates half
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_duplex_set(phy_ctrl_t *pc, int duplex)
{
    return (duplex != 0) ? CDK_E_NONE : CDK_E_PARAM;
}

/*
 * Function:    
 *      bcm8706_phy_duplex_get
 * Purpose:     
 *      Get the current operating duplex mode. If autoneg is enabled, 
 *      then operating mode is returned, otherwise forced mode is returned.
 * Parameters:
 *      pc - PHY control structure
 *      duplex - (OUT) non-zero indicates full duplex, zero indicates half
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_duplex_get(phy_ctrl_t *pc, int *duplex)
{
    *duplex = 1;

    return CDK_E_NONE;
}

/*
 * Function:    
 *      bcm8706_phy_speed_set
 * Purpose:     
 *      Set the current operating speed (forced).
 * Parameters:
 *      pc - PHY control structure
 *      speed - new link speed
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_speed_set(phy_ctrl_t *pc, uint32_t speed)
{
    uint32_t pma_pmd_ctrl, pma_pmd_ctrl2;
    uint32_t o_pma_pmd_ctrl2;
    uint32_t cur_speed;
    int an;
    int ioerr = 0;
    int rv;

    PHY_CTRL_CHECK(pc);

    if (!(speed == 10000 || speed == 1000 || speed == 2500)) {
        return CDK_E_PARAM;
    }

    /* Call up the PHY chain */
    rv = PHY_SPEED_SET(PHY_CTRL_NEXT(pc), speed);
    if (CDK_FAILURE(rv)) {
        return rv;
    }

    /* Leave hardware alone if speed is unchanged */
    rv = PHY_SPEED_GET(pc, &cur_speed);
    if (CDK_SUCCESS(rv) && speed == cur_speed) {
        return CDK_E_NONE;
    }

    if (CDK_SUCCESS(rv)) {
        rv = PHY_AUTONEG_GET(pc, &an);
    }

    /* Leave hardware alone if auto-neg is enabled */
    if (CDK_SUCCESS(rv) && an == 0) {
        if (speed == 10000) {
            int loopback;
            PHY_CTRL_FLAGS(pc) &= ~PHY_F_PASSTHRU;
            /* Store loopback mode over reset */
            rv = PHY_LOOPBACK_GET(pc, &loopback);
            if (CDK_SUCCESS(rv)) {
                rv = PHY_INIT(pc);
            }
            if (CDK_SUCCESS(rv)) {
                ioerr += PHY_BUS_READ(pc, PMA_PMD_CTRL2_REG, &pma_pmd_ctrl2);
                o_pma_pmd_ctrl2 = pma_pmd_ctrl2;
                pma_pmd_ctrl2 &= ~PMA_PMD_CTRL2r_PMA_TYPE_MASK;
                pma_pmd_ctrl2 |= PMA_PMD_CTRL2r_PMA_TYPE_10G;
                if (o_pma_pmd_ctrl2 != pma_pmd_ctrl2) {
                    /* Don't write this register unless it changes */
                    ioerr +=
                        PHY_BUS_WRITE(pc, PMA_PMD_CTRL2_REG, pma_pmd_ctrl2);
                }

                ioerr += PHY_BUS_READ(pc, PMA_PMD_CTRL_REG, &pma_pmd_ctrl);
                pma_pmd_ctrl |= MII_CTRL_SS_LSB;
                ioerr += PHY_BUS_WRITE(pc, PMA_PMD_CTRL_REG, pma_pmd_ctrl);

                /* Restore loopback mode over reset */
                if (loopback) {
                    rv = PHY_LOOPBACK_SET(pc, loopback);
                }
            }
        } else {
            /* in passthru mode */
            PHY_CTRL_FLAGS(pc) |= PHY_F_PASSTHRU;
            ioerr += PHY_BUS_READ(pc, PMA_PMD_CTRL2_REG, &pma_pmd_ctrl2);
            o_pma_pmd_ctrl2 = pma_pmd_ctrl2;
            pma_pmd_ctrl2 &= ~PMA_PMD_CTRL2r_PMA_TYPE_MASK;
            pma_pmd_ctrl2 |= PMA_PMD_CTRL2r_PMA_TYPE_1G;
            if (o_pma_pmd_ctrl2 != pma_pmd_ctrl2) {
                ioerr += PHY_BUS_WRITE(pc, PMA_PMD_CTRL2_REG, pma_pmd_ctrl2);
            }

            ioerr += PHY_BUS_READ(pc, PMA_PMD_CTRL_REG, &pma_pmd_ctrl);
            pma_pmd_ctrl &= ~MII_CTRL_SS_LSB;
            ioerr += PHY_BUS_WRITE(pc, PMA_PMD_CTRL_REG, pma_pmd_ctrl);
        }
    }

    return ioerr ? CDK_E_IO : rv;
}

/*
 * Function:    
 *      bcm8706_phy_speed_get
 * Purpose:     
 *      Get the current operating speed. If autoneg is enabled, 
 *      then operating mode is returned, otherwise forced mode is returned.
 * Parameters:
 *      pc - PHY control structure
 *      speed - (OUT) current link speed
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_speed_get(phy_ctrl_t *pc, uint32_t *speed)
{
    uint32_t pma_pmd_ctrl2, link_stat;
    int an;
    int ioerr = 0;
    int rv = CDK_E_NONE;

    PHY_CTRL_CHECK(pc);

    rv = PHY_AUTONEG_GET(pc, &an);
    if (CDK_SUCCESS(rv)) {
        if (an) {
            ioerr += PHY_BUS_READ(pc, AN_LINK_STAT_REG, &link_stat);
            if (link_stat & AN_LINK_STATUS_1G) {
                *speed = 1000;
            } else {
                *speed = 10000;
            }
        } else {
            ioerr += PHY_BUS_READ(pc, PMA_PMD_CTRL2_REG, &pma_pmd_ctrl2);
            if ((pma_pmd_ctrl2 & PMA_PMD_CTRL2r_PMA_TYPE_MASK) ==
                PMA_PMD_CTRL2r_PMA_TYPE_1G) {
                *speed = 1000;
            } else {
                *speed = 10000;
            }
        }
    }

    return ioerr ? CDK_E_IO : rv;
}

/*
 * Function:    
 *      bcm8706_phy_autoneg_set
 * Purpose:     
 *      Enable or disable auto-negotiation on the specified port.
 * Parameters:
 *      pc - PHY control structure
 *      autoneg - non-zero enables autoneg, zero disables autoneg
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_autoneg_set(phy_ctrl_t *pc, int autoneg)
{
    int ioerr = 0;
    int rv = CDK_E_NONE;

    if (autoneg) {
        /* Full duplex advertisement of 1G on Clause 37 */
        ioerr += PHY_BUS_WRITE(pc, AN_CLAUSE_37_ADVERT_REG,
                               AN_CLAUSE_37_73_ADVERT_FULL_DUPLEX);

        /* Full duplex advertisement of 1G on Clause 73 */
        ioerr += PHY_BUS_WRITE(pc, AN_CLAUSE_73_ADVERT_REG,
                               AN_CLAUSE_37_73_ADVERT_FULL_DUPLEX);

        /* Allow Clause 37 through Clause 73 */
        ioerr += PHY_BUS_WRITE(pc, AN_CLAUSE_37_73_ALLOW_REG,
                               AN_CLAUSE_37_73_VALUE);
        /* Enable Clause 37 AN */
        ioerr += PHY_BUS_WRITE(pc, AN_CLAUSE_37_ENABLE_REG,
                               AN_CLAUSE_37_ENABLE_VALUE);
        ioerr += PHY_BUS_WRITE(pc, AN_CTRL_REG,
                               AN_CTRL_EXT_NXT_PAGE |
                               AN_CTRL_ENABLE |
                               AN_CTRL_RESTART);
    } else {
        /* disable Clause 37 AN */
        ioerr += PHY_BUS_WRITE(pc, AN_CLAUSE_37_ENABLE_REG, 0);
        ioerr += PHY_BUS_WRITE(pc, AN_CTRL_REG, 0);
    }

    /* Call up the PHY chain */
    if (CDK_SUCCESS(rv)) {
        rv = PHY_AUTONEG_SET(PHY_CTRL_NEXT(pc), autoneg);
    }

    return ioerr ? CDK_E_IO : rv;
}

/*
 * Function:    
 *      bcm8706_phy_autoneg_get
 * Purpose:     
 *      Get the current auto-negotiation setting.
 * Parameters:
 *      pc - PHY control structure
 *      autoneg - (OUT) non-zero indicates autoneg enabled
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_autoneg_get(phy_ctrl_t *pc, int *autoneg)
{
    uint32_t ctrl;
    int ioerr = 0;
    int rv = CDK_E_NONE;

    if (autoneg) {
        ioerr += PHY_BUS_READ(pc, AN_CTRL_REG, &ctrl);
        *autoneg = (ctrl & AN_CTRL_ENABLE);
    }

    return ioerr ? CDK_E_IO : rv;
}

/*
 * Function:    
 *      bcm8706_phy_loopback_set
 * Purpose:     
 *      Set the internal PHY loopback mode.
 * Parameters:
 *      pc - PHY control structure
 *      enable - non-zero enables PHY loopback
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_loopback_set(phy_ctrl_t *pc, int enable)
{
    uint32_t pcs_ctrl;
    int ioerr = 0;

    if (PHY_CTRL_FLAGS(pc) & PHY_F_PASSTHRU) {
        return PHY_LOOPBACK_SET(PHY_CTRL_NEXT(pc), enable);
    }

    /* Read loopback control registers */
    ioerr += PHY_BUS_READ(pc, PCS_CTRL_REG, &pcs_ctrl);

    pcs_ctrl &= ~MII_CTRL_LE;
    if (enable) {
        pcs_ctrl |= MII_CTRL_LE;
    }

    /* Write updated loopback control registers */
    ioerr += PHY_BUS_WRITE(pc, PCS_CTRL_REG, pcs_ctrl);

    return ioerr ? CDK_E_IO : CDK_E_NONE;
}

/*
 * Function:    
 *      bcm8706_phy_loopback_get
 * Purpose:     
 *      Get the local PHY loopback mode.
 * Parameters:
 *      pc - PHY control structure
 *      enable - (OUT) non-zero indicates PHY loopback enabled
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_loopback_get(phy_ctrl_t *pc, int *enable)
{
    uint32_t pcs_ctrl;
    int ioerr = 0;

    if (PHY_CTRL_FLAGS(pc) & PHY_F_PASSTHRU) {
        return PHY_LOOPBACK_GET(PHY_CTRL_NEXT(pc), enable);
    }

    /* Read loopback control registers */
    ioerr += PHY_BUS_READ(pc, PCS_CTRL_REG, &pcs_ctrl);

    *enable = 0;
    if ((pcs_ctrl & MII_CTRL_LE)) {
        *enable = 1;
    }

    return ioerr ? CDK_E_IO : CDK_E_NONE;
}

/*
 * Function:    
 *      bcm8706_phy_ability_get
 * Purpose:     
 *      Get the abilities of the PHY.
 * Parameters:
 *      pc - PHY control structure
 *      abil - (OUT) ability mask indicating supported options/speeds.
 * Returns:     
 *      CDK_E_xxx
 */
static int
bcm8706_phy_ability_get(phy_ctrl_t *pc, uint32_t *abil)
{
    PHY_CTRL_CHECK(pc);

    *abil = (PHY_ABIL_10GB | 
             PHY_ABIL_LOOPBACK | PHY_ABIL_XGMII);

    return CDK_E_NONE;
}

/*
 * Function:
 *      bcm8706_phy_config_set
 * Purpose:
 *      Modify PHY configuration value.
 * Parameters:
 *      pc - PHY control structure
 *      cfg - Configuration parameter
 *      val - Configuration value
 *      cd - Additional configuration data (if any)
 * Returns:
 *      CDK_E_xxx
 */
static int
bcm8706_phy_config_set(phy_ctrl_t *pc, phy_config_t cfg, uint32_t val, void *cd)
{
    PHY_CTRL_CHECK(pc);

    switch (cfg) {
    case PhyConfig_Enable:
        return CDK_E_NONE;
    case PhyConfig_PortInterface:
        switch (val) {
        case PHY_IF_GMII:
        case PHY_IF_SGMII:
            return CDK_E_NONE;
        default:
            break;
        }
        break;
    case PhyConfig_Mode:
        if (val == 0) {
            return CDK_E_NONE;
        }
        break;
#if PHY_CONFIG_INCLUDE_XAUI_TX_LANE_MAP_SET
    case PhyConfig_XauiTxLaneRemap: {
        int ioerr = 0;
        int do_swap = 0;
        uint32_t ln_swap;

        if ((val == 0x0123) || (val == 0)) {
            do_swap = 0;
        } else if (val == 0x3210) {
            do_swap = 1;
        } else {
            /* Can't do arbitrary remap */
            break;
        }
        ioerr += PHY_BUS_READ(pc, PHY_XS_XGXS_TX_LANE_SWAP, &ln_swap);
        if (do_swap) {
            ln_swap |= XS_TX_LANE_SWAP;
        } else {
            ln_swap &= ~XS_TX_LANE_SWAP;
        }
        ioerr += PHY_BUS_WRITE(pc, PHY_XS_XGXS_TX_LANE_SWAP, ln_swap);
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
#if PHY_CONFIG_INCLUDE_XAUI_RX_LANE_MAP_SET
    case PhyConfig_XauiRxLaneRemap: {
        int ioerr = 0;
        int do_swap = 0;
        uint32_t ln_swap;

        if ((val == 0x0123) || (val == 0)) {
            do_swap = 0;
        } else if (val == 0x3210) {
            do_swap = 1;
        } else {
            /* Can't do arbitrary remap */
            break;
        }
        ioerr += PHY_BUS_READ(pc, PHY_XS_XGXS_RX_LANE_SWAP, &ln_swap);
        if (do_swap) {
            ln_swap |= XS_RX_LANE_SWAP;
        } else {
            ln_swap &= ~XS_RX_LANE_SWAP;
        }
        ioerr += PHY_BUS_WRITE(pc, PHY_XS_XGXS_RX_LANE_SWAP, ln_swap);
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
#if PHY_CONFIG_INCLUDE_XAUI_TX_POLARITY_SET
    case PhyConfig_XauiTxPolInvert: {
        int ioerr = 0;
        uint32_t tx_invert;
        ioerr += PHY_BUS_READ(pc, PHY_XS_XGXS_TX_POLARITY, &tx_invert);
        if (val) {
            tx_invert |= XS_TX_POLARITY_INVERT;
        } else {
            tx_invert &= ~XS_TX_POLARITY_INVERT;
        }
        ioerr += PHY_BUS_WRITE(pc, PHY_XS_XGXS_TX_POLARITY, tx_invert);
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
#if PHY_CONFIG_INCLUDE_XAUI_RX_POLARITY_SET
    case PhyConfig_XauiRxPolInvert: {
        int ioerr = 0;
        uint32_t rx_invert;
        ioerr += PHY_BUS_READ(pc, PHY_XS_XGXS_RX_POLARITY, &rx_invert);
        if (val) {
            rx_invert |= XS_RX_POLARITY_INVERT;
        } else {
            rx_invert &= ~XS_RX_POLARITY_INVERT;
        }
        ioerr += PHY_BUS_WRITE(pc, PHY_XS_XGXS_RX_POLARITY, rx_invert);
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
#if PHY_CONFIG_INCLUDE_PCS_TX_POLARITY_SET
    case PhyConfig_PcsTxPolInvert: {
        int ioerr = 0;
        uint32_t pcs_invert;
        ioerr += PHY_BUS_READ(pc, PCS_POLARITY, &pcs_invert);
        if (val) {
            pcs_invert |= PCS_TX_POLARITY_INVERT;
        } else {
            pcs_invert &= ~PCS_TX_POLARITY_INVERT;
        }
        ioerr += PHY_BUS_WRITE(pc, PCS_POLARITY, pcs_invert);
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
#if PHY_CONFIG_INCLUDE_PCS_RX_POLARITY_SET
    case PhyConfig_PcsRxPolInvert: {
        int ioerr = 0;
        uint32_t pcs_invert;
        ioerr += PHY_BUS_READ(pc, PCS_POLARITY, &pcs_invert);
        if (val) {
            pcs_invert |= PCS_RX_POLARITY_INVERT;
        } else {
            pcs_invert &= ~PCS_RX_POLARITY_INVERT;
        }
        ioerr += PHY_BUS_WRITE(pc, PCS_POLARITY, pcs_invert);
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
    default:
        break;
    }

    return CDK_E_UNAVAIL;
}

/*
 * Function:
 *      bcm8706_phy_config_get
 * Purpose:
 *      Get PHY configuration value.
 * Parameters:
 *      pc - PHY control structure
 *      cfg - Configuration parameter
 *      val - (OUT) Configuration value
 *      cd - (OUT) Additional configuration data (if any)
 * Returns:
 *      CDK_E_xxx
 */
static int
bcm8706_phy_config_get(phy_ctrl_t *pc, phy_config_t cfg, uint32_t *val, void *cd)
{
    PHY_CTRL_CHECK(pc);

    switch (cfg) {
    case PhyConfig_Enable:
        *val = 1;
        return CDK_E_NONE;
    case PhyConfig_PortInterface:
        *val = PHY_IF_XAUI;
        return CDK_E_NONE;
    case PhyConfig_Mode:
        *val = PHY_MODE_LAN;
        return CDK_E_NONE;
    case PhyConfig_Clause45Devs:
        *val = 0x9a;
        return CDK_E_NONE;
#if PHY_CONFIG_INCLUDE_XAUI_TX_LANE_MAP_SET
    case PhyConfig_XauiTxLaneRemap: {
        int ioerr = 0;
        uint32_t ln_swap;

        ioerr += PHY_BUS_READ(pc, PHY_XS_XGXS_TX_LANE_SWAP, &ln_swap);
        *val = (ln_swap & XS_TX_LANE_SWAP) ? 0x3210 : 0x0123;
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
#if PHY_CONFIG_INCLUDE_XAUI_RX_LANE_MAP_SET
    case PhyConfig_XauiRxLaneRemap: {
        int ioerr = 0;
        uint32_t ln_swap;

        ioerr += PHY_BUS_READ(pc, PHY_XS_XGXS_RX_LANE_SWAP, &ln_swap);
        *val = (ln_swap & XS_RX_LANE_SWAP) ? 0x3210 : 0x0123;
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
#if PHY_CONFIG_INCLUDE_XAUI_TX_POLARITY_SET
    case PhyConfig_XauiTxPolInvert: {
        int ioerr = 0;
        uint32_t tx_invert;

        ioerr += PHY_BUS_READ(pc, PHY_XS_XGXS_TX_POLARITY, &tx_invert);
        *val = (tx_invert & XS_TX_POLARITY_INVERT) ? 1 : 0;
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
#if PHY_CONFIG_INCLUDE_XAUI_RX_POLARITY_SET
    case PhyConfig_XauiRxPolInvert: {
        int ioerr = 0;
        uint32_t rx_invert;

        ioerr += PHY_BUS_READ(pc, PHY_XS_XGXS_RX_POLARITY, &rx_invert);
        *val = (rx_invert & XS_RX_POLARITY_INVERT) ? 1 : 0;
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
#if PHY_CONFIG_INCLUDE_PCS_TX_POLARITY_SET
    case PhyConfig_PcsTxPolInvert: {
        int ioerr = 0;
        uint32_t pcs_invert;

        ioerr += PHY_BUS_READ(pc, PCS_POLARITY, &pcs_invert);
        *val = (pcs_invert & PCS_TX_POLARITY_INVERT) ? 1 : 0;
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
#if PHY_CONFIG_INCLUDE_PCS_RX_POLARITY_SET
    case PhyConfig_PcsRxPolInvert: {
        int ioerr = 0;
        uint32_t pcs_invert;

        ioerr += PHY_BUS_READ(pc, PCS_POLARITY, &pcs_invert);
        *val = (pcs_invert & PCS_RX_POLARITY_INVERT) ? 1 : 0;
        return ioerr ? CDK_E_IO : CDK_E_NONE;
    }
#endif
    default:
        break;
    }

    return CDK_E_UNAVAIL;
}

/*
 * Variable:    bcm8706_drv
 * Purpose:     PHY Driver for BCM8706/BCM8726.
 */
phy_driver_t bcm8706_drv = {
    "bcm8706",
    "BCM8706/BCM8726 10-Gigabit PHY Driver",  
    0,
    bcm8706_phy_probe,                  /* pd_probe */
    bcm8706_phy_notify,                 /* pd_notify */
    bcm8706_phy_reset,                  /* pd_reset */
    bcm8706_phy_init,                   /* pd_init */
    bcm8706_phy_link_get,               /* pd_link_get */
    bcm8706_phy_duplex_set,             /* pd_duplex_set */
    bcm8706_phy_duplex_get,             /* pd_duplex_get */
    bcm8706_phy_speed_set,              /* pd_speed_set */
    bcm8706_phy_speed_get,              /* pd_speed_get */
    bcm8706_phy_autoneg_set,            /* pd_autoneg_set */
    bcm8706_phy_autoneg_get,            /* pd_autoneg_get */
    bcm8706_phy_loopback_set,           /* pd_loopback_set */
    bcm8706_phy_loopback_get,           /* pd_loopback_get */
    bcm8706_phy_ability_get,            /* pd_ability_get */
    bcm8706_phy_config_set,             /* pd_config_set */
    bcm8706_phy_config_get,             /* pd_config_get */
    NULL,                               /* pd_status_get */
    NULL                                /* pd_cable_diag */
};
