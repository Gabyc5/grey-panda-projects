#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;


/* This program analyzes the frequency of words in a given text file. It reads the text, counts 
the occurrences of each word, and outputs the word frequencies to a result file. The analysis 
helps in understanding the distribution and prominence of words within the text */


int findFirstAndThirdFrq(vector<string> givenWords, vector<int> givenCount, int question) {
  int highestFrequencyInFile = givenCount[0];
  int secondHighestFrq = givenCount[0], thrdHighestFrq = givenCount[0];
  for (int i = 1; i < givenCount.size(); i++) {
    int value = givenCount[i];
    if (value > highestFrequencyInFile) {
      thrdHighestFrq = secondHighestFrq;
      secondHighestFrq = highestFrequencyInFile;
      highestFrequencyInFile = value;
    }
    else if (value > secondHighestFrq) {
      thrdHighestFrq = secondHighestFrq;
      secondHighestFrq = value;
    }
    else if (value > thrdHighestFrq) {
      thrdHighestFrq = value;
    } 
  }

  if (question == 1) {
    return highestFrequencyInFile;
  }
  else if (question == 2) {
    return thrdHighestFrq;
  }

  return 0;
}

int main(int argc, char* argv[]) {
  ifstream inFS;
  string line;
  string currWord;
  vector<vector<char>> allSentences;
  vector<char> currSentence;
  vector<string> sentences; 
  vector<string> words;
  vector<int> wordCount;
  vector<vector<string>> strAllSentences;
  vector<string> strCurrSentence;
  
  inFS.open(argv[1]);
  if (!inFS.is_open()) {
    cout << "Error opening input file!" << endl;
  }
  else {
    while (getline(inFS, line)) {
      for (int i = 0; i < line.length(); i++) {  
        if (line.at(i) != '.') {
          line.at(i) = tolower(line.at(i));
          currSentence.push_back(line.at(i));
        }
      
        if (i == line.length() - 1 || line.at(i) == '.') {
          allSentences.push_back(currSentence);
          currSentence.clear();
        }
      }
    }
  }
  inFS.close();
    
  for (int i = 0; i < allSentences.size(); i++) {
    string str(allSentences[i].begin(), allSentences[i].end());
    sentences.push_back(str);
  }

  //Go through sentences and look through words to answer q1 and 2
  for (string currSentence: sentences) {
    istringstream iss(currSentence);
    string currWord;

    strCurrSentence.clear();
    while (iss >> currWord) {
      strCurrSentence.push_back(currWord);
      bool isUnique = true;
      for (int i = 0; i < words.size(); i++) {
        if (words[i] == currWord) {
          isUnique = false;
          wordCount[i]++;
        }
      } 
      if (isUnique) {
        words.push_back(currWord);
        wordCount.push_back(1);
      }
    }
    strAllSentences.push_back(strCurrSentence);
  }

  for (int j = 1; j <= 9; j++) {
    ostringstream outputFileName;
    outputFileName << argv[2] << j << ".txt";
    ofstream outFS;
    outFS.open(outputFileName.str());

    if (!outFS.is_open()) {
      cout << "Output file could not be opened!" << endl;
    }
    else {
      if (j == 1) {
        int highestFrq = findFirstAndThirdFrq(words, wordCount, 1);
        for (int i = 0; i < wordCount.size(); i++) {
          if (wordCount[i] == highestFrq) {
            outFS << words[i] << ":" << highestFrq << endl;
          }
        }
      }
      
      if (j == 2) {
        int thrdFrq = findFirstAndThirdFrq(words, wordCount, 2);
        for (int i = 0; i < wordCount.size(); i++) {
          if (wordCount[i] == thrdFrq) {
            outFS << words[i] << ":" << thrdFrq << endl;
          }
        }
      }
      
      if (j == 3) {
        vector<string> ansWords;
        vector<int> ansSentence;
        int max = 0;

        for (int i = 0; i < strAllSentences.size(); i++) {
          for (int allWords = 0; allWords < words.size(); allWords++) {
            int currCount = 0;
            for (int k = 0; k < strAllSentences[i].size(); k++) {
              if (words[allWords] == strAllSentences[i][k]) {
                currCount++;
              }
            }
            if (currCount > max) {
              max = currCount;
              ansSentence.clear();
              ansWords.clear();
              ansSentence.push_back(i);
              ansWords.push_back(words[allWords]);
            }
            else if (currCount == max) {
              ansSentence.push_back(i);
              ansWords.push_back(words[allWords]);
            }
          }
        }

        for (int i = 0; i < ansWords.size(); i++) {
          outFS << ansWords[i] << ":" << max << ":";
          for (int j = 0; j < strAllSentences[ansSentence[i]].size(); j++) {
            outFS << strAllSentences[ansSentence[i]][j] << " ";
          }
          outFS << endl;
        }
      }
      
      if (j == 4) {
        int maxTheCount = -1;
        vector<int> maxTheLoc;
      
        for (int i = 0; i < strAllSentences.size(); i++) {
          int theCount = 0;
          for (int k = 0; k < strAllSentences[i].size(); k++) {
            if (strAllSentences[i][k] == "the") {
              theCount++;
            }
          }
          if (theCount > maxTheCount) {
            maxTheCount = theCount;
            maxTheLoc.clear();
            maxTheLoc.push_back(i);
          }
          else if (theCount == maxTheCount) {
            maxTheLoc.push_back(i);
          }
        }


        if (!maxTheLoc.empty()) {
          for (int i = 0; i < maxTheLoc.size(); i++) {
            outFS << "the:" << maxTheCount << ":";
            for (int j = 0; j < strAllSentences[maxTheLoc[i]].size(); j++) {
              outFS << strAllSentences[maxTheLoc[i]][j] << " ";
            }
            outFS << endl;
          }
        }
      }
      
      if (j == 5) {
        int maxOfCount = -1;
        vector<int> maxOfLoc;
      
        for (int i = 0; i < strAllSentences.size(); i++) {
          int ofCount = 0;
          for (int k = 0; k < strAllSentences[i].size(); k++) {
            if (strAllSentences[i][k] == "of") {
              ofCount++;
            }
          }
          if (ofCount > maxOfCount) {
            maxOfCount = ofCount;
            maxOfLoc.clear();
            maxOfLoc.push_back(i);
          }
          else if (ofCount == maxOfCount) {
            maxOfLoc.push_back(i);
          }
        }


        if (!maxOfLoc.empty()) {
          for (int i = 0; i < maxOfLoc.size(); i++) {
            outFS << "of:" << maxOfCount << ":";
            for (int j = 0; j < strAllSentences[maxOfLoc[i]].size(); j++) {
              outFS << strAllSentences[maxOfLoc[i]][j] << " ";
            }
            outFS << endl;
          }
        }
      }
      
      if (j == 6) {
        int maxWasCount = -1;
        vector<int> maxWasLoc;
      
        for (int i = 0; i < strAllSentences.size(); i++) {
          int wasCount = 0;
          for (int k = 0; k < strAllSentences[i].size(); k++) {
            if (strAllSentences[i][k] == "was") {
              wasCount++;
            }
          }
          if (wasCount > maxWasCount) {
            maxWasCount = wasCount;
            maxWasLoc.clear();
            maxWasLoc.push_back(i);
          }
          else if (wasCount == maxWasCount) {
            maxWasLoc.push_back(i);
          }
        }
        if (!maxWasLoc.empty()) {
          for (int i = 0; i < maxWasLoc.size(); i++) {
            outFS << "was:" << maxWasCount << ":";
            for (int j = 0; j < strAllSentences[maxWasLoc[i]].size(); j++) {
              outFS << strAllSentences[maxWasLoc[i]][j] << " ";
            }
            outFS << endl;
          }
        }
      }
      
      if (j == 7) {
        int maxTargetCount = -1;
        vector<int> maxTargetLoc;
      
        for (int i = 0; i < strAllSentences.size(); i++) {
          int targetCount = 0;
          for (int k = 0; k < strAllSentences[i].size(); k++) {
            if (strAllSentences[i][k] == "but") {
              if (strAllSentences[i][k+1] == "the") {
                targetCount++;
              }
            }
          }
          if (targetCount > maxTargetCount) {
            maxTargetCount = targetCount;
            maxTargetLoc.clear();
            maxTargetLoc.push_back(i);
          }
          else if (targetCount == maxTargetCount) {
            maxTargetLoc.push_back(i);
          }
        }
        if (!maxTargetLoc.empty()) {
          for (int i = 0; i < maxTargetLoc.size(); i++) {
            outFS << "but the:" << maxTargetCount << ":";
            for (int j = 0; j < strAllSentences[maxTargetLoc[i]].size(); j++) {
              outFS << strAllSentences[maxTargetLoc[i]][j] << " ";
            }
            outFS << endl;
          }
        }
      }
      
      if (j == 8) {
        int maxTargetCount = -1;
        vector<int> maxTargetLoc;
      
        for (int i = 0; i < strAllSentences.size(); i++) {
          int targetCount = 0;
          for (int k = 0; k < strAllSentences[i].size(); k++) {
            if (strAllSentences[i][k] == "it" && k != strAllSentences[i].size() - 1) {
              if (strAllSentences[i][k+1] == "was") {
                targetCount++;
              }
            }
          }
          if (targetCount > maxTargetCount) {
            maxTargetCount = targetCount;
            maxTargetLoc.clear();
            maxTargetLoc.push_back(i);
          }
          else if (targetCount == maxTargetCount) {
            maxTargetLoc.push_back(i);
          }
        }
        if (!maxTargetLoc.empty()) {
          for (int i = 0; i < maxTargetLoc.size(); i++) {
            outFS << "it was:" << maxTargetCount << ":";
            for (int j = 0; j < strAllSentences[maxTargetLoc[i]].size(); j++) {
              outFS << strAllSentences[maxTargetLoc[i]][j] << " ";
            }
            outFS << endl;
          }
        }
      }
      
      if (j == 9) {
        int maxTargetCount = -1;
        vector<int> maxTargetLoc;
      
        for (int i = 0; i < strAllSentences.size(); i++) {
          int targetCount = 0;
          for (int k = 0; k < strAllSentences[i].size(); k++) {
            if (strAllSentences[i][k] == "in" && k != strAllSentences[i].size() - 1) {
              if (strAllSentences[i][k+1] == "my") {
                targetCount++;
              }
            }
          }
          if (targetCount > maxTargetCount) {
            maxTargetCount = targetCount;
            maxTargetLoc.clear();
            maxTargetLoc.push_back(i);
          }
          else if (targetCount == maxTargetCount) {
            maxTargetLoc.push_back(i);
          }
        }
        if (!maxTargetLoc.empty()) {
          for (int i = 0; i < maxTargetLoc.size(); i++) {
            outFS << "in my:" << maxTargetCount << ":";
            for (int j = 0; j < strAllSentences[maxTargetLoc[i]].size(); j++) {
              outFS << strAllSentences[maxTargetLoc[i]][j] << " ";
            }
            outFS << endl;
          }
        }
      }
      
    }
    outFS.close();
  }
}