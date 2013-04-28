#ifndef LINKEDTRIENODE_H
#define LINKEDTRIENODE_H

#include <map>
#include "TrieNode.h"

class LinkedTrieNode: public TrieNode {
	
	public:
		LinkedTrieNode():_value(0) {}
		virtual ~LinkedTrieNode();

	public:
		TrieNode *getChildNodeWithLabel(uint8_t ch);
		void setChildNodeWithLabel(uint8_t ch, TrieNode *node);

		void setValue(uint64_t value) {
			_value = value;
		}
		uint64_t getValue() {
			return _value;
		}

		LinkedTrieNode *createNode() {
			LinkedTrieNode *node = new LinkedTrieNode();
			return node;
		}

		uint32_t sizeOfThisNode() {
			uint32_t size = sizeof(_value) + sizeof(_children);
			return size;
		}

		virtual void clear();

		std::map<uint8_t, TrieNode *> &children() {
			return _children;
		}


	private:
		std::map<uint8_t, TrieNode*> _children;
		uint64_t _value;
};


#endif

