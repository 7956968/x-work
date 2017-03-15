#include <unistd.h> 
#include <stdio.h> 
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>             
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>           
#include <sys/stat.h>
#include <signal.h>
#include <stdarg.h>
#include <string.h>

#include "ini_interface.h"

#define FW_FILE "/tmp/$$update$$.bin"
#define FW_LEN 55574528
//#define FW_FILE "/mnt/disk-4/fwupgrade"
#define WINIE6_STR	"/octet-stream\x0d\x0a\0x0d\0x0a"
#define LINUXFX36_FWSTR "/x-ns-proxy-autoconfig\x0d\x0a\0x0d\0x0a"
#define MACIE5_FWSTR	"/macbinary\x0d\x0a\0x0d\0x0a"
#define OPERA_FWSTR	"/x-macbinary\x0d\x0a\0x0d\0x0a"
#define WIN64IE_FWSTR	"/plain\x0d\x0a\0x0d\0x0a"
#define LINE_FWSTR	"\x0d\x0a\0x0d\0x0a"

#define GZ_STR	"/x-gzip-compressed\x0d\x0a\0x0d\0x0a"   //22
#define CONF_HEADER                     ((char *)"conf")

#define safe_free(p) do{\
    if((p) != NULL)\
    {\
        free((p));\
        (p) = NULL;\
    }\
    }while(0)

static void log_sysupgrade(const char*format, ...)
{		
	FILE* fd = NULL;
	fd = fopen("/tmp/log_sysupgrade", "a+");
	if (fd == NULL) {
		system("touch /tmp/log_sysupgrade_error> /dev/null");
		return ;
	}
    va_list args;
    va_start(args, format);
    vfprintf(fd, format, args);
    va_end(args);
    fclose(fd);
    return;
}


int find_head_offset(char *upload_data)
{
	int head_offset=0 ;
	char *pStart=NULL;
	int iestr_offset=0;
	char *dquote;
	char *dquote1;
	
	if (upload_data==NULL) {
		//fprintf(stderr, "upload data is NULL\n");
		return -1;
	}
    if(strstr(upload_data, WINIE6_STR))
    {
       pStart = strstr(upload_data, WINIE6_STR);
       iestr_offset = 17;
	}else if(strstr(upload_data, LINUXFX36_FWSTR))
	{
	   pStart = strstr(upload_data, LINUXFX36_FWSTR);
       iestr_offset = 26;
	}else if(strstr(upload_data, MACIE5_FWSTR))
	{
	  pStart = strstr(upload_data, MACIE5_FWSTR);
      iestr_offset = 14;
	}else if(strstr(upload_data, OPERA_FWSTR))
	{
	  pStart = strstr(upload_data, OPERA_FWSTR);
      iestr_offset = 16;
	}else if(strstr(upload_data, WIN64IE_FWSTR))
	{
	  pStart = strstr(upload_data, WIN64IE_FWSTR);
	  log_sysupgrade("\nWIN64IE_FWSTR:");
	  log_sysupgrade(WIN64IE_FWSTR);
      iestr_offset = 10;
	}else if(strstr(upload_data, "filename="))
	{
	    pStart = strstr(upload_data, "filename=");
        dquote =  strstr(pStart, "\"");
		if (dquote !=NULL) {
			dquote1 = strstr(dquote, LINE_FWSTR);
			if (dquote1!=NULL) {
				iestr_offset = 4;
				pStart = dquote1;
			}
			else {
				return -1;
			}
		}
	}
	if (!pStart) {
		log_sysupgrade("not found any header info\n");
		int i = 0;
		for (i = 0; i < 20; ++ i)
			log_sysupgrade("%02x ", upload_data[i]);
		return 0;
	}
    //fprintf(stderr,"####%s:%d %d###\n",  __FILE__, __LINE__ , iestr_offset);
	head_offset = (int)(((unsigned long)pStart)-((unsigned long)upload_data)) + iestr_offset;
	//printf("<br>head_offset=%d \n",head_offset);
	return head_offset;
}

int check_fw_sum(unsigned char *buf, int len)
{
	unsigned int dec;
	unsigned int dectmp;
	unsigned int *ibuf;
	unsigned int headlen;
	unsigned int i;
	
	if(len%4 != 0)
		return 0;
	dec = *(unsigned int *)(buf+(0x7c0000-4));
	if(dec==0xffffffff)
		return 1;
	ibuf = (unsigned int *)(buf);
	dectmp = 0;
	headlen=0x7c0000-4;
	for(i=0; i<headlen/4; i++)
	{
		dectmp ^= *(ibuf+i);
	}
	if(dectmp != dec)
		return 0;

	return 1;
}

//ini
/////////////////////////////////////////////////////////////////////{
static char* dm_get_ini(char* file, char* section, char* key)
{
	char *value = malloc(64);
	if (value == NULL) {
		log_sysupgrade("%s:malloc failed\n", __FUNCTION__); 
		return NULL;
	}
	value[0] = '\0';
	if (mozart_ini_getkey(file, section, key, value)|| strlen(value) == 0) {
		if (strlen(value) == 0)
			log_sysupgrade("file = %s, section = %s, key = %s, value equals \"\"\n", file, section, key); 
		else
			log_sysupgrade("Could not get ini,file = %s, section = %s, key = %s.\n", file, section, key); 
		safe_free(value);
		return NULL;		
	}
	return value;
}

char* sys_get_productmodel()
{
	return dm_get_ini("/etc/dmconfig.ini", "product", "product_model");
}

//lt 0:error; eq 0:not match;gt 0: match
int fwmodel_match()
{
	system("rm /tmp/updatedir -rf; cd /tmp/ && tar -zxvf '"FW_FILE"' updatedir/project_num >>/tmp/log_sysupgrade");
	if (access("/tmp/updatedir/project_num", F_OK) !=0) {
		log_sysupgrade("tar zxvf file for  project_num file failed\n");
		return -1;
	}
	char project_num[32] = {0};
	char* npos = NULL;
	FILE* p_f = NULL;
    if ( (p_f = fopen("/tmp/updatedir/project_num", "r")) == NULL) {
        log_sysupgrade("open file %s to read failed, %s\n", project_num, strerror(errno) );
        return -1;
    }
	fgets(project_num, 31, p_f);
	if ( (npos = strchr(project_num, '\n')) != NULL) 
		npos[0] = '\0';
	
	char* my_project_num = sys_get_productmodel();
	if (!my_project_num)
		return -1;
	if (strncmp(my_project_num, project_num, 32) != 0) {
		log_sysupgrade("project num not match,my:%s bin:%s\n", my_project_num, project_num);
		return 0;
	}
	fclose(p_f);
	safe_free(my_project_num);
	return 1;
}

int main()
{
	FILE *fw_fp;
	int fw_len;
//	unsigned char * fw_ptr;
	int fw_head_offset=0;
	int f_size=0;
	
	fprintf(stdout,"Content-type:text/html\r\n\r\n");
	//fprintf(stdout,"nidaye");
	log_sysupgrade("\n\n\n\nsysupgrade start\n");
	system("rm -f '"FW_FILE"'");
	system("sync");
	sleep(1);
	system("echo 3 > /proc/sys/vm/drop_caches");
	sleep(1);
	
	if( (fw_fp=fopen(FW_FILE,"wb+"))==NULL)    // write and read,binary
	{
	    log_sysupgrade("open FW_FILE to write failed!\n");
		printf("there is a error,please reboot the device and upgrade again! <br>");
		fprintf(stdout,"</body>\n");
		fprintf(stdout,"\n</html>\n");
		exit(1);
	}
	
	log_sysupgrade("\nCONTENT_LENGTH:");
	log_sysupgrade(getenv("CONTENT_LENGTH"));
	log_sysupgrade("\n");
	fw_len=atoi(getenv("CONTENT_LENGTH"));
	unsigned char *	fw_ptr=(unsigned char *)malloc(sizeof(unsigned char)*fw_len);
	if(fw_ptr==NULL)
	{
	    log_sysupgrade("fw_ptr==NULL\n");
		printf("there is a error,please reboot the device and upgrade again! <br>");
		goto error_exit;
	}
	//printf("<br> fw_len =%d <br>",fw_len);
	f_size=fread(fw_ptr,1,fw_len,stdin);
	if(f_size!=fw_len)
	{
	    log_sysupgrade("f_size!=fw_len f_size=%d, fw_len=%d\n", f_size, fw_len);
		printf("there is a error,please reboot the device and upgrade again! <br>");
		goto error_exit;
	}
	

	f_size=0;
	fw_head_offset=find_head_offset(fw_ptr);
	if(fw_head_offset==-1)
	{
	    log_sysupgrade("fw_head_offset==-1\n");
		printf("there is a error,please reboot the device and upgrade again! <br>");
		goto error_exit;
	}
	unsigned char* header = fw_ptr+fw_head_offset;


	f_size=fwrite(header,1,(fw_len-fw_head_offset),fw_fp);
	if(f_size!=(fw_len-fw_head_offset))

	{
	    log_sysupgrade("f_size!=true_len, f_size=%d fw_len=%d, head_offset=%d, truelen=%d\n", f_size, fw_len, fw_head_offset, (fw_len-fw_head_offset));
		printf("there is a error,please reboot the device and upgrade again! <br>");
		goto error_exit;
	}else log_sysupgrade("write ok");

	log_sysupgrade("\nfree close\n");
	if(fw_ptr != NULL)
	{
		free(fw_ptr);
		fw_ptr=NULL;
	}
	fclose(fw_fp);
	log_sysupgrade("\ndone");
	
	if (fwmodel_match() <= 0)
		fprintf(stdout,"fwmodel_mismatch");
	else
		fprintf(stdout,"nidaye");

	return 0;
error_exit:
	system("rm -f /tmp/fwupgrade");
	system("sync");
	free(fw_ptr);
	fprintf(stdout,"</body>\n");
	fprintf(stdout,"\n</html>\n");
	return 1;
	
}
