#include "module.h"

#include "representation.h"

namespace moduleChain {

void Module::operator()() {
	executeFunc();
	for (auto& r : provide) {
		r->incProvideCount();
	}
}

}
