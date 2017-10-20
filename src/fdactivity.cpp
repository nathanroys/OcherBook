#include "fdactivity.h"

#include <sys/ioctl.h>
#include <termios.h>

// TODO:  handle !isatty(stdin)

char FdActivity::getKey()
{
    int key;
    struct termios oldTermios, newTermios;

    tcgetattr(0, &oldTermios);
    newTermios = oldTermios;
    newTermios.c_lflag &= ~(ICANON | ECHO);
    newTermios.c_cc[VTIME] = 0;
    newTermios.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &newTermios);

    key = getc(stdin);

    tcsetattr(0, TCSANOW, &oldTermios);

    return (char)key;
}
