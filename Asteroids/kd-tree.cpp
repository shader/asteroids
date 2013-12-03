#include "kd-tree.h"

bool in(Box range, vec3 point) {
	bool inside = true;
	for (int i=0; i<3; i++) {
		inside &= point[i] >= range.lower[i];
		inside &= point[i] <= range.upper[i];
	}
	return inside;
}

bool in(Box range, Box box) {
	bool inside = true;
	for (int i=0; i<3; i++) {
		inside &= box.lower[i] >= box.lower[i];
		inside &= box.upper[i] <= box.upper[i];
	}
	return inside;
}

bool intersecting(Box a, Box b) {
	return min(a.upper, b.upper) > max(a.lower, b.lower);
}

template<typename T>
function<bool(const T&, const T&)> less_than(int axis) {
	return [=](const T &a, const T &b) { return a[axis] < b[axis]; };
}

function<bool(const Box&, const Box&)> less_than(int axis) {
	return [=](const Box &a, const Box &b)->bool {
		if (axis < 3) {
			return a.lower[axis] < b.lower[axis]; 
		} else {
			return a.upper[axis % 3] < b.upper[axis % 3];
		}
	};
}

template<typename Iter> Iter midpoint(Iter start, Iter end) {
  return start + std::distance(start, end) / 2;
}

template<typename T>
vector<T> search_kdtree(typename vector<T>::iterator start, typename vector<T>::iterator end, Box range, vector<T> &matches, Box bounds, int depth) {
	//check if tree is completely in range, and return it
	Box left_bounds, right_bounds;
	left_bounds = right_bounds = bounds;
	if (in(range, bounds)) {
		matches.insert(matches.end(), start, end);
		return matches;
	}
  
	//try again with intersection
	Iter mid = midpoint(start,end);
	if (distance(start,mid)>0) {
		if (lteq(depth % 2)(range.lower_left, *mid)) { //left subtree
			left_bounds.upper[depth % 2] = (*mid)[depth % 2];
			search_tree(start, mid, range, left_bounds, matches, depth+1);
		}
	}

	if (in(range, *mid)) {
		matches.push_back(*mid); //middle
	}

	if (distance(mid, end)>0) {
		if (lteq(depth % 2)(*mid, range.upper_right)) { //right subtree
			right_bounds.lower_left.coords[depth % 2] = mid->coords[depth % 2];
			search_tree(mid+1, end, range, right_bounds, matches, depth+1);
		}
	}

	return matches;
}