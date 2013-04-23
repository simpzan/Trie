#ifndef AMTRIENODE_H
#define AMTRIENODE_H

#include <vector>
#include <cstring>
#include <bitset>
#include "TrieNode.h"
#include <cstdlib>

extern uint64_t _value;
extern uint64_t _count;

class AMTrieNode: public TrieNode {

	public:
		AMTrieNode(): TrieNode() {
                    _nodes = NULL;
			_children = new std::vector<AMTrieNode *>;
		   	_value = 0;
			++_count;
		}

		virtual ~AMTrieNode() {
			if (_children) {
			for (int ci=0; ci<(*_children).size(); ++ci) {
				delete (*_children)[ci];
			}
			delete _children;
			}

			if (_nodes) {
				for (int i=0; i<_childCount; ++i) {
					delete _nodes[i];
				}
				delete _nodes;
			}

		}

	public:
		TrieNode *getChildNodeWithLabel(uint8_t ch);
		void setChildNodeWithLabel(uint8_t ch, TrieNode *node);

		uint64_t getValue() {
			return _value;
		}
		void setValue(uint64_t value) {
			_value = value;
		}

		AMTrieNode *createNode() {
			AMTrieNode *node = new AMTrieNode;
			return node;
		}

		uint32_t sizeOfThisNode() {
			uint32_t size = sizeof(_map) + sizeof(_children);
			return size;
		}

		void compact() {
			_childCount = _children->size();
			_nodes = (AMTrieNode **) malloc(_children->size()*sizeof(TrieNode*));
			for (int ci=0; ci<(*_children).size(); ++ci) {
				_nodes[ci] = (*_children)[ci];
				(*_children)[ci]->compact();
			}
			delete _children;
			_children = NULL;
		}

	private:
		uint64_t _value;
		uint8_t _childCount;
		std::bitset<256> _map;
		std::vector<AMTrieNode *> *_children;
		AMTrieNode **_nodes;
};


#endif
