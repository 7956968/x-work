#include "cgihandle.h"
#include "cgiWireless.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>

#include "event_interface.h"
#include "utils_interface.h"
#include "wifi_interface.h"
#include "update_wpaconfig_file.h"
#include "nor.h"

#include "sysconfig.h"

#define get_power_level_num  1
#define get_Firmware_Edition 5
#define rtl_encryp_control "/proc/rtl_encryp_control"
int error_num=0;
char error_info[1024]="\0";

#define safe_free(p) do{\
    if((p) != NULL)\
    {\
        free((p));\
        (p) = NULL;\
    }\
    }while(0)
///////////////////////////////////////////////////////
//for storage//////////////////////////////////////////
///////////////////////////////////////////////////////
static const unsigned long long G = 1024*1024*1024ull;
static const unsigned long long M = 1024*1024;
static const unsigned long long K = 1024;
static char str[20];

bool check_conn(char *name);

int getsys(xmlNodePtr root);
int setsys(xmlNodePtr root);

cgiHandle grootHandle[]=
{
	{GETSYSSTR, getsys},
	{SETSYSSTR, setsys},
};


int getssid(xmlNodePtr tag, char *retstr);
int getRemoteAP(xmlNodePtr tag, char *retstr);
int getWorkMode(xmlNodePtr tag, char *retstr);
int getFTP(xmlNodePtr tag, char *retstr);
int getSAMBA(xmlNodePtr tag, char *retstr);
int getDMS(xmlNodePtr tag, char *retstr);
int getScan(xmlNodePtr tag, char *retstr);
int getStorage(xmlNodePtr tag, char *retstr);
int getVersion(xmlNodePtr tag, char *retstr);
int getJoinWired(xmlNodePtr tag, char *retstr);
int getWebDAV(xmlNodePtr tag, char *retstr);
int getpower(xmlNodePtr tag, char *retstr);
int getClientStatus(xmlNodePtr tag, char *retstr);
int get3G(xmlNodePtr tag, char *retstr);
int getBtnReset(xmlNodePtr tag, char *retstr);
int getBtnPlaypause(xmlNodePtr tag, char *retstr);
int getBtnVolumeup(xmlNodePtr tag, char *retstr);
int getBtnVolumedown(xmlNodePtr tag, char *retstr);
int getairplay(xmlNodePtr tag, char *retstr);

int getdiskotaupgrade(xmlNodePtr tag, char *retstr);
int getotastatus(xmlNodePtr tag, char *retstr);

tagHandle gtaghandle_get[]=
{
	{FN_GET_SSID, getssid},
	{FN_GET_RemoteAP, getRemoteAP},
	{FN_GET_WorkMode, getWorkMode},
	{FN_GET_APList, getScan},
	{FN_GET_Power, getpower},
	{FN_GET_Storage, getStorage},
	{FN_GET_FTP, getFTP},
	{FN_GET_SAMBA, getSAMBA},
	{FN_GET_DMS, getDMS},
	{FN_GET_DDNS, NULL},
	{FN_GET_WebDAV, getWebDAV},
	{FN_GET_JoinWired,getJoinWired},
	{FN_GET_Version, getVersion},
	{FN_GET_Client_Status,getClientStatus},
	{FN_GET_3G,get3G},
	{FN_GET_BTN_RST,getBtnReset},
	{FN_GET_BTN_PLAYPAUSE,getBtnPlaypause},
	{FN_GET_BTN_VOLUMEUP,getBtnVolumeup},
	{FN_GET_BTN_VOLUMEDOWN,getBtnVolumedown},
	{FN_SET_AIRPLAY_NAME,getairplay},
	{FN_GET_OTAUPGRADE,getdiskotaupgrade},
	{FN_GET_OTAStatus,getotastatus},
};

static inline int false_status(char* retstr, char* errmsg)
{
	error_num++;
	sprintf(retstr,"<Return status=\"false\">%s</Return>", errmsg);
	return 0;
}

void printstr(char *str)
{
#if 0
	FILE *fw_fp;
	int f_size=0;
	if( (fw_fp=fopen("/tmp/cgidebug.txt","a+"))==NULL)    // write and read,binary
	{
		system("touch /tmp/cgidebug_failed");
		exit(1);
	}		
	
	f_size=fwrite(str,1,strlen(str),fw_fp);
	fwrite("\n\n", 2, 2, fw_fp);
	fclose(fw_fp);
#endif
}

int setssid(xmlNodePtr tag, char *retstr);
int setWorkMode(xmlNodePtr tag, char *retstr);
int setJoinWireless(xmlNodePtr tag, char *retstr);
int setJoinWired(xmlNodePtr tag, char *retstr);
int setFTP(xmlNodePtr tag, char *retstr);
int setSAMBA(xmlNodePtr tag, char *retstr);
int setDMS(xmlNodePtr tag, char *retstr);
int Upgrade(xmlNodePtr tag, char *retstr);
int halt(xmlNodePtr tag, char *retstr);
int TimeSync(xmlNodePtr tag, char *retstr);
int setClient(xmlNodePtr tag, char *retstr);
int set3G(xmlNodePtr tag, char *retstr);
int setairplay(xmlNodePtr tag, char *retstr);
int setiperf(xmlNodePtr tag, char *retstr);
int setotastatus(xmlNodePtr tag, char *retstr);
int setled(xmlNodePtr tag, char *retstr);
int setvrswitch(xmlNodePtr tag, char *retstr);

tagHandle gtaghandle_set[]=
{
	{FN_SET_SSID, setssid},
	{FN_SET_WorkMode, setWorkMode},
	{FN_SET_JoinWireless, setJoinWireless},
	{FN_SET_JoinWired, setJoinWired},
	{FN_SET_FTP, setFTP},
	{FN_SET_SAMBA, setSAMBA},
	{FN_SET_DMS, setDMS},
	{FN_SET_DDNS, NULL},
	{FN_SET_WebDAV, NULL},
	{FN_Upgrade,Upgrade},
	{FN_HALT,halt},
	{FN_Time_Sync,TimeSync},
	{FN_SET_Client,setClient},
	{FN_SET_3G,set3G},
	{FN_SET_AIRPLAY_NAME, setairplay},
	{FN_SET_iperf,setiperf},
	{FN_SET_OTAStatus,setotastatus},
	{FN_SET_VRswitch,setvrswitch},
};

void mylogstrTofile(const char*format, ...)
{		
#if 1
    char strlog[8096] = {0};
	FILE* fw_fp;
	//int f_size;
    va_list ap;

	if( (fw_fp=fopen("/tmp/log_cgihandle","a+"))==NULL)    // write and read,binary
	{
		return ;
	}		
	
    va_start(ap, format);
    vsnprintf(strlog, sizeof(strlog), format, ap);
	fwrite(strlog,1,strlen(strlog),fw_fp);
	fputc('\n',fw_fp);
    va_end(ap);
	fclose(fw_fp);
#endif
}

static int send_misc_event(char* name, char* type)
{
	mozart_event event = {	
		.event = {		
			.misc = {		
			},		
		},
		.type = EVENT_MISC,	
	};	
	snprintf(event.event.misc.name, "%s", name);
	snprintf(event.event.misc.type, "%s", type);
		
	int ret = mozart_event_send(event);	
	if (!ret)	
		mylogstrTofile("Send %s-%s connected event successfully.\n", name, type);
	else	
		mylogstrTofile("Send %s-%s connected event unsuccessfully.\n", name, type);
}

#ifdef CGI_LOG	
void cgi_log( char *string){
#if 0
	FILE *fw_fp;
	int f_size=0;
	if( (fw_fp=fopen("/tmp/cgi_log.txt","ab+"))==NULL)    // write and read,binary
	{
		exit(1);
	}		
	f_size=fwrite(string,1,strlen(string),fw_fp);
	fclose(fw_fp);
	return;
#endif
}
#endif
char *xmldecode(char *xml)
{
		int i,j,k;
		char *ret;
		int f;
		char tmp[10];
		
		i = strlen(xml);
		ret = (char *)malloc(i);
		
		k=0;
		for(j=0; j<i;)
		{
				if(xml[j] != '&')
				{
					ret[k]=xml[j];
					k++;
					j++;
				}
				else
				{
					j++;
					f=0;
					while(xml[j] != ';')
					{
							tmp[f++]=xml[j];
							j++;
					}
					tmp[f]=0;
					
					if(strcmp(tmp, "lt") == 0)
					{
							ret[k]='<';
					}
					else if(strcmp(tmp, "gt") == 0)
					{
							ret[k]='>';
					}
					else if(strcmp(tmp, "amp") == 0)
					{
							ret[k]='&';
					}
					else if(strcmp(tmp, "apos") == 0)
					{
							ret[k]='\'';
					}	
					else if(strcmp(tmp, "quot") == 0)
					{
							ret[k]='\"';
					}		
					k++;
					j++;												
				}
		}		
		ret[k]=0;
		return ret;
}


#define ROOTHANDLE_NUM (sizeof(grootHandle)/sizeof(grootHandle[0]))

#define GET_TAGHANDLE_NUM (sizeof(gtaghandle_get)/sizeof(gtaghandle_get[0]))
#define SET_TAGHANDLE_NUM (sizeof(gtaghandle_set)/sizeof(gtaghandle_set[0]))


int roothandle(xmlNodePtr root, xmlNodePtr other)
{
	int i;
	int flag=-1;
	
	for(i = 0; i< ROOTHANDLE_NUM; i++)
	{
		
		if(!strcmp(root,( const xmlChar *)grootHandle[i].tag))
		{
			
			if(grootHandle[i].rootfun)
				flag = grootHandle[i].rootfun(other);
		}
		
	}

	return flag;
}


int getsys(xmlNodePtr root)
{
	int flag=0;
	int tagflag=-1;
	char *tagstr;
	xmlNodePtr curNode=root;//root->children;
	char *relstr;	
	char nodestrp[128];
	//char tmp[200];
	char *tstr;
	int j;
	
	
	relstr = (char *)malloc(8192);
	tagstr = (char *)malloc(8192);
	memset(relstr, 0, 8192);
	memset(tagstr, 0, 8192);
	xmlGetTag(root,nodestrp );
	//while(curNode != NULL)
	while(1)
	{
		int i;
		for(i = 0; i<GET_TAGHANDLE_NUM; i++)
		{
			if(!strcmp(nodestrp,( const xmlChar *)gtaghandle_get[i].tag))
			{
				if(gtaghandle_get[i].tagfun)
				{
					memset(tagstr, 0, 8192);
					tagflag += gtaghandle_get[i].tagfun(curNode, tagstr);
					strcat(relstr, tagstr);
					flag = 1;
				}
			}
		}
		tstr = strstr(curNode, nodestrp);
		j=0;
		while(tstr[j++]!='>');
		curNode = tstr+j;
		memset(nodestrp, 0, 128);
		if(xmlGetNextTag(curNode, nodestrp) == 0)
			break;
		//
		//curNode = curNode->next;
	}
	//printf("%s\n",relstr);

	
	//发送字符串
	postStr(relstr, 0,flag);
	
	free(relstr);
	free(tagstr);
	
	if(flag)
		return tagflag;
	
	return -1;

}

int setsys(xmlNodePtr root)
{
	int flag=0;
	int tagflag=0;
	char tagstr[128];
	char nodestrp[128];
	//char tmp[200];
	
	xmlNodePtr curNode=root;//root->children;

	char *relstr;
	char *tstr;
	int j;

	relstr = (char *)malloc(4096);
	memset(relstr, 0, 4096);
	//printf("Content-type:text/html\r\n\r\n");
	xmlGetTag(root,nodestrp );
	//while(curNode != NULL)
	//sprintf(stderr, "set sys:%s", root);
	
	//sprintf(tmp, "echo \"set sys:%s\n\" >> /tmp/1", root);
	//system(tmp);
	//while(1)
	{
		int i;
		for(i = 0; i<SET_TAGHANDLE_NUM; i++)
		{
			if(!strcmp(nodestrp,( const xmlChar *)gtaghandle_set[i].tag))
			{
				if(gtaghandle_set[i].tagfun)
				{
					memset(tagstr, 0, 128);
					tagflag += gtaghandle_set[i].tagfun(curNode, tagstr);
					strcat(relstr, tagstr);
					flag = 1;
				}
			}
		}
		//curNode = curNode->next;
		
		//tstr = strstr(curNode, nodestrp);
		//j=0;
		///while(tstr[j++]!='>');
		//curNode = tstr+j;
		//memset(nodestrp, 0, 128);
		////if(xmlGetNextTag(curNode, nodestrp) == 0)
		//	break;
	}

	//发送字符串
	postStr(relstr, 1,flag);

	free(relstr);
	
	
	if(flag)
		return tagflag;
	
	return -1;
	
}

static const char *human_fstype(long f_type)
{
	static const struct types {
		long type;
		const char *const fs;
	} humantypes[] = {
		{ 0xADFF,     "affs" },
		{ 0x1Cd1,     "devpts" },
		{ 0x137D,     "ext" },
		{ 0xEF51,     "ext2" },
		{ 0xEF53,     "ext2/ext3" },
		{ 0x3153464a, "jfs" },
		{ 0x58465342, "xfs" },
		{ 0xF995E849, "hpfs" },
		{ 0x9660,     "isofs" },
		{ 0x4000,     "isofs" },
		{ 0x4004,     "isofs" },
		{ 0x137F,     "minix" },
		{ 0x138F,     "minix (30 char.)" },
		{ 0x2468,     "minix v2" },
		{ 0x2478,     "minix v2 (30 char.)" },
		{ 0x4d44,     "msdos" },
		{ 0x4006,     "fat" },
		{ 0x564c,     "novell" },
		{ 0x6969,     "nfs" },
		{ 0x9fa0,     "proc" },
		{ 0x517B,     "smb" },
		{ 0x012FF7B4, "xenix" },
		{ 0x012FF7B5, "sysv4" },
		{ 0x012FF7B6, "sysv2" },
		{ 0x012FF7B7, "coh" },
		{ 0x00011954, "ufs" },
		{ 0x012FD16D, "xia" },
		{ 0x5346544e, "ntfs" },
		{ 0x1021994,  "tmpfs" },
		{ 0x52654973, "reiserfs" },
		{ 0x28cd3d45, "cramfs" },
		{ 0x7275,     "romfs" },
		{ 0x858458f6, "romfs" },
		{ 0x73717368, "squashfs" },
		{ 0x62656572, "sysfs" },
		{ 0, "UNKNOWN" }
	};

	int i;

	for (i = 0; humantypes[i].type; ++i)
		if (humantypes[i].type == f_type)
			break;
	return humantypes[i].fs;
}

char* kscale(unsigned long b, unsigned long bs)
{
	unsigned long long size = b * (unsigned long long)bs;
	if (size > G)
	{
		sprintf(str, "%0.1fGB", size/(G*1.0));
		return str;
	}
	else if (size > M)
	{
		sprintf(str, "%0.1fMB", size/(1.0*M));
		return str;
	}
	else if (size > K)
	{
		sprintf(str, "%0.1fK", size/(1.0*K));
		return str;
	}
	else
	{
		sprintf(str, "%0.1fB", size*1.0);
		return str;
	}
}



////////////////////////////////////////////////////////////
//typedef int (*TAGFUNC)(xmlNodePtr tag, char *retstr);
///////////////////////////////////////////////////////////
int getssid(xmlNodePtr tag, char *retstr)
{
	char name[64]={0};
	char encrypt[64]={0};
	char channel[5]={0};
	char password[64]={0};
	char encrypt_len[5]={0};
	char format[8]={0};
	char mac[32]={0};
	char ifname[16]={0};
	char uci_option_str[64]={0};
	FILE *pp=NULL;
	char buf[16]={0};
	int i=0,j=0;

	if (mozart_ini_getkey(NETCONFIGPATH, NETAPSECTION, "ssid", name))//ssid
	{
		printf("Could not get ssid's value below %s in %s.\n",NETAPSECTION, NETCONFIGPATH);
	}

	if (mozart_ini_getkey(NETCONFIGPATH, NETAPSECTION, "encryption", encrypt))//encrypt
	{
		printf("Could not get encryption's value below %s in %s.\n",NETAPSECTION, NETCONFIGPATH);
	}

	if(!strcmp(encrypt,"none"))
	{
		memset(encrypt,0,strlen(encrypt));
		strcpy(encrypt,"NONE");
	}
	else if( !strcmp(encrypt,"psk2+ccmp") || !strcmp(encrypt,"psk2+tkip") || !strcmp(encrypt,"psk2+tkip+ccmp") )
	{
		memset(encrypt,0,strlen(encrypt));
		strcpy(encrypt,"WPA2");
	}
	else if( !strcmp(encrypt,"psk+ccmp") || !strcmp(encrypt,"psk+tkip") || !strcmp(encrypt,"psk+tkip+ccmp") )
	{
		memset(encrypt,0,strlen(encrypt));
		strcpy(encrypt,"WPA");
	}
	else if( !strcmp(encrypt,"mixed-psk+ccmp") || !strcmp(encrypt,"mixed-psk+tkip") || !strcmp(encrypt,"mixed-psk+tkip+ccmp") )
	{
		memset(encrypt,0,strlen(encrypt));
		strcpy(encrypt,"WPA/WPA2");
	}
	else if( !strcmp(encrypt,"wep") )
	{
		memset(encrypt,0,strlen(encrypt));
		strcpy(encrypt,"WEP");
	}
	else
		NULL;
	strcpy(ifname,"wlan0");
	cgi_get_channel(ifname, channel);  								//channel
	if (mozart_ini_getkey(NETCONFIGPATH, NETAPSECTION, "key", password))//password
	{
		printf("Could not get key's value below %s in %s.\n",NETAPSECTION, NETCONFIGPATH);
	}
	/*if (mozart_ini_getkey("/factory/factoryconfig", "factory", "mac", mac))//mac
	{
		printf("Could not get mac's value below factory in /factory/factoryconfig.\n");
	}*///ifconfig | grep 'wlan0' -A 1 | grep 'HWaddr' | awk '{print $5}'|awk -F : '{print $1$2$3$4$5$6}'
	if((pp=popen("ifconfig | grep 'wlan0' -A 1 | grep 'HWaddr' | awk '{print $5}'|awk -F : '{print $1$2$3$4$5$6}'","r"))!=NULL)
	{
		if((fgets(buf,13,pp))!=NULL)
		{
			strcpy(mac,buf);
		}
	    pclose(pp);
	
	}
	mylogstrTofile("mac=%s\n", mac);

    if(12 != strlen(mac))
    {
        memset(mac,0x0,sizeof(mac));
		strcpy(mac,"845dd7a89900");
	}
	sprintf(retstr,"<SSID name=\"%s\" encrypt=\"%s\" channel=\"%s\" password=\"%s\" encrypt_len=\"\" format=\"\" mac=\"%s\"></SSID>",xmlEncode(name),encrypt,channel,xmlEncode(password),mac);
	return 0;

}

bool check_conn(char *name)
{
	char *url = "www.baidu.com";
	struct hostent *ret = NULL;

	if(name)
		url = name;

	ret = gethostbyname(url);//mylogstrTofile("11111111111111 ret=%d url=%s\n",ret,url);
	if(ret)
		return true;

	return false;
}

#define SSID_SIZE 64
int getRemoteAP(xmlNodePtr tag, char *retstr)
{
	char name[SSID_SIZE]="\0";
	char encrypt[32]="\0";
	char channel[5]="1";
	char password[64]="\0";
	//char status[2]="\0";
	int status=1;
	char uci_option_str[64]="\0";
	char ifname[10]="\0";
	char ip[32]="\0";
	char mask[32]="\0";
	char gateway[32]="\0";
	char dns1[32]="\0";
	char dns2[32]="\0";
	char dns[64]="\0";
	int i=0;
	int j=0;
	char *tmp = NULL;
	system("touch /tmp/aaaa");
	mylogstrTofile("start getRemoteAP\n");
	system("touch /tmp/bbbb");
	wpa_get_ssid(name, SSID_SIZE);
	system("touch /tmp/cccc");
	mylogstrTofile("ssid = %s\n", name);
	#if 0
	if (mozart_ini_getkey(NETCONFIGPATH, NETCLIENTSECTION, "ssid", name))//ssid
	{
		printf("Could not get ssid's value below %s in %s.\n",NETCLIENTSECTION, NETCONFIGPATH);
	}
	if (mozart_ini_getkey(NETCONFIGPATH, NETCLIENTSECTION, "encryption", encrypt))//encrypt
	{
		printf("Could not get encryption's value below %s in %s.\n",NETCLIENTSECTION, NETCONFIGPATH);
	}
	memset(uci_option_str,'\0',64);
	if(!strcmp(encrypt,"none"))
	{
		memset(encrypt,0,strlen(encrypt));
		strcpy(encrypt,"NONE");
	}
	else if( !strcmp(encrypt,"psk2+ccmp") || !strcmp(encrypt,"psk2+tkip") || !strcmp(encrypt,"psk2+tkip+ccmp") )
	{
		memset(encrypt,0,strlen(encrypt));
		strcpy(encrypt,"WPA2");
	}
	else if( !strcmp(encrypt,"psk+ccmp") || !strcmp(encrypt,"psk+tkip") || !strcmp(encrypt,"psk+tkip+ccmp") )
	{
		memset(encrypt,0,strlen(encrypt));
		strcpy(encrypt,"WPA");
	}
	else if( !strcmp(encrypt,"mixed-psk+ccmp") || !strcmp(encrypt,"mixed-psk+tkip") || !strcmp(encrypt,"mixed-psk+tkip+ccmp") )
	{
		memset(encrypt,0,strlen(encrypt));
		strcpy(encrypt,"WPA/WPA2");
	}
	else if( !strcmp(encrypt,"wep") )
	{
		memset(encrypt,0,strlen(encrypt));
		strcpy(encrypt,"WEP");
	}
	else
		NULL;
	strcpy(ifname,"wlan0");
	cgi_get_channel(ifname, channel);  //channel
	if (mozart_ini_getkey(NETCONFIGPATH, NETCLIENTSECTION, "key", encrypt))//password
	{
		printf("Could not get key's value below %s in %s.\n",NETCLIENTSECTION, NETCONFIGPATH);
	}
	#endif
	if(check_conn(NULL))
		status = 0;	
	tmp = xmlEncode(name);
	sprintf(retstr,"<RemoteAP name=\"%s\" encrypt=\"%s\" channel=\"%s\" password=\"%s\" status=\"%d\" ip=\"%s\" mask=\"%s\" gateway=\"%s\" dns1=\"%s\" dns2=\"%s\"></RemoteAP>",tmp,encrypt,channel,xmlEncode(password),status,ip,mask,gateway,dns1,dns2);
	free(tmp);
	//uci_free_context(ctx);
	return 0;
}

char * getPIDformPS(char *iface)
{
	char buffer[BUFSIZ]; 
	FILE *read_fp; 
	int chars_read; 
	char *pid; 
	memset( buffer, 0, BUFSIZ ); 
	char tmpStr[30];
//	printf("int size=%d\n",sizeof(int));


	sprintf(tmpStr,"ps -w |grep %s",iface);
//	printf("tmpStr=%s\n",tmpStr);

	read_fp=popen(tmpStr,"r");
	if(read_fp!=NULL)
		{
		chars_read = fread(buffer, sizeof(char), BUFSIZ-1, read_fp); 
		if (chars_read > 0) 
		{ 
//			printf("buffer=%s\n",buffer);
			pid=strtok(buffer," ");
			if(pid!=NULL)
				{
//				printf("pid2=%s\n",pid);
				pclose(read_fp); 
				return pid;
			}else
				{
				pclose(read_fp); 
				return 0;
			}
		} 
		else 
		{ 
			pclose(read_fp); 
			return 0; 
		} 

	}
}

int getWorkMode(xmlNodePtr tag, char *retstr)
{
	sprintf(retstr,"<WorkMode value=\"1\"></WorkMode>");
	return 0;
}

int getFTP(xmlNodePtr tag, char *retstr)
{

	sprintf(retstr,"<FTP user=\"airmusic\" password=\"12345678\" port=\"21\" path=\"/tmp/mnt\" status=\"OFF\" anonymous_en=\"1\" enable=\"OFF\"></FTP>");
	return 0;
}

int getSAMBA(xmlNodePtr tag, char *retstr)
{
	sprintf(retstr,
	"<SAMBA user=\"airmusic\" password=\"12345678\" port=\"137\" path=\"/tmp/mnt\" status=\"OFF\" anonymous_en=\"1\" enable=\"ON\"></SAMBA>");
	return 0;
}

int getDMS(xmlNodePtr tag, char *retstr)
{
	sprintf(retstr,"<DMS name=\"airmusicDMS\" status=\"OFF\" path=\"/mnt\" enable=\"OFF\"></DMS>");
	return 0;
}

int getWebDAV(xmlNodePtr tag, char *retstr)
{
	strcpy(retstr,"<WebDAV enable=\"OFF\"></WebDAV>");
	return 0;
}


int getScan(xmlNodePtr tag, char *retstr)
{
	cgi_scan("wlan0", retstr);
	return 0;
}

int getStorage(xmlNodePtr tag, char *retstr)
{
#if 0
	FILE* mount_table;
	struct mntent *mount_entry;
	struct statfs fs;
	unsigned long blocks_used;
	unsigned blocks_percent_used;
	char tmpi=0;
	char tmpstr[128]="\0";
	 struct udev *udev;
	 struct udev_enumerate *enumerate;
	 struct udev_list_entry *devices, *dev_list_entry;
	 struct udev_device *dev;
	 int i=0;
	 int j=0;
	  typedef struct myudevstruct{
	  char devpath[20];
	 // char * mountpoint;
	  char pid[10];
	  char vid[10];
	 }myudev;
	 myudev myusb[20];
	 myudev *p=myusb;

	 /* Create the udev object */
	 udev = udev_new();
	 if (!udev) {
	  printf("Can't create udev\n");       
	  exit(1);
	 }
	 
	 enumerate = udev_enumerate_new(udev);
	 udev_enumerate_add_match_subsystem(enumerate, "block");
	 udev_enumerate_scan_devices(enumerate);
	 devices = udev_enumerate_get_list_entry(enumerate);
	 udev_list_entry_foreach(dev_list_entry, devices) {
	  const char *path;
	  
	  path = udev_list_entry_get_name(dev_list_entry);
	  dev = udev_device_new_from_syspath(udev, path);

	  strcpy(p[i].devpath,udev_device_get_devnode(dev));
	  dev = udev_device_get_parent_with_subsystem_devtype(
	         dev,
	         "usb",
	         "usb_device");
	  if (!dev) {
	    break;
	  // printf("Unable to find parent usb device.");
	  // exit(1);
	  }
	 // p=myusb malloc(sizeof(myusb));
	 // myusb[i].pid=malloc(8*sizeof(char));
	  strcpy(p[i].vid,udev_device_get_sysattr_value(dev,"idVendor"));
	  strcpy(p[i].pid,udev_device_get_sysattr_value(dev, "idProduct"));
	  //printf("myusb[%d]:dev=%s,vid=%s,pid=%s\n",i, p[i].devpath,p[i].vid,p[i].pid);
	  i++;
	  udev_device_unref(dev);
	 }
	 /* Free the enumerator object */
	 udev_enumerate_unref(enumerate);

	 udev_unref(udev);

	  mount_table = setmntent("/proc/mounts", "r");
	  if (!mount_table)
	  {
	    fprintf(stderr, "set mount entry error\n");
	    return -1;
	  }
	  //printf("<Storage>");
	  strcpy(retstr,"<Storage>");
	  
	  while (1) {
	    const  char *device;
	    const char *mount_point;
	    if (mount_table) {
	      mount_entry = getmntent(mount_table);
	      if (!mount_entry) {
	        endmntent(mount_table);
	        break;
	      }
	    } 
	    else
	      break;
	    device = mount_entry->mnt_fsname;
	    mount_point = mount_entry->mnt_dir;
	  
	  //  sprintf(retstr, "mount info:mountpoint=%s\n",mount_point);
	    if(strstr(mount_point,"disk")==NULL) continue;
	     tmpi++;
	    if (statfs(mount_point, &fs) != 0) 
	    {
	      fprintf(stderr, "statfs failed!\n");  
	      break;
	    }
	    if ((fs.f_blocks > 0) || !mount_table ) 
	    {
	      blocks_used = fs.f_blocks - fs.f_bfree;
	      blocks_percent_used = 0;

	      if (strcmp(device, "rootfs") == 0)
	        continue;
	      sprintf(tmpstr,"<Section volume=\"%s\"",mount_point+9);//   /tmp/mnt/
	      //if (printf("\n<Section volume=\"%s\"" + 1, device) > 20)
	      //    printf("%1s", "");
	      strcat(retstr,tmpstr);
	      memset(tmpstr,0,128);
	      for (j = 0; j < i; j++)
	      {

	        if(!strcmp(myusb[j].devpath,device))
	        {

	        //  printf("j=%d,myusbdevice=%s,device=%s\n",j,p[j].devpath,device);
	          sprintf(tmpstr," vid=\"%s\" pid=\"%s\"",p[j].vid,p[j].pid);
	        //  printf(" vid=\"%s\" pid=\"%s\"",p[j].vid,p[j].pid);
	        }
	      }
	      strcat(retstr,tmpstr);
	      memset(tmpstr,0,128);
	      
	      char s1[20];
	      char s2[20];
	      char s3[20];
	    //  printf("total blocks=%10d\nblock size=%10d\nfree blocks=%10d\n",fs.f_blocks,fs.f_bsize,fs.f_bfree);
	      strcpy(s1, kscale(fs.f_blocks, fs.f_bsize));
	      strcpy(s2, kscale(fs.f_blocks - fs.f_bfree, fs.f_bsize));
	      strcpy(s3, kscale(fs.f_bavail, fs.f_bsize));
	      sprintf(tmpstr," total=\"%s\" used=\"%s\" free=\"%s\" fstype=\"%s\" ></Section>",
	          s1,
	          s2,
	          s3,
	          human_fstype(fs.f_type)
	          );
	      strcat(retstr,tmpstr);
	      memset(tmpstr,0,128);
	    }
	     
	  }
	  
	  if(tmpi==0)
	  {

	    strcat(retstr,"</Storage>");
	    //strcat(retstr,"</Storage><Return status=\"false\">Disk has already mount PC !</Return>");
	    //printf("</Storage><Return status=\"false\">Empty disk !</Return></getSysInfo>");
	    return -1;
	  }
	 strcat(retstr,"</Storage>");
#endif
	return 0;

}

int getJoinWired(xmlNodePtr tag, char *retstr)
{
	return 0;
}




int getVersion(xmlNodePtr tag, char *retstr)
{	
	char* fw_version = NULL;
	char* version_flag = NULL;
	char* project_number = NULL;
	if ( (project_number = sys_get_projectnum()) == NULL) {
		false_status(retstr,"cant get project number");
		goto out;
	}
	version_flag = sys_get_version_flag();
 	if ( version_flag == NULL || strcmp(version_flag,"1") == 0) 
		fw_version = sys_get_fwversion2();
	else 
		fw_version = sys_get_fwversion();	
	if (fw_version == NULL)
		false_status(retstr,"cant get fw version");
	else
		sprintf(retstr,"<Version fw1=\"%s\" fw2=\"%s-%s\"></Version>",FW_1, fw_version, project_number);
out:
	safe_free(fw_version);
	safe_free(version_flag);
	safe_free(project_number);
	return 0;
}

int getpower(xmlNodePtr tag, char *retstr)
{
	unsigned char be=0;
	unsigned char stat=0;
	unsigned char tmpStatus=0;//normal
	unsigned char bat;
	int fh=NULL;
	fh=open(rtl_encryp_control, O_RDWR);
	if(fh==NULL)
	{
		error_num++;
		sprintf(retstr,"<Return status=\"false\">Get power level error!</Return>");
		return; 
	}
	if(ioctl(fh, get_power_level_num, &be) < 0)
	{
		error_num++;
		sprintf(retstr,"<Return status=\"false\">Get power level error!</Return>");
		close(fh);
		return; 							
	}
	if(ioctl(fh,get_Firmware_Edition,&stat)<0)
	{
		error_num++;
		sprintf(retstr,"<Return status=\"false\">Get power status error!</Return>");
		close(fh);
		return;
	}
	tmpStatus=(stat&0x0f);
	
	if(tmpStatus==3)
	{
		tmpStatus=1;
	}
	if(be<=10)
	{
		tmpStatus=3;
	}
	close(fh);
	sprintf(retstr,"<Power percent=\"%d\" status=\"%d\"></Power>",be,tmpStatus);
}

int getClientStatus(xmlNodePtr tag, char *retstr)
{
	sprintf(retstr,"<Client enable=\"ON\"></Client>");
}

int get3G(xmlNodePtr tag, char *retstr)
{
	return 0;
}

static int set_product_keytest_flag()
{
	system("touch /tmp/product_keytest >> /tmp/log_cgihandle");
	return 0;
}

static int key_has_pressed(char* keyname)
{
	char file[128] = {0};
	snprintf(file, sizeof(file), "/tmp/%s_btn_pressed", keyname);
	if (access(file, F_OK) == 0) {
		remove(file);
		return 1;
	}
	return 0;
}

int getBtnReset(xmlNodePtr tag, char *retstr)
{
	set_product_keytest_flag();
	char* xmlname = FN_GET_BTN_RST;
	if (key_has_pressed("f1") > 0) //f1 is reset button
		sprintf(retstr,"<%s status=\"down\"></%s>", xmlname, xmlname);
	else
		sprintf(retstr,"<%s status=\"up\"></%s>", xmlname, xmlname);
	return 0;
	
}

int getBtnVolumeup(xmlNodePtr tag, char *retstr)
{
	set_product_keytest_flag();
	char* xmlname = FN_GET_BTN_VOLUMEUP;
	if (key_has_pressed("f3") > 0) //use to check f3
		sprintf(retstr,"<%s status=\"down\"></%s>", xmlname, xmlname);
	else
		sprintf(retstr,"<%s status=\"up\"></%s>", xmlname, xmlname);
	return 0;
	
}

int getBtnVolumedown(xmlNodePtr tag, char *retstr)
{
	set_product_keytest_flag();
	char* xmlname = FN_GET_BTN_VOLUMEDOWN;
	if (key_has_pressed("record") > 0) //use to check record
		sprintf(retstr,"<%s status=\"down\"></%s>", xmlname, xmlname);
	else
		sprintf(retstr,"<%s status=\"up\"></%s>", xmlname, xmlname);
	return 0;
	
}

int getBtnPlaypause(xmlNodePtr tag, char *retstr)
{
	set_product_keytest_flag();
	char* xmlname = FN_GET_BTN_PLAYPAUSE;
	if (key_has_pressed("playpause") > 0)
		sprintf(retstr,"<%s status=\"down\"></%s>", xmlname, xmlname);
	else
		sprintf(retstr,"<%s status=\"up\"></%s>", xmlname, xmlname);
	return 0;
}


int getdiskotaupgrade(xmlNodePtr tag, char *retstr)
{
    char* version_flag=NULL;
    char* project_number=NULL;
    char* mac=NULL;
	char* fw_version = NULL;
	int versionflag_int = 0;
	
	if ( (mac = sys_get_mac()) == NULL) {
		false_status(retstr,"cant get mac addr");
		goto out;
	}
	if ( (project_number = sys_get_projectnum()) == NULL) {
		false_status(retstr,"cant get project number");
		goto out;
	}
	version_flag = sys_get_version_flag();
 	if ( version_flag == NULL || strcmp(version_flag,"1") == 0) {
		versionflag_int = 1;
		fw_version = sys_get_fwversion2();
 	}
	else {
		versionflag_int = 0;
		fw_version = sys_get_fwversion();	
	}
	if (!fw_version) {
		false_status(retstr,"getdiskotaupgrade error, cant get fw_version!");	
		goto out;
	}
	sprintf(retstr,"<GetOTAInfo customCode=\"%s\" versionCode=\"%s\" mac=\"%s\" versionflag=\"%d\" time=\"60\"></GetOTAInfo>",
					project_number,fw_version,mac,versionflag_int);
out:
	safe_free(mac);
	safe_free(fw_version);
	safe_free(version_flag);
	safe_free(project_number);
    return 0;
}

int getotastatus(xmlNodePtr tag, char *retstr)
{
	char* version_flag = NULL;
	version_flag = sys_get_version_flag();
 	if ( version_flag == NULL || strcmp(version_flag,"1") == 0) 
		sprintf(retstr,"<OTAStatus status=\"1\"></OTAStatus>");
	else
		sprintf(retstr,"<OTAStatus status=\"0\"></OTAStatus>");
	safe_free(version_flag);
	return 0;
}
///////////////////////////////////////////////////////////
//设置需要先检查参数是否正确，然后应答一个正确或者错误码
//////////////////////////////////////////////////////////

int setssid(xmlNodePtr tag, char *retstr)  //network restart
{
	
	char *name=NULL;
	char *encrypt=NULL;
	char *password=NULL;
	char *tkip_aes=NULL;
	char channel[5]="\0";
	char encrypt_len[5]="\0";
	char format[8]="\0";
	
	char encrypt_config[32]="\0";
	char tkip_aes_config[8]="\0";
	//xmlChar *xml_str;
	
	char str_sp[64]="\0";
	//printf("Content-type:text/html\r\n\r\n");
	char *pxml=NULL;
	
	
	//name="valueStr"
	//valueStr=xmlGetProp(tag,(const xmlChar*)"name");
	if((pxml=xmlGetprop(tag,(const xmlChar*)SSID_SET_name))!=NULL)
	{
		if((name=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
			memset(name,0,strlen(pxml)+1);
			strcpy(name,pxml);
			pxml=NULL;
		}
	}
	else
	{
		error_num++;
		strcat(error_info, PARAMETER_ERROR);
		free(name);
		free(encrypt);
		free(password);
		free(tkip_aes);
		return 1;
	}
	if((pxml=xmlGetprop(tag,(const xmlChar*)SSID_SET_encrypt))!=NULL)
	{
		if((encrypt=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
			memset(encrypt,0,strlen(pxml)+1);
			strcpy(encrypt,pxml);
			pxml=NULL;
		}
	}
	else
	{
		error_num++;
		strcat(error_info, PARAMETER_ERROR);
		free(name);
		free(encrypt);
		free(password);
		free(tkip_aes);
		return 1;
	}
	if((pxml=xmlGetprop(tag,(const xmlChar*)SSID_SET_password))!=NULL)
	{
		if((password=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
			memset(password,0,strlen(pxml)+1);
			strcpy(password,pxml);
			pxml=NULL;
		}
	}
	if((pxml=xmlGetprop(tag,(const xmlChar*)SSID_SET_tkip_aes))!=NULL)
	{
		if((tkip_aes=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
			memset(tkip_aes,0,strlen(pxml)+1);
			strcpy(tkip_aes,pxml);
			pxml=NULL;
		}
	}
	//strcpy(name,xmlGetProp(tag,(const xmlChar*)SSID_SET_name));
	//strcpy(encrypt,xmlGetProp(tag,(const xmlChar*)SSID_SET_encrypt));
	//strcpy(password,xmlGetProp(tag,(const xmlChar*)SSID_SET_password));
	//strcpy(tkip_aes,xmlGetProp(tag,(const xmlChar*)SSID_SET_tkip_aes));
	if( !strlen(name) || strlen(name)>32)
	{
		error_num++;
		strcat(error_info, ERROR_SSID);
		free(name);
		free(encrypt);
		free(password);
		free(tkip_aes);
		return 1;
	}
	if( !strcmp(encrypt,"WEP") )
	{
		if( !strlen(password) || ( strlen(password)!=5 && strlen(password)!=10 && strlen(password)!=13 && strlen(password)!=26 ) )
		{
			error_num++;
			strcat(error_info,ERROR_PASSWORD);
			free(name);
			free(encrypt);
			free(password);
			free(tkip_aes);
			return 1;
		}
	}
	if( !strcmp(encrypt,"WPA") || !strcmp(encrypt,"WPA2") || !strcmp(encrypt,"WPA/WPA2") )
	{
		if( !strlen(password) || strlen(password)<8 )
		{
			error_num++;
			strcat(error_info,ERROR_PASSWORD);
			free(name);
			free(encrypt);
			free(password);
			free(tkip_aes);
			return 1;
		}
	}
	printf("Content-type:text/html\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	printf("<%s><Return status=\"true\" delay=\"5\"></Return></%s>\r\n",SETSYSSTR,SETSYSSTR);
	fflush(stdout);
	
	if(!strcmp(encrypt,"NONE"))
		strcpy(encrypt_config,"none");
	else if(!strcmp(encrypt,"WEP"))
		strcpy(encrypt_config,"wep");
	else if(!strcmp(encrypt,"WPA"))
		strcpy(encrypt_config,"psk");
	else if(!strcmp(encrypt,"WPA2"))
		strcpy(encrypt_config,"psk2");
	else if(!strcmp(encrypt,"WPA/WPA2"))
		strcpy(encrypt_config,"mixed-psk");
	else
		NULL;
	if(tkip_aes!=NULL)
	{
		if(!strcmp(tkip_aes,"tkip"))
			strcpy(tkip_aes_config,"tkip");
		else if(!strcmp(tkip_aes,"aes"))
			strcpy(tkip_aes_config,"ccmp");
		else if(!strcmp(tkip_aes,"tkip/aes"))
			strcpy(tkip_aes_config,"tkip+ccmp");
		else
			NULL;
	}
	if(strlen(name))
	{
		if (mozart_ini_setkey(NETCONFIGPATH, NETAPSECTION, "ssid", name)) {
			printf("Could not write ssid=%s below %s in %s.\n", name, NETAPSECTION, NETCONFIGPATH);
		}
		memset(str_sp,0,64);
		sprintf(str_sp,"ssid_name");
		nor_set(str_sp,name);
//		system(str_sp);
		memset(str_sp,0,64);
	}
	if(!strcmp(encrypt_config,"none") || !strcmp(encrypt_config,"wep"))
	{
		if (mozart_ini_setkey(NETCONFIGPATH, NETAPSECTION, "encryption", encrypt_config)) {
			printf("Could not write encryption=%s below %s in %s.\n", encrypt_config, NETAPSECTION, NETCONFIGPATH);
		}
		memset(str_sp,0,64);
//		sprintf(str_sp,"nor set encryption=%s",encrypt_config);
//		system(str_sp);
		sprintf(str_sp,"encryption");
		nor_set(str_sp,encrypt_config);
		memset(str_sp,0,64);
	}
	else if(!strcmp(encrypt_config,"psk") || !strcmp(encrypt_config,"psk2") || !strcmp(encrypt_config,"mixed-psk"))
	{
		if(strlen(tkip_aes_config)>0)
		{
		    memset(str_sp,0,sizeof(str_sp));
			snprintf(str_sp,sizeof(str_sp),"%s+%s",encrypt_config,tkip_aes_config);
			if (mozart_ini_setkey(NETCONFIGPATH, NETAPSECTION, "encryption", str_sp)) {
				printf("Could not write encryption=%s below %s in %s.\n", str_sp, NETAPSECTION, NETCONFIGPATH);
			}
//			memset(str_sp,0,64);
//			sprintf(str_sp,"nor set encryption=%s+%s",encrypt_config,tkip_aes_config);
//			system(str_sp);
//			sprintf(str_sp,"%s+%s",encrypt_config,tkip_aes_config);
			nor_set("encryption",str_sp);

			memset(str_sp,0,64);
		}
	}
	if(password!=NULL)
	{
		if(strlen(password))
		{
			if (mozart_ini_setkey(NETCONFIGPATH, NETAPSECTION, "key", password)) {
				printf("Could not write key=%s below %s in %s.\n", password, NETAPSECTION, NETCONFIGPATH);
			}
			memset(str_sp,0,64);
//			sprintf(str_sp,"nor set ssid_password=%s",password);
//			system(str_sp);
			sprintf(str_sp,"ssid_password");
			nor_set(str_sp,password);
			memset(str_sp,0,64);
		}
	}
	if(name)
	    free(name);
	if(encrypt)
	    free(encrypt);
	if(password)
	    free(password);
	if(tkip_aes)
	    free(tkip_aes);
	if(pxml)
	    free(pxml);
	system("touch /tmp/wifi_client_is_connecting");
	// system("wifi >/dev/null 2>&1");
//	system("stopwifi.sh>/dev/null 2>&1");
    system("startap.sh>/dev/null 2>&1");
	sleep(5);
	system("rm -f /tmp/wifi_client_is_connecting");
	
	return 0;

}

int setWorkMode(xmlNodePtr tag, char *retstr)  //reboot
{
	return 0;
}
int setJoinWireless(xmlNodePtr tag, char *retstr)  //wlan1 disabled=0;network restart
{
	char ssid_bak[64]={0};
	char password[64]={0};
	char *pxml=NULL;
	char *ttag;
	#if 0
	char *name=NULL;
	char *encrypt=NULL;
	char *tkip_aes=NULL;
	char *channel=NULL;
	int channel_num=0;
	
	char cur_mode[3]="\0";
	
	char encrypt_config[32]="\0";
	char tkip_aes_config[16]="\0";
	
	char str_sp[64]="\0";
	char str_sp_wifilist[64]="\0";


	char uci_option_str[64]="\0";
	char WiredMode[10]="\0";
#endif
	ttag = strstr(tag, JoinWireless_SET_AP);
	if (!ttag) {
		mylogstrTofile("%s find tag(JoinWireless_SET_AP) failed\n", __FUNCTION__);
		goto err;		
	}
	if((pxml=xmlGetprop(ttag,(const xmlChar*)JoinWireless_SET_AP_name))==NULL) {
		mylogstrTofile("%s xmlGetprop(JoinWireless_SET_AP_name) failed\n", __FUNCTION__);
		goto err;
	}
	char *ntmp = xmldecode(pxml);
	if( !ntmp || !strlen(ntmp) || strlen(ntmp)>32) {
		mylogstrTofile("%s invaisld ssid length\n", __FUNCTION__);
		free(ntmp);
		goto err;
	}
	strncpy(ssid_bak,ntmp,sizeof(ssid_bak)-1);
	free(ntmp);
	pxml=NULL;
	
	if((pxml=xmlGetprop(ttag,(const xmlChar*)JoinWireless_SET_AP_password))==NULL) {
		mylogstrTofile("%s xmlGetprop(JoinWireless_SET_AP_password) failed, may be the wifi is open\n",
						__FUNCTION__);
	} else {
		char *ntmp = xmldecode(pxml);
		if( !ntmp || !strlen(ntmp) || strlen(ntmp)>64) {
			mylogstrTofile("%s invaisld password length\n", __FUNCTION__);
			free(ntmp);
			goto err;
		}
		strncpy(password,ntmp,sizeof(password)-1);
		free(ntmp);
		pxml=NULL;
	}
	
	//system("set_client.sh >/dev/null 2>&1");
	if(0 == save_wifi_info(ssid_bak,password))
	{
		mozart_event event = {	
			.event = {		
				.misc = {		
					.name = "dm",			
					.type = "web_conn",		
				},		
			},
			.type = EVENT_MISC,	
		};
		int ret = mozart_event_send(event);	
		if (!ret)	
			mylogstrTofile("Send dm web connected event successfully.\n");
		else	
			mylogstrTofile("Send dm web connected event unsuccessfully.\n");
	}

	return 0;
	
err:
	error_num++;
	strcat(error_info, PARAMETER_ERROR);
	return 1;
#if 0
	if((pxml=xmlGetprop(ttag,(const xmlChar*)JoinWireless_SET_AP_encrypt))!=NULL)
	{
		if((encrypt=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
			memset(encrypt,0,strlen(pxml)+1);
			strcpy(encrypt,pxml);
			pxml=NULL;
		}
	}
	else
	{
		error_num++;
		strcat(error_info, PARAMETER_ERROR);
		free(name);
		free(encrypt);
		free(password);
		free(tkip_aes);
		return 1;
	}
	
	//strcpy(name,xmlGetProp(tag->children,(const xmlChar*)JoinWireless_SET_AP_name));
	//strcpy(encrypt,xmlGetProp(tag->children,(const xmlChar*)JoinWireless_SET_AP_encrypt));
	/************************需要修改************************************/
	if( strcmp(encrypt,"NONE") )
	{
		if((pxml=xmlGetprop(ttag,(const xmlChar*)JoinWireless_SET_AP_password))!=NULL)
		{
			if((password=(char *)malloc(strlen(pxml)+1))!=NULL)
			{
				memset(password,0,strlen(pxml)+1);
				strcpy(password,pxml);
				pxml=NULL;
			}
		}
	}
		//strcpy(password,xmlGetProp(tag->children,(const xmlChar*)JoinWireless_SET_AP_password));
	if( !strcmp(encrypt,"WPA-PSK") || !strcmp(encrypt,"WPA2-PSK") || !strcmp(encrypt,"WPA/WPA2-PSK") )
	{
		if((pxml=xmlGetprop(ttag,(const xmlChar*)JoinWireless_SET_AP_tkip_aes))!=NULL)
		{
			if((tkip_aes=(char *)malloc(strlen(pxml)+1))!=NULL)
			{
				memset(tkip_aes,0,strlen(pxml)+1);
				strcpy(tkip_aes,pxml);
				pxml=NULL;
			}
		}
		
	}
	if( !strlen(name) || strlen(name)>32)
	{
		error_num++;
		strcat(error_info, ERROR_SSID);
		free(name);
		free(encrypt);
		free(password);
		free(tkip_aes);
		return 1;
	}

	if( !strcmp(encrypt,"WPA-PSK") || !strcmp(encrypt,"WPA2-PSK") || !strcmp(encrypt,"WPA/WPA2-PSK") )
	{
		if( !strlen(password) || strlen(password)<8 )
		{
			error_num++;
			strcat(error_info,ERROR_PASSWORD);
			free(name);
			free(encrypt);
			free(password);
			free(tkip_aes);
			return 1;
		}
	}
	if((pxml=xmlGetprop(ttag,(const xmlChar*)JoinWireless_SET_AP_channel))!=NULL)
	{
		if((channel=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
			memset(channel,0,strlen(pxml)+1);
			strcpy(channel,pxml);
			pxml=NULL;
		}
	}
	
	printf("Content-type:text/html\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	printf("<%s><Return status=\"true\" delay=\"20\"></Return></%s>\r\n",SETSYSSTR,SETSYSSTR);
	fflush(stdout);
	
	
	if(encrypt!=NULL)
	{
		if(!strcmp(encrypt,"NONE"))
			strcpy(encrypt_config,"none");
		else if(!strcmp(encrypt,"WEP"))
			strcpy(encrypt_config,"wep");
		else if(!strcmp(encrypt,"WPA-PSK"))
			strcpy(encrypt_config,"psk");
		else if(!strcmp(encrypt,"WPA2-PSK"))
			strcpy(encrypt_config,"psk2");
		else if(!strcmp(encrypt,"WPA/WPA2-PSK"))
			strcpy(encrypt_config,"mixed-psk");
		else
			NULL;
	}
	
	if(tkip_aes!=NULL)
	{
		if(!strcmp(tkip_aes,"tkip"))
			strcpy(tkip_aes_config,"tkip");
		else if(!strcmp(tkip_aes,"aes"))
			strcpy(tkip_aes_config,"ccmp");
		else if(!strcmp(tkip_aes,"tkip/aes"))
			strcpy(tkip_aes_config,"tkip+ccmp");
		else
			NULL;
	}

	if(strlen(name))
	{
	    strncpy(ssid_bak,name,sizeof(ssid_bak));
		//if (mozart_ini_setkey(NETCONFIGPATH, NETCLIENTSECTION, "ssid", name)) {
		//	printf("Could not write ssid=%s below %s in %s.\n", name, NETCLIENTSECTION, NETCONFIGPATH);
		//}
	}
	if(!strcmp(encrypt_config,"none") || !strcmp(encrypt_config,"wep"))
	{
		sprintf(str_sp,"%s",encrypt_config);
		//sprintf(str_sp_wifilist,"wifisavelist.@wifi-iface[50].encryption=%s",encrypt_config);
	}
	else
	{
		sprintf(str_sp,"%s+%s",encrypt_config,tkip_aes_config);
		//sprintf(str_sp_wifilist,"wifisavelist.@wifi-iface[50].encryption=%s+%s",encrypt_config,tkip_aes_config);
	}
	if (mozart_ini_setkey(NETCONFIGPATH, NETCLIENTSECTION, "encryption", str_sp)) {
		printf("Could not write encryption=%s below %s in %s.\n", str_sp, NETCLIENTSECTION, NETCONFIGPATH);
	}
	memset(str_sp,0,64);
	memset(str_sp_wifilist,0,64);
	if(password!=NULL)
	{
		if(strlen(password))
		{
			if((name=(char *)malloc(strlen(password)+1))!=NULL)
			{
				char *ntmp;
			
				memset(name,0,strlen(password)+1);
				ntmp = xmldecode(password);
				strcpy(name,ntmp);
				free(ntmp);
			}
			if (mozart_ini_setkey(NETCONFIGPATH, NETCLIENTSECTION, "key", password)) {
				printf("Could not write key=%s below %s in %s.\n", password, NETCLIENTSECTION, NETCONFIGPATH);
			}
			memset(str_sp,0,64);
			memset(str_sp_wifilist,0,64);
		}
	}

	if(channel!=NULL)
	{
		if(strlen(channel))
		{
			if (mozart_ini_setkey(NETCONFIGPATH, NETRADIOSECTION, "channel", channel)) {
				printf("Could not write channel=%s below %s in %s.\n", channel, NETRADIOSECTION, NETCONFIGPATH);
			}
		}
	}
	system("touch /tmp/wifi_client_is_connecting");

	if(name)
	    free(name);
	if(encrypt)
	    free(encrypt);
	if(password)
	    free(password);
	if(tkip_aes)
	    free(tkip_aes);
	if(channel)
	    free(channel);
	if(pxml)
	    free(pxml);

	system("rm -f /tmp/wifi_client_is_connecting");
	
#endif

}

int save_wifi_info(const char* ssid, const char* pwd)
{
	system("touch /tmp/cgi_wifi_info.ini &> /dev/null");
	if (mozart_ini_setkey("/tmp/cgi_wifi_info.ini", "wifi_info", "ssid", ssid) < 0)
		mylogstrTofile("set wifi ssid failed\n");
	if (mozart_ini_setkey("/tmp/cgi_wifi_info.ini", "wifi_info", "pwd", pwd) < 0)
		mylogstrTofile("set wifi pwd failed\n");
	return 0;
}

int setJoinWired(xmlNodePtr tag, char *retstr)  //wlan1 disabled=1;network restart
{
	return 0;
}
int setFTP(xmlNodePtr tag, char *retstr)
{
	return 0;
}
int setSAMBA(xmlNodePtr tag, char *retstr)
{
	return 0;
}
int setDMS(xmlNodePtr tag, char *retstr)
{
	return 0;
}

int Upgrade(xmlNodePtr tag, char *retstr)
{
#if 0
	int i=0;
	char cmd[64]="\0";
	char op_fw_header[32]="\0";
	char mtd_fw[64]="\0";
	char mnt_path[]="/mnt";
	struct dirent* ent = NULL;
	DIR *pDir;
	char dir[128];
	char fw_path[64];
	struct stat statbuf;
	int hasFW=0;
	FILE *fw_fp;
	memset(dir,0,sizeof(dir));
	memset(fw_path,0,sizeof(fw_path));
	if( (pDir=opendir(mnt_path))==NULL )
	{
		fprintf( stderr, "Cannot open directory:%s\n", mnt_path );
		return 1;
	}
	while( (ent=readdir(pDir))!=NULL )
	{
		snprintf( dir, 128 ,"%s/%s", mnt_path, ent->d_name );
		//printf("%s\n",dir);
		lstat(dir, &statbuf);
		if( S_ISDIR(statbuf.st_mode) )  //is a dir
		{
			if(strcmp( ".",ent->d_name) == 0 || strcmp( "..",ent->d_name) == 0) 
				continue;
			sprintf(fw_path,"%s/update.bin",dir);
			if( (fw_fp=fopen(fw_path,"rb")) != NULL )
			{
				hasFW=1;
				//strcpy(path,fw_path);
				//fclose(fw_fp);
				break;
			}
		}
	}
	
	closedir(pDir);
	
	
	if(hasFW==0)
	{
		error_num++;
		strcat(error_info, "firmware file is not existed!");
		return 1;
	}
//		error_num++;
//		strcat(error_info,fw_path);
//		return 1;

//led blink 
	int fd1;
	fd1=open("/proc/led_net_ioctl",0);
	if(fd1<0)
	{
		error_num++;
		strcat(error_info, "can't open led_net_ioctl\n");
		exit(1);
	}
	ioctl(fd1,0);
	close(fd1);
    	int fd = open("/proc/led_upgrade_ioctl", 0);
    	if(fd < 0)
    	{
		error_num++;
	  	strcat(error_info, "can't open led_upgrade_ioctl\n");
      	 	exit(1);
    	} 
	 ioctl(fd,2);
	 close(fd);

	fseek(fw_fp,4,SEEK_SET);
	fread(op_fw_header,1,7,fw_fp);

	fclose(fw_fp); 
	if( strstr(op_fw_header,"OpenWrt")==NULL )
	{
		error_num++;
		strcat(error_info, "firmware file is wrong!");
		return 1;
	}

	printf("Content-type:text/html\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	printf("<%s><Return status=\"true\" delay=\"160\"></Return></%s>\r\n",SETSYSSTR,SETSYSSTR);
	fflush(stdout);
	sprintf(mtd_fw,"mtd -r write %s firmware",fw_path);
	system(mtd_fw);
#endif
	return 0;
	
}

int halt(xmlNodePtr tag, char *retstr)
{

	printf("Content-type:text/html\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	printf("<%s><Return status=\"true\" ></Return></%s>\r\n",SETSYSSTR,SETSYSSTR);
	fflush(stdout);
	//1.关闭网络
	system("/etc/init.d/network stop");
	//2.关机
	system("halt");
	
	return 0;
}


int TimeSync(xmlNodePtr tag, char *retstr)
{
#if 0
	char *value=NULL;
	char *zone=NULL;
	char *zone_tmp=NULL;
	char zone_str[32]="\0";
	char zone_cur[32]="\0";
	char str_sp[64]="\0";
	struct tm time_tm;
    struct timeval time_tv;
    time_t timep;
	struct timezone time_tz;
	int hour;
	int minutes;
	
	char *pxml=NULL;
	char *pxml1=NULL;
	char uci_option_str[64]="\0";
	memset(zone_str,0,sizeof(zone_str));
	memset(uci_option_str,0,sizeof(uci_option_str));
	memset(zone_cur,0,sizeof(zone_cur));
	
	//Time_Sync_zone
	if((pxml=xmlGetprop(tag,(const xmlChar*)Time_Sync_zone))!=NULL && (pxml1=xmlGetprop(tag,(const xmlChar*)Time_Sync_value))!=NULL)
	{
		if((zone=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
			memset(zone,0,strlen(pxml)+1);
			strcpy(zone,pxml);
			pxml=NULL;
		}
		if((value=(char *)malloc(strlen(pxml1)+1))!=NULL)
		{
			memset(value,0,strlen(pxml1)+1);
			strcpy(value,pxml1);
			pxml1=NULL;
		}
		sscanf(value, "%d-%d-%d %d:%d:%d", &time_tm.tm_year, &time_tm.tm_mon, &time_tm.tm_mday, &time_tm.tm_hour, &time_tm.tm_min, &time_tm.tm_sec);
		time_tm.tm_year -= 1900;
		time_tm.tm_mon -= 1;
		time_tm.tm_wday = 0;
		time_tm.tm_yday = 0;
		time_tm.tm_isdst = 0;
		
		timep = mktime(&time_tm);
		time_tv.tv_sec = timep;
		time_tv.tv_usec = 0;
		time_tz.tz_dsttime=0;
		
		zone_tmp=zone;
		memcpy(zone_str,zone_tmp,3);
		zone_tmp+=3;
		
		
		if(*zone_tmp == '-')
		{
			zone_tmp++;
			strcat(zone_str,zone_tmp);
			sscanf(zone_tmp,"%d:%d",&hour,&minutes);
			time_tz.tz_minuteswest=hour*60+minutes;
			
		}
		else
		{
			zone_str[3]='-';
			zone_tmp++;
			strcat(zone_str,zone_tmp);
			sscanf(zone_tmp,"%d:%d",&hour,&minutes);
			time_tz.tz_minuteswest=hour*60+minutes;
			time_tz.tz_minuteswest=-time_tz.tz_minuteswest;
		}
		
		
		if(strcmp(zone_cur,zone_str)!=0)
		{
			sprintf(str_sp,"system.@system[0].timezone=%s",zone_str);
			uci_set_option_value(str_sp);
			memset(str_sp,0,64);
			sprintf(str_sp,"echo \"%s\">/tmp/TZ",zone_str);
			system(str_sp);
			memset(str_sp,0,64);
		}
		settimeofday(&time_tv, &time_tz);
		
	}
	
	
	//system("uci commit");
	
	free(value);
	free(zone);
	free(pxml);
	free(pxml1);
#endif
	return 0;
}

int setClient(xmlNodePtr tag, char *retstr)
{
	return 0;
}

int set3G(xmlNodePtr tag, char *retstr)
{
	return 0;
}

int setairplay(xmlNodePtr tag, char *retstr)
{

	char *name;
	char *pxml=NULL;	
	char uci_option_str[128]="\0";
	
	if((pxml=xmlGetprop(tag,(const xmlChar*)AIRPLAY_SET_name))!=NULL)
	{
		if(strlen(pxml) == 0)
			goto out;
		
		if((name=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
		
				char *ntmp;
			
				memset(name,0,strlen(pxml)+1);
				ntmp = xmldecode(pxml);
				strcpy(name,ntmp);
				free(ntmp);
		}
	}
	else
	{
out:
		error_num++;
		strcat(error_info, PARAMETER_ERROR);
		free(name);
		return 1;
	}

	//set airplay name
	
	printf("Content-type:text/html\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	printf("<%s><Return status=\"true\" delay=\"30\"></Return></%s>\r\n",SETSYSSTR,SETSYSSTR);
	fflush(stdout);

	if (mozart_ini_setkey("/usr/data/shair.ini", "shair", "name", name)) {
		printf("Could not set name=%s below shair in /usr/data/shair.ini.\n",name);
		//return -1;
	}

	memset(uci_option_str,0,64);
	sprintf(uci_option_str,"nor set airplay_name=\"%s\"",name);
	system(uci_option_str);
	
	free(name);
	system("sync");
	//system("killall mozart");
	sleep(1);
	//system("mozart -sb");
   // system("startap.sh");
	return 0;

	
}

int getairplay(xmlNodePtr tag, char *retstr)
{
	char name[64]="\0";

	if (mozart_ini_getkey("/usr/data/shair.ini", "shair", "name", name)) {
		printf("Could not get name's value below shair in /usr/data/shair.ini.\n");
		//return -1;
	}
	sprintf(retstr,"<%s name=\"%s\"></%s>",FN_GET_AIRPLAY_NAME, name, FN_GET_AIRPLAY_NAME);
	return 0;
}

int setiperf(xmlNodePtr tag, char *retstr)
{
	char *enable=NULL;
	char *pxml=NULL;
	
	if((pxml=xmlGetprop(tag,(const xmlChar*)SET_iperf_enable))!=NULL)
	{
		if((enable=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
			memset(enable,0,strlen(pxml)+1);
			strcpy(enable,pxml);
			pxml=NULL;
		}
		if(!strcmp(enable,"ON"))
		{
			system("killall iperf");
			system("iperf -s >/dev/null 2>&1 &");//not use -D option. because -D make killall doesn't work
		}
		if(!strcmp(enable,"OFF"))
		{
			system("killall iperf");
		}
		
	}
	
	free(enable);
	free(pxml);
	return 0;
}

int setotastatus(xmlNodePtr tag, char *retstr)
{
	char *status=NULL;
	char *pxml=NULL;
	
	if((pxml=xmlGetprop(tag,(const xmlChar*)SET_OTAStatus))!=NULL)
	{
		if((status=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
			memset(status,0,strlen(pxml)+1);
			strcpy(status,pxml);
			free(pxml);

			pxml=NULL;
		}

		
		if(!strcmp(status,"0"))
		{
			system("cfg set version_flag=0");
		}
		else
		{
			system("cfg set version_flag=1");
		}
		free(status);
	}
	
	
	printf("Content-type:text/html\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	printf("<%s><Return status=\"true\" ></Return></%s>\r\n",SETSYSSTR,SETSYSSTR);
	fflush(stdout);

	return 0;
}

int setvrswitch(xmlNodePtr tag, char *retstr)
{
	char *switcher=NULL;
	char *pxml=NULL;
	
	if((pxml=xmlGetprop(tag,(const xmlChar*)SET_VRswitch))!=NULL)
	{
		if((switcher=(char *)malloc(strlen(pxml)+1))!=NULL)
		{
			memset(switcher,0,strlen(pxml)+1);
			strcpy(switcher,pxml);
			free(pxml);

			pxml=NULL;
		}
		if (strcmp(switcher, "on") == 0)
			send_misc_event("dm", "vr_start");
		else if (strcmp(switcher, "off") == 0)
			send_misc_event("dm", "vr_end");
		else
			mylogstrTofile("switch=%s\n",switcher );
		free(switcher);
	}
	
	
	printf("Content-type:text/html\r\n\r\n");
	printf("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
	printf("<%s><Return status=\"true\" ></Return></%s>\r\n",SETSYSSTR,SETSYSSTR);
	fflush(stdout);

	return 0;
}


