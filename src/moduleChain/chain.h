#ifndef MODULECHAIN_CHAIN_H
#define MODULECHAIN_CHAIN_H

#include <algorithm>
#include <iostream>
#include <list>
#include <memory>
#include <ostream>

#include "threadPool/threadPool.h"

#include "representation.h"

namespace moduleChain {

inline std::mutex& getCurrentModuleMutex() {
	static std::mutex m;
	return m;
}
inline Module*& getCurrentModule() {
	static Module* m{nullptr};
	return m;
}

class Chain {
private:
	using ModuleUPtrList = std::vector<std::unique_ptr<Module>>;
	using ModulePtrList  = std::vector<Module*>;
	using RepUPtrList    = std::vector<std::unique_ptr<Representation>>;

	std::string    name;
	ModuleUPtrList modules;
	Module*        parentModule;
	RepUPtrList    repList;

	threadPool::ThreadPool<Module*> threadPool;
public:
	Chain(std::string const& _name, std::vector<std::string> const& _moduleList, int _threadCt=1)
		: name(_name)
		, parentModule(getCurrentModule()) {
		if (parentModule != nullptr) {
			parentModule->addSubChain(this);
		}
		createModules(_moduleList);
		threadPool.spawnThread([](Module* m) {
			(*m)();
		}, _threadCt);
	}

	template<typename T>
	std::mutex& getMutex() {
		static std::mutex mutex;
		return mutex;
	}
	template<typename T>
	std::map<Chain*, Store<T>*>& getRepresentations() {
		static std::map<Chain*, Store<T>*> repMap;
		return repMap;
	}

	template<typename T>
	Store<T>* getRepresentationPtr() {
		std::unique_lock<std::mutex> lock(getMutex<T>());
		auto& repMap = getRepresentations<T>();
		if (repMap.find(this) == repMap.end()) {
			if (parentModule != nullptr) {
				auto ptr = parentModule->getChain()->getRepresentationPtr<T>();
				repMap[this] = ptr;
			} else {
				Store<T>* store = new Store<T>;
				repMap[this] = store;
				repList.push_back(std::move(std::unique_ptr<Representation>(store)));
			}
		}
		return repMap.at(this);
	}

	template<typename T>
	Store<T>& getRepresentation() {
		return *getRepresentationPtr<T>();
	}

	/**
	 * adds a module to the execution list of this Chain
	 */
	template<typename T>
	void addModule(std::string const& _name, void (T::*_execute)(), void (T::*_init)()) {
		getCurrentModuleMutex().lock();
		Module* oldModule = getCurrentModule();
		std::unique_ptr<Module> m {new Module(_name, this)};
		getCurrentModule() = m.get();
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
		modules.push_back(std::move(m));
	}

	void addModuleToExecutionList(Module* m) {
		threadPool.queue(m);
	}

	void run() {
		for (auto& m : modules) {
			m->resetRequirementCount();
		}
		for (auto& r : repList) {
			r->resetProvideCount();
		}
		threadPool.wait();
	}

	void createModules(std::vector<std::string> const& _name);

	void generateDotFile(std::ostream& out) {

		out<<"digraph {"<<std::endl;
		out<<"graph [rankdir=\"TB\"]"<<std::endl<<std::endl;

		out<<"subgraph cluster"<<name<<" {"<<std::endl;
		out<<"label=\""<<name<<"\";"<<std::endl<<std::endl;

		// List all Modules
		for (auto& m : modules) {
			out<<name<<m->getName()<<" [label=\""<<m->getName()<<"\", shap=box, style=filled, fillcolor=\"#CCCCCC\"]"<<std::endl;
		}
		// List all Representations
		for (auto& r : repList) {
			out<<name<<r->getName()<<" [label=\""<<r->getName()<<"\"]"<<std::endl;
		}

		// List all requires and provides to modules
		for (auto& m : modules) {
			for (auto const& r : m->getProvides()) {
				out<<name<<m->getName()<<" -> "<<name<<r->getName()<<std::endl;
			}
			for (auto const& r : m->getRequires()) {
				out<<name<<r->getName()<<" -> "<<name<<m->getName()<<std::endl;
			}

		}

		out<<"}"<<std::endl<<std::endl;



		out<<"}"<<std::endl;
	}
};


}

#endif

