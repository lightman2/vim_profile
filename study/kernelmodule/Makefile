# obj-m += hello.o
# obj-m += proc.o
# obj-m += proc2.o
obj-m :=down_tiny_tty.o
# obj-m :=downloaded_tiny_tty.o vtty_agent_fsm.o  vtty_agent_fsm_get1.o vtty_agent_fsm_get2.o  vtty_agent_send.o vtty_agent.o

# vtty_agent.c  vtty_agent_fsm.c  vtty_agent_fsm_get1.c  vtty_agent_fsm_get2.c  vtty_agent_send.c
down_tiny_tty-objs =downloaded_tiny_tty.o 
# down_tiny_tty-objs =copyofdownloaded_tinytty.o 
# vtty_agent_fsm.o  vtty_agent_fsm_get1.o vtty_agent_fsm_get2.o  vtty_agent_send.o vtty_agent.o

ccflags-m := -std=gnu99
ccflags-y := -std=gnu99 -Wno-declaration-after-statement
# DEBFLAGS = -O -g -DSCULL_DEBUG # "-O" is needed to expand inlines
# $(DEBFLAGS)
PWD=$(shell pwd)
VER=$(shell uname -r)
KERNEL_BUILD=/lib/modules/$(VER)/build


all:
	make   -C  /lib/modules/$(shell uname -r)/build M=$(PWD) modules



clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


install:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) INSTALL_MOD_PATH=$(INSTALL_ROOT) modules_install
