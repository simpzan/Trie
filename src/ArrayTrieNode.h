#ifndef ARRAYTRIENODE_H
#define ARRAYTRIENODE_H

#include <stdint.h>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <typeinfo>
#include <bitset>
#include "TrieNode.h"

#define NODESIZE 256 

class ArrayTrieNode: public TrieNode {

	public:
		ArrayTrieNode() {
			_value = 0;
			memset(_children, 0, sizeof(_children));	
		}
		virtual ~ArrayTrieNode() {
			for (int i=0; i<NODESIZE; ++i) {

				delete _children[i];
				_children[i] = NULL;
			}
		}

	public:
		// return the child with the char in this node.
		virtual TrieNode *getChildNodeWithLabel(uint8_t ch) {
			return _children[ch];
		}
		// set the child with the char in this node.
		virtual void setChildNodeWithLabel(uint8_t ch, TrieNode *node) {
			_children[ch] = node;
		}


		void setValue(uint64_t value) {
			_value = value;
		}
		uint64_t getValue() {
			return _value;
		}

		virtual ArrayTrieNode *createNode() {
			ArrayTrieNode *node = new ArrayTrieNode;
			return node;
		}

		virtual uint32_t sizeOfThisNode() {
			uint32_t size = sizeof(_value) + sizeof(_children);
			return size;
		}	


	protected:
		uint64_t _value;
		TrieNode *_children[NODESIZE]; 	// children links
};

#endif
