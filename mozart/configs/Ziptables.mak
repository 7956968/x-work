ifeq ("$(SUPPORT_ETHLAN)","1")
iptables_DIR := $(TOPDIR)/tools/device-tools/iptables-1.4.21

TARGETS += iptables


define iptables_CONFIGURE_CMDS
endef

define iptables_BUILD_CMDS
	$(MAKE1) -C $(@D)
endef

define iptables_INSTALL_TARGET_CMDS
	$(MAKE1) -C $(@D) DESTDIR=$(MOZART_UPDATER_DIR) install
endef

define iptables_CLEAN_CMDS
	-$(MAKE) -C $(@D) clean
endef

define iptables_UNINSTALL_TARGET_CMDS
	-$(MAKE1) -C $(@D) DESTDIR=$(MOZART_UPDATER_DIR) uninstall
endef

$(eval $(call install_rules,iptables,$(iptables_DIR),target))
endif
