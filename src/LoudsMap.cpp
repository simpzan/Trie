#include <sdsl/util.hpp>
#include "LoudsMap.h"
#include "PTrie.h"
#include "utils.h"
#include "TrieIterator.h"

using namespace std;
using namespace sdsl;

bool LoudsMap::visitNode(TrieNodeInterface &node_r) {
  _trie.visitNode(node_r);
  PTrieNode *node = (PTrieNode *) &node_r;

  uint32_t value = (*node).getValue();
  if (value != 0) {
    _is_tails[_is_tails_pos] = 1;
    _values.append(value);
  }
  ++_is_tails_pos;

  uint32_t link = (*node).get_link();
  if (link != 0) {
    _has_links[_has_links_pos] = 1;
    _links.append(link);
  }
  ++_has_links_pos;

  return true;
}

void LoudsMap::_preBuild(uint32_t node_count) {
  _trie._preBuild(node_count);
  
  bit_vector is_tails(node_count);
  _is_tails.swap(is_tails);
  bit_vector has_links(node_count);
  _has_links.swap(has_links);
  _is_tails_pos = 0;
  _has_links_pos = 0;
}

bool LoudsMap::build(TrieInterface &aTrie) {
  PTrie *trie_p = dynamic_cast<PTrie *>(&aTrie);
  if (trie_p == NULL)  return false;

  PTrie trie = *trie_p;
  uint32_t node_count = trie.getNodeCount() + 1; // + 1 for the super root.
  _preBuild(node_count);

  TrieBfsIterator itr(aTrie);
  PTrieNode *node;
  while (true) {
    node = (PTrieNode *)itr.next();
    if (node == NULL)  break;

    visitNode(*node);
  }
  _postBuild();
}

void LoudsMap::_postBuild() {
  _trie._postBuild();
  util::init_support(_is_tails_rank1, &_is_tails);
  util::init_support(_has_links_rank1, &_has_links);
}

void LoudsMap::display() {
  _trie.display();
  cout << "is tails: " << _is_tails.size() << "  " << ratio(_is_tails_rank1) << endl
    << _is_tails << endl;
  _values.display(cout);
  cout << "has links: " << _has_links.size() << "  " << ratio(_has_links_rank1) << endl 
    << _has_links << endl;
  _links.display(cout);
}

bool LoudsMap::load(std::istream &is) {
  _trie.load(is);

  cout << "is tails" << endl;
  showOffset(is);
  _is_tails.load(is);
  _is_tails_rank1.load(is, &_is_tails);

  cout << "values" << endl;
  showOffset(is);
  _values.read(is);

  _has_links.load(is);
  _has_links_rank1.load(is, &_has_links);

  cout << "links" << endl;
  showOffset(is);
  _links.read(is);
}

bool LoudsMap::serialize(std::ostream &os) {
  _trie.serialize(os);

  showOffset(os);
  _is_tails.serialize(os);
  _is_tails_rank1.serialize(os);
  _values.write(os);

  _has_links.serialize(os);
  _has_links_rank1.serialize(os);
  _links.write(os);
}

bool LoudsMap::findEntry(const char *key, TrieValueType &value) {
  value = 0;
  uint32_t node;
  uint32_t matched_count;
  _followKey(key, node, matched_count);
  if (matched_count != strlen(key))  return false;

  value = getValue(node);
  return value != 0;
}

bool LoudsMap::findEntryLowerBound(const char *pattern, 
    std::string *key, TrieValueType &value) {
  value = 0;
  uint32_t node;
  uint32_t matched_count;
  _followKey(pattern, node, matched_count);
  if (matched_count != strlen(pattern)) {
    const char *suffix = pattern + matched_count;
    uint32_t next_node = findChildLowerBound(node, suffix);
    if (next_node == 0) {
      next_node = generalSibling(node);
    }
    if (next_node == 0) {
      return false;
    }
    node = next_node;
  }
  uint32_t tail_node = leftMostChild(node);
  value = getValue(tail_node);

  if (key) {
    computeKey(*key, tail_node);
  }
  return true;
}


void LoudsMap::computeKey(string &key, uint32_t node) {
  uint32_t child = node;
  uint32_t parent;
  uint8_t ch;
  while (1) {
    string label;
    computeLabel(child, label);
    key = label + key;

    _trie.parent(child, parent, ch);
    key = (char)ch + key;

    if (parent == root())  break;
    child = parent;
  }
}

void LoudsMap::_followKey(const char *key, uint32_t &node, uint32_t &matched_count) {
  matched_count = 0;
  node = root();
  int key_len = strlen(key);
  while (matched_count < key_len) {
    uint32_t next_node;
    uint32_t matched_count_this;
    findChild(node, key + matched_count, next_node, matched_count_this);
    if (next_node == 0) {
      break;
    }

    node = next_node;
    matched_count += matched_count_this;
  }
}

void LoudsMap::computeLabel(uint32_t node, string &label) {
  uint32_t link = getLink(node);
  if (link == 0)  return;

  assert(_label_trie);
  _label_trie->computePrefix(link, label);
}

uint32_t LoudsMap::getLink(uint32_t node) {
  uint32_t has_links_pos = loudsRank0(node - 1) - 1;
  if (_has_links[has_links_pos] == 0)  return 0;

  uint32_t links_pos = hasLinksRank1(has_links_pos) - 1;
  uint32_t link = _links[links_pos];
  return link;
}

TrieValueType LoudsMap::getValue(uint32_t node) {
  // -1 turn rank to offset, -1 for super root
  uint32_t is_tails_pos = loudsRank0(node - 1) - 1;
  if (_is_tails[is_tails_pos] == 0)  return 0;

  uint32_t values_pos = isTailsRank1(is_tails_pos) - 1;
  assert(values_pos < _values.count());
  return _values[values_pos];
}

bool stringStartsWtih(const char *str_long, const char *str_short) {
  int lcp = computeLCP(str_long, str_short);
  return lcp == strlen(str_short);
}

void LoudsMap::findChild(uint32_t node, const char *key, 
    uint32_t &child, uint32_t &matched_count) {
  matched_count = 0;
  uint8_t ch = key[0];
  uint8_t fetched_ch;
  findChildViaChar(node, ch, child, fetched_ch);
  if (child == 0 || fetched_ch != ch) {
    child = 0;
    return;
  }

  const char *suffix = key + 1;
  string label;
  computeLabel(child, label);
  if (!stringStartsWtih(suffix, label.c_str())) {
    child = 0;
    return;
  }

  matched_count = label.size() + 1;
}

uint32_t LoudsMap::findChildLowerBound(uint32_t node, const char *key) {
  uint32_t child;
  uint8_t ch = key[0];
  uint8_t fetched_ch;
  findChildViaChar(node, ch, child, fetched_ch);
  if (child == 0)  return 0;
  if (fetched_ch > ch)  return child;
  assert(fetched_ch == ch);

  const char *suffix = key + 1;
  string label;
  computeLabel(child, label);
  if (string(suffix) <= label)  return child;

  return sibling(child);
}

uint32_t LoudsMap::leftMostChild(uint32_t node) {
  uint32_t child = node;
  TrieValueType value;
  while (true) {
    value = getValue(child);
    if (value != 0)  break;
    child = childSelect(child, 1);
  }
  return child;
}

void LoudsMap::updateLinks(const std::vector<uint32_t> &nodeIds) {
  int count = _links.count();
  for (int i = 0; i < count; ++i) {
    uint32_t link = _links[i] - 1;
    assert(link < nodeIds.size());
    _links[i] = nodeIds[link];
  }
}
