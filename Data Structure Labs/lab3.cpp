#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Node {
public: 
  int data;
  Node* next;
  
};

void addNodeToEndLL(Node** head_ref, int new_data) {
  Node* new_node = new Node();
  new_node->data = new_data;

  Node* last = *head_ref;

  new_node->next = nullptr;

  //if list is empty
  if (*head_ref == nullptr) {
    *head_ref = new_node;
    return;
  }
  while (last->next != nullptr) {
    last = last->next;
  }
  last->next = new_node;
}

void reverseOrderLL(Node** head_ref) {
  Node* prev = nullptr;
  Node* curr = *head_ref;
  Node* next = nullptr;

  while (curr != nullptr) {
    next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
  }
  *head_ref = prev;
}

void printList(Node* node) {
  while (node != nullptr) {
    cout << node->data << endl;
    node = node->next;
  }
}


int main(int argc, char* argv[]) {
  ifstream read;
  read.open(argv[1]);

  if (!read.is_open()) {
    cout << "Error opening input file!!" << endl;
  }
  else {
    Node* head = nullptr;
  
    string num;
    while (read >> num) {
      addNodeToEndLL(&head, stoi(num));
    }
    printList(head);
    cout << endl;
    reverseOrderLL(&head);
    printList(head);
  }
}