#include "geometry.h"
#include "utils.h"
using namespace glm;

bool operator<(const Face &lhs, const Face &rhs) {
	return lhs.midpoint() < rhs.midpoint() ||
		(lhs.midpoint() == rhs.midpoint() && (*(lhs.first) < *(rhs.first) ||
			(*(lhs.first) == *(rhs.first) && (*(lhs.first->next) < *(rhs.first->next) ||
				(*(lhs.first->next) == *(rhs.first->next) && *(lhs.first->next->next) < *(rhs.first->next->next))))));
}
bool operator==(const Face &lhs, const Face &rhs) {
	return *(lhs.first) == (*rhs.first) &&
		*(lhs.first->next) == (*rhs.first->next);
}
bool operator!=(const Face &lhs, const Face &rhs) {
	return !(lhs == rhs);
}

Face::Face(Edge* first) {
	this->first = first;
}

vec3 Face::midpoint() const {
	return (1.0/3) * (first->head->position + first->next->head->position + first->next->next->head->position);
}