#HZB add:to specify update website in config step
if [ -z $1 ] || [ -z $2 ];then
	echo "missing arguments."
	echo "usage: $0 website ota.ini-file"
	exit -1 
fi

sed -i "s/^url =.*$/url = http:\/\/$1/g" $2
