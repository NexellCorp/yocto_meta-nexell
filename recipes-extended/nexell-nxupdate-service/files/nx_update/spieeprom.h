
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
//#include <linux/spi/spidev.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>

#include "spidev.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define	READ_CMD		0x03
#define	WRITE_CMD		0x02
#define	WRITE_ENABLE	0x06
#define	RDSR			0x05

#define SECTOR_REASE		0x20
#define	BLOCK_32K_ERASE		0x52
#define	BLOCK_64K_ERASE		0xd8

#define	ADDR_BYTE	3
#define	CMD_LEN		1

#define	CMD_BUF ADDR_BYTE + CMD_LEN

#define PAGE_SIZE 256

#define WIP		1<<1
#define	BUSY	1<<1
#define WEL 	1<<2

#define RDSR_MAX_CNT	50

int eeprom_init(int fd);
int eeprom_read(int fd, int offset, char * buf, int size);	//offset : 24bit addr read start addr
int eeprom_blk_erase(int fd, char cmd, int offset);						//erase Block 64K
int eeprom_write_enable(int fd);
int eeprom_read_rdsr(int fd);
int eeprom_write(int fd ,int dst, char * buf, int size);
