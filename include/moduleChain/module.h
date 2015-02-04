#ifndef MODULECHAIN_MODULE_H
#define MODULECHAIN_MODULE_H

#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace moduleChain {

class Chain;

class Module {
private:
	std::string              name;      //!< Name of this Module
	Chain*                   chain;     //!< Chain this Module belongs to
	std::vector<void const*> require;   //!< Pointer to all representations that are required
	std::vector<void*>       provide;   //!< Pointer to all representations that are provided
	std::vector<Chain*>      subChains; //!< Pointer to all Chains that are run inside this Module

	std::function<void()> executeFunc;  //!< Execute function of this module

public:
	Module(std::string const& _name, Chain* _chain)
		: name(_name)
		, chain(_chain)
		, executeFunc(nullptr) {
	}

	std::string const& getName() const { return name; }
	Chain* getChain() const { return chain; }
	std::vector<void const*> const& getRequires() const { return require; }
	std::vector<void*>       const& getProvides() const { return provide; }
	std::vector<Chain*> const&      getSubChains() const { return subChains; }

	void addRequire(void const* _r) {
		require.push_back(_r);
	}
	void addProvide(void* _r) {
		provide.push_back(_r);
	}
	void addSubChain(Chain* _chain) {
		subChains.push_back(_chain);
	}



	// Function to set the executor of this Module
	void setExecutors(std::function<void()> _executeFunc) {
		executeFunc = _executeFunc;
	}
	// calls the executeFunction
	void operator()() {
		executeFunc();
	}

};

}

#endif
