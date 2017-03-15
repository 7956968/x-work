var currentlang = lang = "en";
function langPrefix() {
    if (getCookie('LANG') == null) {
        var language = (navigator.language) ? navigator.language : navigator.userLanguage;
        currentlang = lang = language.substring(0, 2)
        //setCookie('LANG_COOKIE', userLang.substring(0,2));
    } else {
        currentlang = lang = getCookie('LANG');
    }
   if((currentlang != "zh"))
	currentlang = "en";

    $("input[lang]").each(function () { $(this).val(message[currentlang][$(this).attr("lang")]); });
    $("[lang]").not("input").each(function () { $(this).html(message[currentlang][$(this).attr("lang")]); });
}

var message = {};
message.zh = {
    "device": "airmusic",
    "server": "服务器名称:",
    "upload": "文件上传成功，要继续升级吗？",
    "uploading": "正在上传固件，请稍等...",
    "language": "简体中文",
    "name": "名称",
    "modif": "最后修改",
    "size": "大小",
    "description":"描述",
    "parent": "返回上级目录",
    "remove": "删除",
    "choose": "选择网络...",
    "unknow":"未知!",

    "joinmode": "接入方式",
    "wireless": "无线",
    "ip": "IP地址:",
    "autoscan": "无线接入:",
    "availableconnection": "可用Wi-Fi热点:",
    "scanningmsg": "正在扫描Wi-Fi热点...",
    "savingconfig": "正在保存设置,请稍候...",
    "loadingmsg": "正在读取数据，请稍候...",
    "refreshalert": "请刷新页面或重新连接！",
    "x8021alert": "不支持连接802.1X加密的Wi-Fi热点！",
    "connectpwdalert": "请输入Wi-Fi的密码!",
    "whopwd": " 的密码：",

    "noap":"没有可用的Wi-Fi热点！",
		
     "selectwifialert": "请选择一个可用的Wi-Fi！",
    "pwdlengthalert":"WEP密码位数错误！",
    "connectfailed":"Wi-Fi连接失败，请确认密码是否正确！",
    "emptyalert":"不能为空！",

    "networkconnect": "外 网 接 入",
    "advancedsetting": "音 频 服 务",
    "upgrade": "固 件 升 级",
    "fw": "版本号:",
    
    "devicename": "设备名称:",
    "security": "加密:",
    "encrypt_len":"加密长度",
    "format":"密码格式",
    "passwd": "密码:",
    "confpasswd": "确认<br/>密码:",
    "join": "加入网络",
    "wait": "请稍后...",
    "tips": "请输入13个有效字符.",
    "tips1": "提示1:在有线网连接模式下，您的设备将自动从您所连接的路由器获取IP地址和DNS。",
    "tips2": "提示2:在无线连接模式下，您需要扫描并连接到一个无线热点。",
    "checkstatus": "检查连接状态...",
    "alert1": "网络连接成功!",
    "alert2": "连接失败!\n请检查密码和网络!",
    "alert3": "超时!",
    "alert4": "请重新连接您的设备以检查是否成功。",
    "alert5": "设置成功！请确保您的网线已正确连接。",
    "warning": "警告",
    "localdnstips": "欢迎使用airmusic",
    "alert7": "请留意设备上的Wi-Fi灯会闪烁。",
    "alert8": "升级的过程中请勿断电或重启设备！",
    "notice": "提示:您需要先从电脑上传固件再执行升级。",
    "done": "完成",
    "back": "返回",
    "browse": "浏览",
    "availablefilealert":"无效的固件！",
    "fileemptyalert": "请您先选择升级文件！",
    "correntfilealert": "请上传正确的BIN文件！",

	"disconnectwan": "确定断开当前连接？",
    "confirm": "确定",
    "cancel": "取消",	
    "emptyname":"请输入设备名。",
    "connecting":"正在连接",

    "pwd": "无线密码:",
    "ssidlengthalert": "不能超过32个字符！",
    "passwordlengthalert": "不能超过32个字符！",
    "savingrestart": "正在重启服务,请稍候...",   
    "enterbbs": "官方QQ群:373075200"
};
message.en = {
    "device": "airmusic",
    "server": "ServerName:",
    "upload": "File uploaded success! Continue to upgrade?",
    "uploading": "Uploading file,please wait...",
    "language": "English",
    "name": "Name",
    "modif": "Modified",
    "size": "Size",
    "description":"Description",
    "parent": "Parent Directory",
    "remove": "Delete",
    "choose": "Choose a Network...",
	
    "ssidlengthalert": "Should be less than 32 characters！",
    "passwordlengthalert": "Should be less than 32 characters！",
    "unknow":"unknow!",
    
    "wired": "Wired",
    "wireless": "Wireless",
    "autoscan": "Wireless access:",
    "availableconnection": "Available Wi-Fi hotspot:",

    "networktype": "External Network Interface:",
    "powerstatus": "Power status:",
    "storagestatus": "Storage status:",
    "scanningmsg": "Scanning for Wi-Fi Hotspot...",
    "savingconfig": "Saving settings, please wait...",
    "loadingmsg": "Loading, please wait...",

    "refreshalert": "Please refresh the page or reconnect your device!",
    "x8021alert": "Wi-Fi hotspot encrypted by 802.1X is not supported!",
    "connectpwdalert": "Please enter Wi-Fi password!",
    "whopwd": "'s password:",
    "noap":"No available Wi-Fi!",
	
    "ipemptyalert": "IP address cannot be empty!",
    "iperroralert": "Wrong IP address!",

    "selectwifialert": "Please select a Wi-Fi network!",
    "pwdlengthalert": "Wrong WEP password digit!",
    "nostorage": "No external disk is detected!",
    "wirednotice": "Connection fails, please make sure the network cable is plugged in!",
    "wirednoticeok": "Wired connection is successful!",
    "connectfailed": "Wi-Fi connection failed, password incorrect!",
    "emptyalert":"Can not be blank!",

    "emptyname":"Device name can't be empty.",
    "connecting":"Connecting to ",
    "errorpasswd1":"Please enter minimum 8 characters under WPA/WPA2 encryption mode!",
    "errorpasswd2":"Passwords do not match, please enter again!",
	"disconnectwan": "Disconnect?",
    "confirm": "OK",
    "cancel": "Cancel",
    "pwd": "Password:",

    "setting": "Basic Settings",
    "networkconnect": "Internet Connection",
    "upgrade": "Upgrade Firmware",
    "advancedsetting": "Music Service",
    "fw": "Firmware:",
    

    "devicename": "DeviceName:",
    "security": "Security:",
    "encrypt_len": "Encrypt<br>Length",
    "format": "Password<br>Format",
    "passwd": "Password:",
    "confpasswd": "Confirm<br>Password:",
    "join": "Join This Network",
    "wait": "Please wait...",
    "tips": "Please input exactly 13 characters.",
    "tips1": "Tips1:In wired network, getting IP address and DNS automatically form the connected router(DHCP mode).",
    "tips2": "Tips2:In wireless Network,you should connect to a wireless access point.",
    "checkstatus": "Checking status...",
    "alert1": "Network connect success !",
    "alert2": "Connection failed !\n Please check your password and network.",
    "alert3": "Time out !",
    "alert4": "Please reconnect to your device.",
    "alert5": "Set success !\nMake sure your wired network connected correctly.",
    "warning": "WARNINGS",
    "localdnstips": "Welcome to use airmusic",
    "alert9": "You have not connected to the internet!",
    "alert10": "Please press the button below to access the internet!",
    "alert11": "After successfull setting, this page will never<br>show up again!",
    "alert7": "Please wait and keep an eye on your device.<br>One LED should blink during upgrading.",
    "alert8": "Do not power off or reboot your device now!",
    "notice": "Notice: You need to upload the firmware from your computer before implementing the upgrade.",
    "done": "Done",
    "back": "Back",
    "browse": "Browse",
    "availablefilealert":"Invalid firmware!",
    "fileemptyalert":"Please select the upgrade file first!",
    "correntfilealert":"Please upload correct .bin file !",
    
    "enterbbs": "support@dmsys.com"
};

function setLanguage(lang) {
	var disablepop = null;
	 if(lang=='zh')
         disablepop="<setSysInfo><SetLanguage value=\"zh\"/></setSysInfo>";
    else
        disablepop="<setSysInfo><SetLanguage value=\"en\"/></setSysInfo>";
    //alert(disablepop);
    $.get(appendTS(api.apiurl), { data: disablepop }, function (result) {
     //alert(result);
    setCookie('LANG', lang);
    window.location.reload();
    });
}

//cookie相关
function getCookie(name) {
    var arr = document.cookie.split("; ");
    for (i = 0; i < arr.length; i++)
        if (arr[i].split("=")[0] == name)
            return unescape(arr[i].split("=")[1]);
    return null;
}
function setCookie(name, value) {
    var today = new Date();
    var expires = new Date();
    expires.setTime(today.getTime() + 1000 * 60 * 60 * 24 * 2000);
    document.cookie = name + "=" + escape(value) + "; expires=" + expires.toGMTString();

}

