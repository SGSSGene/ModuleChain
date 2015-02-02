#include "moduleChain/moduleChain.h"

#include <iostream>


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
		std::cout<<"T2"<<std::endl;
	}

};
REGISTER_MODULE(T1);
REGISTER_MODULE(T2);
REGISTER_MODULE(T3);


int main() {
	moduleChain::Layer l1("test", {"T1", "T2", "T3"});

	while(true) {
		l1.run();
	}

	return 0;
}
