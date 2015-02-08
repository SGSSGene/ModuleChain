all:
	clang++ -std=c++11 -pthread -isystem../threadpool/include -Iinclude -Isrc/ src/moduleChain/module.cpp src/tests/test2.cpp src/tests/test.cpp -ggdb -Wall -Wextra -fmessage-length=0 -o test
#	clang++ -std=c++11 -isystem../threadpool/include -Iinclude -pthread src/test/main.cpp -o test
