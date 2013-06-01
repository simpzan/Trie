#include "FrontCoder.h"
#include <algorithm>
#include "utils.h"

using namespace std;

namespace {

int compareString(const char *l, const char *r) {
  int diff =0;
  while (*l && *r) {
    diff = *l - *r;
    if (diff) {
      return diff;
    }
    ++l;
    ++r;
  }
  diff = *l - *r;
  return diff;
}

}

uint32_t FrontCoderBuilder::sizeAfterInsertKey(const std::string &key) {
  uint8_t lcp = computeLCP(_lastKey.c_str(), key.c_str());
  uint8_t suffix_len = key.size() - lcp + 1;
  uint32_t size_this = suffix_len + 1 + 1;
  return size_this + size();
}

void FrontCoderBuilder::add(const std::string &key) {
  const std::string &thisKey = key;
  const std::string &lastKey = _lastKey;
  assert(key >= lastKey);
  uint8_t lcp = computeLCP(lastKey.c_str(), thisKey.c_str());
  _LCPs.append(lcp);

  uint8_t suffix_len = thisKey.size() - lcp + 1;
  _suffix_lengths.append(suffix_len);

  uint8_t *suffix = (uint8_t *)thisKey.data() + lcp;
  _suffixes.appendValues(suffix, suffix_len);   
  _lastKey = key;
}

bool FrontCoder::find(const string &key, uint32_t &rank) {
  int key_len = key.size();
  int key_count = _LCPs.count();
  string suffix((char *)_suffixes.data());
  string key_suffix(key);
  int lcp = computeLCP(suffix.c_str(), key_suffix.c_str());
  int tmp_index = 1;
  int suffix_offset = _suffix_lengths[0];
  int cmp = compareString(suffix.c_str(), key_suffix.c_str());
  bool found = cmp == 0;
  if (cmp > 0) {
    rank = 0;
    return false;
  }

  while (lcp < key_len && tmp_index < key_count) {
    // skip keys whose lcp is too big.
    while (_LCPs[tmp_index] > lcp) {
      suffix_offset += _suffix_lengths[tmp_index];
      ++tmp_index;
      if (tmp_index == key_count) {
        rank = tmp_index;
        return false;
      }
    }
    if (_LCPs[tmp_index] < lcp) {
      rank = tmp_index;
      return false;
    }

    key_suffix = (char *)key.data() + lcp;
    suffix = (char *)_suffixes.data() + suffix_offset;
    int cmp = compareString(key_suffix.c_str(), suffix.c_str());
    if (cmp < 0) {
      rank = tmp_index;
      return false;
    } 
    found = cmp == 0;

    lcp += computeLCP(key_suffix.c_str(), suffix.c_str());
    suffix_offset += _suffix_lengths[tmp_index];
    ++tmp_index;
  }

  if (lcp != key_len) {
    rank = tmp_index;
    return false;
  } 
  rank = tmp_index - 1;
  return found;
}
