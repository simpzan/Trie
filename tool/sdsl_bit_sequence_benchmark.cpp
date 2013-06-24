#include <fstream>
#include <sdsl/int_vector.hpp> // for the bit_vector class
#include <sdsl/bit_vectors.hpp>
#include <sdsl/util.hpp> // for counting the set bits in a bit_vector 
#include <sdsl/rank_support.hpp> // for rank data structures
#include <sdsl/bp_support.hpp>
#include <sdsl/nearest_neighbour_dictionary.hpp>
#include <sdsl/sd_vector.hpp>
#include <sdsl/gap_vector.hpp>

#include "PTrie.h"
#include "utils.h"
#include "timer.h"

using namespace std;
using namespace sdsl;

void test() {
  cout << "loading tokens" << endl;
  vector<string> tokens;
  loadTokensFromFile("words.sorted", tokens);

  cout << "building trie" << endl;
  PTrie trie;
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    string token = tokens[i];
    trie.addEntry(token.c_str(), token.size());
  }
  
  ofstream os("words.idx");
  assert(os.good());
  //trie.write(os);

  //cout << "searching trie" << endl;
  //TrieValueT value = trie.getEntry("test");
  //assert(value);
}

template <class BV, class Rank, class Select1, class Select0>
void bench_size(BV &bv, Rank &rs, Select1 &s1, Select0 &s0) {
  int size_bv = util::get_size_in_bytes(bv);
  int size_rank = util::get_size_in_bytes(rs);
  int size_s1 = util::get_size_in_bytes(s1);
  int size_s0 = util::get_size_in_bytes(s0);
  int size_all = size_bv + size_rank + size_s1 + size_s0;
  cout << size_bv << " + " << size_rank << "+" << size_s1 << "+" << size_s0 << endl
    << " = " << size_all << endl;
}

template <class BV, class Rank, class Select1, class Select0>
void bench_time(BV &bv, Rank &rs, Select1 &s1, Select0 &s0) {
  int pos = bv.size() / 3 + 3;

  Timer timer;
  int bv_result = bv[pos];
  int r1_result = rs(pos);
  int r0_result = pos - r1_result - 1;
  int s1_result = s1(1);
  int s0_result = s0(1);
  timer.Stop();
  cout << "rank time(us):" << timer.ElapsedTimeCPU() << endl;
  cout << pos << "\t" << bv_result << "\t" << r1_result << "\t" <<
    r0_result << "\t" << s1_result << "\t" << s0_result << endl;
  
}

void flip(bit_vector &bv) {
  int count = bv.size();
  for (int i = 0; i < count; ++i) {
    bv[i] = bv[i] ? 0 : 1;
  }
}

void testIds() {
  const char *filename = "../BTree/words.sorted.idx";
  uint32_t offset = 4;
  ifstream is(filename);
  is.seekg(offset);
  assert(is.good());

  rrr_vector<> bv;
  bv.load(is);
  rrr_select_support<> select(&bv);
  uint32_t size = util::get_size_in_bytes(select);
  cout << size << endl;


  uint32_t id = 0;
  int count = bv.size();
  for (int i = 0; i < count; ++i) {
    if (bv[i] == 0) {
      ++id;
      cout << id << "-" << i << "\t";

      if (id % 1000 == 0)  getchar();
    }
  }
}

void testNND() {
  const char *filename = "../enwiki.sbtrie";
  uint32_t offset = 106968971;
  ifstream is(filename);
  is.seekg(offset);
  assert(is.good());

  bit_vector bv;
  bv.load(is);
  //flip(bv);

  rank_support_v<> rs(&bv);
  select_support_mcl<> ss1(&bv);
  select_support_mcl<0, 1> ss0(&bv);

  uint32_t bit_count = bv.size();
  uint32_t ones = rs.rank(bit_count - 1);
  double ratio = (double)ones / bit_count;
  cout << ratio << endl;

  bench_size(bv, rs, ss1, ss0);
  bench_time(bv, rs, ss1, ss0);

  cout << "\nrrr" << endl;
  rrr_vector<> rrr(bv);
  rrr_rank_support<> rrr_rank(&rrr);
  rrr_select_support<> rrr_select(&rrr);
  rrr_select_support<0> rrr_select0(&rrr);
  bench_size(rrr, rrr_rank, rrr_select, rrr_select0);
  bench_time(rrr, rrr_rank, rrr_select, rrr_select0);

  cout << "\ngap" << endl;
  gap_vector<> gap(bv);
  gap_rank_support<> gap_r(&gap);
  gap_select_support<> gap_s(&gap);
  gap_select_support<> gap_s0(&gap);
  bench_size(gap, gap_r, gap_s, gap_s0);
  bench_time(gap, gap_r, gap_s, gap_s0);

  cout << "\nsd" << endl;
  sd_vector<> sd(bv);
  sd_rank_support<> sd_r(&sd);
  sd_select_support<> sd_s(&sd);
  sd_select_support<> sd_s0(&sd);
  bench_size(sd, sd_r, sd_s, sd_s0);
  bench_time(sd, sd_r, sd_s, sd_s0);

}

int main(int argc, const char *argv[]) {
  //test();
  testNND();
  //testIds();
  return 0;
}
