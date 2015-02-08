#ifndef REPRESENTATION_MODULE_H
#define REPRESENTATION_MODULE_H

#include "module.h"

namespace moduleChain {

class Representation {
protected:
	std::vector<Module*> providedByList;
	std::vector<Module*> requiredByList;
public:
	virtual void const* getInternalPtr() const = 0;
};

}
#endif

