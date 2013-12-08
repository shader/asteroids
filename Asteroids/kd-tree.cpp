#include "kd-tree.h"

bool in(Box range, vec3 point) {
	bool inside = true;
	for (int i=0; i<3; i++) {
		inside &= point[i] >= range.lower[i];
		inside &= point[i] <= range.upper[i];
	}
	return inside;
}

template<typename T>
function<bool(const T&, const T&)> less_than(int axis) {
	return [=](const T &a, const T &b) { return a[axis] < b[axis]; };
}

function<bool(const vec3&, const vec3&)> search_lteq(int axis) {
  return [=](const vec3 &a, const vec3 &b) { return a[axis] <= b[axis]; };
}


template<typename T, typename Iter>
KDTree<T,Iter>::KDTree(Iter begin, Iter end, function<vec3(const T&)> coords) {
	tree = vector<T>(begin, end);
	this->coords = coords;
	make_kdtree<T>(tree.begin(), tree.end(), [=](int axis){ return sort(axis); }, 0);

	bounds.lower = box(tree[0]).lower;
	bounds.upper = box(tree[0]).upper;
	for (auto t=tree.begin(); t!=tree.end(); t++) {
		bounds.lower = min(bounds.lower, coords(*t));
		bounds.upper = max(bounds.upper, coords(*t));
	}
}

template<typename T, typename Iter>
vector<T> KDTree<T,Iter>::Search(Box range) {
	vector<T> matches;
	search_tree(tree.begin(), tree.end(), range, matches, bounds, 0);
}

template<typename T, typename Iter>
function<bool(const T&, const T&)> KDTree<T,Iter>::sort(int) {
	return [=](const T &a, const T &b)->bool {
		return less_than(axis)(coords(a), coords(b));
	};
}

template<typename T, typename Iter>
void KDTree<T,Iter>::search_tree(typename vector<T>::iterator start, typename vector<T>::iterator end, Box range, vector<T> &matches, Box bounds, int depth) {
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
		if (lteq(depth % 3)(range.lower, *mid)) { //left subtree
			left_bounds.upper[depth % 3] = (*mid)[depth % 3];
			search_tree(start, mid, range, left_bounds, matches, depth+1);
		}
	}

	if (in(range, *mid)) {
		matches.push_back(*mid); //middle
	}

	if (distance(mid, end)>0) {
		if (lteq(depth % 3)(*mid, range.upper)) { //right subtree
			right_bounds.lower[depth % 3] = mid->coords[depth % 3];
			search_tree(mid+1, end, range, right_bounds, matches, depth+1);
		}
	}

	return matches;
}