all:
	clang++ -std=c++11 -Iinclude test2.cpp test.cpp -ggdb -Wall -Wextra -fmessage-length=0 -o test
#	clang++ -std=c++11 -isystem../threadpool/include -Iinclude -pthread src/test/main.cpp -o test
