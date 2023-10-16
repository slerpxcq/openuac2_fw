#ifndef _AK4490R_H_
#define _AK4490R_H_

#include <stdint.h>

#define AK4490R_I2C_DEV_ADDR       0x20
#define AK4490R_CONTROL1_ADDR      0x00
#define AK4490R_CONTROL2_ADDR      0x01
#define AK4490R_CONTROL3_ADDR      0x02
#define AK4490R_LCH_ATT_ADDR       0x03
#define AK4490R_RCH_ATT_ADDR       0x04
#define AK4490R_CONTROL4_ADDR      0x05
#define AK4490R_DSD1_ADDR          0x06
#define AK4490R_CONTROL5_ADDR      0x07
#define AK4490R_SOUND_CONTROL_ADDR 0x08
#define AK4490R_DSD2_ADDR          0x09
#define AK4490R_CONTROL6_ADDR      0x0a
#define AK4490R_CONTROL7_ADDR      0x0b
#define AK4490R_CONTROL8_ADDR      0x15

// Control 1
#define AK4490R_ACKS  (1U << 7)
#define AK4490R_EXDF  (1U << 6)
#define AK4490R_ECS   (1U << 5)
#define AK4490R_DIF2  (1U << 3)
#define AK4490R_DIF1  (1U << 2)
#define AK4490R_DIF0  (1U << 1)
#define AK4490R_RSTN  (1U << 0)

// Control 2
#define AK4490R_DZFE  (1U << 7)
#define AK4490R_DZFM  (1U << 6)
#define AK4490R_SD 	  (1U << 5)
#define AK4490R_DFS1  (1U << 4)
#define AK4490R_DFS0  (1U << 3)
#define AK4490R_DEM1  (1U << 2)
#define AK4490R_DEM0  (1U << 1)
#define AK4490R_SMUTE (1U << 0)

// Control 3
#define AK4490R_DP    (1U << 7)
#define AK4490R_ADP	  (1U << 6)
#define AK4490R_DCKS  (1U << 5)
#define AK4490R_DCKB  (1U << 4)
#define AK4490R_MONO  (1U << 3)
#define AK4490R_DZFB  (1U << 2)
#define AK4490R_SELLR (1U << 1)
#define AK4490R_SLOW  (1U << 0)

// Control 4
#define AK4490R_INVL  (1U << 7)
#define AK4490R_INVR  (1U << 6)
#define AK4490R_DFS2  (1U << 1)
#define AK4490R_SSLOW (1U << 0)

// DSD1
#define AK4490R_DDM     (1U << 7)
#define AK4490R_DML     (1U << 6)
#define AK4490R_DMR     (1U << 5)
#define AK4490R_DDMOE   (1U << 4)
#define AK4490R_DDMT1   (1U << 3)
#define AK4490R_DDMT0   (1U << 2)
#define AK4490R_DSDD    (1U << 1)
#define AK4490R_DSDSEL0 (1U << 0)

// Control 5
#define AK4490R_MSTBN   (1U << 7)
#define AK4490R_GC2     (1U << 3)
#define AK4490R_GC1     (1U << 2)
#define AK4490R_GC0     (1U << 1)
#define AK4490R_SYNCE   (1U << 0)

// Sound control
#define AK4490R_SC2     (1U << 2)

// DSD2
#define AK4490R_DSDF    (1U << 1)
#define AK4490R_DSDSEL1 (1U << 0)

// Control 6
#define AK4490R_TDM1  (1U << 7)
#define AK4490R_TDM0  (1U << 6)
#define AK4490R_SDS1  (1U << 5)
#define AK4490R_SDS2  (1U << 4)
#define AK4490R_PW    (1U << 2)

// Control 7
#define AK4490R_ATS1  (1U << 7)
#define AK4490R_ATS0  (1U << 6)
#define AK4490R_SDS0  (1U << 4)
#define AK4490R_TEST  (1U << 0)

// Control 8
#define AK4490R_ADPE   (1U << 7)
#define AK4490R_ADPT1  (1U << 6)
#define AK4490R_ADPT0  (1U << 5)

typedef struct __attribute__ ((packed))
{
	uint8_t control1;
	uint8_t control2;
	uint8_t control3;
	uint8_t lch_att;
	uint8_t rch_att;
	uint8_t control4;
	uint8_t dsd1;
	uint8_t control5;
	uint8_t sound_control;
	uint8_t dsd2;
	uint8_t control6;
	uint8_t control7;
	uint8_t reserved[9];
	uint8_t control8;
} AK4490R_RegisterTypeDef;

uint8_t AK4490R_Init();
uint8_t AK4490R_SetVolume(uint8_t vol);

#endif // _AK4490R_H_
