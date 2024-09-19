#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct Node {
  int data;
  Node* next;
  Node(float val) : data(val), next(nullptr) {}
};

void addNode(Node*& head, int val) {
  Node* newNode = new Node(val);

  if (head == nullptr) {
    head = newNode;
  }
  else {
    Node* current = head;
    while (current->next) {
      current = current->next;
    }
    current->next = newNode;
  }
}

Node* removeElements(Node* head, int target) {
  Node* current = head;

  if (current == nullptr) {
    return nullptr; 
  }
  
  if (current->data == target) {
    return removeElements(current->next, target);
  }
  else {
    current->next = removeElements(current->next, target);
    return current;
  }
}

void printNumbers(const Node* head, const string& filename) {
  ofstream outputFile(filename);

  const Node* current = head;
  while (current) {
    outputFile << current->data << " ";
    current = current->next;
  }

  outputFile.close();
}

int main(int argc, char* argv[]) {
  Node* intList = nullptr;
  ifstream inputFile;
  string line;
  int num;

  int targetVal = stoi(argv[2]);

  inputFile.open(argv[1]);
  getline(inputFile, line);
  istringstream iss(line);
  while (iss >> num) {
    addNode(intList, num);
  }
  inputFile.close();  

  printNumbers(removeElements(intList, targetVal), "output.txt");
  
  return 0;
}