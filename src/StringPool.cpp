#include <queue>

#include "StringPool.h"

using namespace std;

uint32_t StringPool::findOrInsert(const char *key) {
  string keyTmp(key);
  reverse(keyTmp.begin(), keyTmp.end());

  uint32_t result = _keyMap.size();
  StringIntMap::iterator itr = _keyMap.find(keyTmp);
  if (itr == _keyMap.end()) {
    _keyMap[keyTmp] = result;
  } else {
    result = itr->second;
  }
  return result;
}

void StringPool::build() {
  uint32_t size = _keyMap.size();
  _newIds.resize(size);

  for (StringIntMap::iterator itr = _keyMap.begin(); itr != _keyMap.end(); ++itr) {
    _keys.push_back(itr->first);
    _oldIds.push_back(itr->second);
  }

  generateData();
  display();
}

void StringPool::generateData() {
  _louds.addBit(true);
  _louds.addBit(false);
  uint32_t newId = 0; // terminal count in level order.

  queue<Range> queue;
  queue.push(Range(0, _keys.size(), 0));
  while (!queue.empty()) {
    _louds.addBit(true);

    Range thisRange = queue.front();
    queue.pop();

    // if (isTerminalNode(thisRange)) {
      uint32_t oldId = _oldIds[thisRange.left];
      _newIds[oldId] = ++newId;
    // }

    std::vector<Range> subranges;
    getSubranges(thisRange, subranges);
    for (int i=0; i<subranges.size(); ++i) {
      Range subrange = subranges[i];
      _labels.append(_keys[subrange.left][subrange.depth]);
      _louds.addBit(false);

      subrange.depth += 1;
      queue.push(subrange);
    }
  }
}

void StringPool::getSubranges(Range &range, vector<Range> &subranges) {
  while (!range.isEmpty() && isTerminalNode(range)) {
    range.left += 1;
  }
//  if (isTerminalNode(range)) {
//    range.left += 1;
//  }
  if (range.isEmpty()) return;

  int leftTmp = range.left;
  int right = range.right;
  int depth = range.depth;
  for (int i=range.left+1; i < range.right; ++i) {
    if (_keys[i-1][depth] != _keys[i][depth]) {
      Range subrange(leftTmp, i, depth);
      subranges.push_back(subrange);
      leftTmp = i;
    }
  }

  Range subrange(leftTmp, right, depth);
  subranges.push_back(subrange);
}

void StringPool::serialize(ostream& os) {
  LoudsTrieT trie;
  trie.init(*this);
  trie.serialize(os);
}
