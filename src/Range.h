#ifndef RANGE_H
#define RANGE_H

struct Range {
  int left;
  int right;
  int depth;
  Range(int l, int r, int d) : left(l), right(r), depth(d) {}
  bool isEmpty() {  return left == right;  }
};

#endif
