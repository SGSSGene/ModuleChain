#ifndef REPRESENTATION_MODULE_H
#define REPRESENTATION_MODULE_H

#include <memory>

#include "module.h"

namespace moduleChain {

class Representation {
protected:
	std::vector<Module*> providedByList;
	std::vector<Module*> requiredByList;

	std::mutex mutex;
	int provideCount;
public:
	void addProvidedBy(Module* _providedBy) {
		providedByList.push_back(_providedBy);
	}
	void addRequiredBy(Module* _requiredBy) {
		requiredByList.push_back(_requiredBy);
	}
	virtual ~Representation() {}

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
};

template <typename T>
class Store : public Representation {
private:
	T t;
public:
	T* get() {
		return &t;
	}
	T const* get() const {
		return &t;
	}

};

}
#endif

