/**************************************************************
* cfg用来设置出厂配置，并对参数进行加密
* cfg set mac=845dd7001122
* cfg set ssid=airdisk
* cfg set encryption=none(wep,wpa,wpa2,mixed-wpa)
* cfg set password=00000000
* cfg set ip=192.168.222.254
* cfg set dhcp_start=xxx
* cfg set dhcp_end=xxx
* cfg set wpa_cipher=xxx(tkip,aes,tkip/aes)
* 同时，cfg还可以读取配置信息，
* 读取的格式为cfg get xxx
* 得到的参数会打印出来
* Author: liuxiaolong
* time: 2012-12-6
**************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DRIVER_NAME 		"/dev/mtdblock0"  	//uboot mtd
#define START_INDEX	0x40000//256K//0x58000  //start 352K (space of mtdblock0 is 384K);
#define MAX_SPACE 0x42000//256K+4K//352K+4K

#define CFG_DATA_LEN		64
#define CFGFLAG "flag"
#define FW_VERSION "fw_version"
#define QQSN     "qqsn"
#define QQLICENCE "license"
#define QQPID "qqpid"

//update_flag:when update_flag=1,enter system2.else enter system1.

char *g_cfg_parameter[] = {
    [0] = "mac",                [1] = "ssid",
	[2] = "encryption",         [3] = "password", 
	[4] = "ip",	                [5] = "dhcp_start", 
	[6] = "dhcp_end",           [7] = "wpa_cipher",      
	[8] = "airplay_name",       [9] = "dlna_name",
	[10] = "fw_version",        [11] = "host_name",     
	[12] = "model_name",        [13] = "model_number",    
	[14] = "model_description",	[15] = "manufacturer",   
	[16] = "url_manufacturer",  [17] = "dms_name",     
	[18] = "dms_enable",        [19] = "flag",
	[20] = "qqsn",              [21] = "license",
	[22] = "license_res",		[23] = "license_res2",
	[24] = "qqpid",  			[25] = "version_flag", 
	[26] = "update_flag", 
	[27] = NULL,
};

static void usage(void)
{
    int i = 0;
	printf("usage: cfg tool\n");
	printf("config parameters are: mac ssid encryption password ip dhcp_start dhcp_end wpa_cipher\n");
	printf("cfg set [param]=value: cfg set parameter=value \n");
	printf("cfg get [param]	     : cfg get parameter\n");
	printf("cfg list 	         : show all cfg parameter and value\n");
	printf("cfg checkflag 	     : check whether config parameters has been set\n");
	printf("cfg setflag 	     : set the flag to 1\n");
	printf("cfg clearflag 	     : set the flag to 0\n");
	printf("cfg erase 	         : erase all param to null\n");

	while(g_cfg_parameter[i])
	{
        printf("%s  ",g_cfg_parameter[i]);
		if((i)&&(i%4 == 0))
			printf("\n");
		i++;
	}
	printf("\n");

}

//检查标志位，如果标志位为pisen，说明配置参数已经被设置，否则
//则没有被设置
static int check_cfg_flag(FILE *fp)
{
    cfg_get(fp,CFGFLAG);
}
//将标志位设置为pisen，如果参数被写入，则必须将标志位设为pisen
static set_cfg_flag(FILE *fp)
{
	cfg_set(fp,"flag=1");
}
//清除标志位
static clear_cfg_flag(FILE *fp)
{
    cfg_set(fp,"flag=0");
}
//检查mac地址是否合法
static int check_mac(char *mac_addr)
{
	int i=0;
	if(strlen(mac_addr)!=12)
	{
		return 1;
	}
	for(i=0;i<12;i++)
	{
		if(!isxdigit(mac_addr[i]))
		{
			return 1;
		}
	}
	return 0;
}

//获取参数
int cfg_get(FILE *fp,char *argv)
{
	unsigned char arg_str[CFG_DATA_LEN*3+1]={0};
    int i = 0,ret = -1,offset_index=0;

//	if(argv==NULL)
//		return 1;

	while(g_cfg_parameter[i])
	{
        if(!strcmp(argv,g_cfg_parameter[i]))
        {
			offset_index = START_INDEX+(i*CFG_DATA_LEN);
			if(offset_index >= MAX_SPACE)
			{
				printf("[WARNING] offset_index >= MAX_SPACE\n");
			}

            fseek(fp,offset_index,SEEK_SET);
			if(!strncmp(argv,QQLICENCE,strlen(QQLICENCE))) 
          	  ret = fread(arg_str,CFG_DATA_LEN,3,fp);
			else
          	  ret = fread(arg_str,CFG_DATA_LEN,1,fp);				
			if(ret < 0) 
			{
			    printf("[ERROR] read error!\n");
				return -1;
			}
			if(strlen(arg_str))
			    printf("%s=%s\n",argv,arg_str);
			break;
		}
		i++;
	}
	return 0;
}


//设置参数
int cfg_set(FILE *fp,char *argv)
{
	unsigned char str_sp[CFG_DATA_LEN*3+1]={0};
    char value[CFG_DATA_LEN*3+1]={0};
	char temp_mac[32]={0};
	int offset_index = 0;
	int ret = 0;
    int i = 0,j = 0,k = 0;

//	if(argv==NULL)
//		return ret;
	while(g_cfg_parameter[i])
	{
        if(!strncmp(g_cfg_parameter[i],argv,strlen(g_cfg_parameter[i])))
        {  
            memset(value,0x0,sizeof(value));
            strncpy(value,argv+strlen(g_cfg_parameter[i])+1,sizeof(value)-1);//ignore"xxx="
            if(i==0)//mac
            {
				if(check_mac(value))
				{
					return -1;
				}

				for(j = 0; j < 18; j+=3)
				{
                    temp_mac[j] = *(value+(k++));
					temp_mac[j+1] = *(value+(k++));
					if(j<15)
					    temp_mac[j+2] = ':';
				}
				sprintf(str_sp,"echo \'%s\' >/usr/data/mac.txt",temp_mac);	
				system(str_sp);
				system("sync");
			}
			offset_index = START_INDEX+(i*CFG_DATA_LEN);
			if(offset_index >= MAX_SPACE)
			{
                printf("[WARNING] offset_index >= MAX_SPACE\n");
			}
			fseek(fp,offset_index,SEEK_SET);
			if(!strncmp(argv,QQLICENCE,strlen(QQLICENCE))) 
				ret = fwrite(value,CFG_DATA_LEN,3,fp);
			else
				ret = fwrite(value,CFG_DATA_LEN,1,fp);				
			if(ret < 0) 
			{
			    printf("[ERROR] write error\n");
				return -1;
			}


			if(!strncmp(argv,CFGFLAG,strlen(CFGFLAG)) || !strncmp(argv,FW_VERSION,strlen(FW_VERSION)))
			{
			    ret = 0;
			}
			else
			{
			    ret = 1;
				system("sync");
			}
			break;
		}
		i++;
	}
	return ret;
}
void cfg_erase(FILE *fp)
{
    int i = 0,offset_index = 0;
    char zeroarray[CFG_DATA_LEN]={0};
	while(g_cfg_parameter[i])
	{
		offset_index = START_INDEX+(i*CFG_DATA_LEN);
		if(offset_index >= MAX_SPACE)
		{
			printf("[WARNING] offset_index >= MAX_SPACE\n");
		}
		fseek(fp,offset_index,SEEK_SET);
		fwrite(zeroarray,CFG_DATA_LEN,1,fp);
		i++;
	}

}

void cfg_list(FILE *fp)
{
	int i=0;

	while(g_cfg_parameter[i])
	{
        cfg_get(fp,g_cfg_parameter[i]);
		//printf("\n");
		i++;
	}
}

int main(int argc,char *argv[])
{
    FILE *fp = NULL;

	if(argc < 2 || !strcmp(argv[1],"-h"))
	{
		usage();
		exit(0);
	}

	if(!strcmp(argv[1],"get")&&argv[2])
	{
		fp=fopen(DRIVER_NAME,"rb");
		if(fp==NULL)
		{
			//printf("open /dev/mtdblock0 failed\n");
			exit(1);
		}
		cfg_get(fp,argv[2]);
	}
	else if(!strcmp(argv[1],"set")&&argv[2])
	{
		fp=fopen(DRIVER_NAME,"rb+");
		if(fp==NULL)
		{
			//printf("open /dev/mtdblock0 failed\n");
			exit(1);
		}
		if(cfg_set(fp,argv[2])>0 && strncmp(argv[2], "update_flag", strlen("update_flag")) != 0 )
	        set_cfg_flag(fp);
	}
	else if(!strcmp(argv[1],"checkflag"))
	{
		fp=fopen(DRIVER_NAME,"rb");
		if(fp==NULL)
		{
			//printf("open /dev/mtdblock0 failed\n");
			exit(1);
		}
		check_cfg_flag(fp);
	}
	else if(!strcmp(argv[1],"setflag"))
	{
		fp=fopen(DRIVER_NAME,"rb+");
		if(fp==NULL)
		{
			//printf("open /dev/mtdblock0 failed\n");
			exit(1);
		}
		set_cfg_flag(fp);
	}
	else if(!strcmp(argv[1],"clearflag"))
	{
		fp=fopen(DRIVER_NAME,"rb+");
		if(fp==NULL)
		{
			//printf("open /dev/mtdblock0 failed\n");
			exit(1);
		}
		clear_cfg_flag(fp);
	}
	else if(!strcmp(argv[1],"list"))
	{
		fp=fopen(DRIVER_NAME,"rb");
		if(fp==NULL)
		{
			//printf("open /dev/mtdblock0 failed\n");
			exit(1);
		}
		cfg_list(fp);
	}
	else if(!strcmp(argv[1],"erase"))
	{
		fp=fopen(DRIVER_NAME,"rb+");
		if(fp==NULL)
		{
			//printf("open /dev/mtdblock0 failed\n");
			exit(1);
		}
		cfg_erase(fp);
	}/**/
	else
	{
		usage();
	}
	if(fp)
		fclose(fp);
	return 0;
}
