#ifndef		__DEF_H__
#define		__DEF_H__

//#define DEBUG
#if defined (DEBUG)
#define debug(msg...)   { printf(msg); }
#else
#define debug(msg...)   do{} while (0)
#endif


#define PARTMAP_UPDATE  "partmap_update.txt"
#define UPDATE_CMD      "update_cmd.txt"
#define UPDATE_DEV_PART_MAX  (20)

/* device types */
#define UPDATE_DEV_EEPROM    (1<<0)  /*  name "eeprom" */
#define UPDATE_DEV_NAND      (1<<1)  /*  name "nand" */
#define UPDATE_DEV_MMC       (1<<2)  /*  name "mmc" */
#define UPDATE_DEV_MEM       (1<<3)  /*  name "mem" */

/* filesystem types */
#define UPDATE_FS_2NDBOOT    (1<<0)  /*  name "boot" <- bootable */
#define UPDATE_FS_BOOT       (1<<1)  /*  name "boot" <- bootable */
#define UPDATE_FS_RAW        (1<<2)  /*  name "raw" */
#define UPDATE_FS_FAT        (1<<4)  /*  name "fat" */
#define UPDATE_FS_EXT4       (1<<5)  /*  name "ext4" */
#define UPDATE_FS_UBI        (1<<6)  /*  name "ubi" */
#define UPDATE_FS_UBIFS      (1<<7)  /*  name "ubifs" */
#define UPDATE_FS_RAW_PART   (1<<8)  /*  name "emmc" */
#define UPDATE_FS_ENV		 (1<<9)  /*  name "env" */

#define UPDATE_FS_MASK       (UPDATE_FS_RAW | UPDATE_FS_FAT | UPDATE_FS_EXT4 | UPDATE_FS_UBI | UPDATE_FS_UBIFS | UPDATE_FS_RAW_PART)

#define BLK_SIZE			512
#define RW_SIZE				4096

#define	MMC_DEV_NAME		"/dev/mmcblk0"
#define	MMC_TEMP			"mmc_cap"
#define	PROC_MOUNT			"/proc/mounts"
#define	PROC_TEMP			"/tmp/mounts_tmp"
#define	SFDISK_TEMP			"sfdisk_tmp"
#define	UBOOT_TEMP			"/tmp/uboot_tmp"
#define	DDPT				"/usr/bin/ddpt"
#define	PSPLASH				"/usr/bin/psplash-write"
#define	MKFS_EXT4			"/sbin/mkfs.ext4"

/* EEPROM defines */
#define SECBOOT_OFFSET 0
#define SECBOOT_SIZE 16 * 1024

#define UBOOT_OFFSET 64 * 1024
#define UBOOT_SIZE (512-64) * 1024
#define TEXT_BASE 0x40c00000
#define CONFIG_EEPROM_ADDRESS_STEP     3

#define ERASE_BLOCK_SIZE    32*1024

#define POLY 0x04C11DB7L

#define SPIROM_DEVICE "/dev/spidev0.0"


struct update_fs_type {
    char *name;
    unsigned int fs_type;
};

struct update_part {
    char device[32];
    int dev_no;
    char partition_name[32];
    unsigned int fs_type;
    unsigned long long start;
    unsigned long long length;
    char file_name[32];
    int part_num;
};

struct sfdisk_part {
	unsigned long long start_sector;
	unsigned long long sector_size;
	unsigned int	id;
};


struct boot_dev_eeprom {
    char    addr_step;
    char    resv0[3];
    unsigned int resv1;
    unsigned int crc32;
};

struct boot_dev_mmc {
    char port_no;
    char resv0[3];
    char rese1;
    unsigned int crc32;        /* not use : s5p6818 */
};

union boot_dev_data {
    struct boot_dev_eeprom spirom;
    struct boot_dev_mmc mmc;
};

#define U_BOOT_LOAD_ADDRESS 0x43C00000
//#define U_BOOT_LOAD_ADDRESS 0x88800000
#define SIGNATURE_ID        ((((u_int32_t)'N')<< 0) | (((u_int32_t)'S')<< 8) | (((u_int32_t)'I')<<16) | (((u_int32_t)'H')<<24))

struct boot_dev_head {
#if 1
    unsigned int vector[8];                     // 0x000 ~ 0x01C
    unsigned int vector_rel[8];                 // 0x020 ~ 0x03C
    unsigned int dev_addr;                      // 0x040

    /* boot device info */
    unsigned int  load_size;                    // 0x044
    unsigned int  load_addr;                    // 0x048
    unsigned int  jump_addr;                    // 0x04C
    union boot_dev_data bdi;                    // 0x050~0x058

    unsigned int  resv_pll[4];                  // 0x05C ~ 0x068
    unsigned int  resv_pll_spread[2];           // 0x06C ~ 0x070
    unsigned int  resv_dvo[5];                  // 0x074 ~ 0x084
    char resv_ddr[36];                          // 0x088 ~ 0x0A8
    unsigned int  resv_axi_b[32];               // 0x0AC ~ 0x128
    unsigned int  resv_axi_d[32];               // 0x12C ~ 0x1A8
    unsigned int  resv_stub[(0x1F8-0x1A8)/4];   // 0x1AC ~ 0x1F8
    unsigned int  signature;                    // 0x1FC    "NSIH"
#else
    uint32_t vector[8];						/* 0x000 ~ 0x01f */
    uint32_t vector_rel[8];					/* 0x020 ~ 0x03f */

    uint32_t _reserved0[4];					/* 0x040 ~ 0x04f */

    uint32_t loadsize;						/* 0x050 */
    uint32_t crc32;							/* 0x054 */
    uint64_t loadaddr;						/* 0x058 ~ 0x05f */
    uint64_t startaddr;						/* 0x060 ~ 0x067 */

    uint8_t unified;						/* 0x068 */
    uint8_t bootdev;						/* 0x069 */
    uint8_t _reserved1[6];					/* 0x06a ~ 0x06f */

    uint8_t validslot[4];					/* 0x070 ~ 0x073 */
    uint8_t loadorder[4];					/* 0x074 ~ 0x077 */

    uint32_t _reserved2[2];					/* 0x078 ~ 0x07f */

    union nx_devicebootinfo dbi[4];			/* 0x080 ~ 0x0ff */

    uint32_t pll[8];						/* 0x100 ~ 0x11f */
    uint32_t pllspread[8];					/* 0x120 ~ 0x13f */

    uint32_t dvo[12];						/* 0x140 ~ 0x16f */

    struct nx_ddrinitinfo dii;				/* 0x170 ~ 0x19f */

    union nx_ddrdrvrsinfo sdramdrvr;		/* 0x1a0 ~ 0x1a7 */

    struct nx_ddrphy_drvdsinfo phy_dsinfo;	/* 0x1a8 ~ 0x1b7 */

    uint16_t lvltr_mode;					/* 0x1b8 ~ 0x1b9 */
    uint16_t flyby_mode;					/* 0x1ba ~ 0x1bb */

    uint8_t _reserved3[15*4];				/* 0x1bc ~ 0x1f7 */

    /* version */
    uint32_t buildinfo;						/* 0x1f8 */

    /* "NSIH": nexell system infomation header */
    uint32_t signature;						/* 0x1fc */
#endif
};


/**
 * this code from u-boot/common/decompress_ext4.h,c
 */
/**
 * this code from u-boot/common/decompress_ext4.h,c
*/
typedef struct ext4_file_header {
    unsigned int magic;
    unsigned short major;
    unsigned short minor;
    unsigned short file_header_size;
    unsigned short chunk_header_size;
    unsigned int block_size;
    unsigned int total_blocks;
    unsigned int total_chunks;
    unsigned int crc32;
} ext4_file_header;

typedef struct ext4_chunk_header {
    unsigned short type;
    unsigned short reserved;
    unsigned int chunk_size;
    unsigned int total_size;
} ext4_chunk_header;

#define EXT4_FILE_HEADER_MAGIC  0xED26FF3A
#define EXT4_FILE_HEADER_MAJOR  0x0001
#define EXT4_FILE_HEADER_MINOR  0x0000
#define EXT4_FILE_BLOCK_SIZE    0x1000

#define EXT4_FILE_HEADER_SIZE   (sizeof(struct ext4_file_header))
#define EXT4_CHUNK_HEADER_SIZE  (sizeof(struct ext4_chunk_header))

#define EXT4_CHUNK_TYPE_RAW     0xCAC1
#define EXT4_CHUNK_TYPE_FILL    0xCAC2
#define EXT4_CHUNK_TYPE_NONE    0xCAC3
#define SECTOR_BITS             9   /* 512B */


#endif /* __DEF_H__ */
