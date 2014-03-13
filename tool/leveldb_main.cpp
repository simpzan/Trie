#include <iostream>
#include <sstream>
#include <assert.h>
#include <vector>
#include <string>

#include "utils.h"
#include "timer.h"
#include "leveldb/db.h"

using namespace std;

void loadTokens(const char *fname, vector<string> &tokens) {
  string file_content;
  bool result = loadfile(fname, file_content);
  assert(result);

  splitString((char*)file_content.data(), file_content.size(), '\n', tokens);
}

string Int2String(int value_int) {
  stringstream ss;
  ss << value_int;
  return ss.str();
}

int String2Int(const string &value_string) {
  int value_int;
  stringstream ss(value_string);
  ss >> value_int;
  return value_int;
}

void importData(const vector<string> &tokens, leveldb::DB *db) {
  for (int i = 0; i < tokens.size(); ++i) {
    string token = tokens[i];
    string value = Int2String(token.size());
    db->Put(leveldb::WriteOptions(), token, value);
  }
}

void queryData(const vector<string> &tokens, leveldb::DB *db) {
  int step = 1000;
  int count = tokens.size();
  string value;
  Timer t;
  for (int i = 0; i < count; i+=step) {
    string token = tokens[i];
    db->Get(leveldb::ReadOptions(), token, &value);
  }
  t.Report(count/step);
}

int main(int argc, const char *argv[])
{
  assert(argc == 4);
  const char *idx_name = argv[1];
  string input_file = argv[3];
  string mode = argv[2];

  leveldb::DB* db;
  leveldb::Options options;
  options.create_if_missing = true;
  leveldb::Status status = leveldb::DB::Open(options, idx_name, &db);
  assert(status.ok());

  vector<string> tokens;
  loadTokens(input_file.c_str(), tokens);

  if (mode == "--import") {
    importData(tokens, db);
  } else if (mode == "--query"){
    queryData(tokens, db);
  }

  delete db;
  return 0;
}
