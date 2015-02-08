#ifndef REPRESENTATION_MODULE_H
#define REPRESENTATION_MODULE_H

#include "module.h"

namespace moduleChain {

class Representation {
protected:
	std::vector<Module*> providedByList;
	std::vector<Module*> requiredByList;
public:
	Representation(Module* _providedBy, Module* _requiredBy) {
		if (nullptr != _providedBy) {
			providedByList.push_back(_providedBy);
		}
		if (nullptr != _requiredBy) {
			requiredByList.push_back(_requiredBy);
		}
	}
	virtual void const* getInternalPtr() const = 0;
};

}
#endif

