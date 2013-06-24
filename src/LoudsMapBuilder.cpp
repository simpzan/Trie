#include <sdsl/util.hpp>
#include "LoudsMapBuilder.h"
#include "PTrie.h"
#include "utils.h"
#include "TrieIterator.h"

using namespace std;
using namespace sdsl;

bool LoudsMapBuilder::visitNode(TrieNodeInterface &node_r) {
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

void LoudsMapBuilder::_preBuild(uint32_t node_count) {
  _trie._preBuild(node_count);
  
  bit_vector is_tails(node_count);
  _is_tails.swap(is_tails);
  bit_vector has_links(node_count);
  _has_links.swap(has_links);
  _is_tails_pos = 0;
  _has_links_pos = 0;

  _values.clear();
  _links.clear();
}

bool LoudsMapBuilder::build(TrieInterface &aTrie) {
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
}


//void LoudsMapBuilder::display() {
  //_trie.display();
  //cout << "is tails: " << _is_tails.size() << "  " << ratio(_is_tails_rank1) << endl
    //<< _is_tails << endl;
  //_values.display(cout);
  //cout << "has links: " << _has_links.size() << "  " << ratio(_has_links_rank1) << endl 
    //<< _has_links << endl;
  //_links.display(cout);
//}

bool LoudsMapBuilder::load(std::istream &is) {
  _trie.load(is);

  _is_tails.load(is);
  _values.read(is);

  _has_links.load(is);
  _links.read(is);
}

bool LoudsMapBuilder::serialize(std::ostream &os) {
  _trie.serialize(os);

  _is_tails.serialize(os);
  _values.write(os);

  _has_links.serialize(os);
  _links.write(os);
}

void generate(const vector<uint32_t> &nodeIds, Vector<uint32_t> &dac) {
  int count = dac.count();
  Vector<uint32_t> links(count);
  for (int i = 0; i < count; ++i) {
    uint32_t link = dac[i] - 1;
    assert(link < nodeIds.size());
    links[i] = nodeIds[link];
  }
  dac.swap(links);
}

void LoudsMapBuilder::updateLinks(const std::vector<uint32_t> &nodeIds) {
  generate(nodeIds, _links);
}

void LoudsMapBuilder::updateValues(const std::vector<uint32_t> &offsets) {
  generate(offsets, _values);
}
