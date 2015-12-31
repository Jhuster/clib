multicast library
----------

an UDP multicast library & demo on Linux Platform

Building
----------

    $ gcc -o test.out test.c mc_recver.c mc_sender.c

Usage
----------

    $ ./test.out [-c as client] [-s as server] [-h help] 
    example:
        $ ./test.out -s

    Pls open 2 shell command window

    $ ./test.out -s
    $ ./test.out -c

Contact
----------
Email：lujun.hust@gmail.com
