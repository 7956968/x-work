include $(TOPDIR)/configs/config.mk


all install clean uninstall: 
	if [ "$(SUPPORT_FS)" = "ext4" ];then \
		make -C web_cgi $@; \
	elif [ "$(SUPPORT_FS)" = "cramfs" ];then \
		make -C web_cgi $@; \
	elif [ "$(SUPPORT_FS)" = "ubifs" ];then \
		make -C web_cgi $@; \
	else \
		echo "unknow fs:$(SUPPORT_FS)"&& term; \
	fi
	
.PHONY:all clean install uninstall $(SUBDIR)
