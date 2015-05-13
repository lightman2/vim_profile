include version.ver
all: test
# all: part1 part2 part3 part4
PARM1=123
CONFIG_MODVERSIONS=Y
CONFIG_MODVERSION=$(CONFIG_MODVERSIONS)

CFLAGS += -I/workspace/guoding/EDA/EDA_BCM7241/stblinux-3.3-1.3/include
# CFLAGS += -DDIAG_VERSION="\"$(VERSION)\""
# CFLAGS += -DMEMORY_LEAK_CHECK
# CFLAGS += -DGUI_DEBUG
OPENWRT_BUILD=
# ifeq ($(CONFIG_EXTERNAL_TOOLCHAIN),)
ifneq ($(OPENWRT_BUILD),1)
	disp=1
else
	disp=2
endif




KBUILD_BUILTIN := $(if $(CONFIG_MODVERSIONS),1)
test:
	gcc main.c
	echo $(disp)
	echo $(CONFIG_MODVERSION)


abc:
	@$(CC) $(CFLAGS) main.c > $@

part4: 
	echo  "part4"
	@echo version = $(VERSION) 

part1: 
	echo  "part1"
	echo $(PARM1)
	echo $(KBUILD_BUILTIN)
	echo $(shell pwd;)
	echo $(shell (find ../ -name *.c|xargs ls -alt);)

part2: part4 
	echo  "part2"

# all: part1

part3: 
	echo  "part3"

part5:
	if [ -s ./Makefile ]; then\
			echo "modules_symvers zero";\
		else\
			echo "modules_symvers not zero";\
	fi


eqtest:
	if [ 2 = 1 ] || [ 1 == 2 ];then\
		echo  aaa;\
	fi
	
ifeqtest:
	abc=11;
	echo $(abc);
ifeq ($(abc),)
	echo aaa;
else
	echo bbb;
endif

iftest:
	if [ ${abc} ]; then\
		echo aaa;\
	else \
		echo bbb;\
	fi
	echo $(wildcard *.c)

dashe:
	if [ -e 123 ] ; then \
		echo "fool you are";\
	fi


