#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

string makeDigIntoWord(char num) {
  string numWords[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

  if('0' <= num && num <= '9') {
    int idx = num - '0';
    return numWords[idx];
  }
  else {
    return string(1,num);
  }
}

int main(int argc, char* argv[]) {
  ifstream read;
  ofstream write;
  
  read.open(argv[1]);
  write.open(argv[2]);

  if (read.is_open() && write.is_open()) {
    string line;
    while (getline(read, line)) {
      istringstream iss(line);
      string word;
      bool firstWord = true;

      while (iss >> word) {
        if (!firstWord) {
          write << " ";
        }
        firstWord = false;
        string modifiedWord;
        for (char ch: word) {
          modifiedWord += makeDigIntoWord(ch);
        }
        write << modifiedWord;
      }
      write << endl;
    }
  }
  else {
    cout << "Something went wrong! Check your file names! \n";
  }

  read.close();
  write.close();

  return 0;
}