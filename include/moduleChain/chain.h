#ifndef MODULECHAIN_CHAIN_H
#define MODULECHAIN_CHAIN_H

#include <iostream>
#include <memory>
#include <algorithm>

namespace moduleChain {

inline std::mutex& getCurrentModuleMutex() {
	static std::mutex m;
	return m;
}
inline std::shared_ptr<Module>& getCurrentModule() {
	static std::shared_ptr<Module> m(nullptr);
	return m;
}

class Chain {
private:
	std::string name;
	std::vector<std::shared_ptr<Module>> modules;
	std::vector<Module*> executionOrder;
	Module* parentModule;
public:
	Chain(std::string const& _name, std::vector<std::string> const& _moduleList)
		: name(_name)
		, parentModule(getCurrentModule().get()) {
		if (parentModule != nullptr) {
			parentModule->addSubChain(this);
		}
		createModules(_moduleList);
	}

	template<typename T>
	std::mutex& getMutex() {
		static std::mutex mutex;
		return mutex;
	}
	template<typename T>
	std::map<Chain*, std::shared_ptr<T>>& getRepresentations() {
		static std::map<Chain*, std::shared_ptr<T>> repMap;
		return repMap;
	}

	template<typename T>
	std::shared_ptr<T> getRepresentationPtr() {
		std::unique_lock<std::mutex> lock(getMutex<T>());
		auto& repMap = getRepresentations<T>();
		if (repMap.find(this) == repMap.end()) {
			if (parentModule != nullptr) {
				auto ptr = parentModule->getChain()->getRepresentationPtr<T>();
				repMap[this] = ptr;
			} else {
				repMap[this] = std::make_shared<T>();
			}
		}
		return repMap.at(this);
	}

	template<typename T>
	T& getRepresentation() {
		return *getRepresentationPtr<T>();
	}

	/**
	 * adds a module to the execution list of this Chain
	 */
	template<typename T>
	void addModule(std::string const& _name, void (T::*_execute)(), void (T::*_init)()) {
		getCurrentModuleMutex().lock();
		std::shared_ptr<Module> oldModule = getCurrentModule();
		std::shared_ptr<Module> m = std::make_shared<Module>(_name, this);
		getCurrentModule() = m;
		std::shared_ptr<T> t = std::make_shared<T>();
		getCurrentModule() = oldModule;
		getCurrentModuleMutex().unlock();


		auto executor = [t, _execute, _init]() {
			static bool firstRun{true};
			if (firstRun) {
				if (_init) {
					(t.get()->*_init)();
				}
				firstRun = false;
			}
			if (_execute) {
				(t.get()->*_execute)();
			}
		};
		m->setExecutors(executor);
		modules.push_back(m);
	}
	void computeExecutionList() {
		executionOrder.clear();
		for (auto& m : modules) {
			for (auto& l : m->getSubChains()) {
				l->computeExecutionList();
			}
		}
		std::vector<Module*> moduleVertexes;
		std::vector<std::pair<Module*, Module*>> moduleEdges;

		for (auto m1 : modules) {
			for (auto r1 : m1->getProvides()) {
				for (auto m2 : modules) {
					for (auto r2 : m2->getRequires()) {
						if (r1 == r2) {
							moduleEdges.push_back({m1.get(),  m2.get()});
						}
					}
				}
			}
			moduleVertexes.push_back(m1.get());
		}

		while(!moduleVertexes.empty()) {
			for (auto& m : moduleVertexes) {
				int ct(0);
				for (auto& p : moduleEdges) {
					if (p.second == m) {
						++ct;
						break;
					}
				}
				if (ct == 0) {
					executionOrder.push_back(m);
					moduleEdges.erase(std::remove_if(moduleEdges.begin(), moduleEdges.end(), [m](std::pair<Module*, Module*> _p) {
						return m == _p.first;
					}), moduleEdges.end());
					m = moduleVertexes.back();
					moduleVertexes.pop_back();
					break;
				}
			}
		}
	}
	void runImpl() {
		for (auto& m : executionOrder) {
			(*m)();
			for (auto& l: m->getSubChains()) {
				l->runImpl();
			}
		}
	}
	void run() {
		computeExecutionList();
		runImpl();

	}

	void createModules(std::vector<std::string> const& _name);
};


}

#endif

