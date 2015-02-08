#ifndef REPRESENTATION_MODULE_H
#define REPRESENTATION_MODULE_H

#include "module.h"

namespace moduleChain {

class Representation {
public:
	std::vector<Module*> providedByList;
	std::vector<Module*> requiredByList;
	virtual void const* getInternalPtr() const = 0;
};

}
#endif

