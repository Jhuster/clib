broadcast
----------

a broadcast demo in Linux

Building
----------

    $ gcc -o broadcast.out main.c broadcaster.c

Usage
----------

    $ ./broadcast.out [-t work mode [send/recv]] [-p port] [-h help]     
    example:
        ./broadcast.out -t send -p 8000

    Pls open 2 shell command window

    $ ./broadcast.out -t send
    $ ./broadcast.out -t recv

Contact
----------
Email：lujun.hust@gmail.com
