#ifndef MODULECHAIN_MODULEFACTORY_H
#define MODULECHAIN_MODULEFACTORY_H

namespace moduleChain {

class ModuleFactory {
public:
	static std::map<std::string, std::function<void(Chain&)>>& getModuleMap() {
		static std::map<std::string, std::function<void(Chain&)>> moduleMap;
		return moduleMap;
	}

	ModuleFactory(std::string const& _name, std::function<void(Chain&)> _func) {
		registerModule(_name, _func);
	}

	static void registerModule(std::string const& _name, std::function<void(Chain&)> _func) {
		if (getModuleMap().find(_name) != getModuleMap().end()) {
			std::cerr<<"moduleChain: overriding module register for "<<_name<<std::endl;
		}
		getModuleMap()[_name] = _func;
	}

	static void createInstance(Chain& _Chain, std::string const& _name) {
		if (getModuleMap().find(_name) == getModuleMap().end()) {
			std::cerr<<"moduleChain: Can't create instance of module, not registered: "<<_name<<std::endl;
			return;
		}
		getModuleMap().at(_name)(_Chain);
	}
};

inline void Chain::createModules(std::vector<std::string> const& _nameList) {
	for (auto const& name : _nameList) {
		ModuleFactory::createInstance(*this, name);
	}
}




}

#endif
