#ifndef MODUDLECHAIN_STORE_H
#define MODUDLECHAIN_STORE_H

#include "representation.h"

namespace moduleChain {

template <typename T>
class Store : public Representation {
private:
	T t;
	std::function<void()> cleanupFunc;
public:
	Store(std::function<void()> _cleanupFunc)
		: cleanupFunc(_cleanupFunc) {
	}
	virtual ~Store() override {
		cleanupFunc();
	}
	T* get() {
		return &t;
	}
	T const* get() const {
		return &t;
	}
	virtual std::string getName() const {
		return typeid(t).name();
	}


};


}

#endif
