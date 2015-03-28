#include "module.h"

#include "chain.h"
#include "representation.h"

namespace moduleChain {

void Module::operator()() {
	executeFunc();
	for (auto& r : provide) {
		r->incProvideCount();
	}
}
void Module::resetRequirementCount() {
	std::unique_lock<std::mutex> lock(mutex);
	requirementCount = 0;
	if (requirementCount == int(require.size())) {
		chain->addModuleToExecutionList(this);
	}
}

void Module::incRequirementCount() {
	std::unique_lock<std::mutex> lock(mutex);
	++requirementCount;
	if (requirementCount == int(require.size())) {
		chain->addModuleToExecutionList(this);
	}
}
bool Module::hasRequirementCountMax() {
	return requirementCount == int(require.size());
}

}
