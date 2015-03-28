#include "moduleChain/moduleChain.h"

#include <iostream>
#include <unistd.h>

class R1 {
public:
	static int ct;
	R1()  { ++ct; }
	~R1() { --ct; }
	int x;
};
int R1::ct {0};
class R2 {
public:
	static int ct;
	R2()  { ++ct; }
	~R2() { --ct; }

	int x;
};
int R2::ct {0};

class T1 {
	moduleChain::Require<R1> r1;
	moduleChain::Provide<R2> r2;

public:
	static int ct;
	T1()  { ++ct; }
	~T1() { --ct; }

	void init() {
		std::cout<<"init, only called once"<<std::endl;
	}
	void execute() {
		std::cout<<"T1: start"<<std::endl;
		sleep(1);
		std::cout<<"T1: end"<<std::endl;
	}
};
int T1::ct {0};
class T3 {
	moduleChain::Provide<R1> r1;
public:
	void execute() {
		std::cout<<"T3: start"<<std::endl;
		sleep(1);
		std::cout<<"T3: end"<<std::endl;
	}

};
class T2 {
	moduleChain::Require<R2> r2;
public:
	static int ct;
	T2()  { ++ct; }
	~T2() { --ct; }

	void execute() {
		std::cout<<"T2: start"<<std::endl;
		sleep(1);
		std::cout<<"T2: end"<<std::endl;
	}
};
int T2::ct {0};
class T4 {
	moduleChain::Require<R1> r1;
	moduleChain::Provide<R2> r2;
public:
	void execute() {
		std::cout<<"T4: start"<<std::endl;
		sleep(1);
		std::cout<<"T4: end"<<std::endl;
	}
};

REGISTER_MODULE(T1);
REGISTER_MODULE(T2);
REGISTER_MODULE(T3);
REGISTER_MODULE(T4);


void runChain() {
	moduleChain::Chain chain("testChain", {"T1", "T2", "T3", "T4"}, 2);

	for (int i(0); i<1; ++i) {
		std::cout<<std::endl<<"loop: "<<i<<std::endl;
		chain.run();
	}
}
void printData() {
	std::cout<<"R1: "<<R1::ct<<std::endl;
	std::cout<<"R2: "<<R2::ct<<std::endl;
	std::cout<<"T1: "<<T1::ct<<std::endl;
	std::cout<<"T2: "<<T2::ct<<std::endl;
}
void test1() {
	printData();
	runChain();
	printData();
	runChain();
	printData();
}
void test2() {
	try {
		moduleChain::Chain chain("testChain", {"T1"}, 2);
		chain.run();
	} catch (std::exception& e) {
		std::cout<<"Faild running chain. "<<e.what()<<std::endl;
	}
}
int main() {
	test2();
	//test1();

	return 0;
}
