#include "STrieBuilder.h"

using namespace std;

void checkStrings(vector<string> &tokens) {
    cout << "token count:" << tokens.size() << endl;
    if (tokens.size() ==0) return;

    string lastkey = tokens[0];
    assert(lastkey.length() > 0);
    for (int i=1; i<tokens.size(); ++i) {
        string thiskey = tokens[i];
        assert(thiskey > lastkey);
        lastkey = thiskey;
    }
//    tokens.resize(tokens.size() - 1);
}

void build(string &input, string &output) {
    vector<string> tokens;
    loadTokensFromFile(input.c_str(), tokens);
    checkStrings(tokens);
    STrieBuilder builder(tokens);
    builder.build(output.c_str());
}

int main(int argc, const char *argv[]) {
  assert(argc > 1);
  string inputFile(argv[1]);
  string outputFile(inputFile + ".strie");
  if (argc > 2) {
    outputFile = argv[2];
  }
  build(inputFile, outputFile);
  return 0;
}
