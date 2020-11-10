#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

using TFreqs = unordered_map<string_view, int>;
  
std::string buf;

static TFreqs CountWordFreqs(const string& path) {
  ifstream is(path, ios::binary);

  if (!is) {
    throw runtime_error("Could not open input file");
  }

  TFreqs result;
  result.reserve(1000000);

  buf.resize(256 * 1024 * 1024);

  is.read(&buf[0], buf.size());

  auto sz = is.gcount();
 
  int start = 0; 
  for (int i = 0; i < sz; ++i) {
    if (isalpha(buf[i])) {
      buf[i] |= ' ';
      start = i++;

      while (isalpha(buf[i])) {
        buf[i] |= ' ';
        ++i;
      }

      string_view v(&buf[start], i - start);

      auto it = result.find(v);
      if (it != result.end()) {
        ++it->second;
      } else {
        result.emplace(move(v), 1);
      }
    } 
  }
  
  return result;
}

static TFreqs CountWordFreqsSlow(const string& path) {
  ifstream is(path, ios::in);

  if (!is) {
    throw runtime_error("Could not open input file");
  }

  TFreqs result;

  char c;
  string word;

  while (is.get(c)) {
    if (isalpha(c)) {
      word += tolower(c);
    } else if (word.size()) {
      ++result[word];
      word.clear();
    }
  }  

  if (word.size()) {
    ++result[word];
  }

  return result;
}

static void PrintWordFreqs(const TFreqs& freqs, const string& path) {
  vector<pair<string_view, int>> stats;
  stats.reserve(freqs.size());
  
  for (const auto& p : freqs) {
    stats.push_back(p);
  }

  sort(stats.begin(), stats.end(), [](const auto& l, const auto& r) {
    return l.second == r.second ? l.first < r.first : l.second > r.second;
  });
  
  ofstream os(path);

  if (!os) {
    throw runtime_error("Could not open output file");
  }

  if (stats.empty()) {
    os << ' ';
    return;
  }

  auto statIt = stats.begin();

  os << statIt->second << ' ' << statIt->first;
  ++statIt;

  while (statIt != stats.end()) {
    os << '\n' << statIt->second << ' ' << statIt->first;
    ++statIt;
  }
}

int main(int argc, char** argv) {
  setlocale(LC_ALL, "C");
  
  if (argc != 3) {
    cerr << "Usage: ./freq in.txt out.txt" << endl;

    return 1;
  }
  
  (void)CountWordFreqsSlow;

  try {
    const auto freqs = CountWordFreqs(argv[1]);
    PrintWordFreqs(freqs, argv[2]);
  } catch (const exception& ex) {
    cerr << ex.what() << endl;
    
    return 2;
  }

  return 0;
}
