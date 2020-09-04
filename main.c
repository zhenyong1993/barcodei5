#include <linux/input.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "barcode.h"
#include "boxconfig.h"

#define SCANNER_DEV "/dev/input/event11"

int fd;
int read_nu;
char trans(unsigned short data);
bool shiftswitch=false;

int getBarcodeIndex()
{
    char          name[64];           /* RATS: Use ok, but could be better */
    char          buf[256] = { 0, };  /* RATS: Use ok */
    int           fd = 0;
    int           i;
    for (i = 0; i < 32; i++)
    {
        sprintf(name, "/dev/input/event%d", i);
        if ((fd = open(name, O_RDONLY, 0)) >= 0)
        {
            ioctl(fd, EVIOCGNAME(sizeof(buf)), buf);
            printf("/dev/input/event%d :name: %s\n", i, buf);
            close(fd);
            if(strstr(buf,"HID") != NULL) return i;
        }
    }
        return -1;

}

int main(int argc, char *argv[])
{
    struct input_event buff;
    barcode* bc = new barcode;
    char a[4096] = {};

    int i=-1;
    char name[32];
    while(1)
    {
        i=getBarcodeIndex();
        if(-1 != i)
        {
            printf("HID found at index %d\n",i);
            sprintf(name, "/dev/input/event%d", i);
            break;
        }else
        {
            sleep(1);
            printf("HID not found, waiting 1s\n");
        }
    }

    while(1)
    {
        fd = open(name, O_RDONLY);//打开usb扫描枪设备
        if (fd < 0)
        { 
                perror("can not open device usbscanner!");
                close(fd);
                sleep(1);
        }else
        {
            break;
        }
    }
    i = 0;
    printf("--fd:%d--\n",fd);
    int res = 0;
    while(1)
    {
//                memset(&buff, 0, sizeof(buff));
        while(res == 0)
        {
            res = read(fd,&buff,sizeof(struct input_event));
        }
        if(res < 0)
        {
            printf("broken barcode, waiting 1s\n");
            close(fd); sleep(1);
            fd = open(name, O_RDONLY);//打开usb扫描枪设备
            res = 0;
            continue;
        }
//if(buff.type != 4) printf("type is %d\n", buff.type);
        printf("res is %d\n", res);
        printf("type:%d code:%d value:%d\n",buff.type,buff.code,buff.value); 
        res = 0; //reset res
        if(buff.type != 1) continue;
//handle case sensitive
        if(buff.code == 42)//shift
        {
            if(buff.value==1){shiftswitch=true;}
            else if(buff.value==0){shiftswitch=false;}
            continue;
        }
//handle case sensitive
        if(buff.code != 28 && buff.value == 1)//CRLF=28
        {
            a[i] = trans(buff.code);
            if('?'==a[i]) continue;
            i++;
            printf("----- %c %d-------\n", a[i-1],buff.code);
        }
        else if(buff.code==28 && buff.value == 1)
        {
            printf("========= str is %s ========\n", a);
            bc -> onGetStr(a);
            i=0;
            memset(a, 0, 4096);
        }
    }
    close(fd);
    return 1;
}

char trans(unsigned short data)
{
    if(false==shiftswitch)
    {
        switch(data)
        {
            case 2 : return '1';
            case 3 : return '2';
            case 4 : return '3';
            case 5 : return '4';
            case 6 : return '5';
            case 7 : return '6';
            case 8 : return '7';
            case 9 : return '8';
            case 10: return '9';
            case 11: return '0';
            case 12: return '-';
            case 13: return '=';
            case 14: return '\b';
            case 15: return '\t';
            case 16: return 'q';
            case 17: return 'w';
            case 18: return 'e';
            case 19: return 'r';
            case 20: return 't';
            case 21: return 'y';
            case 22: return 'u';
            case 23: return 'i';
            case 24: return 'o';
            case 25: return 'p';
            case 26: return '[';
            case 27: return ']';
            case 30: return 'a';
            case 31: return 's';
            case 32: return 'd';
            case 33: return 'f';
            case 34: return 'g';
            case 35: return 'h';
            case 36: return 'j';
            case 37: return 'k';
            case 38: return 'l';
            case 39: return ';';
            case 40: return '\'';
            case 44: return 'Z';
            case 45: return 'x';
            case 46: return 'c';
            case 47: return 'v';
            case 48: return 'b';
            case 49: return 'n';
            case 50: return 'm';
            case 51: return ',';
            case 52: return '.';
            case 53: return '/';
            case 57: return ' ';
            case 42: return '?';//shift key
            default: return '?';
        }
    }
    else
    {
        switch(data)
        {
            case 2 : return '!';
            case 3 : return '@';
            case 4 : return '#';
            case 5 : return '$';
            case 6 : return '%';
            case 7 : return '^';
            case 8 : return '&';
            case 9 : return '*';
            case 10: return '(';
            case 11: return ')';
            case 12: return '_';
            case 13: return '+';
            case 14: return '\b';
            case 15: return '\t';
            case 16: return 'Q';
            case 17: return 'W';
            case 18: return 'E';
            case 19: return 'R';
            case 20: return 'T';
            case 21: return 'Y';
            case 22: return 'U';
            case 23: return 'I';
            case 24: return 'O';
            case 25: return 'P';
            case 26: return '[';
            case 27: return ']';
            case 30: return 'A';
            case 31: return 'S';
            case 32: return 'D';
            case 33: return 'F';
            case 34: return 'G';
            case 35: return 'H';
            case 36: return 'J';
            case 37: return 'K';
            case 38: return 'L';
            case 39: return ':';
            case 40: return '\"';
            case 44: return 'Z';
            case 45: return 'X';
            case 46: return 'C';
            case 47: return 'V';
            case 48: return 'B';
            case 49: return 'N';
            case 50: return 'M';
            case 51: return '<';
            case 52: return '>';
            case 53: return '?';
            case 57: return ' ';
            case 42: return '?';//shift key
            default: return '?';
        }

    }
}
