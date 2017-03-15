#this file is added by DAMAI, make the eth ip to be 192.168.10.1
fix_ip="192.168.10.1"
ip=$(ifconfig eth0 | grep "inet addr" | awk '{ print $2}' | awk -F: '{print $2}')
if [ "${fix_ip}" != "${ip}" ];then
	echo "configure eth0 ip to ${fix_ip}" 
	ifconfig eth0 ${fix_ip}
fi
ps -w |grep "udhcpd /etc/udhcpd_eth.conf" | grep -v grep > /dev/null || udhcpd /etc/udhcpd_eth.conf