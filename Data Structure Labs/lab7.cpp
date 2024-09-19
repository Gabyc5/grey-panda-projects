#include <iostream>
#include <fstream>
using namespace std;

class Stack {
  public:
    int top;
    char stackArr[100];
    Stack() {top = -1; }
    void push(char addVal);
    char pop();
    char peek();
    bool isEmpty();
};

void Stack::push(char addVal) {
  if (top >= 999) {
    cout << "There is not enough room for adding new input" << endl;
    return;
  }
  stackArr[++top] = addVal;
}

char Stack::pop() {
  if (top < 0) {
    cout << "There are no elements in this stack" << endl;
    return 0;
  }
  
  char topVal = stackArr[top--];
  return topVal;
}

char Stack::peek() {
  if (top < 0) {
    cout << "There are no elements in this stack" << endl;
    return 0;
  }

  char topVal = stackArr[top];
  return topVal;
}

bool Stack::isEmpty() {
  return (top < 0);
}

int main(int argc, char** argv) {
  Stack diffSigns;
  ifstream inFS;
  ofstream outFS;
  char sign;

  inFS.open(argv[1]);
  while(inFS.get(sign)) {
    if (sign == '(' || sign == '[' || sign == '{') {
      diffSigns.push(sign);
    }
    else if ((sign == ')' && !diffSigns.isEmpty() && diffSigns.peek() == '(') || (sign == ']' && !diffSigns.isEmpty() && diffSigns.peek() == '[') || (sign == '}' && !diffSigns.isEmpty() && diffSigns.peek() == '{')) {
      diffSigns.pop();
    }
  }
  inFS.close();  

  //Checking to see if stack is empty, if it is all signs match
  outFS.open(argv[2]);
  if (diffSigns.isEmpty()) {
    outFS << "True";
  }
  else {
    outFS << "False";
  }
  outFS.close();

  return 0;
}