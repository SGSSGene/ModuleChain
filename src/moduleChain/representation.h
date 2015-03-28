#ifndef REPRESENTATION_MODULE_H
#define REPRESENTATION_MODULE_H

#include <memory>
#include <typeinfo>

#include "module.h"

namespace moduleChain {

class Representation {
protected:
	std::vector<Module*> providedByList;
	std::vector<Module*> requiredByList;

	std::mutex mutex;
	int provideCount;
public:
	virtual ~Representation() {}

	void addProvidedBy(Module* _providedBy) {
		providedByList.push_back(_providedBy);
	}
	void addRequiredBy(Module* _requiredBy) {
		requiredByList.push_back(_requiredBy);
	}

	void resetProvideCount() {
		std::unique_lock<std::mutex> lock(mutex);
		provideCount = 0;
	}
	void incProvideCount() {
		std::unique_lock<std::mutex> lock(mutex);
		++provideCount;
		if (provideCount == int(providedByList.size())) {
			for (auto& m : requiredByList) {
				m->incRequirementCount();
			}
		}
	}
	int getLengthOfProviderList() const {
		return int(providedByList.size());
	}
	bool hasProvideCount() const {
		return provideCount == int(providedByList.size());
	}
	virtual std::string getName() const = 0;
};

}
#endif

