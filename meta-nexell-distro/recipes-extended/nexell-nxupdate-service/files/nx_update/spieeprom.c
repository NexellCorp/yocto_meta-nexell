#include "spieeprom.h"

static uint8_t mode = SPI_MODE_3 | SPI_CPOL | SPI_CPHA;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay = 0;

void pabort(char *s)
{
	perror(s);
	/*abort();*/
}

int eeprom_init(int fd)
{
	int ret;
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
	{
		perror("can't set spi mode");
		return ret;
	}
	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
	{
		perror("can't get spi mode");
		return ret;
	}
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		perror("can't set max speed hz");
		return ret;
	}
	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		perror("can't get max speed hz");
		return ret;
	}
	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

	return ret;
}

int eeprom_read(int fd,int offset, char * buf, int size)
{
    int ret;

    uint8_t * tx;
    uint8_t * rx;

    tx = (uint8_t *)malloc(size+CMD_BUF);
    rx = (uint8_t *)malloc(size+CMD_BUF);
    memset ( rx , 0xcc,size+CMD_BUF );
    memset ( tx , 0x0a,size+CMD_BUF );
    tx[0] =  READ_CMD;

    if(ADDR_BYTE == 3)
    {
        tx[1] = (offset >> 16);
        tx[2] = (offset >> 8);
        tx[3] = (offset );
    }
    else if(ADDR_BYTE == 2)
    {
        tx[1] = (offset >> 8);
        tx[2] = (offset);
    }
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = size + CMD_BUF,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret == 1)
    {
        perror("can't send spi message");
        return -1;
    }
    memcpy(buf, rx+CMD_BUF,size);
    free(tx);
    free(rx);
    return ret;
}
int eeprom_read_rdsr(int fd)
{
    int ret;
    uint8_t tx[4]={0,0,0,0};
    uint8_t rx[4];

    tx[0] =  RDSR;

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = ARRAY_SIZE(tx),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret == 1)
    {
        perror("can't send spi message");
        return -1;
    }

    return rx[1];

}

int eeprom_blk_erase(int fd, char cmd, int offset)
{
    int ret,cnt = RDSR_MAX_CNT;
    uint8_t tx[4];
    uint8_t rx[4];

    tx[0] =  cmd;

    if(ADDR_BYTE == 3)
    {
        tx[1] = (offset >> 16);
        tx[2] = (offset >> 8);
        tx[3] = (offset );
    }
    else if(ADDR_BYTE == 2)
    {
        tx[1] = (offset >> 8);
        tx[2] = (offset);
    }

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,

        .len = ARRAY_SIZE(tx),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret == 1)
    {
        return -1;
    }

    do{
    usleep(1000);
    ret = eeprom_read_rdsr(fd);
    ret = ret & BUSY;
    }   while(ret != 0 && cnt--);

    return 0;
}

int eeprom_write_enable(int fd)
{
    int ret;
    uint8_t tx[4] ={ 6,6};
    uint8_t rx[4] = {0xaa,0xaa,};

    int cnt = RDSR_MAX_CNT;
    tx[0] =  WRITE_ENABLE;
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 1,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

     do{
         usleep(10000);
         ret = eeprom_read_rdsr(fd);
     }   while(ret != 2  && cnt--);

    return ret;

}

int eeprom_write(int fd, int dst, char * buf, int size)
{
	int ret,offset=0,cnt = RDSR_MAX_CNT;
	uint8_t tx[PAGE_SIZE+CMD_BUF];
	uint8_t rx[PAGE_SIZE+CMD_BUF];

	int write_size =0;

	tx[0] =  WRITE_CMD;

	do{
		eeprom_write_enable(fd);
		if(size > PAGE_SIZE)
		{
			write_size = PAGE_SIZE - (dst%PAGE_SIZE);
		}
		else
		{
			if((PAGE_SIZE - (dst%PAGE_SIZE))  > size)
				write_size = size;
			else
				write_size = (PAGE_SIZE - (dst%PAGE_SIZE));
		}

		if(ADDR_BYTE == 3)
		{
			tx[1] = (dst >> 16);
			tx[2] = (dst >> 8);
			tx[3] = (dst );
		}
		else if(ADDR_BYTE == 2)
		{
			tx[1] = (dst >> 8);
			tx[2] = (dst );
		}

		memcpy(tx+CMD_BUF,&buf[offset],write_size);

		struct spi_ioc_transfer tr = {
			.tx_buf = (unsigned long)tx,
			.rx_buf = (unsigned long)rx,
			.len = write_size+CMD_BUF,
			.delay_usecs = delay,
			.speed_hz = speed,
			.bits_per_word = bits,
		};

		ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
		if (ret == 1)
		{
			perror("can't send spi message");
			return -1;
		}

		size -= write_size;
		offset += write_size;
		dst += write_size;

		do{
			usleep(1000);
			ret = eeprom_read_rdsr(fd);
			ret = ret & (BUSY|WEL);
		}while(ret != 0 && cnt-- );

	}while(size > 0);

	return 0;

}
