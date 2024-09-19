#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


struct Node{
  int data;
  Node* right;
  Node* down;
  Node(int val) : data(val), right(nullptr), down(nullptr) {}
};

void addNode(Node*& head, int val) {
  Node* newNode = new Node(val);
  if (head == nullptr) {
    head = newNode;
  }
  else {
    Node* current = head;
    while (current->right) {
      current = current->right;
    }
    current->right = newNode;
  }
}

void addRow(Node*& head, Node*& tempRowHeader) {
  if (head == nullptr) {
    head = tempRowHeader;
  }
  else {
    Node* current = head;
    while (current->down) {
      current = current->down;
    }
    current->down = tempRowHeader;
  }
}

void addRemainingDownPointers(Node*& head) {
  Node* mainR = head;
  Node* topR = mainR;
  Node* bottomR = mainR->down;

  while(bottomR) {
    while (topR) {
      topR->down = bottomR;
      topR = topR->right;
      bottomR = bottomR->right;
    }
    mainR = mainR->down;
    topR = mainR;
    bottomR = mainR->down;    
  }
}

void printList(const Node* head) {
  const Node* currRow = head;
  while (currRow) {
    const Node* current = currRow;
    while (current) {
      cout << current->data << " ";
      current = current->right;
    }
    cout << endl;
    
    currRow = currRow->down;
  }
}

int main(int argc, char* argv[]) {
  ifstream matrx;
  Node* matrixLst = nullptr;
  Node* currentRow = nullptr;
  
  matrx.open(argv[1]);

  if (!matrx.is_open()) {
    cout << "Could not open matrix files!" << endl;
  }
  else {
    string line;
    int num;
    while(getline(matrx, line)) {
      istringstream iss(line);
      while (iss >> num) {
        addNode(currentRow, num);
      }
      addRow(matrixLst, currentRow);
      currentRow = nullptr;
    }
  }

  matrx.close();

  addRemainingDownPointers(matrixLst);
  printList(matrixLst);

  
  return 0;
}