#include "Utilities.h"

using namespace std;

void assertWithString(bool assert, const std::string &str) {
  if (assert)  return;
  std::cout << "Assert failed:" << endl;
  std::cout << str << endl;
  exit(1);
}

void reportLength(istream &is, uint32_t lastPosition, const string &msg) {
  uint32_t size = (uint32_t)is.tellg() - lastPosition;
  cout << msg << " : " << lastPosition << "," << size << endl;
}
