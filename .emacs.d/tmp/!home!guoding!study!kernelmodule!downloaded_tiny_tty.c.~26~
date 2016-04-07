#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>

#include "downloaded_tiny_tty.h"

#define DRIVER_VERSION "v2.0"
#define DRIVER_AUTHOR "Greg Kroah-Hartman <greg@kroah.com>"
#define DRIVER_DESC "Tiny TTY driver"

/* Module information */
MODULE_AUTHOR( DRIVER_AUTHOR );
MODULE_DESCRIPTION( DRIVER_DESC );
MODULE_LICENSE("GPL");

static const struct file_operations hello_proc_fops;//dgl_temp
static struct tty_driver *tiny_tty_driver;
static int proc_write_hello(struct file *file, const unsigned char *buffer,unsigned long count,void *data) ;


static int tty_device_num;

#define DELAY_TIME		HZ * 2	/* 2 seconds per character */
#define TINY_DATA_CHARACTER	't'

#define TINY_TTY_MAJOR		240	/* experimental range */
#define TINY_TTY_MINORS		4	/* only have 4 devices */

static struct tty_port zwave_port;

#define STRINGLEN 1024

unsigned char tty_proc_buffer[STRINGLEN]; //currently use one buffer for test 

struct tiny_serial {
	struct tty_struct	*tty;		/* pointer to the tty for this device */
	int			open_count;	/* number of times this port has been opened */
	struct semaphore	sem;		/* locks this structure */
	struct timer_list	*timer;

	/* for tiocmget and tiocmset functions */
	int			msr;		/* MSR shadow */
	int			mcr;		/* MCR shadow */

	/* for ioctl fun */
	struct serial_struct	serial;
	wait_queue_head_t	wait;
	struct async_icount	icount;
};

static struct tiny_serial *tiny_table[TINY_TTY_MINORS];	/* initially all NULL */


static void tiny_timer(unsigned long timer_data)
{
	struct tiny_serial *tiny = (struct tiny_serial *)timer_data;
	struct tty_struct *tty;
	struct tty_port *port;
	int i;
	/* char data[1] = {TINY_DATA_CHARACTER};//this is an example dgl_temp */
	int data_size = STRINGLEN;//size not ture dgl_temp copy all data
    printk("tiny_timmer %d\n", __LINE__);
	if (!tiny)
		return;
    printk("tiny_timmer %d\n", __LINE__);

	tty = tiny->tty;
	port = tty->port;
    printk("tiny_timmer %d\n", __LINE__);
	/* /\* send the data to the tty layer for users to read.  This doesn't */
	/*  * actually push the data through unless tty->low_latency is set *\/ */
	for (i = 0; i < data_size; ++i) {
		if (!tty_buffer_request_room(port, 1))
			tty_flip_buffer_push(port);//dgl_temp
		/* tty_insert_flip_char(port, data[i], TTY_NORMAL); */
		tty_insert_flip_char(port, tty_proc_buffer[i], TTY_NORMAL);
	}
    /* tty_insert_flip_string(port, tty_proc_buffer, data_size); */
    printk("tiny_timmer %d\n", __LINE__);
    
	tty_flip_buffer_push(port);

	/* resubmit the timer again */
	tiny->timer->expires = jiffies + DELAY_TIME;
    printk("tiny_timmer %d\n", __LINE__);

	add_timer(tiny->timer);
}

static int tiny_open(struct tty_struct *tty, struct file *file)
{
	struct tiny_serial *tiny;
	struct timer_list *timer;
	int index;
	/* initialize the pointer in case something fails */
	tty->driver_data = NULL;
    

	/* get the serial object associated with this tty pointer */
	index = tty->index;
	tiny = tiny_table[index];
	if (tiny == NULL) {
		/* first time accessing this device, let's create it */
		tiny = kmalloc(sizeof(*tiny), GFP_KERNEL);
		if (!tiny)
			return -ENOMEM;

		sema_init(&tiny->sem, 1);
		tiny->open_count = 0;
		tiny->timer = NULL;

		tiny_table[index] = tiny;
	}

	down(&tiny->sem);

	/* save our structure within the tty structure */
	tty->driver_data = tiny;
	tiny->tty = tty;

	++tiny->open_count;
	if (tiny->open_count == 1) {
		/* this is the first time this port is opened */
		/* do any hardware initialization needed here */

		/* create our timer and submit it */
		if (!tiny->timer) {//old timer need change maybe dgl_temp
			timer = kmalloc(sizeof(*timer), GFP_KERNEL);
			if (!timer) {
				up(&tiny->sem);
				return -ENOMEM;
			}
			tiny->timer = timer;
		}
        init_timer(tiny->timer);
		tiny->timer->data = (unsigned long )tiny;
		tiny->timer->expires = jiffies + DELAY_TIME;
		tiny->timer->function = tiny_timer;
		add_timer(tiny->timer);
        printk("tiny_open %d\n", __LINE__);

	}

    tty_port_tty_set(&zwave_port, tty);//dgl_temp location unsure
	up(&tiny->sem);
    printk("tiny_open quit %d\n", __LINE__);

	return 0;
}

static void do_close(struct tiny_serial *tiny)
{
	down(&tiny->sem);

	if (!tiny->open_count) {
		/* port was never opened */
		goto exit;
	}

	--tiny->open_count;
	if (tiny->open_count <= 0) {
		/* The port is being closed by the last user. */
		/* Do any hardware specific stuff here */

		/* shut down our timer */
		del_timer(tiny->timer);
	}
exit:
	up(&tiny->sem);
}

static void tiny_close(struct tty_struct *tty, struct file *file)
{
	struct tiny_serial *tiny = tty->driver_data;

	if (tiny)
		do_close(tiny);
}	

static int tiny_write(struct tty_struct *tty, 
		      const unsigned char *buffer, int count)
{
    printk("tiny_write\n");
	struct tiny_serial *tiny = tty->driver_data;
	int i;
	int retval = -EINVAL;//dgl_change
    char ttyprocname[20];
	if (!tiny)
		return -ENODEV;

	down(&tiny->sem);

	if (!tiny->open_count)
    {
		/* port was not opened */
		goto exit;
    }

	/* fake sending the data out a hardware port by
	 * writing it to the kernel debug log.
	 */
    //printk("tiny write index = %d \n", tty->index);
	printk(KERN_DEBUG "%s - \n", __FUNCTION__);
  	for (i = 0; i < count; ++i)
    {
		printk("%02x ", buffer[i]);
    }
	printk("\n");
    /* if(count<STRINGLEN) */
    /*     buffer[count]='\0'; */
    /* else */
    /*     buffer[STRINGLEN-1]='\0' */;
    proc_write_hello(NULL, buffer, count,NULL);//dgl_temp
exit:
    printk("tiny_write exit\n");

	up(&tiny->sem);
	return retval;
}

static int tiny_write_room(struct tty_struct *tty) 
{
	struct tiny_serial *tiny = tty->driver_data;
	int room = -EINVAL;

	if (!tiny)
		return -ENODEV;

	down(&tiny->sem);
	
	if (!tiny->open_count) {
		/* port was not opened */
		goto exit;
	}

	/* calculate how much room is left in the device */
	room = 255;

exit:
	up(&tiny->sem);
	return room;
}

#define RELEVANT_IFLAG(iflag) ((iflag) & (IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK))

static void tiny_set_termios(struct tty_struct *tty, struct ktermios *old_termios)
{
	unsigned int cflag;

	cflag = tty->termios.c_cflag;

	/* check that they really want us to change something */
	if (old_termios) {
		if ((cflag == old_termios->c_cflag) &&
		    (RELEVANT_IFLAG(tty->termios.c_iflag) ==
		     RELEVANT_IFLAG(old_termios->c_iflag))) {
			printk(KERN_DEBUG " - nothing to change...\n");
			return;
		}
	}

	/* get the byte size */
	switch (cflag & CSIZE) {
		case CS5:
			printk(KERN_DEBUG " - data bits = 5\n");
			break;
		case CS6:
			printk(KERN_DEBUG " - data bits = 6\n");
			break;
		case CS7:
			printk(KERN_DEBUG " - data bits = 7\n");
			break;
		default:
		case CS8:
			printk(KERN_DEBUG " - data bits = 8\n");
			break;
	}
	
	/* determine the parity */
	if (cflag & PARENB)
		if (cflag & PARODD)
			printk(KERN_DEBUG " - parity = odd\n");
		else
			printk(KERN_DEBUG " - parity = even\n");
	else
		printk(KERN_DEBUG " - parity = none\n");

	/* figure out the stop bits requested */
	if (cflag & CSTOPB)
		printk(KERN_DEBUG " - stop bits = 2\n");
	else
		printk(KERN_DEBUG " - stop bits = 1\n");

	/* figure out the hardware flow control settings */
	if (cflag & CRTSCTS)
		printk(KERN_DEBUG " - RTS/CTS is enabled\n");
	else
		printk(KERN_DEBUG " - RTS/CTS is disabled\n");
	
	/* determine software flow control */
	/* if we are implementing XON/XOFF, set the start and 
	 * stop character in the device */
	if (I_IXOFF(tty) || I_IXON(tty)) {
		unsigned char stop_char  = STOP_CHAR(tty);
		unsigned char start_char = START_CHAR(tty);

		/* if we are implementing INBOUND XON/XOFF */
		if (I_IXOFF(tty))
			printk(KERN_DEBUG " - INBOUND XON/XOFF is enabled, "
				"XON = %2x, XOFF = %2x", start_char, stop_char);
		else
			printk(KERN_DEBUG" - INBOUND XON/XOFF is disabled");

		/* if we are implementing OUTBOUND XON/XOFF */
		if (I_IXON(tty))
			printk(KERN_DEBUG" - OUTBOUND XON/XOFF is enabled, "
				"XON = %2x, XOFF = %2x", start_char, stop_char);
		else
			printk(KERN_DEBUG" - OUTBOUND XON/XOFF is disabled");
	}

	/* get the baud rate wanted */
	printk(KERN_DEBUG " - baud rate = %d", tty_get_baud_rate(tty));
}

/* Our fake UART values */
#define MCR_DTR		0x01
#define MCR_RTS		0x02
#define MCR_LOOP	0x04
#define MSR_CTS		0x08
#define MSR_CD		0x10
#define MSR_RI		0x20
#define MSR_DSR		0x40

static int tiny_tiocmget(struct tty_struct *tty)
{
	struct tiny_serial *tiny = tty->driver_data;

	unsigned int result = 0;
	unsigned int msr = tiny->msr;
	unsigned int mcr = tiny->mcr;

	result = ((mcr & MCR_DTR)  ? TIOCM_DTR  : 0) |	/* DTR is set */
             ((mcr & MCR_RTS)  ? TIOCM_RTS  : 0) |	/* RTS is set */
             ((mcr & MCR_LOOP) ? TIOCM_LOOP : 0) |	/* LOOP is set */
             ((msr & MSR_CTS)  ? TIOCM_CTS  : 0) |	/* CTS is set */
             ((msr & MSR_CD)   ? TIOCM_CAR  : 0) |	/* Carrier detect is set*/
             ((msr & MSR_RI)   ? TIOCM_RI   : 0) |	/* Ring Indicator is set */
             ((msr & MSR_DSR)  ? TIOCM_DSR  : 0);	/* DSR is set */

	return result;
}

static int tiny_tiocmset(struct tty_struct *tty, unsigned int set,
			 unsigned int clear)
{
	struct tiny_serial *tiny = tty->driver_data;
	unsigned int mcr = tiny->mcr;

	if (set & TIOCM_RTS)
		mcr |= MCR_RTS;
	if (set & TIOCM_DTR)
		mcr |= MCR_RTS;

	if (clear & TIOCM_RTS)
		mcr &= ~MCR_RTS;
	if (clear & TIOCM_DTR)
		mcr &= ~MCR_RTS;

	/* set the new MCR value in the device */
	tiny->mcr = mcr;
	return 0;
}

static int tiny_proc_show(struct seq_file *m, void *v)
{
	struct tiny_serial *tiny;
	int i;

	seq_printf(m, "tinyserinfo:1.0 driver:%s\n", DRIVER_VERSION);
	for (i = 0; i < TINY_TTY_MINORS; ++i) {
		tiny = tiny_table[i];
		if (tiny == NULL)
			continue;

		seq_printf(m, "%d\n", i);
	}

	return 0;
}

#define tiny_ioctl tiny_ioctl_tiocgserial
static int tiny_ioctl(struct tty_struct *tty, unsigned int cmd,
		      unsigned long arg)
{
	struct tiny_serial *tiny = tty->driver_data;

	if (cmd == TIOCGSERIAL) {
		struct serial_struct tmp;

		if (!arg)
			return -EFAULT;

		memset(&tmp, 0, sizeof(tmp));

		tmp.type		= tiny->serial.type;
		tmp.line		= tiny->serial.line;
		tmp.port		= tiny->serial.port;
		tmp.irq			= tiny->serial.irq;
		tmp.flags		= ASYNC_SKIP_TEST | ASYNC_AUTO_IRQ;
		tmp.xmit_fifo_size	= tiny->serial.xmit_fifo_size;
		tmp.baud_base		= tiny->serial.baud_base;
		tmp.close_delay		= 5*HZ;
		tmp.closing_wait	= 30*HZ;
		tmp.custom_divisor	= tiny->serial.custom_divisor;
		tmp.hub6		= tiny->serial.hub6;
		tmp.io_type		= tiny->serial.io_type;

		if (copy_to_user((void __user *)arg, &tmp, sizeof(struct serial_struct)))
			return -EFAULT;
		return 0;
	}
	return -ENOIOCTLCMD;
}
#undef tiny_ioctl

#define tiny_ioctl tiny_ioctl_tiocmiwait
static int tiny_ioctl(struct tty_struct *tty, unsigned int cmd,
		      unsigned long arg)
{
	struct tiny_serial *tiny = tty->driver_data;

	if (cmd == TIOCMIWAIT) {
		DECLARE_WAITQUEUE(wait, current);
		struct async_icount cnow;
		struct async_icount cprev;

		cprev = tiny->icount;
		while (1) {
			add_wait_queue(&tiny->wait, &wait);
			set_current_state(TASK_INTERRUPTIBLE);
			schedule();
			remove_wait_queue(&tiny->wait, &wait);

			/* see if a signal woke us up */
			if (signal_pending(current))
				return -ERESTARTSYS;

			cnow = tiny->icount;
			if (cnow.rng == cprev.rng && cnow.dsr == cprev.dsr &&
			    cnow.dcd == cprev.dcd && cnow.cts == cprev.cts)
				return -EIO; /* no change => error */
			if (((arg & TIOCM_RNG) && (cnow.rng != cprev.rng)) ||
			    ((arg & TIOCM_DSR) && (cnow.dsr != cprev.dsr)) ||
			    ((arg & TIOCM_CD)  && (cnow.dcd != cprev.dcd)) ||
			    ((arg & TIOCM_CTS) && (cnow.cts != cprev.cts)) ) {
				return 0;
			}
			cprev = cnow;
		}

	}
	return -ENOIOCTLCMD;
}
#undef tiny_ioctl

#define tiny_ioctl tiny_ioctl_tiocgicount
static int tiny_ioctl(struct tty_struct *tty, unsigned int cmd,
		      unsigned long arg)
{
	struct tiny_serial *tiny = tty->driver_data;

	if (cmd == TIOCGICOUNT) {
		struct async_icount cnow = tiny->icount;
		struct serial_icounter_struct icount;

		icount.cts	= cnow.cts;
		icount.dsr	= cnow.dsr;
		icount.rng	= cnow.rng;
		icount.dcd	= cnow.dcd;
		icount.rx	= cnow.rx;
		icount.tx	= cnow.tx;
		icount.frame	= cnow.frame;
		icount.overrun	= cnow.overrun;
		icount.parity	= cnow.parity;
		icount.brk	= cnow.brk;
		icount.buf_overrun = cnow.buf_overrun;

		if (copy_to_user((void __user *)arg, &icount, sizeof(icount)))
			return -EFAULT;
		return 0;
	}
	return -ENOIOCTLCMD;
}
#undef tiny_ioctl

/* static int tiny_ioctl_deviceadd(struct tty_struct *tty, unsigned int cmd, unsigned long arg) */
/* { */
/*     char device_name[20]; */
/*     int ret = 0; */
/*     sprintf(device_name, "ttty%d", ++tty_device_num); */
/*     ret = proc_create(device_name, 0x777, NULL, &hello_proc_fops); */
/*     printk("%s %d\n", __func__, __LINE__); */
/*     if(ret!=0) */
/*         return ret; */
/*     ret = tty_register_device(tiny_tty_driver, tty_device_num, NULL);//null means no parent dgl_note */
/*     return ret; */
/* } */

/* the real tiny_ioctl function.  The above is done to get the small functions in the book */
static int tiny_ioctl(struct tty_struct *tty, unsigned int cmd,
		      unsigned long arg)
{
    int ret = 0;
    printk("%s cmd = %0x %d\n", __func__, cmd , __LINE__);
	switch (cmd) {
	case TIOCGSERIAL:
		return tiny_ioctl_tiocgserial(tty, cmd, arg);
	case TIOCMIWAIT:
		return tiny_ioctl_tiocmiwait(tty, cmd, arg);
	case TIOCGICOUNT:
		return tiny_ioctl_tiocgicount(tty, cmd, arg);
    /* case DEVICEADD: */
    /*     return tiny_ioctl_deviceadd(tty, cmd, arg); */
	}
    printk("%s cmd = %0x %d\n", __func__, cmd , __LINE__);
	return -ENOIOCTLCMD;
}

static int tiny_proc_open(struct inode *inode, struct file *file)
{
    
        return single_open(file, tiny_proc_show, NULL);
}


static const struct file_operations serial_proc_fops = {
        .owner          = THIS_MODULE,
        .open           = tiny_proc_open,
	.read           = seq_read,
        .llseek         = seq_lseek,
        .release        = single_release,
};

static struct tty_operations serial_ops = {
	.open = tiny_open,
	.close = tiny_close,
	.write = tiny_write,
	.write_room = tiny_write_room,
	.set_termios = tiny_set_termios,
	.proc_fops	= &serial_proc_fops,
	.tiocmget = tiny_tiocmget,
	.tiocmset = tiny_tiocmset,
	.ioctl = tiny_ioctl,
};
//dgl_add proc process ,currently only support one ttydevice


int temp , len;


static int proc_write_hello(struct file *file,const unsigned char *buffer,unsigned long count,void *data) {
    //int len;
    /* if (count = STRINGLEN) */
    /*     len = STRINGLEN - 1; */
    /* else */
    /*     len = count; */
    if(count >= STRINGLEN)
    {
        len = STRINGLEN - 1;
    }
    else
        len = count;
    temp = len;
    printk("buffer = %s   count = %d\n", buffer, count);
    memset(tty_proc_buffer, 0, STRINGLEN);
/*    int ret = copy_from_user(tty_proc_buffer, buffer, len);*/
    sprintf(tty_proc_buffer, buffer, len);
    printk("written: %d buffer3 = %s, end\n", len, tty_proc_buffer);
    return len;
}

static int proc_read_hello(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{
    if(count>temp)
    {
        count=temp;
    }
    temp = temp - count;
    copy_to_user(buf, tty_proc_buffer, count);
    if(count==0)
        temp=len;
    return count;
}



static int hello_proc_show(struct seq_file *m, void *v) {
  seq_printf(m, "Hello proc!\n");
  return 0;
}

static int hello_proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations hello_proc_fops = {//dgl_note need fur change
  .owner = THIS_MODULE,
  .open = hello_proc_open,
  .read = proc_read_hello, //seq_read,
  .write = proc_write_hello,//seq_write,
  .llseek = seq_lseek,
  .release = single_release,
};


static int __init tiny_init(void)
{
	int retval;
	int i;

    printk("\nenter tiny dgl\n");
	/* allocate the tty driver */
	tiny_tty_driver = alloc_tty_driver(TINY_TTY_MINORS);
	if (!tiny_tty_driver)
		return -ENOMEM;
    tty_port_init(&zwave_port);
    //zwave_port.low_latency = 1;//dgl_temp_temp set low latency value for read
	/* initialize the tty driver */
	tiny_tty_driver->owner = THIS_MODULE;
	tiny_tty_driver->driver_name = "tiny_tty";
	tiny_tty_driver->name = "ttty";
	tiny_tty_driver->major = TINY_TTY_MAJOR,
	tiny_tty_driver->type = TTY_DRIVER_TYPE_SERIAL,
	tiny_tty_driver->subtype = SERIAL_TYPE_NORMAL,
	tiny_tty_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV,
	tiny_tty_driver->init_termios = tty_std_termios;
	tiny_tty_driver->init_termios.c_cflag = B9600 | CS8 | CREAD | HUPCL | CLOCAL;
	tty_set_operations(tiny_tty_driver, &serial_ops);
	tty_port_link_device(&zwave_port, tiny_tty_driver, 0);//dgl_add
	/* register the tty driver */
	retval = tty_register_driver(tiny_tty_driver);
	if (retval) {
		printk(KERN_ERR "failed to register tiny tty driver");
		put_tty_driver(tiny_tty_driver);
		tty_port_destroy(&zwave_port);
		return retval;
	}
    char  procfile_name_init[20];
	for (i = 0; i < TINY_TTY_MINORS; ++i)
    {
        if(sprintf(procfile_name_init, "ttty%d", i)>=0)
        {
            printk("%s\n", procfile_name_init);
            proc_create(procfile_name_init, 0x777, NULL, &hello_proc_fops);
        }
		tty_register_device(tiny_tty_driver, i, NULL);//null means no parent dgl_note
        tty_device_num++;
    }
	printk(KERN_INFO DRIVER_DESC " " DRIVER_VERSION);
	return retval;
}

static void __exit tiny_exit(void)
{
	struct tiny_serial *tiny;
	int i;
    printk("exiting tiny_tty\n");
	for (i = 0; i < TINY_TTY_MINORS; ++i)
    {
		tty_unregister_device(tiny_tty_driver, i);
        tty_device_num--;
    }
	tty_unregister_driver(tiny_tty_driver);
	put_tty_driver(tiny_tty_driver);//dgl_added remove driver
    char procfile_name_exit[20];
	/* shut down all of the timers and free the memory */
	for (i = 0; i < TINY_TTY_MINORS; ++i) {
        if(sprintf(procfile_name_exit,"ttty%d", i)>=0)
        {
            printk("%s %d\n", __func__, __LINE__);
            remove_proc_entry(procfile_name_exit, NULL);
        }
		tiny = tiny_table[i];
		if (tiny) {
			/* close the port */
			while (tiny->open_count)
				do_close(tiny);

			/* shut down our timer and free the memory */
			del_timer(tiny->timer);
			kfree(tiny->timer);
			kfree(tiny);
			tiny_table[i] = NULL;
		}
	}
}





module_init(tiny_init);
module_exit(tiny_exit);
