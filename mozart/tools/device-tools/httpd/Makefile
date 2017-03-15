include $(TOPDIR)/configs/config.mk

INSTALL = install

install:
	$(INSTALL) -d $(MOZART_UPDATER_DIR)/etc
	$(INSTALL) -d $(MOZART_UPDATER_DIR)/usr/mips
	cp -rf ./etc/*  $(MOZART_UPDATER_DIR)/etc/
	cp -rf ./mips/*  $(MOZART_UPDATER_DIR)/usr/mips/

clean distclean uninstall:

.PHONY:all clean install uninstall $(TARGET)