#ifndef MODULECHAIN_MODULE_H
#define MODULECHAIN_MODULE_H

#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace moduleChain {

class Chain;
class Representation;

class Module {
private:
	std::string                  name;      //!< Name of this Module
	Chain*                       chain;     //!< Chain this Module belongs to
	std::vector<Representation*> require;   //!< Pointer to all representations that are required
	std::vector<Representation*> provide;   //!< Pointer to all representations that are provided
	std::vector<Chain*>          subChains; //!< Pointer to all Chains that are run inside this Module

	std::function<void()> executeFunc;  //!< Execute function of this module

	mutable std::mutex mutex;
	int requirementCount;
public:
	Module(std::string const& _name, Chain* _chain)
		: name(_name)
		, chain(_chain)
		, executeFunc(nullptr) {
	}

	std::string const& getName() const { return name; }
	Chain* getChain() const { return chain; }
	std::vector<Representation*> const& getRequires() const { return require; }
	std::vector<Representation*> const& getProvides() const { return provide; }
	std::vector<Chain*> const&      getSubChains() const { return subChains; }

	void addRequire(Representation* _r) {
		require.push_back(_r);
	}
	void addProvide(Representation* _r) {
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
	void operator()();

	void resetRequirementCount() {
		std::unique_lock<std::mutex> lock(mutex);
		requirementCount = 0;
	}
	void incRequirementCount() {
		std::unique_lock<std::mutex> lock(mutex);
		++requirementCount;
	}
	bool hasRequirementCount() const {
		std::unique_lock<std::mutex> lock(mutex);
		return requirementCount == int(require.size());
	}

};

}

#endif
