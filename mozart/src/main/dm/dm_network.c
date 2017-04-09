#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <string.h>

#include "mozart_config.h"

#include "wifi_interface.h"
#include "dm6291_led.h"
#include "mozart_app.h"
#include "dm_mozart.h"
#include "dm_network.h"
#include "ini_interface.h"


static int g_wirestate_now = LINK_DOWN;
static int g_wirestate_old = LINK_DOWN;

int get_wire_state()
{
	return g_wirestate_now;
}

int set_wire_state(int i)
{
	g_wirestate_now = i;
	return 0;
}

#define STR_MAX_LEN_NET 128

int get_WIRED_state(void) 
{ 
	char buffertt[STR_MAX_LEN_NET] = {0};
	FILE *read_fp = NULL; 
	int chars_read = 0; 
	int ret = 0; 
 
	read_fp = popen("ifconfig eth0 | grep RUNNING", "r"); 
	if ( read_fp != NULL ) 
	{ 
		chars_read = fread(buffertt, sizeof(char), STR_MAX_LEN_NET-1, read_fp); 
		if (chars_read > 0) 
		{ 
			ret = 1; 
		} 
		else 
		{ 
			ret = 0; 
		} 
		pclose(read_fp); 
	} 
	else 
	{ 
		ret = 0; 
	} 
	return ret; 
} 

int check_wired_link_up(void)
{
	char cmdStr[128] = {0};
	
	g_wirestate_now = get_WIRED_state();
	if(g_wirestate_now)
	{
		if(LINK_DOWN== g_wirestate_old)//link down ->link up
		{
			printf("eth0 is up\n");
			memset(cmdStr,0x0,sizeof(cmdStr));
			strcpy(cmdStr,"kill -9 `ps -w|grep udhcpc|grep eth0|awk -F \" \" '{print $3}'`");
			system(cmdStr);
			system("udhcpc -t 0 -i eth0 -b -p /var/run/dhcp-eth0.pid -O rootpath -R &");//need 5s
			printf("eth0 is up 11\n");
			sleep(5);
		}
	}
	else
	{
		if(LINK_UP == g_wirestate_old)//linkup to link down
		{
			printf("eth0 is down\n");
			memset(cmdStr,0x0,sizeof(cmdStr));
			strcpy(cmdStr,"kill -9 `ps -w|grep udhcpc|grep eth0|awk -F \" \" '{print $3}''`");
			printf("eth0 is down 11\n");
			system(cmdStr);
		}
	}
	g_wirestate_old = g_wirestate_now;
	return g_wirestate_now;
}

#ifdef WIRE


static ether_state_t ether_state_cur = ETHER_NULL;
ether_state_t ether_state_old = ETHER_NULL;


void runudhcpc(char *networkdevice)
{
    char deviceName[32] = {0};
    char cmdstr[64] = {0};
	system("killall udhcpc");
	if (mozart_ini_getkey("/usr/data/shair.ini", "shair", "name", deviceName)) {
		printf("Could not get name's value below shair in /usr/data/shair.ini.\n");
	}
	if(strlen(deviceName) < 1)
		strcpy(deviceName,"airmusic");
	snprintf(cmdstr,sizeof(cmdstr),"udhcpc -i %s -x hostname:\"%s\" -q &",networkdevice,deviceName);
	system(cmdstr);
	return;
}
int check_wire()
{
	wifi_info_t network_infor;
	wifi_ctl_msg_t switch_wifi = {0};
	//static wifi_mode_t wifi_mode_old = AP;	
	network_infor = get_wifi_mode();
	ether_state_cur = network_infor.ether_state;
	if((ETHER_CONNECT == ether_state_cur) && (WIFI_NULL != network_infor.wifi_mode) )
	//if((ETHER_CONNECT == ether_state_cur) && (ETHER_CONNECT != ether_state_old) )
	{//disable wifi
		dm_info("wire up, stop wifi\n");
//		DM6291_SetLedBrightness_gpio(DM6291_LED_WIFI,DM6291_LED_ON);
		//wifi_mode_old = network_infor.wifi_mode;
		mozart_stop();
		stopall(1);
		mozart_system("killall ntpd > /dev/null 2>&1");
		mozart_system("killall dnsmasq > /dev/null 2>&1");
		switch_wifi.cmd = STOP_WIFI;//SW_AP;//
		switch_wifi.force = true;
		strcpy(switch_wifi.name, get_app_name());
		if(request_wifi_mode(switch_wifi) != true)
			printf("JJJHHHERROR: [%s] Request STOP_WIFI Failed, Please Register First!!!!\n", get_app_name());
		runudhcpc("eth0");
		sleep(1);
		mozart_system("ntpd -nq &");
		mozart_system("dnsmasq &");
		//getaaaaip();
		startall(1);
	//dmr_start();//must restart dmr
	}
	else if((ETHER_CONNECT != ether_state_cur)&&(WIFI_NULL == network_infor.wifi_mode))
	//else if((ETHER_CONNECT != ether_state_cur)&&(ETHER_CONNECT == ether_state_old))
	{//enable wifi
		//must do some confirm
    	sleep(3);
  		network_infor = get_wifi_mode();
		if (WIFI_NULL == network_infor.wifi_mode) {
			dm_info("wire down, wifi switch to sta mode\n");
			system("ifconfig wlan0 up");
			mozart_stop();
			stopall(1);
			/*if(AP == wifi_mode_old)
				switch_wifi.cmd = SW_AP;
			else*/
			switch_wifi.cmd = SW_STA;
			switch_wifi.force = true;
			memset(switch_wifi.param.switch_ap.ssid, 0, sizeof(switch_wifi.param.switch_ap.ssid));
			strcpy(switch_wifi.name, get_app_name());
			if(request_wifi_mode(switch_wifi) != true)
				printf("JJJHHHERROR: [%s] Request SW_STA Failed, Please Register First!!!!\n", get_app_name());
			startall(1);
		}
		//dmr_start();//must restart dmr
	}//printf("jjjhhh network_infor.wifi_mode=%d ether_state_cur=%d",network_infor.wifi_mode,ether_state_cur);
	ether_state_old = ether_state_cur;
			
	/*else
	{
		check_wired_link_up();
	}	*/
	return 0;
}
#endif

int stop_wifi_mode(void)
{	
	wifi_ctl_msg_t stop_wifi;
	memset(&stop_wifi, 0, sizeof(wifi_ctl_msg_t));
	stop_wifi.cmd = STOP_WIFI;
	request_wifi_mode(stop_wifi);	
	return 0;
}

int stop_wifi_mode_and_clear(void)
{	
	stop_wifi_mode();	
	system("echo \"ctrl_interface=/var/run/wpa_supplicant\" > /usr/data/wpa_supplicant.conf");	
	system("echo \"ap_scan=1\" >> /usr/data/wpa_supplicant.conf");
	return 0;
}

void create_wifi_info(const char *ssid, const char *psk, int priority)
{	
	char cmd[128];
	system("echo \"network={\" >> /usr/data/wpa_supplicant.conf");
	system("echo \"scan_ssid=1\" >> /usr/data/wpa_supplicant.conf");	
	sprintf(cmd, "echo \"ssid=\\\"%s\\\"\" >> /usr/data/wpa_supplicant.conf", ssid);	
	system(cmd);
	snprintf(cmd, 128, "echo \"psk=\"%s\"\" >> /usr/data/wpa_supplicant.conf", psk);
	system(cmd);
	system("echo \"bssid=\" >> /usr/data/wpa_supplicant.conf");	snprintf(cmd, 128, "echo \"priority=%d\" >> /usr/data/wpa_supplicant.conf", priority);	
	system(cmd);	
	system("echo \"}\" >> /usr/data/wpa_supplicant.conf");
}


extern char* app_name;
int dm_wifi_mode_switch(int cmd) 
{
	wifi_ctl_msg_t new_mode;
	memset(&new_mode, 0, sizeof(new_mode));
	new_mode.cmd = cmd;
	new_mode.force = true;
	strcpy(new_mode.name, app_name);
	if (cmd == SW_STA)
		new_mode.param.switch_sta.sta_timeout = 60;		
	if(request_wifi_mode(new_mode) != true)
		printf("ERROR: [%s] Request Network Failed, Please Register First!!!!\n", app_name);
	return 0;
}
int dm_switch_sta_by_ssid(const char* ssid, const char* pwd) 
{
	stop_wifi_mode_and_clear();
	create_wifi_info(ssid, pwd, 1);
	dm_wifi_mode_switch(SW_STA);
	return 0;
}

int dm_switch_sta_by_file()
{
	char ssid[64] = {0}; 
	char pwd[128] = {0}; 
	if (mozart_ini_getkey("/tmp/cgi_wifi_info.ini", "wifi_info", "ssid", ssid)) {  
		printf("Could not get %s's value below %s in %s.\n", "ssid", "wifi_info", "/tmp/cgi_wifi_info.ini");             
		return -1;     
	}
	if (mozart_ini_getkey("/tmp/cgi_wifi_info.ini", "wifi_info", "pwd", pwd)) {  
		printf("Could not get %s's value below %s in %s.\n", "pwd", "wifi_info", "/tmp/cgi_wifi_info.ini");             
		return -1;     
	}
	return dm_switch_sta_by_ssid(ssid, pwd);
}

int dm_switch_sta_by_invaildssid() 
{
	return dm_switch_sta_by_ssid("invalid-ssid", "invalid-pwd");
}

//set ethernet  to fixed ip.this step is necessary for being a lan port.
int set_eth_ip()
{
	#if (SUPPORT_ETHLAN==1)
	system("fix_eth_ip.sh");
	#endif
	return 0;
}

int add_nat_for_ethlan()
{
	printf("let ether to be lan port\n");
	system("echo \"1\" > /proc/sys/net/ipv4/ip_forward");
	system("iptables -t nat -A POSTROUTING -s 192.168.10.0/24 -o wlan0 -j MASQUERADE");
	return 0;
}



#if 0
int dm_get_wifi_mode()
{
	wifi_info_t infor = get_wifi_mode();
	return infor.wifi_mode;
}

int dm_wifi_config_with_ssidkey(char* ssid, char* key)
{	
	wifi_ctl_msg_t new_mode;
	memset(&new_mode, 0, sizeof(wifi_ctl_msg_t));	
	new_mode.cmd = SW_STA;
	new_mode.force = true;
	new_mode.param.switch_sta.sta_timeout = 30;
	strncpy(new_mode.param.switch_sta.ssid, ssid, 64);
	strncpy(new_mode.param.switch_sta.psk, key, 128);

	strcpy(new_mode.name, app_name);
	if(request_wifi_mode(new_mode) != true)
		printf("ERROR: [%s] Request Network Failed, Please Register First!!!!\n", app_name);
	return 0;
}

int dm_web_conn()
{
	char* ssid = sys_get_stassid();
	char* key = sys_get_stakey();
	dm_wifi_config_with_ssidkey(ssid, key);
	safe_free(ssid);
	safe_free(key);
	return 0;
}
#endif
