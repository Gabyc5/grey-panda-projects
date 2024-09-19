#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/* This program solves the Tower of Hanoi puzzle for different numbers of pegs and disks. It generates and writes the steps required to move the disks from the source peg to the destination peg using the specified number of pegs. The steps are recorded in an output file */


// Added clearFile function due to the nature of appending to output file 
// file needs to be cleared in case the same file name is called again
void clearFile(const string& filename) {
  ofstream outFS(filename, ios::out | ios::trunc);
  outFS.close();
}

void tower5Pegs(int diskAmount, char src, char dest, char temp1, char temp2, char temp3, string filename) {
  ofstream outFS(filename, ios::app);
  if (diskAmount == 0) {
    return;
  }
  if (diskAmount == 1) {
    outFS << diskAmount << " " << src << " " << temp1 << endl;
    outFS << diskAmount << " " << temp1 << " " << temp2 << endl;
    outFS << diskAmount << " " << temp2 << " " << temp3 << endl;
    outFS << diskAmount << " " << temp3 << " " << dest << endl;
    return;
  }
  if (diskAmount == 2) {
    outFS << diskAmount - 1 << " " << src << " " << temp1 << endl;
    outFS << diskAmount << " " << src << " " << dest << endl;
    outFS << diskAmount - 1 << " " << temp1 << " " << temp2 << endl;
    outFS << diskAmount - 1 << " " << temp2 << " " << temp3 << endl;
    outFS << diskAmount - 1 << " " << temp3 << " " << dest << endl;
    return;
  }
  tower5Pegs(diskAmount - 3, src, temp1, temp2, temp3, dest, filename);
  outFS << diskAmount - 2 << " " << src << " " << temp2 << endl;
  outFS << diskAmount - 1 << " " << src << " " << temp3 << endl;
  outFS << diskAmount << " " << src << " " << dest << endl;
  outFS << diskAmount - 1 << " " << temp3 << " " << dest << endl;
  outFS << diskAmount - 2 << " " << temp2 << " " << dest << endl;  
  tower5Pegs(diskAmount - 3, temp1, dest, src, temp2, temp3, filename);
  outFS.close();
}

void tower4Pegs(int diskAmount, char src, char dest, char temp1, char temp2, string filename) {
  ofstream outFS(filename, ios::app);
  if (diskAmount == 0) {
    return;
  }
  if (diskAmount == 1) {
    outFS << diskAmount << " " << src << " " << temp1 << endl;
    outFS << diskAmount << " " << temp1 << " " << temp2 << endl;
    outFS << diskAmount << " " << temp2 << " " << dest << endl;
    return;
  }
  if (diskAmount == 2) {
    outFS << diskAmount - 1 << " " << src << " " << temp1 << endl;
    outFS << diskAmount << " " << src << " " << dest << endl;
    outFS << diskAmount - 1 << " " << temp1 << " " << temp2 << endl;
    outFS << diskAmount - 1 << " " << temp2 << " " << dest << endl;
    return;
  }
  tower4Pegs(diskAmount - 2, src, temp1, temp2, dest, filename);
  outFS << diskAmount - 1 << " " << src << " " << temp2 << endl;
  outFS << diskAmount << " " << src << " " << dest << endl;
  outFS << diskAmount - 1 << " " << temp2 << " " << dest << endl;
  tower4Pegs(diskAmount - 2, temp1, dest, src, temp2, filename);
  outFS.close();
}

void tower3Pegs(int diskAmount, char src, char dest, char temp, string filename) {
  ofstream outFS(filename, ios::app);
  if (diskAmount > 0) {
    tower3Pegs(diskAmount - 1, src, temp, dest, filename);
    outFS << diskAmount << " " << src << " " << dest << endl;
    tower3Pegs(diskAmount - 1, temp, dest, src, filename);
  }
  outFS.close();
}


int main(int argc, char** argv) {
  int numPegs = stoi(argv[1]);
  int numDisks = stoi(argv[2]);
  string givenFile = argv[3];

  if (numPegs == 3) {
    clearFile(givenFile);
    tower3Pegs(numDisks, 'A', 'C', 'B', givenFile);
  }
  else if (numPegs == 4) {
    clearFile(givenFile);
    tower4Pegs(numDisks, 'A', 'D', 'B', 'C', givenFile);
  }
  else if (numPegs == 5) {
    clearFile(givenFile);
    tower5Pegs(numDisks, 'A', 'E', 'B', 'C', 'D', givenFile);
  }

  return 0;
}