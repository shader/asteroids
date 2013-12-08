#pragma once
#include "object.h"
#include <algorithm>
#include "utils.h"
#include "kd-tree.h"

bool in(Box range, Box box);
bool intersecting(Box &a, Box &b);
function<bool(const Box&, const Box&)> less_than(int axis);

template<typename T, typename Iter>
class BoxTree {
public:
	function<Box(const T&)> box;
	vector<T> tree;
	BoxTree();
	BoxTree(function<Box(const T&)> box);
	BoxTree(Iter begin, Iter end, function<Box(const T&)> box);
	
	void Sort();
	void Sort(Iter begin, Iter end);

	vector<T> Search(Box range);
	vector<T> Search(T);
private:
	function<bool(const T&, const T&)> compare(int axis);
	void search_tree(typename vector<T>::iterator start, typename vector<T>::iterator end, Box range, vector<T> &matches, int depth);
};