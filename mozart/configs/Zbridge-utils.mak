ifeq ("1","0")
bridge-utils_DIR := $(TOPDIR)/tools/device-tools/bridge-utils-1.4

TARGETS += bridge-utils


define bridge-utils_CONFIGURE_CMDS
endef

define bridge-utils_BUILD_CMDS
	$(MAKE1) -C $(@D)
endef

define bridge-utils_INSTALL_TARGET_CMDS
	$(MAKE1) -C $(@D) DESTDIR=$(MOZART_UPDATER_DIR) install
endef

define bridge-utils_CLEAN_CMDS
	-$(MAKE) -C $(@D) clean
endef

define bridge-utils_UNINSTALL_TARGET_CMDS
	-$(MAKE1) -C $(@D) DESTDIR=$(MOZART_UPDATER_DIR) uninstall
endef

$(eval $(call install_rules,bridge-utils,$(bridge-utils_DIR),target))
endif
