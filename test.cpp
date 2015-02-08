#include "moduleChain/moduleChain.h"

#include <iostream>
#include <unistd.h>


class R1 {
public:
	int x;
};
class R2 {
public:
	int x;
};

class T1 {
	moduleChain::Require<R1> r1;
	moduleChain::Provide<R2> r2;

public:
	void init() {
		std::cout<<"init, only called once"<<std::endl;
	}
	void execute() {
		std::cout<<"T1"<<std::endl;
	}
};
class T3 {
	moduleChain::Provide<R1> r1;
public:
	void execute() {
		std::cout<<"T3"<<std::endl;
	}

};
class T2 {
	moduleChain::Require<R2> r2;
public:
	void execute() {
		std::cout<<"start T2"<<std::endl;
		sleep(1);
		std::cout<<"end   T2"<<std::endl;
	}
};
class T4 {
	moduleChain::Require<R2> r2;
public:
	void execute() {
		std::cout<<"start T4"<<std::endl;
		sleep(1);
		std::cout<<"end   T4"<<std::endl;
	}
};

REGISTER_MODULE(T1);
REGISTER_MODULE(T2);
REGISTER_MODULE(T3);
REGISTER_MODULE(T4);


int main() {
	moduleChain::Chain l1("test", {"T1", "T2", "T3", "T4"}, 2);

	for (int i(0); i<10; ++i) {
		std::cout<<std::endl<<"loop: "<<i<<std::endl;
		l1.run();
	}

	return 0;
}
