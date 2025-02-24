FRESULT fat_mount();
FRESULT fat_open_file(FIL *fp, char *filename);
void fat_unmount();
void fat_close_file(FIL *fp);
void fat_printf(FIL *fp, char *str);