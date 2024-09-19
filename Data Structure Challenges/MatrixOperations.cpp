#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstring>
using namespace std;


/* This code performs operations on matrices, including calculating the product of two matrices 
and computing the transpose of a matrix. It reads matrices from an input file, processes them 
according to specified operations, and writes the results to an output file */


struct Node{
  float data;
  Node* right;
  Node* down;
  Node(float val) : data(val), right(nullptr), down(nullptr) {}
};

void addNode(Node*& head, float val) {
  Node* newNode = new Node(val);
  Node* topRow = head;
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

void printList(const Node* head, string& filename) {
  ofstream outputFile(filename);

  const Node* currRow = head;
  while (currRow) {
    const Node* current = currRow;
    while (current) {
      outputFile << fixed << setprecision(1) << current->data << " ";
      current = current->right;
    }
    outputFile << endl;
    
    currRow = currRow->down;
  }

  outputFile.close();
}

void printDeterm(float ans, string& filename) {
  ofstream outputFile(filename);
  outputFile << fixed << setprecision(1) << round(ans) << endl;
  outputFile.close();
}


void addMatrix(Node* head1, Node* head2, Node*& addMatrx) {
  int modData;
  Node* addMatrxRow = nullptr;
  
  Node* currRow1 = head1;
  Node* currRow2 = head2;
  while (currRow1 && currRow2) {
    Node* current1 = currRow1;
    Node* current2 = currRow2;
    while (current1 && current2) {
      modData = current1->data + current2->data;
      addNode(addMatrxRow, modData);
      current1 = current1->right;
      current2 = current2->right;
    } 
    addRow(addMatrx, addMatrxRow);
    addMatrxRow = nullptr;
    currRow1 = currRow1->down;
    currRow2 = currRow2->down;
  }
}

void subMatrix(Node* head1, Node* head2, Node*& subMatrx) {
  float modData;
  Node* subMatrxRow = nullptr;
  
  Node* currRow1 = head1;
  Node* currRow2 = head2;
  while (currRow1 && currRow2) {
    Node* current1 = currRow1;
    Node* current2 = currRow2;
    while (current1 && current2) {
      modData = current1->data - current2->data;
      addNode(subMatrxRow, modData);
      current1 = current1->right;
      current2 = current2->right;
    } 
    addRow(subMatrx, subMatrxRow);
    subMatrxRow = nullptr;
    currRow1 = currRow1->down;
    currRow2 = currRow2->down;
  }
}

void multMatrix(Node* head1, Node* head2, Node*& multMatrx) {
  Node* row1 = head1;
  Node* col2 = head2;
  Node* resultRow = nullptr;

  while (row1) {
    col2 = head2;
    while (col2) {
      float sum = 0.0;
      Node* current1 = row1;
      Node* current2 = col2;
      while(current1 && current2) {
        sum += current1->data * current2->data;
        current1 = current1->right;
        current2 = current2->down;
      }
      addNode(resultRow, sum);
      col2 = col2->right;
    }
    addRow(multMatrx, resultRow);
    resultRow = nullptr;
    row1 = row1->down;
  }
}

void transpMatrix(Node* head, Node*& transpMatrx) {
  Node* currNd = head;
  Node* currLine = currNd;
  Node* resultRow = nullptr;
  
  while (currNd) {
    while (currLine) {
      addNode(resultRow, currLine->data);
      currLine = currLine->down;
    }
    addRow(transpMatrx, resultRow);
    resultRow = nullptr;
    currNd = currNd->right;
    currLine = currNd;
  }
}

float determinant(Node* head) {
  Node* mainTop = head;
  Node* belowTop = head->down;
  
  while (belowTop) {
    Node* topNode = mainTop;
    Node* mainBottom = belowTop;
    Node* bottomNode = belowTop;
    while (mainBottom) {
      float ratioX = (-1) * (bottomNode->data / topNode->data);
      while (bottomNode) {
        float val = bottomNode->data;
        bottomNode->data = (ratioX * topNode->data) + val;
        topNode = topNode->right;
        bottomNode = bottomNode->right;
      }
      topNode = mainTop;
      mainBottom = mainBottom->down;
      bottomNode = mainBottom;
    }
    mainTop = mainTop->down->right;
    belowTop = mainTop->down;
  }

  Node* mainRow = head;
  int valToMove = -1;
  float result = 1;
  while (mainRow) {
    Node* curr = mainRow;
    valToMove++;
    for (int i = 0; i < valToMove; i++) {
      curr = curr->right;
    }
    result *= curr->data;
    mainRow = mainRow->down;
  }
  return result;
}

int main(int argc, char* argv[]) {
  ifstream matrx1;
  ifstream matrx2;
  string answerFile;
  Node* matrix1Head = nullptr;
  Node* matrix2Head = nullptr;
  Node* currentRow = nullptr;
  Node* addMatrxHead = nullptr;
  Node* subMatrxHead = nullptr;
  Node* multMatrxHead = nullptr;
  Node* traMatrxHead = nullptr;
  int matrix1Col = 0, matrix1Rows = 0, matrix2Col = 0, matrix2Rows = 0;

  ostringstream arg1;
  arg1 << argv[1];
  
  if (arg1.str() == "add" || arg1.str() == "sub" || arg1.str() == "mul") {
    ostringstream arg2;
    arg2 << argv[2];
    ostringstream arg3;
    arg3 << argv[3];
    ostringstream arg4;
    arg4 << argv[4];
    
    matrx1.open(arg2.str());
    matrx2.open(arg3.str());
    if (!matrx1.is_open() || !matrx2.is_open()) {
      cout << "Could not open matrix files!" << endl;
    }
    else {
      string line;
      float num;
      while(getline(matrx1, line)) {
        matrix1Col = 0;
        istringstream iss(line);
        while (iss >> num) {
          addNode(currentRow, num);
          matrix1Col++;
        }
        addRow(matrix1Head, currentRow);
        matrix1Rows++;
        currentRow = nullptr;
      }
      while(getline(matrx2, line)) {
        matrix2Col = 0;
        istringstream iss(line);
        while (iss >> num) {
          addNode(currentRow, num);
          matrix2Col++;
        }
        addRow(matrix2Head, currentRow);
        matrix2Rows++;
        currentRow = nullptr;
      } 
    }
    matrx1.close();
    matrx2.close();
    
    // Fix down pointers
    addRemainingDownPointers(matrix1Head);
    addRemainingDownPointers(matrix2Head);

    if (arg1.str()  == "add") {
      answerFile = arg4.str();
      addMatrix(matrix1Head, matrix2Head, addMatrxHead);
      printList(addMatrxHead,answerFile);
    }
    else if (arg1.str()  == "sub") {
      answerFile = arg4.str();
      subMatrix(matrix1Head, matrix2Head, subMatrxHead);
      printList(subMatrxHead,answerFile);
    }
    else if (arg1.str()  == "mul") {
      answerFile = arg4.str();
      if (matrix1Col == matrix2Rows) {
        multMatrix(matrix1Head, matrix2Head, multMatrxHead);
        printList(multMatrxHead, answerFile);
      }
      else {
        cout << "Cannot multiply matrices because columns and rows aren't compatible" << endl;
      }
    }
  }
  else if (arg1.str()== "tra") {
    ostringstream arg2;
    arg2 << argv[2];
    ostringstream arg3;
    arg3 << argv[3];
    
    answerFile = arg3.str();
    matrx1.open(arg2.str());
    if (!matrx1.is_open()) {
      cout << "Could not open matrix files!" << endl;
    }
    else {
      string line;
      float num;
      while(getline(matrx1, line)) {
        matrix1Col = 0;
        istringstream iss(line);
        while (iss >> num) {
          addNode(currentRow, num);
          matrix1Col++;
        }
        addRow(matrix1Head, currentRow);
        matrix1Rows++;
        currentRow = nullptr;
      }
    }
    matrx1.close();
    addRemainingDownPointers(matrix1Head);
    
    transpMatrix(matrix1Head, traMatrxHead);
    printList(traMatrxHead, answerFile);
  }
  else if (arg1.str() == "det") {
    ostringstream arg2;
    arg2 << argv[2];
    ostringstream arg3;
    arg3 << argv[3];
    
    answerFile = arg3.str();
    matrx1.open(arg2.str());
    if (!matrx1.is_open()) {
      cout << "Could not open matrix files!" << endl;
    }
    else {
      string line;
      float num;
      while(getline(matrx1, line)) {
        matrix1Col = 0;
        istringstream iss(line);
        while (iss >> num) {
          addNode(currentRow, num);
          matrix1Col++;
        }
        addRow(matrix1Head, currentRow);
        matrix1Rows++;
        currentRow = nullptr;
      }
    }
    matrx1.close();
    addRemainingDownPointers(matrix1Head);
    
    printDeterm(determinant(matrix1Head), answerFile);
  }

  return 0;
}