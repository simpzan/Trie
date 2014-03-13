#include "PTrieLoudsBuilder.h"

#include <queue>
#include "Trie.h"
#include "timer.h"

using namespace std;

bool PTrieLoudsBuilder::build(const std::string &filename) {
  _outputStream.open(filename.c_str());
  assert(_outputStream.good());

    cout << "extract strings" << endl;
    Timer t;
  extractStringLabels();
    t.Report(1);
    
    cout << "label trie" << endl;
    t.Restart();
    generateLabelTrieLouds();
    t.Report(1);
    
    cout << "main trie" << endl;
    t.Restart();
  generateMainTrieLouds();
    t.Report(1);
    
    cout << "links update" << endl;
    t.Restart();
  updateLinks();
    t.Report(1);
    
    cout << "saving" << endl;
    t.Restart();
  saveToFile();
    t.Report(1);
}

void PTrieLoudsBuilder::saveToFile() {
  
  _labelTrieLouds.serialize(_outputStream);

  cout << _outputStream.tellp() << endl;

  LoudsMapT mainTrieLouds;
  mainTrieLouds.init(*this);
  mainTrieLouds.serialize(_outputStream);

  cout << _outputStream.tellp() << endl;
}

void PTrieLoudsBuilder::updateLinks() {
  for (int i=0; i<_links.count(); ++i) {
    _links[i] = nodeIds[_links[i]-1];
  }
}

void PTrieLoudsBuilder::generateMainTrieLouds() {
  _louds.addBit(true);
  _louds.addBit(false);

  queue<Range> queue;
  queue.push(Range(0, _keys.size(), 0));
  while (!queue.empty()) {
    _louds.addBit(true);

    Range thisRange = queue.front();
    queue.pop();
    bool isTerminal = isTerminalNode(thisRange);
    _isTerminal.addBit(isTerminal);
    if (isTerminal) {
      _values.append(thisRange.left);
    }

    vector<Range> subranges;
    getSubranges(thisRange, subranges);
    for (int i=0; i<subranges.size(); ++i) {
      Range subrange = subranges[i];
      _branchChars.append(_keys[subrange.left][subrange.depth]);
      _louds.addBit(false);

      subrange.depth = findNextDepth(subrange);
      queue.push(subrange);
    }
  }
}

void PTrieLoudsBuilder::generateLabelTrieLouds() {
  LoudsTrieBuilder builder;
  _labelMap.convert(builder, nodeIds);
  _labelTrieLouds.init(builder);
}


void PTrieLoudsBuilder::extractStringLabels() {
    _hasLinks.addBit(false);
  queue<Range> queue;
  queue.push(Range(0, _keys.size(), 0));
  while (!queue.empty()) {
    Range thisRange = queue.front();
    queue.pop();

    vector<Range> subranges;
    getSubranges(thisRange, subranges);
    for (int i=0; i<subranges.size(); ++i) {
      Range subrange = subranges[i];
      int depth = findNextDepth(subrange);

      const char *key = _keys[subrange.left].c_str();
      int lenLabel = depth - subrange.depth;
      string label(key + subrange.depth, lenLabel);

      recordLabel(label);

      subrange.depth = depth;
      queue.push(subrange);
    }
  }
}

void PTrieLoudsBuilder::recordLabel(const string &label) {
  if (label.length() == 1) {
    _hasLinks.addBit(false);   
    return;
  }

  _hasLinks.addBit(true);

  const char *suffix = label.c_str() + 1;
  uint32_t id = _labelMap.insertKey(suffix);
    assert(id);
  _links.append(id);
}

int PTrieLoudsBuilder::findNextDepth(const Range &range) {
  int length = _keys[range.left].length();
  for (int i = range.depth; i<length; ++i) {
    Range aRange(range.left, range.right, i);
    if (!isSameOnThisDepth(aRange))  return i;
  }
  return length;
}

bool PTrieLoudsBuilder::isSameOnThisDepth(Range &range) {
  for (int i=range.left+1; i<range.right; ++i) {
    if (_keys[i-1][range.depth] != _keys[i][range.depth]) return false;
  }
  return true;
}

void PTrieLoudsBuilder::getSubranges(Range &range, vector<Range> &subranges) {
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


