/*
 * cpu_cfg.h
 *
 *  Created on: 2012-02-20
 *      Author: kuu
 */

#ifndef CPU_CFG_H_
#define CPU_CFG_H_

#define	CPU_CFG_NAME_EN					DEF_ENABLED
#define CPU_CFG_NAME_SIZE				16u
#define CPU_CFG_TS_EN					DEF_ENABLED
#define CPU_CFG_TS_TMR_EN				DEF_ENABLED
#define CPU_CFG_TS_32_EN				DEF_ENABLED
#define CPU_CFG_TS_64_EN				DEF_DISABLED
#define CPU_CFG_TS_TMR_SIZE				CPU_WORD_SIZE_32

#define CPU_CFG_INT_DIS_MEAS_EN			DEF_DISABLED /* DEF_ENABLED */
#define CPU_CFG_INT_DIS_MEAS_OVRHD_NBR	1u

#endif /* CPU_CFG_H_ */
