#ifndef LINKEDTRIE_H
#define LINKEDTRIE_H

#include "LinkedTrieNode.h"
#include "ArrayTrieNode.h"
#include "Trie.h"

using namespace std;

class LinkedTrie :public Trie {
	public:
		LinkedTrie() : Trie(new LinkedTrieNode) {}

		void buildDfudsTrie() {
			LinkedTrieNode *node = (LinkedTrieNode *) root();
			_dfuds.push_back(false); 	// make it balance.
			buildDfudsTrieR(node);
			display();
		}

		void write(std::ostream &os) {
			writeBitArray(os, _dfuds);
			writeBitArray(os, _isTerminal);
			
		}

		uint64_t getEntry2(const char *key) {
			uint64_t value = 0;
			int keyLen = strlen(key);
			uint64_t offset = 1;
			for (int i = 0; i < keyLen; i++) {
				int ch = key[i];
				uint64_t next = getChild(offset, ch);
				
				cout << i << ":" <<(char)ch << endl;				
				cout << offset << " -> " << next << endl;

				if (!next) {
					return value;
				}
				offset = next;
			}

			uint64_t valueOffset = rank(_dfuds, offset, true);
			value = _values[valueOffset];

			return value;
		}

	private:

		uint64_t rank(vector<bool> &array, uint64_t offset, bool element) {
			assert(offset < array.size());

			uint64_t count = 0;
			for (int i = 0; i < offset; i++) {
				if (array[i] != element) {
					continue;
				}
				++count;
			}
			return count;
		}

		uint64_t select(vector<bool> &array, uint64_t count, bool element) {
			assert(count < array.size());

			int tmpCount = 0;
			for (int ai = 0; ai < array.size(); ai++) {
				if (array[ai] != element) {
					continue;
				}
				++tmpCount;
				if (count == tmpCount) {
					return ai;
				}
			}

			assert(false);
			return 0;
		}

		uint64_t findClose(vector<bool> &array, uint64_t offset) {
			vector<bool> queue;

			for (int i = offset+1; i < array.size(); i++) {
				if (array[i]) {
					if (queue.empty()) {
						return i;
					}
					queue.pop_back();
				} else {
					queue.push_back(array[i]);
				}
			}
			
			assert(false);
			return 0;
		}

		uint64_t getChild(uint64_t offset, uint8_t ch) {
			uint64_t openRank = rank(_dfuds, offset, false) - 1;
			uint64_t closeRank = offset - openRank - 1;
			uint64_t closeOffset = select(_dfuds, closeRank+1, true);
			int degree = closeOffset - offset;

			int i;
			for (i = 0; i < degree; i++) {
				if (_labels[openRank+i] == ch) {
					break;
				}
			}
			if (i == degree) {
				return 0;
			}

			++i;
			uint64_t newOffset = closeOffset - i;
			assert(!_dfuds[newOffset]);
			uint64_t result = findClose(_dfuds, newOffset) + 1;
			return result;
		}

		void buildDfudsTrieR(LinkedTrieNode *node) {
			_values.push_back(node->getValue());

			std::map<uint8_t, TrieNode *> &children = node->children();
			for (map<uint8_t, TrieNode *>::iterator itr = children.begin();
					itr != children.end();
					++itr) {
				_dfuds.push_back(false);
				_labels.push_back(itr->first);
			}
			_dfuds.push_back(true);

			bool isTerminal = children.empty();
			_isTerminal.push_back(isTerminal);

			for (map<uint8_t, TrieNode*>::iterator itr = children.begin();
					itr != children.end();
					++itr) {
				buildDfudsTrieR((LinkedTrieNode*) itr->second);
			}
		}

		template <typename CollectionT>
			void displayCollection(CollectionT &collection) {
				for (typename CollectionT::iterator itr = collection.begin();
						itr != collection.end();
						++itr) {
					cout << *itr;
				}
				cout << endl;
			}

		void display() {
			cout << "dfuds:" << endl;
			displayCollection(_dfuds);
			cout << "labels" << endl;
			displayCollection(_labels);
		}

		uint8_t byteFromArray(vector<bool> &array, uint64_t offset) {
			uint8_t byte = 0;
			for (int i = 0; i < 8; i++) {
				uint8_t mask = 1 << i;
				if (array[i]) {
					byte |= mask;
				}
			}
			return byte;
		}

		void writeBitArray(ostream &os, vector<bool> &array) {
			uint64_t bitSize = array.size();
			uint64_t size = (bitSize + 7) / 8;
			uint8_t *buf = new uint8_t[size];
			
			for (int i = 0; i < bitSize; i += 8) {
				uint8_t byte = byteFromArray(array, i);
				buf[i/8] = byte;
			}

			os.write((char *)&size, sizeof(size));
			os.write((char *)buf, size);
			
			delete buf;
		}

	private:
		std::vector<bool> _dfuds;
		std::vector<uint8_t> _labels;
		std::vector<bool> _isTerminal;
		std::vector<uint64_t> _values;

};

#endif

