#pragma once
#include "object.h"
#include <algorithm>
#include "utils.h"

bool in(Box range, vec3 point);
bool in(Box range, Box box);

bool intersecting(Box a, Box b);

template<typename T>
function<bool(const T&, const T&)> less_than(int axis);
function<bool(const Box&, const Box&)> less_than(int axis);

template<typename Iter> Iter midpoint(Iter start, Iter end);

template<typename T>
inline void make_kdtree(typename vector<T>::iterator start, typename vector<T>::iterator end, function<function<bool(const T&, const T&)>(int)> less_than, int depth) {
	if (std::distance(start,end)<1) return;
	auto mid = midpoint(start, end);
	nth_element(start, mid, end, less_than(depth % 2));
	make_kdtree(start, mid, less_than, depth+1);
	make_kdtree(mid + 1, end, less_than, depth+1);
}

template<typename T>
vector<T> search_kdtree(typename vector<T>::iterator start, typename vector<T>::iterator end, Box range, vector<T> &matches, Box bounds, int depth);

