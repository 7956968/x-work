CC = mipsel-linux-gcc
CXX= mipsel-linux-g++
STRIP = mipsel-linux-strip

CFLAGS = -I$(MOLIB_DIR)/usr/include -I$(MOLIB_UPDATER_DIR)/usr/include
LDFLAGS = -L$(MOLIB_DIR)/usr/lib -L$(MOLIB_UPDATER_DIR)/usr/lib

CFLAGS += -g -Wall -O2 -I$(MOZART_DIR)/usr/include -I$(MOZART_UPDATER_DIR)/usr/include
LDFLAGS += -lpthread -L$(MOZART_DIR)/usr/lib -L$(MOZART_UPDATER_DIR)/usr/lib -lm -lwifi -ljson-c -lutils -lini  -ltips -lplayer -llinklist -levent -lvolume -lasound 

DESTDIR ?= $(MOZART_UPDATER_DIR)
PREFIX = /usr
SBIN_DIR = $(PREFIX)/sbin
CGI_BIN = /usr/mips/cgi-bin
INITDIR = /etc/init.d/
WWW_BIN = /var/www
CONFDIR =  /etc

CP=cp -rf
INSTALL = install

TARGET = SysInfo SysUpgrade sysupdate cfg nor

#CFG_TARGET = cfg nor check_stm8 KeepChannel mnt_pc_dc ctl_wifi_for_3_0

#LED_TARGET = led_control_reload led_on


#ifdef ENCRYPT_EN
#TARGET += encrypt
#endif

XML_CGI_OBJ= main.o cgiget.o cgihandle.o cgipost.o uci_for_cgi.o cgiWireless.o nor_control.o update_wpaconfig_file.o sysconfig.o

all: $(TARGET) # $(LED_TARGET) install

sysupdate:sysupdate.o cgiget.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
SysInfo: $(XML_CGI_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -liw
SysUpgrade:SysUpgrade.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
cfg:cfg.o uci_for_cgi.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
nor:nor.o nor_control.o uci_for_cgi.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#mm:mm.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#test_uart:test_uart.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#led_control_reload:led_control_reload.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#led_on:led_on.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#set_mac:set_mac.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#StmUpgrade:StmUpgrade.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#ArtUpgrade:ArtUpgrade.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#BootUpgrade:BootUpgrade.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#check_stm8:check_stm8.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#KeepChannel:KeepChannel.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#encrypt:encrypt.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^	

#mnt_pc_dc:mnt_pc_dc.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#ctl_wifi_for_3_0:ctl_wifi_for_3_0.o
#	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
install:
	$(INSTALL) -d $(DESTDIR)$(CGI_BIN)
	$(INSTALL) -d $(DESTDIR)$(SBIN_DIR)
	$(INSTALL) -d $(DESTDIR)$(INITDIR)
#	$(INSTALL) -d $(DESTDIR)$(WWW_BIN)
	$(INSTALL) -d $(DESTDIR)$(CONFDIR)

#	$(CP) web_www/* $(DESTDIR)$(WWW_BIN)/
#	$(CP) conf/httpd.conf $(DESTDIR)$(CONFDIR)/
	$(INSTALL) -m 755 sysupdate $(DESTDIR)$(CGI_BIN)/
	$(INSTALL) -m 755 SysInfo $(DESTDIR)$(CGI_BIN)/
	$(INSTALL) -m 755 SysUpgrade $(DESTDIR)$(CGI_BIN)/
#	$(INSTALL) -m 755 cfg $(DESTDIR)$(SBIN_DIR)/
#	$(INSTALL) -m 755 nor $(DESTDIR)$(SBIN_DIR)/
#	$(INSTALL) -m 755 sysupdate $(DESTDIR)$(SBIN_DIR)

clean:
	-rm -rf *.o $(TARGET) $(CFG_TARGET)

uninstall:
	-rm -f $(DESTDIR)$(WWW_BIN)/*
	-rm -f $(DESTDIR)$(WWW_BIN)/
	-rm -f $(DESTDIR)$(CONFDIR)/*
	-rm -f $(DESTDIR)$(CONFDIR)/
	-rm -f $(DESTDIR)$(CGI_BIN)/sysupdate
	-rm -f $(DESTDIR)$(CGI_BIN)/SysInfo
	-rm -f $(DESTDIR)$(CGI_BIN)/SysUpgrade
	-rm -f $(DESTDIR)$(CGI_BIN)/
	-rm -f $(DESTDIR)$(SBIN_DIR)/cfg
	-rm -f $(DESTDIR)$(SBIN_DIR)/nor
	-rm -f $(DESTDIR)$(SBIN_DIR)/
	-rm -f $(DESTDIR)$(INITDIR)/S99webserver.sh
