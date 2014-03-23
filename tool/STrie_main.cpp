#include "STrie.h"
#include "timer.h"

using namespace std;

const uint32_t RUNS = 5;

void runARun(STrie &trie, std::vector<string> &tokens) {
  uint32_t value;
  bool found;
  const char* token;
  uint32_t loopCount = tokens.size();

  for (int i=0; i<loopCount; ++i) {
    token = tokens[i].c_str();
    found = trie.find(token, value);
    if (!found) {
      cout << token << ":" << value << endl;
      assert(found);
      assert(value);
    }
  }
}

void runTest(STrie &trie, std::vector<string> &tokens) {

  double cpuTime = 0;
  double wallTime = 0;
  for (int i=0; i<RUNS; ++i) {
    Timer timer;
    runARun(trie, tokens);
    timer.Stop();
    double ct = timer.ElapsedTimeCPU()/1000000;
    double wt = timer.ElapsedTime()/1000000;
    cout << i << "     (s) cpu:" << ct << " wall:" << wt << endl;
    cpuTime += ct;
    wallTime += wt;
    sleep(3);
  }
  cpuTime /= RUNS;
  wallTime /= RUNS;
  cout << "RunAvg(s) cpu:" << cpuTime << "  wall:" << wallTime << endl;

  uint32_t count = tokens.size();
  cpuTime /= count;
  wallTime /= count;
  cpuTime *= 1000000;
  wallTime *= 1000000;
  cout << "PatternAvg(us) cpu:" << cpuTime << " wall:" << wallTime << endl;
}

void run(string &indexFile, string &patternFile) {
  STrie trie;
  trie.load(indexFile.c_str());

  std::vector<string> tokens;
  loadTokensFromFile(patternFile.c_str(), tokens);

  runTest(trie, tokens);
}

int main(int argc, const char *argv[]) {
  assert(argc > 2);
  string indexFile(argv[1]);
  string patternFile(argv[2]);

  run(indexFile, patternFile);

  return 0;
}
