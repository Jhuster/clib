1. How to build

$ gcc -o broadcast.out main.c broadcaster.c

2. How to test

$ ./broadcast.out -h

./broadcast.out [-t work mode [send/recv]] [-p port] [-h help] 
example:
	./broadcast.out -t send -p 8000

open 2 shell command window

$ ./broadcast.out -t send
$ ./broadcast.out -t recv

