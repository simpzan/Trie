#ifndef RE_TRIE_NODE_H
#define RE_TRIE_NODE_H

#include <stdint.h>
#include <string>
#include <map>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/smart_ptr.hpp>
#include "TrieNodeInterface.h"
#include "TrieNode.h"

class PTrieNode : public TrieNodeInterface {
  public:
  PTrieNode() : _value(0), _label(""), _link(0), 
  _link_node(NULL), _parent(NULL), _id(0) {
    _id = ++counter;
  }

  virtual ~PTrieNode() {}

  virtual TrieNodeInterface *getChildWithCharLabel(uint8_t ch) {
    return getChildNodeWithLabel(ch);
  }
  virtual PTrieNode *getChildNodeWithLabel(uint8_t ch) {
    ChildrenMapIterator itr = _children.find(ch);
    if (itr == _children.end())  return NULL;

    return itr->second;
  }
  virtual void setChildNodeWithLabel(uint8_t ch, PTrieNode *node) {
    ChildrenMapIterator itr = _children.find(ch);
    if (itr != _children.end()) {
      _children.replace(itr, node).release();
    } else {
      _children.insert(ch, node);
    }
    node->_parent = this;
  }

  virtual PTrieNode *createNode() {
    PTrieNode *node = new PTrieNode;
    return node;
  }

  void findChildViaLink(const char *key, PTrieNode *&nextNode, int &matched_count);
  void findChildViaLabel(const char *key, PTrieNode *&nextNode, int &matched_count);
  void collectLabels(std::vector<std::string> &labels);
  void collectLabelNodes(std::vector<PTrieNode *> &nodes);
  void clear() {
    _children.clear();
    _value = 0;
    _label.clear();
    _link = 0;
    _link_node = NULL;
    _parent = NULL;
  }

  void assignNodeIds(uint32_t counter = 0);
  void traverseDFS(TrieNodeVisitorInterface &visitor);

  virtual TrieValueType getValue() {  return _value;  }
  virtual void setValue(TrieValueType value) {  _value = value;  }

  std::string &get_label() {  return _label;  }
  void set_label(const std::string &label) {  _label = label;  }

  uint32_t get_link() {  return _link;  }
  void set_link(uint32_t link) {  _link = link;  }

  PTrieNode *get_link_node() {  return _link_node;  }
  void set_link_node(PTrieNode *node) {  _link_node = node;  }

  PTrieNode *get_parent() {  return _parent;  }
  void set_parent(PTrieNode *parent) {  _parent = parent;  }

  virtual void set_id(uint32_t id) {  _id = id;  }
  virtual uint32_t get_id() {  return _id;  }

  void getCharLabels(std::vector<uint8_t> &labels) {
    for (ChildrenMapIterator itr = _children.begin();
        itr != _children.end(); 
        ++itr) {
      labels.push_back(itr->first);
    }
  }

  private:
typedef boost::ptr_map<uint8_t, PTrieNode>::iterator ChildrenMapIterator;
  boost::ptr_map<uint8_t, PTrieNode> _children;
  TrieValueType _value;
  std::string _label;
  uint32_t _link;
  PTrieNode *_link_node;
  PTrieNode *_parent;
  uint32_t _id;
  static int counter;
};

#endif
