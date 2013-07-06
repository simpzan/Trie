#include <vector>
#include <iostream>
#include <sdsl/int_vector.hpp>
#include <sdsl/enc_vector_theo.hpp>
#include <sdsl/enc_vector_prac2.hpp>
#include <sdsl/enc_vector.hpp>
#include <sdsl/enc_vector_dna.hpp>
#include <libcds/array.h>
#include "Vector.h"
#include "DACWrapper.h"
#include "timer.h"

using namespace std;
using namespace sdsl;

void test_enc_vector_dna(vector<uint32_t> &offsets, uint32_t size) {
  enc_vector_dna<> encoded(offsets);
  int size_after = util::get_size_in_bytes(encoded);
  float ratio = (float)size_after / size;
  cout << ratio << endl;
}

void test_enc_vector(vector<uint32_t> &offsets, uint32_t size) {
  enc_vector<> encoded(offsets);
  int size_after = util::get_size_in_bytes(encoded);
  float ratio = (float)size_after / size;
  cout << ratio << endl;
}

void test_dac(vector<uint32_t> &offsets, uint32_t size) {
  cout << "dac" << endl;
  Timer t;
  DACWrapper dac(offsets.data(), offsets.size());
  t.Report();
  stringstream ss;
  dac.write(ss);
  int size_after = ss.str().size();
  float ratio = (float)size_after / size;
  cout << ratio << endl;
}

void test_cds(vector<uint32_t> &offsets, uint32_t size) {
  cout << "cds" << endl;
  using namespace cds::basic;
  vector<unsigned long int> data;
  int count = offsets.size();
  for (int i = 0; i < count; ++i) {
    data.push_back(offsets[i]);
  }

  Timer t;
  Array *ap = Array::Create(data);
  t.Report();
  int size_after = ap->GetSize();
  float ratio = (float)size_after / size;
  cout << ratio << endl;
}

int main(int argc, const char *argv[])
{
  const char *filename = "../enwiki.sbtrie";
  uint32_t offset = 107017691;
  ifstream is(filename);
  is.seekg(offset);
  assert(is.good());

  Vector<uint32_t> offsets;
  offsets.read(is);
  offsets[1] = 7295u;
  cout << offsets[1] << endl;

  int count = offsets.count();
  int size = sizeof(uint32_t) * count;
  cout << size << endl;

  vector<uint32_t> data(10000, 0);
  data[1] = 4967290u;
  //vector<uint32_t> &data = offsets.vector();
 // test_enc_vector_dna(data, size);
  test_enc_vector(data, size);
  test_dac(data, size);
  test_cds(data, size);
  return 0;
}
