ifeq (${SUPPORT_APACHE},1)
target := web_cgi
web_cgi_DIR := $(TOPDIR)/tools/device-tools/web_cgi
web_cgi_DEPENDENCIES := wireless_tools 

TARGETS += web_cgi
TARGETS1 += web_cgi

define web_cgi_PATCH_CMDS
endef

define web_cgi_CONFIGURE_CMDS
endef

define web_cgi_BUILD_CMDS
	echo "building web_cgi"
	$(MAKE) -C $(@D) 
endef

define web_cgi_INSTALL_TARGET_CMDS
	$(web_cgi_BUILD_CMDS)
	$(MAKE) -C $(@D) install
endef

define web_cgi_CLEAN_CMDS
	-$(MAKE) -C $(@D) clean
endef

define web_cgi_UNINSTALL_TARGET_CMDS
	-$(MAKE) -C $(@D) uninstall
endef

$(eval $(call install_rules,web_cgi,$(web_cgi_DIR),target))
endif