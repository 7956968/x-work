#ifndef DM_NETWORK_H
#define DM_NETWORK_H

#define LINK_UP 1
#define LINK_DOWN 0

int get_wire_state();
int set_wire_state(int i);
int check_wire();
int dm_wifi_mode_switch(int cmd) ;
int dm_get_wifi_mode();
int dm_switch_sta_by_file();
int set_eth_ip();
int add_nat_for_ethlan();
int dm_switch_sta_by_file();
#endif
