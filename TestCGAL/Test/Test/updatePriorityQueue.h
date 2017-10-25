#ifndef __UPDATE_PRIORITY_QUEUE__
#define __UPDATE_PRIORITY_QUEUE__
#include <queue>
#include "Common_Djikstra.h"

template<class _Ty,class _Container,class _Pr>
class UpdatablePriorityQueue : public std::priority_queue<_Ty,_Container,_Pr> {

public:
	void updatePriority(DjiElement element,double newPriority) {
		std::vector<int>::iterator it;
		bool ifFound = false;

		for (DjiElement &lElement : this->c) {
			if (lElement.getPoint() == element.getPoint()) {
				ifFound = true;
				lElement.updateDistance(newPriority);
			}
		}

		if (!ifFound) {
			element.updateDistance(newPriority);
			this->push(element);
		}

		std::make_heap(c.begin(), c.end(),this->comp);
	}
};
#endif




