#include <stdio.h>
#include <fcntl.h>

int main(void)
{
        int device;
        char buf[3]="abc";

        device = open("/dev/bigdisk", O_RDWR);
        if (device < 0) {
        	perror("open()");
        	return 1;
        }

        if (lseek(device, 4096, SEEK_SET) < 0) {
            perror("lseek: ");
            return 1;
        }
        if (write(device, &buf, 3) < 0) {
            perror("write: ");
            return 1;
        }

        close(device);

        return 0;
}
