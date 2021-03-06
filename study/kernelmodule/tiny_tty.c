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
#include <linux/mutex.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <asm/unistd.h>
#include <asm/uaccess.h>
//#include <linux/string.h>
#include "tiny_tty.h"

/*assume tty->index was the proc number*/

#define DRIVER_VERSION "v1.0"
#define DRIVER_AUTHOR "dgl"
#define DRIVER_DESC "TTY driver"

/* Module information */
MODULE_AUTHOR( DRIVER_AUTHOR );
MODULE_DESCRIPTION( DRIVER_DESC );
MODULE_LICENSE("GPL");

#define PRINTFLAG 3
#define LEVEL_MUST_PRINTK 1
#define LEVEL_DBG_PRINTK 2

#if (PRINTFLAG - LEVEL_DBG_PRINTK )
#define  print_debug(FORMAT, ...) printk("%s %d    " FORMAT  , __func__, __LINE__, __VA_ARGS__)
#else
#define print_debug(...)
#endif

static int tty_device_num;
#define DELAY_TIME		HZ * 2	/* 2 seconds per character */
#define ZWAVE_TEC_DATA_CHARACTER	't'

#define ZWAVE_TEC_TTY_MAJOR		240	/* experimental range */
#define ZWAVE_TEC_TTY_MINORS	   20	/* only have 4 devices */
#define STRINGLEN 1024


static dev_occupied[ZWAVE_TEC_TTY_MINORS];
typedef struct _BUFFER_ {
char buf[STRINGLEN];
int len;
}BUFFER,*PBUFFER;



/* static BUFFER buf_pool[ZWAVE_TEC_TTY_MINORS][10];// = {{0}}; */
static BUFFER buf_pool[ZWAVE_TEC_TTY_MINORS][10];// = {{0}};


int buf_rd_index[ZWAVE_TEC_TTY_MINORS] = {0}, buf_wr_index[ZWAVE_TEC_TTY_MINORS] = {0};
static int flag[ZWAVE_TEC_TTY_MINORS];


struct file *file = NULL;
#define MY_FILE "/var/tmp/vtty/ttty%d_up"



static int len;
unsigned char tty_proc_buffer[STRINGLEN]; //currently use one buffer for test 
static int proc_up_write_zwave(struct file *file, const unsigned char *buffer,unsigned long count,void *data) ;
static int proc_zwave_write_zwave(int index,const unsigned char *buffer,unsigned long count,void *data);
static struct proc_dir_entry *zwave_proc_dir = NULL;

struct zwave_tec_serial {
	struct tty_port	port;		/* pointer to the tty for this device */
	struct mutex	port_write_mutex;
	struct timer_list	*timer;

	/* for tiocmget and tiocmset functions */
	int			msr;		/* MSR shadow */
	int			mcr;		/* MCR shadow */

	/* for ioctl fun */
	struct serial_struct	serial;
	wait_queue_head_t	wait;
	struct async_icount	icount;
};

static struct zwave_tec_serial *zwave_tec_table[ZWAVE_TEC_TTY_MINORS];	/* initially all NULL */

#if 0
static void zwave_tec_timer(unsigned long timer_data)
{
	struct zwave_tec_serial *zwave_tec_ = (struct zwave_tec_serial *)timer_data;
	struct tty_port *port;
	int i;
	char data[1] = {ZWAVE_TEC_DATA_CHARACTER};
	int data_size = 1;

	if (!zwave_tec_)
		return;

	port = &zwave_tec_->port;

	/* send the data to the tty layer for users to read.  This doesn't
	 * actually push the data through unless tty->low_latency is set */
	/* FIXME: when data_size increase,
	 * we need to call tty_flip_buffer_push during tty_insert_flip_char */
	tty_buffer_request_room(port, data_size);
	for (i = 0; i < data_size; ++i) {
		tty_insert_flip_char(port, data[i], TTY_NORMAL);
	}
	tty_flip_buffer_push(port);

	/* resubmit the timer again */
	zwave_tec_->timer->expires = jiffies + DELAY_TIME;
	add_timer(zwave_tec_->timer);
}
#endif

static int dev_malloc(void)
{
    int i ;
    for(i=0;i<ZWAVE_TEC_TTY_MINORS; i++)
    {
        if(dev_occupied[i]==0)
        {
            dev_occupied[i]=1;
            return i;
        }
    }
    return -1;
}

static int dev_free(int i)
{
    if(i<ZWAVE_TEC_TTY_MINORS&& i>=0)
    {
        dev_occupied[i] = 0;
        return 0;
    }
    else
        return -1;    
}

static int dev_name_get(char *devname)//inline
{
    char *numberend;
    long int numbernum;
    int rc;
    numberend = strstr(devname, "_up");
    int len = numberend - (devname+4);
    char number[10];
    memset(number, 0, 10);
    snprintf(number, len+1, devname+4);
	rc = kstrtol(number, 10, &numbernum);
    if(rc!=0)
        return -1;
    else
        return numbernum;

  //  numbernum = strtol(number, &endptr, 10);
}

/*
 * this is the first time this port is opened
 * do any hardware initialization needed here
 */
static int zwave_tec_activate(struct tty_port *tport, struct tty_struct *tty)
{
	/* struct zwave_tec_serial *zwave_tec_; */
	/* struct timer_list *timer; */

	// zwave_tec_ = container_of(tport, struct zwave_tec_serial, port);

	// /* create our timer and submit it */
	// if (!zwave_tec_->timer) {
	// 	timer = kmalloc(sizeof(*timer), GFP_KERNEL);
	// 	if (!timer) {
	// 		return -ENOMEM;
	// 	}
	// 	init_timer(timer);
	// 	zwave_tec_->timer = timer;
	// }
	// zwave_tec_->timer->data = (unsigned long )zwave_tec_;
	// zwave_tec_->timer->expires = jiffies + DELAY_TIME;
	// zwave_tec_->timer->function = zwave_tec_timer;
	// add_timer(zwave_tec_->timer);
	return 0;
}

/*
 * The port is being closed by the last user.
 * Do any hardware specific stuff here *
 */
static void zwave_tec_shutdown(struct tty_port *tport){
	struct zwave_tec_serial *zwave_tec_;

	zwave_tec_ = container_of(tport, struct zwave_tec_serial, port);

	/* shut down our timer */
	// del_timer(zwave_tec_->timer);
}

static int zwave_tec_open(struct tty_struct *tty, struct file *file)
{
	struct zwave_tec_serial *zwave_tec_;
	int index;
	struct tty_port *port;
	int status;

	/* initialize the pointer in case something fails */
	tty->driver_data = NULL;

	/* get the serial object associated with this tty pointer */
	index = tty->index;
	zwave_tec_ = zwave_tec_table[index];

	port = &zwave_tec_->port;

	status = tty_port_open(port, tty, file);

	if(!status) {
		/* save our structure within the tty structure */
		tty->driver_data = zwave_tec_;
	}

	return status;
}


static void zwave_tec_close(struct tty_struct *tty, struct file *file)
{
	struct zwave_tec_serial *zwave_tec_ = tty->driver_data;
	struct tty_port *port;

	port = &zwave_tec_->port;

	if (zwave_tec_)
		tty_port_close(port, tty, file);
}	

static int zwave_tec_write(struct tty_struct *tty, 
                      const unsigned char *buffer, int count)
{
	// struct tty_port *port;
	// int i;
	// char data[1] = {ZWAVE_TEC_DATA_CHARACTER};
	// int data_size = 1;

	// if (!zwave_tec_)
	// 	return;

	// port = &zwave_tec_->port;

	/* send the data to the tty layer for users to read.  This doesn't
	 * actually push the data through unless tty->low_latency is set */
	/* FIXME: when data_size increase,
	 * we need to call tty_flip_buffer_push during tty_insert_flip_char */
	// tty_buffer_request_room(port, data_size);
	// for (i = 0; i < data_size; ++i) {
	// 	tty_insert_flip_char(port, data[i], TTY_NORMAL);
	// }
	// tty_flip_buffer_push(port);



	struct zwave_tec_serial *zwave_tec_ = tty->driver_data;
	int i;
	int retval;
	struct tty_port *port;
	unsigned long flags;

	if (!zwave_tec_)
		return -ENODEV;

	mutex_lock(&zwave_tec_->port_write_mutex);

	port = &zwave_tec_->port;
	spin_lock_irqsave(&port->lock, flags);
	if (!port->count) {
		spin_unlock_irqrestore(&port->lock, flags);
		/* port was not opened */
		retval = -EINVAL;
		goto exit;
	}
	spin_unlock_irqrestore(&port->lock, flags);

	/* fake sending the data out a hardware port by
	 * writing it to the kernel debug log.
	 */

	printk(KERN_DEBUG "%s - ", __FUNCTION__);
	for (i = 0; i < count; ++i)
		printk("%02x ", buffer[i]);
	printk("\n");
//

//&zwave_tec_table[i]->por
    printk("tty->index = %d\n", tty->index);
    proc_zwave_write_zwave(tty->index, buffer, count,NULL);//dgl_temp write proc write the array
	//tty_insert_flip_string(port, buffer, count);
    //tty_flip_buffer_push(port);
//
	retval = count;
		
exit:
	mutex_unlock(&zwave_tec_->port_write_mutex);
	return retval;
}

static int zwave_tec_write_room(struct tty_struct *tty) 
{
	struct zwave_tec_serial *zwave_tec_ = tty->driver_data;
	int room = -EINVAL;
	struct tty_port *port;
	unsigned long flags;

	if (!zwave_tec_)
		return -ENODEV;

	mutex_lock(&zwave_tec_->port_write_mutex);
	
	port = &zwave_tec_->port;
	spin_lock_irqsave(&port->lock, flags);
	if (!port->count) {
		spin_unlock_irqrestore(&port->lock, flags);
		/* port was not opened */
		goto exit;
	}
	spin_unlock_irqrestore(&port->lock, flags);

	/* calculate how much room is left in the device */
	room = 255;

exit:
	mutex_unlock(&zwave_tec_->port_write_mutex);
	return room;
}

#define RELEVANT_IFLAG(iflag) ((iflag) & (IGNBRK|BRKINT|IGNPAR|PARMRK|INPCK))

static void zwave_tec_set_termios(struct tty_struct *tty, struct ktermios *old_termios)
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

static int zwave_tec_tiocmget(struct tty_struct *tty)
{
	struct zwave_tec_serial *zwave_tec_ = tty->driver_data;

	unsigned int result = 0;
	unsigned int msr = zwave_tec_->msr;
	unsigned int mcr = zwave_tec_->mcr;

	result = ((mcr & MCR_DTR)  ? TIOCM_DTR  : 0) |	/* DTR is set */
        ((mcr & MCR_RTS)  ? TIOCM_RTS  : 0) |	/* RTS is set */
        ((mcr & MCR_LOOP) ? TIOCM_LOOP : 0) |	/* LOOP is set */
        ((msr & MSR_CTS)  ? TIOCM_CTS  : 0) |	/* CTS is set */
        ((msr & MSR_CD)   ? TIOCM_CAR  : 0) |	/* Carrier detect is set*/
        ((msr & MSR_RI)   ? TIOCM_RI   : 0) |	/* Ring Indicator is set */
        ((msr & MSR_DSR)  ? TIOCM_DSR  : 0);	/* DSR is set */

	return result;
}

static int zwave_tec_tiocmset(struct tty_struct *tty,
                         unsigned int set, unsigned int clear)
{
	struct zwave_tec_serial *zwave_tec_ = tty->driver_data;
	unsigned int mcr = zwave_tec_->mcr;

	if (set & TIOCM_RTS)
		mcr |= MCR_RTS;
	if (set & TIOCM_DTR)
		mcr |= MCR_RTS;

	if (clear & TIOCM_RTS)
		mcr &= ~MCR_RTS;
	if (clear & TIOCM_DTR)
		mcr &= ~MCR_RTS;

	/* set the new MCR value in the device */
	zwave_tec_->mcr = mcr;
	return 0;
}

static int  zwave_tec_tty_proc_show(struct seq_file *m, void *v)
{
	struct zwave_tec_serial *zwave_tec_;
	int i;

	seq_printf(m, "zwave_tec_serinfo:1.0 driver:%s\n", DRIVER_VERSION);
	for (i = 0; i < ZWAVE_TEC_TTY_MINORS; ++i) {
		zwave_tec_ = zwave_tec_table[i];
		if (zwave_tec_ == NULL)
			continue;
		seq_printf(m, "%d\n", i);
	}
	return 0;
}

#define zwave_tec_ioctl zwave_tec_ioctl_tiocgserial
static int zwave_tec_ioctl(struct tty_struct *tty,
                      unsigned int cmd, unsigned long arg)
{
	struct zwave_tec_serial *zwave_tec_ = tty->driver_data;

	if (cmd == TIOCGSERIAL) {
		struct serial_struct tmp;

		if (!arg)
			return -EFAULT;

		memset(&tmp, 0, sizeof(tmp));

		tmp.type		= zwave_tec_->serial.type;
		tmp.line		= zwave_tec_->serial.line;
		tmp.port		= zwave_tec_->serial.port;
		tmp.irq			= zwave_tec_->serial.irq;
		tmp.flags		= ASYNC_SKIP_TEST | ASYNC_AUTO_IRQ;
		tmp.xmit_fifo_size	= zwave_tec_->serial.xmit_fifo_size;
		tmp.baud_base		= zwave_tec_->serial.baud_base;
		tmp.close_delay		= 5*HZ;
		tmp.closing_wait	= 30*HZ;
		tmp.custom_divisor	= zwave_tec_->serial.custom_divisor;
		tmp.hub6		= zwave_tec_->serial.hub6;
		tmp.io_type		= zwave_tec_->serial.io_type;

		if (copy_to_user((void __user *)arg, &tmp, sizeof(struct serial_struct)))
			return -EFAULT;
		return 0;
	}
	return -ENOIOCTLCMD;
}
#undef zwave_tec_ioctl

#define zwave_tec_ioctl zwave_tec_ioctl_tiocmiwait
static int zwave_tec_ioctl(struct tty_struct *tty,
                      unsigned int cmd, unsigned long arg)
{
	struct zwave_tec_serial *zwave_tec_ = tty->driver_data;

	if (cmd == TIOCMIWAIT) {
		DECLARE_WAITQUEUE(wait, current);
		struct async_icount cnow;
		struct async_icount cprev;

		cprev = zwave_tec_->icount;
		while (1) {
			add_wait_queue(&zwave_tec_->wait, &wait);
			set_current_state(TASK_INTERRUPTIBLE);
			schedule();
			remove_wait_queue(&zwave_tec_->wait, &wait);

			/* see if a signal woke us up */
			if (signal_pending(current))
				return -ERESTARTSYS;

			cnow = zwave_tec_->icount;
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
#undef zwave_tec_ioctl

#define zwave_tec_ioctl zwave_tec_ioctl_tiocgicount
            static int zwave_tec_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg)
            {
                struct zwave_tec_serial *zwave_tec_ = tty->driver_data;

                if (cmd == TIOCGICOUNT) {
                    struct async_icount cnow = zwave_tec_->icount;
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
#undef zwave_tec_ioctl

/* the real zwave_tec_ioctl function.  The above is done to get the small functions in the book */
static int zwave_tec_ioctl(struct tty_struct *tty,
                      unsigned int cmd, unsigned long arg)
{
    printk("ioctl  cmd = %d\n", cmd);
	switch (cmd) {
	case TIOCGSERIAL:
		return zwave_tec_ioctl_tiocgserial(tty, cmd, arg);
	case TIOCMIWAIT:
		return zwave_tec_ioctl_tiocmiwait(tty, cmd, arg);
	case TIOCGICOUNT:
		return zwave_tec_ioctl_tiocgicount(tty, cmd, arg);
    case ZWAVE_ADD_DEV:
        printk("%s %d add device called\n", __func__, __LINE__);
        return dev_malloc();
    case ZWAVE_DEL_DEV:
        printk("%s %d del device called\n", __func__, __LINE__);
        return dev_free((int)arg);
	}

	return -ENOIOCTLCMD;
}

static int zwave_tec_tty_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, zwave_tec_tty_proc_show, NULL);
}

static const struct file_operations zwave_tec_tty_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= zwave_tec_tty_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static struct tty_operations serial_ops = {
	.open = zwave_tec_open,
	.close = zwave_tec_close,
	.write = zwave_tec_write,
	.write_room = zwave_tec_write_room,
	.set_termios = zwave_tec_set_termios,
	.tiocmget = zwave_tec_tiocmget,
	.tiocmset = zwave_tec_tiocmset,
	.ioctl = zwave_tec_ioctl,
	.proc_fops       = &zwave_tec_tty_proc_fops,
};

static const struct tty_port_operations zwave_tec_port_ops = {
	.activate		= zwave_tec_activate,
	.shutdown		= zwave_tec_shutdown,
};

//the proc direction write function
static int proc_up_write_zwave(struct file *file,const unsigned char *buffer,unsigned long count,void *data) {
    int index = -1;
    if(count >= STRINGLEN)
    {
        len = STRINGLEN - 1;
    }
    else
        len = count;
    
    index = dev_name_get(file->f_path.dentry->d_iname);
    if(index<0)
    {
        print_debug("dev_name_get error %d\n", 1);
        return index;
    }
	tty_insert_flip_string(&zwave_tec_table[index]->port, buffer, len);
    tty_flip_buffer_push(&zwave_tec_table[index]->port);//dgl_note
    print_debug("written: %d buffer3 = %s, end\n", len, tty_proc_buffer);
    return len;
}
static int writeoffset;
struct mutex	LOCK_SELECT;


struct file* file_open(const char* path, int flags, int rights) {
    struct file* filp = NULL;
    mm_segment_t oldfs;
    int err = 0;

    oldfs = get_fs();
    set_fs(get_ds());
    filp = filp_open(path, flags, rights);
    set_fs(oldfs);
    if(IS_ERR(filp)) {
        err = PTR_ERR(filp);
        return NULL;
    }
    return filp;
}

static  int selectfile_for_notify(char *filename)
{
    char buf[128];
    struct file *file = NULL;
    mm_segment_t old_fs;
    int i = 0;
    if(file == NULL)
        file = filp_open(filename, O_RDWR | O_APPEND | O_CREAT, 0644);
    if (IS_ERR(file)) {
        printk("error occured while opening file %s, exiting...\n", filename);
        return 0;
    }
    sprintf(buf,"%s %d", "The Messages.",i);
    //printk("mesg:%s\n",buf);
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    file->f_op->write(file, (char *)buf, sizeof(buf), &file->f_pos);
    set_fs(old_fs);
    filp_close(file, NULL);  
    file = NULL;
    return 0;

}
//the dev direction write function 
static int proc_zwave_write_zwave(int index,const unsigned char *buffer,unsigned long count,void *data) {
    char notify_name[30];
    memset(notify_name, 0, 30);
    if(count >= STRINGLEN)
    {
        len = STRINGLEN - 1;
    }
    else
        len = count;
    printk("buffer = %s   count = %lu, writeoffset = %d tty_proc_buffer = %x\n", buffer, count, writeoffset,(unsigned int)tty_proc_buffer);
    if (writeoffset +len < STRINGLEN)
    {
        memcpy(tty_proc_buffer + writeoffset, buffer, len+1);//dgl_temp change to memcpy 
        writeoffset +=len+1;
    }

    memcpy(buf_pool[index][buf_wr_index[index]].buf, buffer, len);
    buf_pool[index][buf_wr_index[index]].len = len;
    buf_wr_index[index]++;
    if(buf_wr_index[index]>=10)
    	buf_wr_index[index] = 0;
    flag[index]++;
    snprintf(notify_name, 30, MY_FILE,index);
    selectfile_for_notify(notify_name);

   /* printk("file name = %s\n", file->f_path.entry->d_iname); */
//    printk("file name = %s\n", file->f_path->dentry->d_iname);  //->d_iname);

/*    int ret = copy_from_user(tty_proc_buffer, buffer, len);*/
	//sprintf(tty_proc_buffer,buffer);//dgl_temp_old
    //snprintf(tty_proc_buffer,len, buffer);
    print_debug("written: %d buffer3 = %s, end\n", len, tty_proc_buffer);
    return len;
}


static int proc_read_zwave(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{
    int index = -1;
    char notify_filename[30];
        print_debug("filp name = %s\n", filp->f_path.dentry->d_iname);
    index = dev_name_get(filp->f_path.dentry->d_iname);
    if(index <0)
    {
        printk("%s dev_name_get failed\n", __func__);
        return index;
    }

    print_debug("in proc_read_zwave, flag: %d\n",flag[index]);
    if(flag[index]>0)
    {
        int bytesread;
        copy_to_user(buf, buf_pool[index][buf_rd_index[index]].buf, buf_pool[index][buf_rd_index[index]].len);
        bytesread = buf_pool[index][buf_rd_index[index]].len;
        print_debug("buf_wr_index=%d,buf_rd_index:%d\n",buf_wr_index[index],buf_rd_index[index]);

        buf_rd_index[index]++;
        if(buf_rd_index[index]>=10)
            buf_rd_index[index] = 0;
        flag[index]--;
        if(buf_rd_index[index]!=buf_wr_index[index])
        {
            snprintf(notify_filename,30, MY_FILE, index);
            selectfile_for_notify(notify_filename);
        }
        return bytesread;
    }
    else
        return 0;
}



static int zwave_proc_show(struct seq_file *m, void *v) {
    print_debug("%s\n", __func__);
    seq_printf(m, "Zwave proc!\n");
    return 0;
}

static int zwave_proc_open(struct inode *inode, struct  file *file) {
    print_debug("%s\n", __func__);
    print_debug("%s\n", file->f_path.dentry->d_iname);
   /* printk("file name = %s\n", file->f_path.entry->d_iname); */

    return single_open(file, zwave_proc_show, NULL);
}

static const struct file_operations zwave_proc_up_fops = {//dgl_note need fur change
    .owner = THIS_MODULE,
    .open = zwave_proc_open,
    .read = proc_read_zwave, //seq_read,
    .write = proc_up_write_zwave,//seq_write,
    .llseek = seq_lseek,
    .release = single_release,//.poll
};

#if 0

static int get_file_name(char *pathname, struct file *file, struct path *path)
{
    char *tmp;
    spin_lock(&files->file_lock);
    file = fcheck_files(files, fd);
    if(!file){
        spin_unlock(&files->file_lock)
            return -ENOENT;
    }
    path = &file->f_path;
    path_get(path);
    spin_unlock(&files->file_lock);

    tmp = (char *)__get_free_page(GFP_TEMPORARY);
    if (!tmp){
        path_put(path);
        return -ENOMEM;
    }
    pathname = d_path(path, tmp, PAGE_SIZE);
    path_put(&path);
    
    if (IS_ERR(pathname)){
        free_page((unsigned long)tmp);
        return PTR_ERR(pathname);
    }
}
#endif
static struct tty_driver *zwave_tec_tty_driver;


static int __init zwave_tec_init(void)
{
	int retval;
	int i;
	struct zwave_tec_serial *zwave_tec_;

	/* allocate the tty driver */
	zwave_tec_tty_driver = alloc_tty_driver(ZWAVE_TEC_TTY_MINORS);
	if (!zwave_tec_tty_driver)
		return -ENOMEM;
    /* int j=dev_name_get("ttty1231_up"); */
    /* printk("devnum = %d\n", j); */

	/* initialize the tty driver */
	zwave_tec_tty_driver->owner = THIS_MODULE;
	zwave_tec_tty_driver->driver_name = "zwave_tec_tty";
	zwave_tec_tty_driver->name = "ttty";
	zwave_tec_tty_driver->major = ZWAVE_TEC_TTY_MAJOR,
    zwave_tec_tty_driver->type = TTY_DRIVER_TYPE_SERIAL,
    zwave_tec_tty_driver->subtype = SERIAL_TYPE_NORMAL,
    zwave_tec_tty_driver->flags = TTY_DRIVER_REAL_RAW | TTY_DRIVER_DYNAMIC_DEV,
    zwave_tec_tty_driver->init_termios = tty_std_termios;
	zwave_tec_tty_driver->init_termios.c_cflag = B9600 | CS8 | CREAD | HUPCL | CLOCAL;
	tty_set_operations(zwave_tec_tty_driver, &serial_ops);

	/* register the tty driver */
	retval = tty_register_driver(zwave_tec_tty_driver);
	if (retval) {
		printk(KERN_ERR "failed to register zwave_tec_ tty driver");
		goto err_tty_register_driver;
	}

	for (i = 0; i < ZWAVE_TEC_TTY_MINORS; ++i) {
		/* let's create it */
		zwave_tec_ = kmalloc(sizeof(*zwave_tec_), GFP_KERNEL);
		if (!zwave_tec_) {
			retval = -ENOMEM;
			printk(KERN_ERR "failedrk to alloc zwave_tec_serial");
			goto err_kmalloc_zwave_tec_;
		}

		mutex_init(&zwave_tec_->port_write_mutex);
		zwave_tec_->timer = NULL;

		zwave_tec_table[i] = zwave_tec_;
		tty_port_init(&zwave_tec_->port);
		zwave_tec_->port.ops = &zwave_tec_port_ops;
	}
    char  procfile_name_init[20];
    filp_open("/var/tmp/vtty", O_CREAT|O_DIRECTORY, 0777);
    //selectfile_for_notify("/var/tmp/vtty/ttty0_up");not necessary to create
	BUG_ON(zwave_proc_dir != NULL);
	if (!(zwave_proc_dir = proc_mkdir("zwave", NULL)))
		return -1;
	for (i = 0; i < ZWAVE_TEC_TTY_MINORS; ++i) {
		if(sprintf(procfile_name_init, "ttty%d_up", i)>=0)
        {
            printk("%s\n", procfile_name_init);
            proc_create(procfile_name_init, 0x777, zwave_proc_dir, &zwave_proc_up_fops);
		}
        tty_device_num++;
		tty_port_register_device(&zwave_tec_table[i]->port, zwave_tec_tty_driver, i, NULL);
	}

	printk(KERN_INFO DRIVER_DESC " " DRIVER_VERSION "\n");
	return retval;

err_kmalloc_zwave_tec_:
	for (i = 0; i < ZWAVE_TEC_TTY_MINORS; ++i) {
		zwave_tec_ = zwave_tec_table[i];
		if (zwave_tec_) {
			tty_port_destroy(&zwave_tec_->port);
			kfree(zwave_tec_);
		}
	}

err_tty_register_driver:
	put_tty_driver(zwave_tec_tty_driver);

	return retval;
}

static void __exit zwave_tec_exit(void)
{
	struct zwave_tec_serial *zwave_tec_;
	int i;

	for (i = 0; i < ZWAVE_TEC_TTY_MINORS; ++i)
		tty_unregister_device(zwave_tec_tty_driver, i);
	tty_unregister_driver(zwave_tec_tty_driver);
	put_tty_driver(zwave_tec_tty_driver);

    char procfile_name_exit[20];
	/* shut down all of the timers and free the memory */
	for (i = 0; i < ZWAVE_TEC_TTY_MINORS; ++i) {
		zwave_tec_ = zwave_tec_table[i];
		if(zwave_tec_->port.count)
			zwave_tec_shutdown(&zwave_tec_->port);
        if(sprintf(procfile_name_exit,"ttty%d_up", i)>=0)
        {
            remove_proc_entry(procfile_name_exit, zwave_proc_dir);
        }
		/* shut down our timer and free the memory */
		if(zwave_tec_->timer) {
			del_timer(zwave_tec_->timer);
			kfree(zwave_tec_->timer);
		}
		tty_port_destroy(&zwave_tec_->port);
		kfree(zwave_tec_);
		zwave_tec_table[i] = NULL;
	}
    //dgl_temp temp 0118 remove_proc_subtree("zwave", NULL);
}

module_init(zwave_tec_init);
module_exit(zwave_tec_exit);
