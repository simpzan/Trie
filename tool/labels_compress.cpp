#include "Vector.h"
#include "DACWrapper.h"

using namespace std;

void encode(const vector<uint8_t> &labels, vector<uint32_t> &encoded) {
  int count = labels.size();
  if (count == 0)  return;
 
  int i = 0; 
  int j = 0;
  while (i < count) {
    encoded.push_back(labels[i]);
    ++i;
    ++j;

    for (; i < count; ++i) {
      int diff = labels[i] - labels[i - 1];
      if (diff <= 0)  break;
      encoded.push_back(diff);
    }
  }

  cout << "nodes:" << j << endl;
}

int main(int argc, const char *argv[])
{
  ifstream is("labels.vector");
  assert(is.good());

  Vector<uint8_t> labels;
  labels.read(is);

  vector<uint32_t> encoded;
  encode(labels.vector(), encoded);

  int count = labels.count();
  for (int i = 0; i < count; ++i) {
    cout << labels[i] << " - " << encoded[i] << endl;
  }
  

  DACWrapper dac(encoded);
  dac[0];
  dac[1];
  string suffix2 = ".com";
  string filename = '9' + suffix2;
  ofstream os_compressed(filename.c_str());
  assert(os_compressed);
  dac.write(os_compressed);
  return 0;
}
