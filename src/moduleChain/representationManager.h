#ifndef MODULECHAIN_REPRESENTATIONMANAGER_H
#define MODULECHAIN_REPRESENTATIONMANAGER_H

#include <thread>
#include "store.h"

namespace moduleChain {

class Chain;

template<typename R>
class RepresentationManager {
public:
	static std::shared_ptr<Store<R>> getRepresentationPtr(Chain* _chain) {
		static std::mutex mutex;
		static std::map<Chain*, std::weak_ptr<Store<R>>> repMap;

		std::unique_lock<std::mutex> lock(mutex);
		if (repMap.find(_chain) == repMap.end()) {
			auto store = std::make_shared<Store<R>>([&, _chain]() {
				std::unique_lock<std::mutex> lock(mutex);
				repMap.erase(repMap.find(_chain));
			});
			repMap[_chain] = store;
			return store;
		}
		return repMap.at(_chain).lock();
	}
};

}
#endif
