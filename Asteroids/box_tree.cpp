#include "box_tree.h"

bool in(Box range, Box box) {
	bool inside = true;
	for (int i=0; i<3; i++) {
		inside &= box.lower[i] >= range.lower[i];
		inside &= box.upper[i] <= range.upper[i];
	}
	return inside;
}

bool intersecting(Box &a, Box &b) {
	return min(a.upper, b.upper) >= max(a.lower, b.lower);
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

//returns true if search range intersects both subtrees
function<bool(const Box&, const Box&)> search_cmp(int axis) {
	return [=](const Box &range, const Box &median)->bool {
		if (axis < 3) {
			return median.lower[axis] <= range.upper[axis]; 
		} else {
			return median.upper[axis % 3] >= range.lower[axis % 3];
		}
	};
}


template<typename T, typename Iter>
function<bool(const T&, const T&)> BoxTree<T,Iter>::compare(int axis) {
	return [=](const T &a, const T &b)->bool {
		return less_than(axis)(box(a), box(b));
	};
}

template<typename T, typename Iter>
void BoxTree<T,Iter>::Sort() {
	make_kdtree<T>(tree.begin(), tree.end(), [=](int axis){ return compare(axis); }, 0);
}

template<typename T, typename Iter>
void BoxTree<T,Iter>::Sort(Iter begin, Iter end) {
	tree = vector<T>(begin, end);
	Sort();
}

template<typename T, typename Iter>
BoxTree<T,Iter>::BoxTree(Iter begin, Iter end, function<Box(const T&)> box) {
	this->box = box;
	Sort(begin, end);
}

template<typename T, typename Iter>
BoxTree<T,Iter>::BoxTree(function<Box(const T&)> box) {
	this->box = box;
}

template<typename T, typename Iter>
BoxTree<T,Iter>::BoxTree() {}

template<typename T, typename Iter>
void BoxTree<T,Iter>::search_tree(typename vector<T>::iterator start, typename vector<T>::iterator end, Box range, vector<T> &matches, int depth) {
	if (std::distance(start, end) <= 0) return;

	auto mid = midpoint(start,end);
	//check midpoint for intersection
	if (intersecting(range, box(*mid))) {
		matches.push_back(*mid);
	}
  
	if (depth % 6 < 3) {
		//always search left subtree
		search_tree(start, mid, range, matches, depth+1);

		//compare lower coordinate of elements with upper coordinate of search range, and search only left side
		if (search_cmp(depth % 6)) {
			search_tree(mid+1, end, range, matches, depth+1);
		}
	} else {
		//always search right subtree
		search_tree(mid+1, end, range, matches, depth+1);
		
		//compare upper coordinate of elements with lower coordinate of search range, and conditionally search left subtree
		if (search_cmp(depth % 6)) {
			search_tree(start, mid, range, matches, depth+1);
		}
	}
}

template<typename T, typename Iter>
vector<T> BoxTree<T, Iter>::Search(T obj) {
	vector<shared_ptr<Object>> matches;
	search_tree(tree.begin(), tree.end(), box(obj), matches, 0);

	return matches;
}

template<typename T, typename Iter>
vector<T> BoxTree<T, Iter>::Search(Box range) {
	vector<shared_ptr<Object>> matches;
	search_tree(tree.begin(), tree.end(), range, matches, 0);

	return matches;
}

template BoxTree< shared_ptr<Object>, list< shared_ptr<Object> >::iterator >;