#include "object.h"

class KdTree {

};

function<bool(const vec3&, const vec3&)> less_than(int axis) {
  return [=](const vec3 &a, const vec3 &b) { return a[axis] < b[axis]; };
}

template<typename Iter> Iter midpoint(Iter start, Iter end) {
  return start + distance(start, end) / 2;
}

int i=0;
template<typename Iter> void make_tree(Iter start, Iter end, int depth) {
  if (distance(start,end)<1) return;
  Iter mid = midpoint(start, end);
  nth_element(start, mid, end, less_than(depth % 2));
  mid->depth = depth;
  make_tree(start, mid, depth+1);
  make_tree(mid + 1, end, depth+1);
}
