<a name="1.8.0"></a>
# 1.8.0 (2016-08-22)


### Bug Fixes

* **airplay:** remove a compile warning 66c7d41
* **ALSA:** strip alsa-lib/smixer/*.so 495d309
* **app-config:** fix compile error and mask app-config 4b0f980
* **app-config:** fix switch network bug abbc583
* **appserver:** fix compiler warning 5ad498f
* **appserver:** remove use of client_command_info b1139bd
* **BSA:** add #if (SUPPORT_BSA_BLE == 1) to mozart.c 7553aff
* **BSA:** add bluetooth_mode_pthread function macro expansion 8c89d56
* **BSA:** modify BLE device name display "No Services" problem 1622bed
* **BSA:** modify bsa aec resample parameter set d5da062
* **BSA:** modify bsa cancel auto_reconnect_thread logic d15b874
* **BSA:** modify mozart_bluetooth_ble_set_visibility invocation point 8f28f4c
* **BSA:** resolve BT display "Broadcom Bluetooth device" name when BT init cd1c377
* **cgi:** fix compile error because of alarm refactor 75848ed
* **configuration:** remove useless configuration. 32c2217
* **event:** fix key is invalid when quick release. c775a31
* **ingenicplayer:** fix set_seek and play_queue 4af7fbd
* **ingenicplayer:** stop musicplayer playback when musiclist is empty ae1bcab
* **lapsule:** will not start lapsule service if not enabled 6d6173a
* **LAPSULE:** add SUPPORT_LAPSULE Macro expansion to lapsule code a3949a5
* **linein:** fix the bug linein will not be closed fb53a72
* **localplayer:** add LOCAPLAYER domain_status judge when mozart_localplayer_stop_playback() ab03941
* **locaplayer:** add SUPPORT_LOCALPLAYER in such code 24df786
* **module:** don't switch to cloud play mode when wifi is AP 21690f1
* **module:** snd source is LAPSULE when VR is active. 4cd60e0
* **mozart:** delete s in mozart 0379495
* **mozart:** fix compile error because of ingenicplayer add 2ff378d
* **mozart:** ingenicplayer play local music in tfcard_scan_done_callback 326bddc
* **mtd-utils:** fix make mtd-utils bug 36eb215
* **musicplayer:** default volume is 0 bc87ee9
* **musicplayer:** Don't call mozart_module_stop, when snd_source don't change. 281f6a5
* **musicplayer:** when music start, check if musicplayer is acitve. ce7b8fc
* **network:** stop the player when config or switch wifi. be3b65b
* **network:** initialize new_mode in mozart_wifi_mode() acc8ee1
* **nvgen:** fix compile error 4897d12
* **nvgen:** fix nvgen bug 7fb07ab
* **nvgen:** modify old method name: update_block --> update_times 3feb8d7
* **rootfs:** decrease erase block count 1d61098
* **rootfs:** fix canot mount usrdata bug on cramfs. f965e6a
* **test:** fix compile error because of libs update ff1c917
* **test-musiclist:** change some func name 48f2ec3
* **test-network-manager:** ingore STA_CONNECT_STARTING cf2f6b7
* **updater:** complete updater code. 7b57606
* **updater:** fix compile error 9f822dd
* **updater:** fix download error on some http server. 4d6e44b
* **updater:** fix download size calc error 3c34bf9
* **updater:** fix memoey leak 83f0a00
* **updater:** fix some bug in updater 7e03b28
* **updater:** fix write flash bug 32fca09
* delete tmp_config cdddef3
* **updater:** link libplayer.so libsharememory.so 7d90eca
* **updater:** modify the position of check update version 6c6782f
* **updater:** modify update process follow libnvrw update. e4c8f2a
* **volume:** set default volume domain is MUSIC_VOLUME, not BT_MUSIC_VOLUME on volumedown. 7e8306d
* can't mount usrdata. 9fac35e
* **vr:** fix player resume failed, when asr failed. b473c64
* **vr-iflytek:** modify compile err be6ae9c
* **vr-speech:** fixed a search song bug while the song has a same name with film 1e7e95b
* **ximalaya:** play music when system starting 2300c52

### Features

* **airplay:** add new interfaces in airplay module fbd8f1b
* **alarm:** add alarm config 2c261b9
* **alarm:** set rtc time and start-up alarm 3eb99e7
* **alarm:** support alarm settings by app dfb919e
* **amazon:** add amazon support 2593f57
* **appserver:** add device scope 893cfaf
* **appserver:** add ingenicplayer config and appserver callback func d17ece0
* **appserver:** add new command : get_device_name / set_device_name 6631a8b
* **bsa:** merge bt_ble event logic 9bc1a4f
* **BSA:** add avk_set_abs_vol event handle 53fa466
* **BSA:** add ble_disc_new and ble_disc_complete event c61e0ee
* **BSA:** add bluetooth auto reconnect logic 41d6fbb
* **BSA:** add bluetooth ring evt code logic e4a8273
* **BSA:** add bluetooth sec_link_down misc type 0608660
* **BSA:** add BSA avk volume sync function 99103b9
* **BSA:** add bsa ble hid support 50ad929
* **BSA:** add BSA connect\disconnect tts 0734403
* **BSA:** add bt discovery info print afeed83
* **BSA:** add disc_new event handler c639b40
* **BSA:** add gets the current music play information logic processing addc7fb
* **BSA:** add hs ring start\hang up EVT 772a91d
* **BSA:** add hs_vgs logic 1485e8f
* **BSA:** add Listen for notifications function 0cfdc43
* **BSA:** add mozart_bluetooth_parse_eir_manuf_specific function handle 265a5dd
* **BSA:** modify BT_VOLUME to BT_CALL_VOLUME, add BT_MUSIC_VOLUME d6c1bf9
* **cloud_music:** add cloud music module, include ximalaya and jushang. 43e398f
* **configs:** Using official wpa_supplicant and hostapd tools for realtek module 2e621da
* **event:** KEY_MODE, KEY_VOLUMEUP, KEY_VOLUMEDOWN, KEY_PLAYPAUSE support long press. 0170491
* **gen-updatepkg:** support 'make update` 2fc0496
* **ingenicplayer:** add new interface for playing local tracks 159c078
* **ingenicplayer:** add play shortcut func cd548be
* **ingenicplayer:** add play vr tracks interface and tracks change event d1ee206
* **ingenicplayer:** call mozart_module_ingenicplayer_start in ingenicplayer_play_queue f4c1cbf
* **ingenicplayer:** change the interface of play tracks 310b72b
* **jushang:** support jushang a368e8b
* **libtts_jietong:** add pansy libtts_jietong support c4a4805
* **liburl:** compile liburl to file system b691bdf
* **main:** add check new version thread 6592a24
* **Makefile:** add halley2 sfc nand oss and alsa config 7e8b40f
* **minigui:** transplant minigui to device-tools 1619c8b
* **mozart:** add mozart music player f2c02ac
* **mozart:** adjust the directory structure of mozart 953873f
* **mozart:** can be controllered by ingenicplayer app 45ea241
* **mozart:** support alsa on 16MB spi norflash. fcb75b7
* **mozart:** support new module preemption method fa56ec7
* **mozartplayer:** move ingenicplayer to mozartplayer 83568c8
* **mtd-utils:** add flash_erase command 6ebaf5c
* **mtd-utils:** compile ubi-tools only on needed. 1450099
* **mtd-utils:** compile ubi-utils when compile mtd-utils package. 26fd72d
* **network:** replace the network_manager with a newer version. d385566
* **network-manager:** add realtek airkiss test example 34c1950
* **network-manager:** add realtek simple config test example 68e4e3e
* **nvgen:** add nvgen tool a6af6b6
* **nvgen:** pad nv.img to 2 * erasesize d534820
* **nvgen:** support nvinfo->magic 626c31a
* **player:** add support two mplayer channels simultaneously function de03f2f
* **render:** support next/previous song by qplay a1d7629
* **rootfs:** alias ps='ps wwl' 3e093df
* **rootfs:** cancel automatically mount /dev/mmcblk when boot ext4 f09cedf
* **rootfs:** modify S00system_init.sh 60e3385
* **rootfs:** modify S00system_init.sh when make ext4 and ubifs 9255c61
* **rootfs:** support multi storage on mkfs & mkotafs 6f16dfe
* **rootfs:** support some cmd completion on C-r at startup 03d5eb7
* **rootfs:** support updaterfs + apfs on ubifs df01e02
* **test-misc:** get more nvinfo 082d55f
* **test-misc:** move tools/device-tools/test to tools/device-tools/test/misc f0173c8
* **test-musiclist:** add music list test 9e70227
* **test-network-manager:** add AP_STATUS message test 4909f8c
* **test-network-manager:** add AP-STA-DISCONNECTED test ed0c376
* **test-network-manager:** add CHECKMODE test 802ab6c
* **test-network-manager:** add cooee and atalk test 7946033
* **test-network-manager:** add eth test 219e7eb
* enable ximalaya, vr_speech in trunk_v1.1_ap6212 and enable webrtc in canna_v2.0_ 06d3d16
* **test-network-manager:** add ip change test example 24a5941
* **test-network-manager:** add max wifi info count test 5193df3
* **test-network-manager:** add sta disconnect, auto reconnect test and update checkpatch 40ac665
* **test-network-manager:** add STA test 0a7ffd3
* **test-network-manager:** add SW_AP test d9055fc
* **test-network-manager:** add SW_NETCFG test 10cb477
* **test-network-manager:** add update_wifi_priority test d49d634
* enable webrtc in canna_v1.0_ap6212a_config bc13e54
* support canna_v1.0_ap6212a_ubifs_config. 1f0c92d
* **test-network-manager:** check WIFI_MODE message 7566c7f
* **ui:** add libsmartui d91fa23
* **updater:** add emmc ota support 10c07b4
* **updater:** add sound control in updater e1c6874
* ximalaya instead of lapsule in mozart_snd_source_switch 5210a7c
* **updater:** add spinand updater func 27a2adc
* **updater:** check if nv_info is null e9ca34e
* **updater:** check md5 after download and write flash 9def292
* **updater:** complete md5 check func: after download & after update 1761786
* **vr:** add asr control led on and off 216ba5a
* **ximalaya:** add ximalaya_get_keyword() func ddfa24e
* **ximalaya:** support ximalaya OpenAPI 5af0dc5

### Performance Improvements

* **BSA:** add bt hs call evt code logic 31ba470
* **BSA:** add mozart_bluetooth_hs_get_call_state return value judge 4f7346c
* **BSA:** add mozart_bluetooth_set_visibility(0, 0) when bt connected, add mozart_bluetoot 873adfa
* **BSA:** modify hh_client_num position af7ee21
* **BSA:** modify manu_data.company_id print format 004bb0d
* **BSA:** modify mozart_bluetooth_auto_reconnect() define, modify auto_reconnect number of cf6c579
* **BSA:** modify mozart_bluetooth_ble_configure_adv_data() struct 7a1aadd
* **BSA:** optimised disc_complete and disc_new event code 2a24769
* **BSA:** optimize mozart_bluetooth_auto_reconnect return value judge 32a78ae
* **BSA:** Organize and optimize BSA interface 0476a49
* **updater:** change block read buffersize: 4KB to 128KB 61db288



<a name="1.7.0"></a>
