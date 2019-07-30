/***************************************************************************/
/*                                                                         */
/*                         HARDware PCEngine                               */
/*                                                                         */
/* This header deals with definition of structure and functions used to    */
/* handle the pc engine hardware in itself (RAM, IO ports, ...) which were */
/* previously found in pce.h                                               */
/*                                                                         */
/***************************************************************************/
#ifndef _INCLUDE_HARD_PCE_H
#define _INCLUDE_HARD_PCE_H

#include <stdio.h>

#include "config.h"
#include "cleantypes.h"

#if defined(SHARED_MEMORY)
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#endif

#define PSG_VOICE_REG           0	/* voice index */

#define PSG_VOLUME_REG          1	/* master volume */

#define PSG_FREQ_LSB_REG        2	/* lower 8 bits of 12 bit frequency */

#define PSG_FREQ_MSB_REG        3	/* actually most significant nibble */

#define PSG_DDA_REG             4
#define PSG_DDA_ENABLE          0x80	/* bit 7 */
#define PSG_DDA_DIRECT_ACCESS   0x40	/* bit 6 */
#define PSG_DDA_VOICE_VOLUME    0x1F	/* bits 0-4 */

#define PSG_BALANCE_REG         5
#define PSG_BALANCE_LEFT        0xF0	/* bits 4-7 */
#define PSG_BALANCE_RIGHT       0x0F	/* bits 0-3 */

#define PSG_DATA_INDEX_REG      6

#define PSG_NOISE_REG           7
#define PSG_NOISE_ENABLE        0x80	/* bit 7 */

/**
  * Exported functions to access hardware
  **/

void hard_init(void);
void hard_term(void);

void IO_write(uint16 A, uchar V);
uchar IO_read(uint16 A);
void bank_set(uchar P, uchar V);

extern void (*write_memory_function) (uint16, uchar);
extern uchar(*read_memory_function) (uint16);

#define Wr6502(A,V) ((*write_memory_function)((A),(V)))

#define Rd6502(A) ((*read_memory_function)(A))

void dump_pce_cpu_environment();

/**
  * Global structure for all hardware variables
  **/

#include "shared_memory.h"

/**
  * Exported variables
  **/

extern struct_hard_pce *hard_pce;
// The global structure for all hardware variables

#define io (*p_io)

extern IO *p_io;
// the global I/O status

extern uchar *RAM;
// mem where variables are stocked (well, RAM... )
// in reality, only 0x2000 bytes are used in a coregraphx and 0x8000 only
// in a supergraphx

extern uchar *WRAM;
// extra backup memory
// This memory lies in Interface Unit or eventually in RGB adaptator

extern uchar *VRAM;
// Video mem
// 0x10000 bytes on coregraphx, the double on supergraphx I think
// contain information about the sprites position/status, information
// about the pattern and palette to use for each tile, and patterns
// for use in sprite/tile rendering

extern uint16 *SPRAM;
// SPRAM = sprite RAM
// The pc engine got a function to transfert a piece VRAM toward the inner
// gfx cpu sprite memory from where data will be grabbed to render sprites

extern uchar *Pal;
// PCE->PC Palette convetion array
// Each of the 512 available PCE colors (333 RGB -> 512 colors)
// got a correspondancy in the 256 fixed colors palette

extern uchar *VRAM2, *VRAMS;
// These are array to keep in memory the result of the linearisation of
// PCE sprites and tiles

extern uchar *vchange, *vchanges;
// These array are boolean array to know if we must update the
// corresponding linear sprite representation in VRAM2 and VRAMS or not
// if (vchanges[5] != 0) 6th pattern in VRAM2 must be updated

#define scanline (*p_scanline)

extern uint32 *p_scanline;
// The current rendered line on screen

extern uchar *PCM;
// The ADPCM array (0x10000 bytes)

//! A pointer to know where we're currently reading data in the cd buffer
extern uchar *cd_sector_buffer;

//! The real buffer into which data are written from the cd and in which we
//! takes data to gives it back throught the cd ports
extern uchar *cd_read_buffer;

//! extra ram provided by the system CD card
extern uchar *cd_extra_mem;

//! extra ram provided by the super system CD card
extern uchar *cd_extra_super_mem;

//! extra ram provided by the Arcade card
extern uchar *ac_extra_mem;

//! remaining useful data in cd_read_buffer
extern uint32 pce_cd_read_datacnt;

//! number of sectors we must still read on cd
extern uchar cd_sectorcnt;

//! number of the current command of the cd interface
extern uchar pce_cd_curcmd;

extern uchar *zp_base;
// pointer to the beginning of the Zero Page area

extern uchar *sp_base;
// pointer to the beginning of the Stack Area

extern uchar *mmr;
// Value of each of the MMR registers

extern uchar *IOAREA;
// physical address on emulator machine of the IO area (fake address as it has to be handled specially)

//! 
extern uchar *PageR[8];
extern uchar *ROMMapR[256];

extern uchar *PageW[8];
extern uchar *ROMMapW[256];

//! False "ram"s in which you can read/write (to homogeneize writes into RAM, BRAM, ... as well as in rom) but the result isn't coherent
extern uchar *trap_ram_read;
extern uchar *trap_ram_write;

// physical address on emulator machine of each of the 256 banks

#define cyclecount (*p_cyclecount)

extern uint32 *p_cyclecount;
// Number of elapsed cycles

#define cyclecountold (*p_cyclecountold)

extern uint32 *p_cyclecountold;
// Previous number of elapsed cycles

#define external_control_cpu (*p_external_control_cpu)

extern int32 *p_external_control_cpu;

extern const uint32 TimerPeriod;
// Base period for the timer

// registers:

#if defined(SHARED_MEMORY)

#define reg_pc	(*p_reg_pc)
#define reg_a   (*p_reg_a)
#define reg_x   (*p_reg_x)
#define reg_y   (*p_reg_y)
#define reg_p   (*p_reg_p)
#define reg_s   (*p_reg_s)

extern uint16 *p_reg_pc;
extern uchar *p_reg_a;
extern uchar *p_reg_x;
extern uchar *p_reg_y;
extern uchar *p_reg_p;
extern uchar *p_reg_s;

#else
extern uint16 reg_pc;
extern uchar reg_a;
extern uchar reg_x;
extern uchar reg_y;
extern uchar reg_p;
extern uchar reg_s;
#endif

// These are the main h6280 register, reg_p is the flag register

#define cycles (*p_cycles)

extern uint32 *p_cycles;
// Number of pc engine cycles elapsed since the resetting of the emulated console

/**
  * Definitions to ease writing
  **/

#define	VRR	2
enum _VDC_REG {
	MAWR,						/*  0 *//* Memory Address Write Register */
	MARR,						/*  1 *//* Memory Adress Read Register */
	VWR,						/*  2 *//* VRAM Read Register / VRAM Write Register */
	vdc3,						/*  3 */
	vdc4,						/*  4 */
	CR,							/*  5 *//* Control Register */
	RCR,						/*  6 *//* Raster Compare Register */
	BXR,						/*  7 *//* Horizontal scroll offset */
	BYR,						/*  8 *//* Vertical scroll offset */
	MWR,						/*  9 *//* Memory Width Register */
	HSR,						/*  A *//* Unknown, other horizontal definition */
	HDR,						/*  B *//* Horizontal Definition */
	VPR,						/*  C *//* Higher byte = VDS, lower byte = VSW */
	VDW,						/*  D *//* Vertical Definition */
	VCR,						/*  E *//* Vertical counter between restarting of display */
	DCR,						/*  F *//* DMA Control */
	SOUR,						/* 10 *//* Source Address of DMA transfert */
	DISTR,						/* 11 *//* Destination Address of DMA transfert */
	LENR,						/* 12 *//* Length of DMA transfert */
	SATB						/* 13 *//* Adress of SATB */
};

#define	NODATA	   0xff
#define	ENABLE	   1
#define	DISABLE	   0

#define AC_ENABLE_OFFSET_BASE_6 0x40
#define AC_ENABLE_OFFSET_BASE_A 0x20
#define AC_INCREMENT_BASE 0x10
#define AC_USE_OFFSET 0x02
#define AC_ENABLE_INC 0x01

#endif
