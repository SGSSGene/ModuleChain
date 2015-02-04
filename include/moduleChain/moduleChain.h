#ifndef MODULECHAIN_MODULECHAIN_H
#define MODULECHAIN_MODULECHAIN_H

#include "module.h"
#include "chain.h"
#include "moduleFactory.h"


namespace moduleChain {

template<typename T>
class Require {
private:
	T const& _rep;
public:
	Require()
		: _rep(getCurrentModule()->getChain()->getRepresentation<T>()) {
		getCurrentModule()->addRequire(&_rep);
	}
	T const* operator->() const {
		return &_rep;
	}
	T const& operator*() const {
		return _rep;
	}
};

template<typename T>
class Provide {
private:
	T& _rep;
public:
	Provide()
		: _rep(getCurrentModule()->getChain()->getRepresentation<T>()) {
		getCurrentModule()->addProvide(&_rep);
	}
	T const& operator *() const {
		return _rep;
	}
	T& operator *() {
		return _rep;
	}

	T const* operator->() const {
		return &_rep;
	}
	T* operator->() {
		return &_rep;
	}
};


#define REGISTER_MODULE(CLASS) \
namespace { \
	moduleChain::ModuleFactory m##CLASS(#CLASS, [](moduleChain::Chain& chain) { \
		chain.addModule<CLASS>(#CLASS, &CLASS::execute); \
	}); \
}

}

#endif
