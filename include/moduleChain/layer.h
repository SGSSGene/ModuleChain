#ifndef MODULECHAIN_LAYER_H
#define MODULECHAIN_LAYER_H

#include <iostream>
#include <algorithm>

namespace moduleChain {

inline std::mutex& getCurrentModuleMutex() {
	static std::mutex m;
	return m;
}
inline Module*& getCurrentModule() {
	static Module* m(nullptr);
	return m;
}

class Layer {
//private:
public:
	std::string name;
	std::vector<Module*> modules;
	std::vector<Module*> executionOrder;
	Module* parentModule;
public:
	Layer(std::string const& _name, std::vector<std::string> const& _moduleList)
		: name(_name)
		, parentModule(getCurrentModule()) {
		if (parentModule != nullptr) {
			parentModule->addSubLayer(this);
		}
		createModules(_moduleList);
	}

	template<typename T>
	std::mutex& getMutex() {
		static std::mutex mutex;
		return mutex;
	}
	template<typename T>
	std::map<Layer*, T*>& getRepresentations() {
		static std::map<Layer*, T*> repMap;
		return repMap;
	}

	template<typename T>
	T* getRepresentationPtr() {
		std::unique_lock<std::mutex> lock(getMutex<T>());
		auto& repMap = getRepresentations<T>();
		if (repMap.find(this) == repMap.end()) {
			if (parentModule != nullptr) {
				auto ptr = parentModule->getLayer()->getRepresentationPtr<T>();
				repMap[this] = ptr;
			} else {
				repMap[this] = new T();
			}
		}
		return repMap.at(this);
	}

	template<typename T>
	T& getRepresentation() {
		return *getRepresentationPtr<T>();
	}

	/**
	 * adds a module to the execution list of this layer
	 */
	template<typename T>
	void addModule(std::string const& _name, void (T::*_execute)()) {
		getCurrentModuleMutex().lock();
		Module* m = new Module{_name, this};
		getCurrentModule() = m;
		T* t = new T();
		getCurrentModule() = nullptr;
		getCurrentModuleMutex().unlock();

		m->setExecutors(std::bind(_execute, t));
		modules.push_back(m);
	}
	void computeExecutionList() {
		executionOrder.clear();
		for (auto& m : modules) {
			for (auto& l : m->getSubLayers()) {
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
							moduleEdges.push_back({m1,  m2});
						}
					}
				}
			}
			moduleVertexes.push_back(m1);
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
					std::cout<<"First run "<<m->getName()<<std::endl;
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
			for (auto& l: m->getSubLayers()) {
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

