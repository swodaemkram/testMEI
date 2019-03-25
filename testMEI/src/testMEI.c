/*
 ============================================================================
 Name        : testMEI.c
 Author      : Mark Meadows
 Version     :blah
 Copyright   : copyright 2018
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "do_crc.h"


int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |=  CS7;         /* 7-bit characters  pipes = yes ampersands and Tildes= no */
    tty.c_cflag |=  PARENB;     /* even parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */

    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}


int main()

{
    char *portname = "/dev/ttyUSB0";
    int fd;
    //int wlen;

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate 9600, 7 bits, even parity, 1 stop bit */
    set_interface_attribs(fd, B9600);
    //set_mincount(fd, 0);                /* set to pure timed read */

/*
==============================================================================================
Finished with Comm port setup
==============================================================================================
 */


    sleep(1);


    char buff[]="\x02\x08\x60\x00\x00\x05\x03";

    //char buff[] = "\x02\x08\x60\x00\x00\x05\x03\x6d";
    unsigned int crc_val;    //setup for return from crc

    int buff_len = sizeof(buff);
    crc_val =  do_crc(buff,buff_len);// Lets do CRC
    printf("\nThis is the value that will be added to the packet %02x\n",crc_val);
    printf("\n%02x%02x%02x%02x%02x%02x%02x%02x\n",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],crc_val);



    exit(0);
    write(fd,buff,sizeof(buff));

    tcdrain(fd);    /* delay for output */


    /* simple noncanonical input */
    do {
        unsigned char buf[80];
        int rdlen;

        rdlen = read(fd, buf, sizeof(buf) - 1);
        if (rdlen > 0) {

//#ifdef DISPLAY_STRING
            buf[rdlen] = 0;
            printf("Read %d: \"%s\"\n", rdlen, buf);
//#else /* display hex */
            unsigned char   *p;
            printf("Read %d:", rdlen);
            for (p = buf; rdlen-- > 0; p++)
                printf(" 0x%x", *p);
            printf("\n");
//#endif
        } else if (rdlen < 0) {
           // printf("Error from read: %d: %s\n", rdlen, strerror(errno));
        } else {  /* rdlen == 0 */
            printf("Timeout from read\n");
        }
        /* repeat read to get full message */
    } while (1);

}




void set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 10;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0){


        printf("Error : \n");
}

}
