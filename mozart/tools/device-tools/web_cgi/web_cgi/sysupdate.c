#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "cgiget.h"
#define FW_DEV "/dev/mtdblock5"
#define FW_FILE "/tmp/$$update$$.bin"
#define FW_LEN 8126464
#define BUF_MAX 0x80000
#define INVALID_FW 200

//////////////////////////////////
static int CheckProgressCount(const char *progress)
{
	FILE *pp;
	char buf[64] = {0};
	char cmd[128] = {0};
	//char *str1;
	int find=0;
	
	//buf=malloc(MAX_LEN);
	snprintf(cmd,sizeof(cmd),"ps -w |grep %s | grep -v grep | wc -l",progress);
	
	if((pp=popen(cmd,"r"))==NULL)
	{
		perror("popen");
		//free(buf);
		return -1;
	}
	if(fgets(buf,64,pp)!=NULL)
	    find = atoi(buf);
	
	pclose(pp);
	
	//free(buf);
	return find;
}
////////////////////////////////////////////

void sysupdate_log(const char*format, ...)
{	
	FILE* fd = NULL;
	fd = fopen("/tmp/log_sysupdate", "a+");
	if (fd == NULL) {
		system("touch /tmp/log_sysupdate_error> /dev/null");
		return ;
	}
    va_list args;
    va_start(args, format);
    vfprintf(fd, format, args);
    va_end(args);
    fclose(fd);
    return;
}

int main(int argc, char const *argv[])
{
	int status = 0 ;	
	sysupdate_log("start sysupdate\n");
/*
	FILE *fw_fp=NULL;
	FILE *fp_dev=NULL;
	unsigned char * fw_ptr;
	int f_size=0;
	int fd;*/
	char *cgistr;
/*
	int nor_seek;
	int file_seek;
	int nor_size;
	int len;
	char teststr[100];*/
	fprintf(stdout,"Content-type:text/html\r\n\r\n");
	
	cgistr = GetStringFromWeb();  

	if (access(FW_FILE, F_OK) !=0) {
		sysupdate_log(FW_FILE" is not exist, exiting\n");
		return -1;
	}
	if (CheckProgressCount("updatefw") > 0) {
		sysupdate_log("another updatefw process has started, kill it\n");
		if (system("killall updatefw") == -1)
			sysupdate_log("system-killall failed, error\n");
	}
	if ( (status = system("updatefw -d /tmp &> /tmp/updatefw_log")) == -1) {
		sysupdate_log("system-updatefw failed, error\n");
		fprintf(stdout,"error");
		return -1;
	}
	return 0;
}
