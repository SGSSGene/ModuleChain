#ifndef MODULECHAIN_MODULECHAIN_H
#define MODULECHAIN_MODULECHAIN_H

#include "module.h"
#include "chain.h"
#include "representation.h"
#include "moduleFactory.h"


namespace moduleChain {

template<typename T>
class Require : public Representation {
private:
	T const& _rep;
public:
	Require()
		: _rep(getCurrentModule()->getChain()->getRepresentation<T>()) {
		getCurrentModule()->addRequire(this);
	}
	T const* operator->() const {
		return &_rep;
	}
	T const& operator*() const {
		return _rep;
	}
	virtual void const* getInternalPtr() const override {
		return &_rep;
	}

};

class ProvideBase {
};

template<typename T>
class Provide : public Representation {
private:
	T& _rep;
public:
	Provide()
		: _rep(getCurrentModule()->getChain()->getRepresentation<T>()) {
		getCurrentModule()->addProvide(this);
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
	virtual void const* getInternalPtr() const override {
		return &_rep;
	}

};


template<typename T>
using PtrToMemFunc = void (T::*)(void);

#define GET_FUNCTION(FUNC) \
class GetFunction_##FUNC { \
public: \
	template<typename T> \
	static auto _get(int) -> decltype(&T::FUNC){ \
		return &T::FUNC; \
	} \
\
	template<typename T> \
	static PtrToMemFunc<T> _get(long) { \
		return nullptr; \
	} \
\
	template<typename T> \
	static PtrToMemFunc<T> get() { \
		return _get<T>(int(0)); \
	} \
};

GET_FUNCTION(execute)
GET_FUNCTION(init)

#define REGISTER_MODULE(CLASS) \
namespace { \
	moduleChain::ModuleFactory m##CLASS(#CLASS, [](moduleChain::Chain& chain) { \
		chain.addModule<CLASS>(#CLASS, moduleChain::GetFunction_execute::get<CLASS>(), \
		                               moduleChain::GetFunction_init::get<CLASS>()); \
	}); \
}

}

#endif
