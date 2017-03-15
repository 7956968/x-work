//this file is to load system config in system spicified way.eg:cfg;ini with x1000;uci with openwrt;
#include <memory.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include <ini_interface.h>


#define safe_free(p) do{\
    if((p) != NULL)\
    {\
        free((p));\
        (p) = NULL;\
    }\
    }while(0)

void mylogstrTofile(const char*format, ...);

//ini
/////////////////////////////////////////////////////////////////////{
static char* dm_get_ini(char* file, char* section, char* key)
{
	char *value = malloc(64);
	if (value == NULL) {
		mylogstrTofile("%s:malloc failed\n", __FUNCTION__); 
		return NULL;
	}
	value[0] = '\0';
	if (mozart_ini_getkey(file, section, key, value)|| strlen(value) == 0) {
		if (strlen(value) == 0)
			mylogstrTofile("file = %s, section = %s, key = %s, value equals \"\"\n", file, section, key); 
		else
			mylogstrTofile("Could not get ini,file = %s, section = %s, key = %s.\n", file, section, key); 
		safe_free(value);
		return NULL;		
	}
	return value;
}

char* sys_get_fwversion()
{
	return dm_get_ini("/etc/dmconfig.ini", "product", "fw_version");
}


char* sys_get_fwversion2()
{
	return dm_get_ini("/etc/dmconfig.ini", "product", "fw_version2");
}
/////////////////////////////////////////////////////////////////////}

//cfg
/////////////////////////////////////////////////////////////////////{
#define N 128
static void del_n(char *str)
{	
    int i=0;
	while(str[i])
	{
	    if('\n' == str[i])
		{
		    str[i]=0;
			if(i>0 && str[i-1]=='\r')
			str[i-1]=0;		
		}
		i++;
	}
}
//1,ok,0,param not set or wrong param
static char* cfg_get_str(char *param)
{
	int malloc_size = 64;
	char* result = NULL;
	result = malloc(malloc_size);
	if (!result) {
		mylogstrTofile("%s:malloc failed\n", __FUNCTION__); 
		goto err;
	}
	memset(result, 0, malloc_size);
	
    char get_str[N]={0};	
    char tmp[N]={0};	
    FILE *fp;	

	sprintf(get_str,"cfg get \'%s\'",param);	
    fp=popen(get_str,"r");	
	fgets(tmp,N,fp);	
    pclose(fp);	
    del_n(tmp);	
    if(strlen(tmp)<=1) {
		mylogstrTofile("get cfg param[%s] failed!!\n", param);
		goto err;
    }	
    strncpy(result,tmp+strlen(param)+1, malloc_size-1);		
	return result;
err:
	safe_free(result);
	return NULL;
}

//检查mac地址是否合法
static int check_mac(char *mac_addr)
{
	int i=0;
	if(strlen(mac_addr)!=12)
	{
		mylogstrTofile("mac len is not 12! mac=%s", mac_addr); 
		return 1;
	}
	for(i=0;i<12;i++)
	{
		if(!isxdigit(mac_addr[i]))
		{
			mylogstrTofile("mac contain invaild char! mac=%s", mac_addr); 
			return 1;
		}
	}
	return 0;
}

static void mac_turn_upper(char* mac)
{
	int i = 0;
	for (i = 0; i < strlen(mac); ++i) 
		mac[i] = toupper(mac[i]);
	return;
}

char* sys_get_mac()
{
	char* mac = NULL;
	if ( !(mac = cfg_get_str("mac")))
		goto err;
	
	if (check_mac(mac) != 0)
		goto err;
	mac_turn_upper(mac);
	return mac;
err:
	safe_free(mac);
	return NULL;
}

char* sys_get_projectnum()
{
	return cfg_get_str("fw_version");//fw_version in cfg is project num actually
}
char* sys_get_version_flag()
{
	return cfg_get_str("version_flag");
}
/////////////////////////////////////////////////////////////////////}