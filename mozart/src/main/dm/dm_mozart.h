#ifndef DMMOZART_H
#define DMMOZART_H

#define c_Print_Ctrl_Off "\033[0m"
#define c_CharColor_yellow      "\033[1;33m"

#define dm_info(fmt, args...)	fprintf(stdout, c_CharColor_yellow"%s.%s [info] "fmt""c_Print_Ctrl_Off,__FILE__, __func__, ##args)
int check_play_status();
int dm_set_airkissing(int i);
void dm_playpause();
void dm_play();
void dm_pause();
int in_backup_system();
int domain_playing();
int in_backup_system();
int test_domain(char*);

#define safe_free(p) do{\
    if((p) != NULL)\
    {\
        free((p));\
        (p) = NULL;\
    }\
    }while(0)

#endif
