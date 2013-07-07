#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, const char *argv[])
{
  assert(argc == 2);
  float count = atof(argv[1]);
  cout << count << endl;

  uint32_t size = count * 1024 * 1024 * 1024;
  vector<char> v(size, 0);
  return 0;
}
