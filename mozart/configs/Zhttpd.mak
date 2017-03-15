ifeq (${SUPPORT_APACHE},1)
target := httpd
httpd_DIR := $(TOPDIR)/tools/device-tools/httpd

TARGETS += httpd
TARGETS1 += httpd

httpd_patch_DIR := $(TOPDIR)/configs/httpd_patches
define httpd_PATCH_CMDS
endef

define httpd_CONFIGURE_CMDS
endef

define httpd_BUILD_CMDS
	echo "building httpd"
	$(MAKE) -C $(@D) 
endef

define httpd_INSTALL_TARGET_CMDS
	$(httpd_BUILD_CMDS)
	$(MAKE) -C $(@D) install
endef

define httpd_CLEAN_CMDS
	-$(MAKE) -C $(@D) clean
endef

define httpd_UNINSTALL_TARGET_CMDS
	-$(MAKE) -C $(@D) uninstall
endef

$(eval $(call install_rules,httpd,$(httpd_DIR),target))
endif