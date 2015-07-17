#! /bin/bash

# This is a script for auto build
# plesae run the script in the root directory of the project

#code_base=$PWD/vdsl;
code_base=/mnt/sdb1/autobuild/vdsl;
image_path=/mnt/sdb1/autobuild/image;
code_base_ccsp=/mnt/sdb1/autobuild/vdsl/vdsl2_v2/ccsp;
code_base_ccsp_simu=/mnt/sdb1/autobuild/vdsl/vdsl2_qsdk_v0.11.1/ccsp_simu;
prev_git_version=`git ls-remote $code_base|grep "refs\/heads\/vdsl2_gw"|awk '{print ($1)}'`;
cur_git_version=$prev_git_version;
prev_git_version_ccsp=`git ls-remote $code_base_ccsp|grep "refs\/heads\/vdsl2_gw"|awk '{print ($1)}'`;
cur_git_version_ccsp=$prev_git_version_ccsp;
#first_build set to 1
first_build=0;
ret=0;
MAILDEST=cdc-vdsl2-dev@cisco.com
commitauthor="";
errorlog="";

export PATH=$PATH:/mnt/sdb1/autobuild/vdsl/ATT_LSC_Gateway/linux/buildroot-2009.01/build_mips/staging_dir/usr/bin/


echo "This is a auto build script for vdsl"
echo prev_git_version=$prev_git_version

sendmail()
{
        mailx -s "$3" -a $4 -r $1 $2<~/111;
}

#sendmail()
#{
#        mailx -s "$3" -r $1 $2 < $4;
#}

send_build_success_mail()
{
    sendmail cdc-vdsl2-dev@cisco.com $MAILDEST "Build Succeedsgit version $1" ~/111;
}




send_build_error_mail()
{
    rm $code_base/$cur_git_version.buildlog.tgz
    if [ -f $code_base/$cur_git_version._ikan.buildlog.txt ]
    then 
        tar -zcvf $code_base/$cur_git_version.buildlog.tgz $code_base/$cur_git_version.buildlog.txt  $code_base/$cur_git_version._ikan.buildlog.txt $code_base/$cur_git_version._simu.buildlog.txt $code_base/$cur_git_version._v2.buildlog.txt;
    else
        tar -zcvf $code_base/$cur_git_version.buildlog.tgz $code_base/$cur_git_version.buildlog.txt  $code_base/$cur_git_version._simu.buildlog.txt $code_base/$cur_git_version._v2.buildlog.txt;
    fi
    sendmail cdc-vdsl2-dev@cisco.com $MAILDEST "Build error on git version:$cur_git_version Module:$1" $code_base/$cur_git_version.buildlog.tgz;
}

send_build_error_simple_mail()
{
    rm $code_base/$cur_git_version.buildlog.tgz
    if [ -f $code_base/$cur_git_version._ikan.buildlog.txt ]
    then 
        tar -zcvf $code_base/$cur_git_version.buildlog.tgz $code_base/$cur_git_version.buildlog.txt $code_base/$cur_git_version._ikan.buildlog.txt $code_base/$cur_git_version._simu.buildlog.txt $code_base/$cur_git_version._v2.buildlog.txt;
    else
        tar -zcvf $code_base/$cur_git_version.buildlog.tgz $code_base/$cur_git_version.buildlog.txt $code_base/$cur_git_version._simu.buildlog.txt $code_base/$cur_git_version._v2.buildlog.txt;
    fi

    sendmail cdc-vdsl2-dev@cisco.com $MAILDEST "build error for last commit $1"    $code_base/$cur_git_version.buildlog.tgz;
#    sendmail cdc-vdsl2-dev@cisco.com "guoding@cisco.com" "build error for last commit $1"  $code_base/$cur_git_version.buildlog.tgz;
}

test_sendmail()
{
    sendmail cdc-vdsl2-dev@cisco.com guoding@cisco.com "build error on git version $1"  ~/111;
}

#sub_ccsp_build_qual()
#{
#    ret=0;
#    cd $code_base;
#    cd vdsl2_qsdk_v0.11.1;
#    cp ../qualcomm_gui . -rf
#    make package/CCSP/clean;
#    make V=s package/CCSP/compile;
#    ret=$?;
#    make package/CCSP/install;
#    cd ..;
#    return $ret;
#}


sub_ccsp_build_v2()
{
    ret=0;
    cd $code_base;
    cd vdsl2_v2;
    cp ../qualcomm_gui . -rf
    make package/CCSP/clean;
    make V=s package/CCSP/compile;
    ret=$?;
    make package/CCSP/install;
    cd ..;
    return $ret;
}


sub_qualcomm_build()
{    
    image_copy;
    cd $code_base;
    cd vdsl2_qsdk_v0.11.1;
    cp ../qualcomm_gui . -rf
    make -f Makefile.cyt all_vdsl2 V=s
    return $?;
}

#vdsl2-simu_qualcomm
sub_ccsp_build_simu()
{
    cd $code_base;
    cd vdsl2_qsdk_v0.11.1/ccsp_simu;
    git pull;
    source ccsp_setenv_vdsl2.sh simu_qualcomm;
    ./ccsp_build vdsl2 simu_qualcomm ga;
    if [ "$?" -ne "0" ]
    then
        return 1
    fi
    return 0;    
}

sub_vdsl2_v2_build_small_auto()
{
#temp no more build ;this
    cd $code_base;
#    cp -raf ~/vdsl2_v0.11.z.orig vdsl2_v2 
    cd vdsl2_v2 
    image_copy;
    cp  ../vdsl2_v0.11.z/* . -raf 

    if [ -d ccsp ]
    then
        cd ccsp;
        git pull;
        cd ..;
    else
        git clone guoding@10.74.120.193:/opt/ccsp_repos/ccsp3_0.git ccsp;
        cd ccsp;
        git checkout vdsl2_gw;
        cd -;
    fi

    make -f Makefile.cyt all_vdsl2 V=s;
    return $?;
}


sub_vdsl2_v2_build()
{
#temp no more build ;this
    cd $code_base;
    mkdir vdsl2_v2;
    cp -raf ~/vdsl2_v0.11.z.orig/* vdsl2_v2 
    image_copy;
    cd vdsl2_v2; 
    cp  ../vdsl2_v0.11.z/* . -ra ;

    cp ../qualcomm_gui . -rf;
    if [ -d ccsp ]
    then
        cd ccsp;
        git pull;
        cd ..;
    else
        git clone guoding@10.74.120.193:/opt/ccsp_repos/ccsp3_0.git ccsp;
        cd ccsp;
        git checkout vdsl2_gw;
        cd -;
    fi

    make -f Makefile.cyt all_vdsl2 V=s;
    return $?;
}


sub_ikanos_build()
{
#return 0;
#temp no more build ;this
    cd $code_base;
    cd  ATT_LSC_Gateway/linux
    cp ../../ikanos_8_5_1_1/* . -rf
    #change customer settings  of makefile
    sed -i 's/\/home\/zhdu\/data\/ATT_LSC_Gateway\/linux\//\/home\/ciscouser\/vdsl2\/ATT_LSC_Gateway\/linux\//g' fusiv_src/user/imf/imfLocalUserApp/Makefile
    if [ -d ccsp2.2 ]
    then
        cd ccsp2.2;
        git pull;
        cd ..;
    else
        git clone guoding@10.74.120.193:/opt/ccsp_repos/ccsp3_0.git ccsp2.2;
        cd ccsp2.2;
        git checkout vdsl2_gw;
        cd -;
    fi
    cp sample_config/kernel_sampcfg_2_6_28_vx185LT_AVDSL_RG_SPI linux-2.6.28/.config;
    cp sample_config/buildroot_sampcfg_2_6_28_vx185LT_AVDSL_RG_SPI buildroot-2009.01/.config;
    make toolchain;
    export PATH=$PATH:/mnt/sdb1/autobuild/vdsl/ATT_LSC_Gateway/linux/buildroot-2009.01/build_mips/staging_dir/usr/bin/;
    cd  buildroot-2009.01/ ;
    make initramfs;
    if [ "$?" -ne "0" ]
    then
        return 1
    fi
    cd ..;
    make all;
    if [ "$?" -ne "0" ]
    then
        return 1
    fi
    cd linux-2.6.28;
    export BUILDROOT_SRCDIR=../buildroot-2009.01/;
    make
    if [ "$?" -ne "0" ]
    then
        cd ../../../
        return 1
    fi
    cd ../../../
    return 0;
}




night_build()
{
    errorlog="";
    ret=0;
    rm -rf $code_base;
    git clone guoding@10.74.120.193:/opt/vdsl2.git $code_base;
    cd $code_base;
    mkdir vdsl2_v2;
    mkdir vdsl2_qsdk_v0.11.1;
    cd vdsl2_v2;
    git clone guoding@10.74.120.193:/opt/ccsp_repos/ccsp3_0.git ccsp;
    cd ccsp;
    git checkout vdsl2_gw;
    cd $code_base;
    cd vdsl2_qsdk_v0.11.1;
#    cp ~/dl.tgz .;
#    tar -zxvf dl.tgz;
#    git clone guoding@10.74.120.193:/opt/ccsp_repos/ccsp3_0.git ccsp;
    git clone guoding@10.74.120.193:/opt/ccsp_repos/ccsp3_0.git ccsp_simu;
    cd ccsp_simu;
    git checkout vdsl2_gw;
    cd -;
    cp -ar ~/ATT_LSC_Gateway $code_base 

#    cp ../vdsl2_qsdk_v0.11.1 ../vdsl2ikanos;
    rm $code_base/$cur_git_version.buildlog.txt;
    touch $code_base/$cur_git_version.buildlog.txt;
    touch $code_base/$cur_git_version._ikan.buildlog.txt;
    touch $code_base/$cur_git_version._v2.buildlog.txt;
    touch $code_base/$cur_git_version._simu.buildlog.txt;
    sub_ikanos_build>> $code_base/$cur_git_version._ikan.buildlog.txt 2>>$code_base/$cur_git_version._ikan.buildlog.txt;
    prev_git_version_ccsp=`git ls-remote $code_base_ccsp|grep "refs\/heads\/vdsl2_gw"|awk '{print ($1)}'`;
    cur_git_version_ccsp=$prev_git_version_ccsp;
    if [ "$?" -ne "0" ]
    then
        ret=1;
        errorlog+="ikan ";
    fi
#dgl_temp
    sub_vdsl2_v2_build>>$code_base/$cur_git_version._v2.buildlog.txt 2>>$code_base/$cur_git_version._v2.buildlog.txt;
    if [ "$?" -ne "0" ]
    then
        ret=1;
        errorlog+="vdsl_v2";
    fi

#    sub_qualcomm_build>> $code_base/$cur_git_version.buildlog.txt 2>>$code_base/$cur_git_version.buildlog.txt;
#    if [ "$?" -ne "0" ]
#    then
#        ret=1;
#        errorlog+="qual ";
#    fi

    sub_ccsp_build_simu >> $code_base/$cur_git_version._simu.buildlog.txt 2>>$code_base/$cur_git_version._simu.buildlog.txt;
    if [ "$?" -ne "0" ]||[ $ret -ne "0" ]
    then
        errorlog+="simu ";
        send_build_error_mail  $errorlog;
    else
        send_build_success_mail $cur_git_version;
    fi
    image_install;
}

#build all code (if no ccsp changes) 
auto_build()
{
    ret=0;
    errorlog="";
    cd $code_base;
    rm $code_base/$cur_git_version.buildlog.txt;
    touch $code_base/$cur_git_version.buildlog.txt;
    rm $code_base/$cur_git_version._v2.buildlog.txt;
    touch $code_base/$cur_git_version._v2.buildlog.txt;
    rm $code_base/$cur_git_version._ikan.buildlog.txt;
    touch $code_base/$cur_git_version._ikan.buildlog.txt;
    rm $code_base/$cur_git_version._simu.buildlog.txt;
    touch $code_base/$cur_git_version._simu.buildlog.txt;
#    git show --pretty="format:" --name-only $cur_git_version|grep 
    git show --pretty="format:" --name-only $cur_git_version|grep ikanos_8_5_1_1
    if [ "$?" -eq "0" ]
    then
      echo "ikanos_build"
      sub_ikanos_build>>$code_base/$cur_git_version._ikan.buildlog.txt 2>>$code_base/$cur_git_version._ikan.buildlog.txt;
      if [ "$?" -eq "0" ]
      then
         ret=1 
         errorlog+="ikan";
      fi
#        image_copy;
    fi

    git show --pretty="format:" --name-only $cur_git_version|grep vdsl2_v0.11.z
    if [ "$?" -eq "0" ]
    then
      echo "vdsl_v2"
      sub_vdsl2_v2_build_small_auto>>$code_base/$cur_git_version._v2.buildlog.txt 2>>$code_base/$cur_git_version._v2.buildlog.txt;
      if [ "$?" -eq "0" ]
      then
         ret=1 
         errorlog+="vdsl_v2";
      fi
        image_copy;
    fi


#    git show --pretty="format:" --name-only $cur_git_version|grep vdsl2_qsdk_v0.11.1
#    if [ "$?" -eq "0" ]
#    then
#      sub_vdsl2_v2_build_small_auto>>$code_base/$cur_git_version.buildlog.txt 2>>$code_base/$cur_git_version.buildlog.txt;
#      if [ "$?" -ne "0" ]
#      then
#         ret=1 
#         errorlog+="qualcom";
#      fi
#    fi
#    cd ..;
#    if [ $ret -ne "0" ]
#    then
#        send_build_error_simple_mail $errorlog;
#    fi
    image_install;
    return $ret;
}
#build all code (if ccsp changes) 
auto_all_build()
{
    ret=0;
    errorlog="";
    cd $code_base;
    echo "auto all build start"
    git pull;
#1009
#rebuild ikan
    rm $code_base/$cur_git_version.buildlog.txt;
    touch $code_base/$cur_git_version.buildlog.txt;
    rm $code_base/$cur_git_version._v2.buildlog.txt;
    touch $code_base/$cur_git_version._v2.buildlog.txt;
    rm $code_base/$cur_git_version._ikan.buildlog.txt;
    touch $code_base/$cur_git_version._ikan.buildlog.txt;
    rm $code_base/$cur_git_version._simu.buildlog.txt;
    touch $code_base/$cur_git_version._simu.buildlog.txt;

    git show --pretty="format:" --name-only $cur_git_version|grep ikanos_8_5_1_1
    if [ "$?" -eq "0" ]
    then
      echo "ikanos_build"
      sub_ikanos_build>>$code_base/$cur_git_version._ikan.buildlog.txt 2>>$code_base/$cur_git_version._ikan.buildlog.txt;
      if [ "$?" -eq "0" ]
      then
         ret=1 
         errorlog+="ikan";
      fi
#        image_copy;
    fi

    git show --pretty="format:" --name-only $cur_git_version|grep vdsl2_v0.11.z
    if [ "$?" -eq "0" ]
    then
      echo "vdsl_v2"
      sub_vdsl2_v2_build_small_auto>>$code_base/$cur_git_version._v2.buildlog.txt 2>>$code_base/$cur_git_version._v2.buildlog.txt;
      if [ "$?" -eq "0" ]
      then
         ret=1 
         errorlog+="vdsl_v2";
      fi
    fi

#    sub_ccsp_build_qual>>$code_base/$cur_git_version.buildlog.txt 2>>$code_base/$cur_git_version.buildlog.txt;
    sub_ccsp_build_v2>>$code_base/$cur_git_version.buildlog.txt 2>>$code_base/$cur_git_version.buildlog.txt;
    if [ "$?" -ne "0" ]
    then
       ret=1 
       errorlog+="ccsp";
    fi
#rebuild qualcomm
#    sub_qualcomm_build>>$code_base/$cur_git_version.buildlog.txt 2>>$code_base/$cur_git_version.buildlog.txt;
#    if [ "$?" -ne "0" ]
#    then
#       ret=1 
#       errorlog+="qualcom";
#    fi
#rebuild simu
    sub_ccsp_build_simu>>$code_base/$cur_git_version._simu.buildlog.txt 2>>$code_base/$cur_git_version._simu.buildlog.txt;
    if [ "$?" -ne "0" ]
    then
       ret=1 
       errorlog+="simu";
    fi
    cd $code_base;
    echo "auto all build exit"
    if [ $ret -ne "0" ]
    then
        send_build_error_simple_mail $errorlog;
    fi
    image_install;
    return $ret;
}

image_install()
{
    TODAY=`date|awk '{print $2$3$4}'`;
    mkdir $image_path/$TODAY -p;
    cd $code_base;
#    cp vdsl2_qsdk_v0.11.1/bin/ipq806x/tftpboot/nornand-ipq806x-single.img ~/image/$TODAY
#    cp vdsl2_qsdk_v0.11.1/bin/ipq806x/tftpboot/nand-ipq806x-single.img ~/image/$TODAY
    cp vdsl2_v2/bin/ipq806x/tftpboot/nornand-ipq806x-single.img $image_path/$TODAY/nornand-ipq806x-single_new.img

    cp vdsl2_v2/bin/ipq806x/tftpboot/nand-ipq806x-single.img $image_path/$TODAY/nand-ipq806x-single_new.img
    cp ATT_LSC_Gateway/./linux/linux-2.6.28/kernel.img $image_path/$TODAY
    cp $code_base/*.txt $image_path/$TODAY
    echo have problem on block:\n   $errorlog >$image_path/$TODAY/build_info
    cd -;
}

image_copy()
{
    cd $code_base;
#    cp ATT_LSC_Gateway/./linux/linux-2.6.28/kernel.img vdsl2_qsdk_v0.11.1
    cp ATT_LSC_Gateway/./linux/linux-2.6.28/kernel.img vdsl2_v2/
    cd -;
}

touch ~/111;

#sendmail cdc-vdsl2-dev@cisco.com "guoding@cisco.com " "build error for last commit $1"  ~/vdsl/aa.txt;
#auto_all_build
#cur_git_version_ccsp=1a13a91e2536dfc5c839c4a70111bb55b508d5bf;
#send_build_error_simple_mail ikan;
#night_build
#night_build
if [ $# -ne 0 ]
then
    $1;
fi
while true
do
    cd $code_base_ccsp
    echo "pulling ccsp:"
    git pull;
    cd ..;
    cd $code_base_ccsp_simu
    echo "pulling ccsp simu"
    git pull;
    cd ..;

    cur_git_version_ccsp=`git ls-remote $code_base_ccsp|grep "refs\/heads\/vdsl2_gw"|awk '{print ($1)}'`;
    if [ "$cur_git_version_ccsp" != "$prev_git_version_ccsp" ] || [ "X"$first_build == "X1" ]
	then
	    echo "do auto_all_build on version $cur_git_version_ccsp";
	    prev_git_version_ccsp=$cur_git_version_ccsp;
        auto_all_build;
	    first_build=0;
    else
#check if only vdsl changed
        cd $code_base;
        echo "pulling vdsl:"
        git pull;
        cd ..;
        cur_git_version=`git ls-remote $code_base|grep "refs\/heads\/vdsl2_gw"|awk '{print ($1)}'`;
        if  [ "$cur_git_version" != "$prev_git_version" ] || [ "X"$first_build == "X1" ]
	    then
	        echo "do auto_build on version $cur_git_version";
	        prev_git_version=$cur_git_version;
            auto_build;
	        first_build=0;
	    fi
    fi

#timing for night build
    date|awk '{print $4}'|grep '00:00:';
    if [ "$?" -eq "0" ]
    then
      night_build;
    fi
    echo "sleep 40";
	sleep 40;
done


