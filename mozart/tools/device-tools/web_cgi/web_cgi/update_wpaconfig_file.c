#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "linklist_interface.h"
#include "update_wpaconfig_file.h"

typedef struct wpa_conf_info{
	char ssid[64];
	char psk[128];
	char key_mgmt[32];
	char bssid[24];
	char priority[8];
} WiFi_Info;

#define WPA_CONF "/usr/data/wpa_supplicant.conf"

char *wpa_buf = "ctrl_interface=/var/run/wpa_supplicant/\nap_scan=1\n";
bool find = false, discover = false;
bool new_config = true;
List wpa_info_list;
List wifi_info_list;
WiFi_Info *information, *get_element;

size_t os_strlen(const char *s)
{
	const char *p = s;
	while (*p)
		p++;
	return p - s;
}

static int hex2num(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}


int rewrite_wpaconfig_file(List *list)
{
	int j,wpa_conf_fd;
	char tmp[256] = {};

	wpa_conf_fd = open(WPA_CONF, O_RDWR | O_TRUNC, 00644);
	if(wpa_conf_fd < 0){
		perror("open file fail");
		return -1;
	}
	write(wpa_conf_fd, wpa_buf, strlen(wpa_buf));

	//printf("WiFi WPA Info List len = %ld\n", list_get_length(list));
	for(j = 0; j < list_get_length(list); j++){
		get_element = list_get_element(list, j);
		if(get_element == NULL){
			if(j == 0){
				printf("[ERROE]WPA Configure Info List Head is NULL!!!!\n");
				return -1;
			}
			else
				continue;
		}
		write(wpa_conf_fd, "network={\nscan_ssid=1\n", strlen("network={\nscan_ssid=1\n"));

		memset(tmp, 0, 256);
		sprintf(tmp, "ssid=%s\n", get_element->ssid);
		//printf("#====SSID====#: %s\n",tmp);
		write(wpa_conf_fd, tmp, strlen(tmp));

		if(strncmp(get_element->key_mgmt, "NONE", strlen("NONE"))){
			memset(tmp, 0, 256);
			sprintf(tmp, "psk=%s\n", get_element->psk);
			//printf("#====PSK====#: %s\n",tmp);
			write(wpa_conf_fd, tmp, strlen(tmp));
			//write(wpa_conf_fd, "key_mgmt=WPA-PSK\n", strlen("key_mgmt=WPA-PSK\n"));
			//printf("#====Key_Mgmt====#: WPA_PSK\n");
		}
		else{
			write(wpa_conf_fd, "key_mgmt=NONE\n", strlen("key_mgmt=NONE\n"));
			//printf("#====Key_Mgmt====#: NONE\n");
		}

		memset(tmp, 0, 256);
		sprintf(tmp, "bssid=%s\n", get_element->bssid);
		//printf("#====BSSID====#: %s\n",tmp);
		write(wpa_conf_fd, tmp, strlen(tmp));

		memset(tmp, 0, 256);
		sprintf(tmp, "priority=%s\n", get_element->priority);
		//printf("#====Priority====#: %s\n",tmp);
		write(wpa_conf_fd, tmp, strlen(tmp));

		write(wpa_conf_fd, "}\n", strlen("}\n"));
	}
	close(wpa_conf_fd);
	list_destroy(list, NULL);

}


int update_wpaconfig_file(char *ssid, char* pwd, char* bssid)
{
	FILE *fp;
	char *ps, *pe, *buffer;
	int file_size = 0;
	char buf[256] = {}, tmp[64] = {};
	int prior = 0,size;

	fp = fopen(WPA_CONF,"r+");
	if(!fp){
		perror("fopen");
		return -1;
	}
#if 1
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	buffer =  (char *)malloc(file_size * sizeof(char));
	memset(buffer, 0, file_size * sizeof(char));
	fread(buffer, file_size, sizeof(char), fp);
	if((ps = strstr(buffer,"priority="))){
		while((pe = strstr(ps,"priority="))){
			ps = strstr(pe,"\n");
			strncpy(tmp, pe + 9, strlen(pe + 9) - strlen(ps));
			if(prior < atoi(tmp))
				prior = atoi(tmp);
		}
		//printf("Wpa_supplicant Max Priority is %d\n",prior);
		free(buffer);
	}
#endif
	memset(tmp, 0, 64);
	sprintf(tmp, "\"%s\"", ssid);
	list_init(&wifi_info_list);
	fseek(fp, 0, SEEK_SET);
	while(fgets(buf, sizeof(buf), fp)){
		if(buf[0] == '#')
			continue;

		if (strstr(buf, "ssid=\"")) {
			information = (WiFi_Info *)malloc(sizeof(WiFi_Info));
			memset(information, 0, sizeof(WiFi_Info));
			size = strlen(strstr(buf, "ssid=")) - strlen("ssid=") - strlen("\n");
			strncpy(information->ssid, strstr(buf, "ssid=") + strlen("ssid="), size);
			//printf("====SSID====: [%s]\n", information->ssid);
			if(!strncmp(information->ssid, tmp, strlen(tmp))){
				find = true;
				new_config = false;
			}
		}
		if (strstr(buf, "psk=")) {
			size = strlen(strstr(buf, "psk=")) - strlen("psk=") - strlen("\n");
			strncpy(information->psk, strstr(buf, "psk=") + strlen("psk="), size);
			if(find == true){
				if(strlen(pwd) != 0){
					memset(tmp, 0, 64);
					sprintf(tmp, "\"%s\"", pwd);
					if(strncmp(information->psk, tmp, strlen(tmp))){
						memset(information->psk, 0, strlen(information->psk));
						strncpy(information->psk, tmp, strlen(tmp));
					}
				}
				else{
					memset(information->psk, 0, strlen(information->psk));
					memset(information->key_mgmt, 0, strlen(information->key_mgmt));
					strncpy(information->key_mgmt, "NONE", strlen("NONE"));
				}
			}
			//printf("====PSK====: [%s]\n", information->psk);
		}
		if (strstr(buf, "key_mgmt=")) {
			size = strlen(strstr(buf, "key_mgmt=")) - strlen("key_mgmt=") - strlen("\n");
			strncpy(information->key_mgmt, strstr(buf, "key_mgmt=") + strlen("key_mgmt="), size);
			//printf("====Key_Mgmt====: [%s]\n", information->key_mgmt);
			if(find == true){
				if(strlen(pwd) != 0){
					memset(information->key_mgmt, 0, strlen(information->key_mgmt));
					memset(tmp, 0, 64);
					sprintf(tmp, "\"%s\"", pwd);
					memset(information->psk, 0, strlen(information->psk));
					strncpy(information->psk, tmp, strlen(tmp));
				}
			}
		}
		if (strstr(buf, "bssid=")) {
			size = strlen(strstr(buf, "bssid=")) - strlen("bssid=") - strlen("\n");
			strncpy(information->bssid, strstr(buf, "bssid=") + strlen("bssid="), size);
			if(find == true){
				if(bssid != NULL){
					if(strncmp(information->bssid, bssid, strlen(bssid)))
						strncpy(information->bssid, bssid, strlen(bssid));
				}
			}
			//printf("====BSSID====: [%s]\n", information->bssid);
		}
		if (strstr(buf, "priority=")) {
			size = strlen(strstr(buf, "priority=")) - strlen("priority=") - strlen("\n");
			strncpy(information->priority, strstr(buf, "priority=") + strlen("priority="), size);
			if(find == true){
				memset(tmp, 0, 64);
				sprintf(tmp, "%d", prior + 1);
				strncpy(information->priority, tmp, strlen(tmp));
				find = false;
			}
			//printf("====Priority====: [%s]\n", information->priority);

			list_insert(&wifi_info_list,information);
		}
	}
	if(new_config == true){
		information = (WiFi_Info *)malloc(sizeof(WiFi_Info));
		memset(information, 0, sizeof(WiFi_Info));
		memset(tmp, 0, 64);
		sprintf(tmp, "\"%s\"", ssid);
		strncpy(information->ssid, tmp, strlen(tmp));

		if(strlen(pwd) != 0){
			memset(tmp, 0, 64);
			sprintf(tmp, "\"%s\"", pwd);
			strncpy(information->psk, tmp, strlen(tmp));
			memset(information->key_mgmt, 0, strlen(information->key_mgmt));
			//strncpy(information->key_mgmt, "WPA-PSK\n", strlen("WPA-PSK\n"));
		}
		else{
			memset(information->psk, 0, strlen(information->psk));
			strncpy(information->key_mgmt, "NONE\n", strlen("NONE\n"));
		}

		if(bssid != NULL)
			strncpy(information->bssid, bssid, strlen(bssid));

		memset(tmp, 0, 64);
		sprintf(tmp, "%d", prior + 1);
		strncpy(information->priority, tmp, strlen(tmp));

		list_insert(&wifi_info_list,information);
	}
	fclose(fp);
	new_config = true;
	rewrite_wpaconfig_file(&wifi_info_list);

	return 0;
}

int filter_special_char(char *pwd)
{
	int i;
	char tmp[64] = {}, ascii[8] = {};
	//printf("Before filter, recv pwd data is %s\n", pwd);
	strncpy(tmp, pwd, strlen(pwd));

	for(i = 0; i < strlen(pwd); i++){
		memset(ascii, 0 , 8);
		sprintf(ascii, "%d\n", tmp[i]);
		//printf("ASCII: %s\n",ascii);
	}
	return os_strlen(pwd);
}


int create_wpa_config_file(char* ssid , char* password, char* bssid)
{
	int wpa_conf_fd, len;
	char priority[32] = {0};
	char ssid_name[64] = {0};
	char psk[128] = {0}, pwd[64] = {0};
	char bssidl[24] = {0};
	//unsigned char mac_addr[ETH_ALEN];

	if(strlen(ssid) == 0){
		mylogstrTofile("[Warnning]: ssid is null !!!!!!\n");
		return 0;
	}
    //mylogstrTofile("aaaaaaaaaa");
	len = filter_special_char(password);
	if(len == 0){
		mylogstrTofile("The length of password is 0 and Unencrypted networks\n",os_strlen(password));
		memset(pwd, 0, 64);
	}
	else{
		mylogstrTofile("Valid length of the password:%d\n",len);
		strncpy(pwd, password, len);
	}

	if(os_strlen(pwd) != 0){
		if((os_strlen(pwd) < 8) || (os_strlen(pwd) > 63)){
			mylogstrTofile("[ERROR]: Invalid passphrase length %lu (expected: 8..63)\n",os_strlen(pwd));
			return -1;
		}
	}
#if 0
	if(hwaddr_aton(value, mac_addr)){
		return -1;
	}
#endif
	if(bssid != NULL){
		if((os_strlen(bssid) != 17) && (os_strlen(bssid) != 0)){
			mylogstrTofile("[ERROR]: Invalid bssid format!!(expected: xx:xx:xx:xx:xx:xx,length:17 or 0)\n");
			return -1;
		}
	}

	if(access(WPA_CONF,F_OK) != 0){
		mylogstrTofile("[%s] is not exist, Create!\n",WPA_CONF);
		wpa_conf_fd = open(WPA_CONF, O_CREAT | O_RDWR | O_APPEND, 00644);
		if(wpa_conf_fd < 0){
			perror("open file fail");
			return -1;
		}

		sprintf(ssid_name, "ssid=\"%s\"\n", ssid);
		if(strlen(pwd) != 0)
			sprintf(psk, "psk=\"%s\"\n", pwd);
		sprintf(priority, "priority=%d\n", 1);

		write(wpa_conf_fd, wpa_buf, strlen(wpa_buf));
		write(wpa_conf_fd, "network={\nscan_ssid=1\n", strlen("network={\nscan_ssid=1\n"));
		write(wpa_conf_fd, ssid_name, strlen(ssid_name));
		if(strlen(pwd) != 0){
			write(wpa_conf_fd, psk, strlen(psk));
			//write(wpa_conf_fd, "key_mgmt=WPA-PSK\n", strlen("key_mgmt=WPA-PSK\n"));
		}
		else
			write(wpa_conf_fd, "key_mgmt=NONE\n", strlen("key_mgmt=NONE\n"));

		if(bssid != NULL){
			sprintf(bssidl,"bssid=%s\n",bssid);
			write(wpa_conf_fd, bssidl, strlen(bssidl));
		}
		else
			write(wpa_conf_fd, "bssid=\n", 7);

		write(wpa_conf_fd, priority, strlen(priority));
		write(wpa_conf_fd, "}\n", 2);
		close(wpa_conf_fd);
		goto exit;
	}
	if(update_wpaconfig_file(ssid, pwd, bssid) != 0)
		return -1;
exit:
	mylogstrTofile("%s create successfully!\n",WPA_CONF);
	return 0;
}


