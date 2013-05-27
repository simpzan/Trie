#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "FrontCoder.h"
#include "utils.h"
#include "timer.h"

using namespace std;

int main(int argc, const char *argv[])
{
  vector<string> tokens;
  bool result = loadTokensFromFile("words.sorted", tokens);
  assert(result);
  
  FrontCoderBuilder coderBuilder;
  stringstream ss;
  coderBuilder.save(ss);
  cout << "encoded" << endl;

  FrontCoder coder;
  coder.load(ss);

  string token;
  uint32_t rank;
  int count = tokens.size();

  Timer timer;
  for (int i = 0; i < count; ++i) {
    token = tokens[i];
    coder.find(token, rank);
    if (i != rank) {
      cout << token << " expected:" << i << " actual:" << rank << endl;
    }
    assert(i == rank);
  }
  timer.Stop();
  cout << "count:" << count << endl;
  cout << "cpu time(s):" << timer.ElapsedTimeCPU() / 1000000
    << " avg(us):" << timer.ElapsedTimeCPU()/count << endl
    << "wall time(s):" << timer.ElapsedTime() / 1000000
    << " avg(us):" << timer.ElapsedTime()/count << endl;
  return 0;
}
