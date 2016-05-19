#export abc
#let abc=1
#let abc+=1
#echo abc=$abc
#echo < $1 >
#export 1=22
#echo 2: $1
#if [ -z $abc ]
#then
#    echo  abc not exist
#else
#    echo abc exist
#fi

#echo $abc==""


#func1(){
#    return 123
#}
#func1
#let abc=$?+1
#echo $abc
#sed -n "/$PRODUCT_MODEL/p" version.ini

#export sbcis=250
#sbcis=111

#include version.ini

#CUSTOMER_CONFIG=bgc
#if [ "gen" =  $CUSTOMER_CONFIG ];then
#    @echo ciscosb
#export CUSTOMER_CONFIG=bgc
#export CUSTOMER_CONFIG2=bgc
#if [ "$CUSTOMER_CONFIG" = "bgc" ] || [ "$CUSTOMER_CONFIG" = "gen" ] || [ "$CUSTOMER_CONFIG" = "pt" ]; then
#    export CUSTOMER_DIR=generic/$CUSTOMER_CONFIG
#else
#    export CUSTOMER_DIR=$CUSTOMER_CONFIG
#fi 
#echo $0
ret=0;
#test()
#{
#    ls aaabbccd;
#    return $ret; 
#}


#if [ -d aaa ]
#then 
#    echo "1 directory detection fills"
#else
#    echo "2 directory detection fills"
#fi

#test;
#echo $?


#if [ $ret -eq "100" ]
#then
#    echo $ret eq 100
#fi


#echo 111;
#ls adfafa;

#if [ "$?" -ne "0" ]||[ $ret -ne "0" ]
#then
#    echo aaaaaaaaaaaaaaaa
#else
#    echo $?
#fi

echo "\$\$"$$

echo "\$\@"$@
echo "\$\*"$*

echo "\$\0"$0
echo "entering circulation"
for var in $*
do
    echo "\$var = $var"
done


echo "\$\@"$@
echo "\$\*"$*

echo "\$1"$1

echo "\$\@"$@
echo "\$\*"$*

#abc="abc"
abcset=`echo "abcset"`

#test "$abc+set
if test  "${abc+set}"="set"; 
then
    echo $abcset + "aaa"
else
#    echo $abc
#    echo ${abc +set}
    echo $abcset
fi

#if test "${with_kbuild+set}" = set; then : 

