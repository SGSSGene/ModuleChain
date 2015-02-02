#ifndef MODULECHAIN_MODULE_H
#define MODULECHAIN_MODULE_H

#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace moduleChain {

class Layer;
class Module {
private:
	std::string              name;      //!< Name of this Module
	Layer*                   layer;     //!< Layer this Module belongs to
	std::vector<void const*> require;   //!< Pointer to all representations that are required
	std::vector<void*>       provide;   //!< Pointer to all representations that are provided
	std::vector<Layer*>      subLayers; //!< Pointer to all layers that are run inside this Module

	std::function<void()> executeFunc;  //!< Execute function of this module

public:
	Module(std::string const& _name, Layer* _layer)
		: name(_name)
		, layer(_layer)
		, executeFunc(nullptr) {
	}

	std::string const& getName() const { return name; }
	Layer* getLayer() const { return layer; }
	std::vector<void const*> const& getRequires() const { return require; }
	std::vector<void*>       const& getProvides() const { return provide; }
	std::vector<Layer*> const&      getSubLayers() const { return subLayers; }

	void addRequire(void const* _r) {
		require.push_back(_r);
	}
	void addProvide(void* _r) {
		provide.push_back(_r);
	}
	void addSubLayer(Layer* _layer) {
		subLayers.push_back(_layer);
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
