#!/bin/sh

[ -z "$OCHER_IP" ] && OCHER_IP=192.168.1.69

(
    sleep 1
    echo "root"
    echo "cd /usr/local"
    echo "mkdir ocher"
    echo "cd ocher"
    echo "lcd build"
    echo "put ocher"
    echo "quit"
) | ftp $OCHER_IP

(
    sleep 1
    echo "root"
    sleep 1
    echo "cd /usr/local/ocher"
    echo "chmod +x ocher"
) | telnet $OCHER_IP
#    echo "killall nickel ocher"
#    echo "nohup ./ocher -vv &"
