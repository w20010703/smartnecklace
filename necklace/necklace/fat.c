#include "declaration.h"
#include "fat.h"

FRESULT fat_mount()
{
	FRESULT ret;
	ret = f_mount(&FatFs, "0:", 1);
	return(ret);
}

FRESULT fat_open_file(FIL *fp, char *filename)
{
	FRESULT ret;
	ret = f_open(fp, filename, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
	return(ret);
}

void fat_unmount()
{
	f_mount(0, "0:", 1);
}

void fat_close_file(FIL *fp)
{
	f_close(fp);
}

void fat_printf(FIL *fp, char *str)
{
	f_puts(str, fp);
}

void fat_gets(FIL *fp, char *str)
{
	f_gets(str, 100, fp);
}