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
	Edge* e = first;
	do {
		e->left = this;
		if (e->twin != NULL) e->twin->right = this;
		e=e->next;
	} while (e != first);
}

Face::Face(vec3 a, vec3 b, vec3 c) {
	first = new Edge(a, b);
	first->left = this;
	first->attach(new Edge(b, c));
	first->next->attach(new Edge(c, a));
	first->next->next->attach(first);
}

vec3 Face::midpoint() const {
	return (1.0/3) * (first->head->position + first->next->head->position + first->next->next->head->position);
}

void Face::split() {
	Edge *e = first;
	do {
		Edge *next = e->next;
		e->split();
		e = next;
	} while (e != first);
	first = first->next;
}