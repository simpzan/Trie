#include <iostream>
#include <sdsl/bit_vectors.hpp>

using namespace sdsl;
using namespace std;

int main(int argc, const char *argv[])
{
  bit_vector bv;
  bv.resize(1);
  bv[0] = 1;
  cout << bv[0] << endl;

  bv.resize(0);
  bv.resize(1);
  cout << bv[0] << endl;

  return 0;
}
