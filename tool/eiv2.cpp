#include <vector>
#include <iostream>
#include <sdsl/int_vector.hpp>
#include <sdsl/enc_vector_theo.hpp>
#include <sdsl/enc_vector_prac2.hpp>
#include <sdsl/enc_vector.hpp>
#include <sdsl/enc_vector_dna.hpp>

using namespace std;
using namespace sdsl;

int main(int argc, const char *argv[])
{
  int count = 8000000;
  vector<uint32_t> offsets(count);
  for (int i = 0; i < count; ++i) {
    offsets[i] = i*2+3;
  }
  
  int size = sizeof(uint32_t) * count;
  cout << size << endl;

  enc_vector_dna<> encoded(offsets);
  int size_after = util::get_size_in_bytes(encoded);
  float ratio = (float)size_after / size;
  cout << ratio << endl;
  
  return 0;
}
