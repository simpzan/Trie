#ifndef PTRIE_LOUDS_BUILDER_H
#define PTRIE_LOUDS_BUILDER_H

#include <vector>
#include <string>
#include <list>
#include <map>
#include <Vector.h>
#include "Trie.h"
#include "LoudsTrie.h"
#include "BitVectorBuilder.h"
#include "STrie.h"
#include "Range.h"
#include "StringPool.h"


class STrieBuilder : public ILoudsMapBuilder, public ILoudsTrieBuilder {
 public:
  STrieBuilder(std::vector<std::string> &keys) : _keys(keys) {}
  ~STrieBuilder() {}

  bool build(const std::string &filename);

 private:
  bool isTerminalNode(Range &range) {
    return _keys[range.left].size() == range.depth;
  }
  void getSubranges(Range &range, std::vector<Range> &subranges);
  bool isSameOnThisDepth(Range &range);
  int findNextDepth(const Range &range);
  void recordLabel(const std::string &label);
  void extractStringLabels();
  void generateLabelTrieLouds();
  void updateLinks();
  void generateMainTrieLouds();
  void saveToFile();

  virtual sdsl::bit_vector &is_tails() { return _isTerminal.underlyingBV(); }
  virtual Vector<uint32_t> &values() {  return _values;  }
  virtual sdsl::bit_vector &has_links() {  return _hasLinks.underlyingBV(); }
  virtual Vector<uint32_t> &links() { return _links; }
  virtual ILoudsTrieBuilder &trie() {  return *this;  }

  virtual sdsl::bit_vector &louds() {  return _louds.underlyingBV();  }
  virtual Vector<uint8_t> &labels() { return _branchChars; }

 private:
  typedef StringPool MapT;
  typedef std::vector<std::string> Strings;

  Strings &_keys;
  std::ofstream _outputStream;
  MapT _labelMap;
  vector<uint32_t> nodeIds;

  BitVectorBuilder _louds;
  Vector<uint8_t> _branchChars;

  BitVectorBuilder _hasLinks;
  Vector<uint32_t> _links;

  BitVectorBuilder _isTerminal;
  Vector<uint32_t> _values;

  LoudsTrieT _labelTrieLouds;
};

#endif
