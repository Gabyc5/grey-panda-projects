#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;

/* This program converts infix mathematical expressions (with operators like +, -, *, /, and ^) 
to postfix notation using the Shunting Yard algorithm. It supports expressions with parentheses and operators of varying precedence. The converted postfix expression is then evaluated to 
provide the result */


class charStack {
  public: 
      int top;
      char stackArr[100];
      charStack() {top = -1; }
      void add(char addVal);
      char pop();
      char peek();
      int size();
      bool isEmpty();
};

void charStack::add(char addVal) {
  if (top >= 90) {
    return;
  }
  stackArr[++top] = addVal;
}

char charStack::pop() {
  if (top < 0) {
    return 0;
  }
  char topVal = stackArr[top--];
  return topVal;
}

char charStack::peek() {
  if (top < 0) {
    return 0;
  }
  char topVal = stackArr[top];
  return topVal;
}

int charStack::size() {
  return (top + 1);
}

bool charStack::isEmpty() {
  return (top < 0);
}



class fltStack {
  public:
      int top;
      float stackArr[100];
      fltStack() {top = -1; }
      void add(float addVal);
      float pop();
};

void fltStack::add(float addVal) {
  if (top >= 90) {
    return;
  }
  stackArr[++top] = addVal;
}

float fltStack::pop() {
  if (top < 0) {
    return 0;
  }
  float topVal = stackArr[top--];
  return topVal;
}



int operatorOrder(char op) {
  if (op == '^') {
    return 3;
  }
  else if (op == '/' || op == '*') {
    return 2;
  }
  else if (op == '+' || op == '-') {
    return 1;
  }
  else {
    return -1;
  }
  
}

string postfixConvertor(string exprs) {
  charStack oprt;
  string ans = "";

  for (int i = 0; i < exprs.length(); i++) {
    char c = exprs[i];

    if (c >= '0' && c <= '9') {
      ans += c;
    }
    else if (c == '(') {
      oprt.add(c);      
    }
    else if (c == ')') {
      while (oprt.peek() != '(') {
        ans += oprt.peek();
        oprt.pop();
      }
      oprt.pop();
    }
    else {
      while (!oprt.isEmpty() && operatorOrder(c) <= operatorOrder(oprt.peek())) {
        ans += oprt.peek();
        oprt.pop();
      }
      oprt.add(c);
    }
  }

  while (!oprt.isEmpty()) {
    ans += oprt.peek();
    oprt.pop();
  }
  return ans;
}

float postfixEval(string postfx) {
  fltStack answer;
  
  bool valid = false;
  int num = 0;
  int operators = 0;
  for (int i = 0; i < postfx.length(); i++) {
    if (postfx[i] >= '0' && postfx[i] <= '9') {
      num++;
    }
    else if (postfx[i] == '^' || postfx[i] == '*' || postfx[i] == '/' || postfx[i] == '+' || postfx[i] == '-') {
      operators++;
    }
}

  if (('0' <= postfx[0] && postfx[0] <= '9')  && ('0' <= postfx[1] && postfx[1] <= '9') && (postfx[num+operators-1] == '^' || postfx[num+operators-1] == '*' || postfx[num+operators-1] == '/' || postfx[num+operators-1] == '+' || postfx[num+operators-1] == '-')) {
    if (operators == num - 1) {
      valid = true;
    }
  }

  if (valid) {
    for (int i = 0; i < postfx.length(); i++) {
      if ('0' <= postfx[i] && postfx[i] <= '9') {
        if (postfx[i] == '0') {
          answer.add(0);
        }
        else if (postfx[i] == '1') {
          answer.add(1);
        }
        else if (postfx[i] == '2') {
          answer.add(2);
        }
        else if (postfx[i] == '3') {
          answer.add(3);
        }
        else if (postfx[i] == '4') {
          answer.add(4);
        }
        else if (postfx[i] == '5') {
          answer.add(5);
        }
        else if (postfx[i] == '6') {
          answer.add(6);
        }
        else if (postfx[i] == '7') {
          answer.add(7);
        }
        else if (postfx[i] == '8') {
          answer.add(8);
        }
        else if (postfx[i] == '9') {
          answer.add(9);
        }
      }
      else if (postfx[i] == '^') {
        float val1 = answer.pop();
        float val2 = answer.pop();

        float ans = pow(val2, val1);
        answer.add(ans);
      }
      else if (postfx[i] == '*') {
        float val1 = answer.pop();
        float val2 = answer.pop();

        float ans = val2 * val1;
        answer.add(ans);
      }
      else if (postfx[i] == '/') {
        float val1 = answer.pop();
        float val2 = answer.pop();

        float ans = val2 / val1;
        answer.add(ans);
      }
      else if (postfx[i] == '+') {
        float val1 = answer.pop();
        float val2 = answer.pop();

        float ans = val2 + val1;
        answer.add(ans);
      }
      else if (postfx[i] == '-') {
        float val1 = answer.pop();
        float val2 = answer.pop();

        float ans = val2 - val1;
        answer.add(ans);
      }
    }
    return answer.pop();
  }
  else {
    return -1;
  }
}

int main(int argc, char** argv) {
  int choice = stoi(argv[1]);
  string givenExprss = argv[2];
  string filename = argv[3];

  ofstream outFS;
  outFS.open(filename);

  if (choice == 2) {
    outFS << postfixConvertor(givenExprss) << endl;    
  }
  else if (choice == 3) {
    if (postfixEval(givenExprss) == -1) {
      outFS << "nv" << endl;
    }
    else {
      outFS << fixed << setprecision(1) << postfixEval(givenExprss) << endl;
    }
  }

  outFS.close();
  
  return 0;
}