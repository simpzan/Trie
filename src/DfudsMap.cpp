#include "DfudsMap.h"

using namespace std;

namespace {
void ratio(rank_support_v<> &rank)  {
  uint32_t bitCount = rank.size();
  uint32_t ones = rank.rank(bitCount - 1);
  double r = (double)ones/ bitCount;

  cout << r << endl;
}

void showOffset(std::ostream &os) {
  uint32_t offset = os.tellp();
  cout << "offset: " << offset << endl;
}
} // namespace

void DfudsMap::updateLinks(const vector<uint32_t> &nodeIds) {
  Vector<uint32_t> &links = _links;

  int count = links.count();
  for (int i = 0; i < count; ++i) {
    uint32_t link = links[i] - 1;
    assert(link < nodeIds.size());
    links[i] = nodeIds[link];
  }
}

bool DfudsMap::build(TrieInterface &trie) {
    uint32_t node_count = trie.getNodeCount();
    _preBuild(node_count);
    trie.traverseDFS(*this);
    _postBuild();
}

bool DfudsMap::load(istream &is) {
  _trie.load(is);

  _is_keys.load(is);
  _is_keys_rank_support.load(is, &_is_keys);
  _values_DAC.read(is);

  _has_links.load(is);
  _has_links_support.load(is, &_has_links);
  _links_DAC.read(is);
}

bool DfudsMap::serialize(ostream &os) {
  _trie.serialize(os);

  _is_keys.serialize(os);
  _is_keys_rank_support.serialize(os);
  _values_DAC.write(os);

  _has_links.serialize(os);
  _has_links_support.serialize(os);
  _links_DAC.write(os);
}

void DfudsMap::_preBuild(int node_count) {
  _trie._preBuild(node_count);

  _is_keys.resize(node_count);
  _is_keys_pos = 0;
  _values.clear();
  _values_DAC.clear();

  _has_links.resize(node_count);
  _has_links_pos = 0;
  _links.clear();
}

bool DfudsMap::visitNode(TrieNodeInterface &aNode) {
  _trie.visitNode(aNode);
  PTrieNode &node = (PTrieNode &)aNode;

  uint32_t value = node.getValue();
  if (value != 0) {
    _is_keys[_is_keys_pos] = 1;
    _values.append(value);
  }
  ++_is_keys_pos;

  uint32_t link = node.get_link();
  if (link != 0) {
    _has_links[_has_links_pos] = 1;
    _links.append(link);
  }
  ++_has_links_pos;
}

void DfudsMap::_postBuild() {
  _trie._postBuild();

  util::init_support(_is_keys_rank_support, &_is_keys);
  util::init_support(_has_links_support, &_has_links);
  _values_DAC.setData(_values.data(), _values.count());
  _links_DAC.setData(_links.data(), _links.count());
}

bool DfudsMap::findEntry(const char *key, TrieValueType &value) {
  assert(false);
}
