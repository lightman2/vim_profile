#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/vfs.h>    /* or <sys/statfs.h> */
#include <sys/mount.h>

static char *sdDevName[4] = {"mmcblk0", "mmcblk1", "mmcblk2", "mmcblk3"};
#define TMP_SD_FILE        "/tmp/sd_diskname"
int detectSDcardStatus( int pos, char *sdDevFullName)
{
    char buf[256];
    char path[64];
    int i = 0, ret, j = 0;
    FILE *fd;

    memset(buf, '\0', sizeof(buf));

    for( i =0; i< sizeof(sdDevName)/sizeof(sdDevName[0]); i++)
    {
        memset(path, '\0', sizeof(path));
        sprintf(path, "/sys/block/%s/device", sdDevName[i]);
        ret = readlink(path, buf, sizeof(buf));

        if( sdDevFullName != NULL )
        {
            memset(buf, '\0', sizeof(buf));
            memset(path, '\0', sizeof(path));
            sprintf(path, "/dev/%s", sdDevName[i]);
            sprintf(buf, "fdisk -l \"%s\" | grep \"%s\" | grep -v Disk|awk  \'NR==1 {printf $1}\'> %s", path, path, TMP_SD_FILE);
/*            IPC_Client_Request(REPLY_KEY_0, buf);*/
            system(buf);
            fd = fopen(TMP_SD_FILE, "r");
            if( NULL == fd )
            {
                printf("Fail to open %s\n", TMP_SD_FILE);
                return -1;
            }
            memset(path, '\0', sizeof(path));
            fgets(path, sizeof(path), fd);
            sprintf(sdDevFullName, "%s", path);
            fclose(fd);
        }
        return 0;
    }

    return -1;
}

void main()
{
    char devname[256];
    memset(devname,0, sizeof(devname));
    detectSDcardStatus( 1, devname);
    printf("devname = %s\n", devname);
}

