#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
using namespace std;

/* This program benchmarks and measures the execution time of various algorithms. It tests different functions for prefix averaging and example operations on arrays, and records the execution time in milliseconds. The time complexity and performance of the functions are 
evaluated based on the input size */


// FIX MILLISECOND TIME - FIX ans

// For all the following functions minus main
// x: input array
// n: length of array

double* prefixAverage1(double* x, int len){
  double* a = new double[len];
  for(int j = 0; j < len; j++){
    double total = 0;
    for(int i = 0; i <= j; i++){
      total += x[i];
    }
    a[j] = total/(j+1);
  }
  return a;
}

double* prefixAverage2(double* x, int len){
  double* a = new double[len];
  double total = 0;
  for(int j = 0; j < len; j++){
    total += x[j];
    a[j] = total/(j+1);
  }
  return a;
}

int example1(int* x, int len) {
  int total = 0;
  for(int i = 0; i < len; i++){
    total += x[i];
  }
  return total;
}

int example2(int* x, int len) {
  int total = 0;
  for(int i = 0; i < len; i+=2){
    total += x[i];
  }
  return total;
}

int example3(int* x, int len) {
  int total = 0;
  for(int i = 0; i < len; i++){
    for(int k = 0; k <= i; k++){
      total += x[i];
    }
  }
  return total;
}

int example4(int* x, int len) {
  int prefix = 0;
  int total = 0;
  for(int i = 0; i < len; i++){
    prefix += x[i];
    total += prefix;
  }
  return total;
}

int example5(int* first, int* second, int len) {
  int count = 0;
  for(int i = 0; i < len; i++){
    int total = 0;
    for(int j = 0; j < len; j++){
      for(int k = 0; k <= j; k++){
        total += first[k];
      }
    }
    if(second[i] == total){
      count++;
    }
  }
  return count;
}


int main(int argc, char** argv) {
  ostringstream function_name;
  function_name << argv[1];

  int arrSize = stoi(argv[3]); 

  double* array1_prefx = new double[arrSize];
  int* array1_ex = new int[arrSize];
  int* array2 = nullptr;
  if (function_name.str() == "e5") {
    array2 = new int[arrSize];
  }

  for (int i = 0; i < arrSize; i++) {
    array1_prefx[i] = 0;
    array1_ex[i] = 0;
    if (array2 != nullptr) {
      array2[i] = 0;
    }
  }

  ofstream outFS(argv[2]);
  for (int i = 1; i <= arrSize; i++) {
    clock_t start, end;
    double totalTime, ans;

    if (function_name.str() == "p1") {
      start = clock();
      prefixAverage1(array1_prefx, pow(10,i));
      end = clock();
    }
    else if (function_name.str() == "p2") {
      start = clock();
      prefixAverage2(array1_prefx, pow(10,i));
      end = clock();
    }
    else if (function_name.str() == "e1") {
      start = clock();
      example1(array1_ex, pow(10,i));
      end = clock();
    }
    else if (function_name.str() == "e2") {
      start = clock();
      example2(array1_ex, pow(10,i));
      end = clock();
    }
    else if (function_name.str() == "e3") {
      start = clock();
      example3(array1_ex, pow(10,i));
      end = clock();
    }
    else if (function_name.str() == "e4") {
      start = clock();
      example4(array1_ex, pow(10,i));
      end = clock();
    }
    else {
      start = clock();
      example5(array1_ex, array2, pow(10,i));
      end = clock();
    }

    totalTime = double(end - start) / CLOCKS_PER_SEC * 1000;
    //ans = log10(totalTime);
    ans = totalTime;

    outFS << ans << endl;
  }
  outFS.close();
  return 0;
}