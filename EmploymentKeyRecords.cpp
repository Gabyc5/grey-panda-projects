#include <iostream>
#include <string>
#include <fstream>
using namespace std;

/* This program manages employee records and their associated keys. It provides functionality
to read employee data from a file, display employee details, add and return keys for employees,
and save updated key statuses to a file. The program includes options for viewing all employees,
checking which employees have specific keys, and performing updates on the keys each employee possesses */

struct Employee{
   string name;
   int nKeysPossessed;
   string keys[5];
};

bool reader(string input_filename, Employee employees[], int& nEmployees) {
  bool ans = true;
  ifstream inFS;
  inFS.open(input_filename);
  
  if (inFS.is_open() == false) {
    ans = false;
  } 
  else {
    inFS >> nEmployees;
    inFS.ignore(); 
    for (int i = 0; i < nEmployees; ++i) {
      getline(inFS, employees[i].name); 
      inFS >> employees[i].nKeysPossessed; 
      for (int j = 0; j < employees[i].nKeysPossessed; ++j) {
        inFS >> employees[i].keys[j];
      }
      inFS.ignore();
    }
  }
  
  inFS.close(); 
  return ans;
} 

void writer(string output_filename, Employee employees[], int nEmployees) {
  ofstream outFS;
  outFS.open(output_filename);
  
  outFS << nEmployees << endl;
  for (int i = 0; i < nEmployees; ++i) {
    outFS << employees[i].name << endl;
    outFS << employees[i].nKeysPossessed << " ";
    for (int j = 0; j < employees[i].nKeysPossessed; ++j) {
      outFS << employees[i].keys[j];
      if (j != ((employees[i].nKeysPossessed) - 1)) {
        outFS << " ";
      }
    }
    outFS << endl;
  }
  outFS.close();
} 

bool addKeyForEmployee(Employee employees[], int nEmployees, string emp_name, string newKey) {
  bool ans;
  bool is_duplicate = false;
  int found_count = 0;
  int num_keys;
  int found_idx;

  for (int i = 0; i < nEmployees; ++i) {
    if (employees[i].name == emp_name) {
      found_idx = i;
      found_count ++;
      num_keys = employees[i].nKeysPossessed;
      for (int j = 0; j < num_keys; ++j) {
        if (employees[i].keys[j] == newKey) {
          is_duplicate = true;
        }
      }
    }
  }

  if (found_count == 0) {
    ans = false;
    cout << "Cannot find the specified employee!" << endl;
  }
  else if (num_keys == 5) {
    ans = false;
    cout << "This employee already has 5 keys!" << endl;
  }
  else if (is_duplicate == true) {
    ans = false;
    cout << "This employee already has this key!"<< endl;
  }
  else {
    ans = true;
    employees[found_idx].keys[num_keys] = newKey;
    employees[found_idx].nKeysPossessed += 1;
  }

  return ans;
}

bool returnAKey(Employee employees[], int nEmployees, string emp_name, string returnKey) {
  bool is_located = false;
  int found_empl = 0;
  int empl_idx;
  int idxCount = 0;
  string tempArr[5];
  
  for (int i = 0; i < nEmployees; ++i) {
    if (employees[i].name == emp_name) {
      empl_idx = i;
      found_empl ++;
    }
  }
  for (int j = 0; j < employees[empl_idx].nKeysPossessed; ++j) {
    if (employees[empl_idx].keys[j] == returnKey) {
      is_located = true;
    }
    else {
      tempArr[idxCount] = employees[empl_idx].keys[j];
      idxCount ++;
    }
  }

  if (found_empl == 0) {
    cout << "Cannot find the specified employee!" << endl;
  }
  else if (is_located == false) {
    cout << "This employee does not have the specified key!" << endl;
  }
  else {
    for (int i = 0; i < idxCount; ++i) {
      employees[empl_idx].keys[i] = tempArr[i];
    }
    employees[empl_idx].nKeysPossessed -= 1;
  }

  return is_located;
} 

int main() {
  string filename;
  int numEmpl;
  Employee emplArray[10];
  int option = 0;

  cout << "Please enter key file name to start: ";
  cin >> filename;
  if (!(reader(filename, emplArray, numEmpl))) {
    cout << "File not found, exiting the program..." << endl;
    exit(1);
  }
  else {
    // General Menu
    while (option != 7) {
      cout << "Please select from the following options: " << endl;
      cout << "  1. show all employees and their keys" << endl;
      cout << "  2. show the keys an employee possesses" << endl;
      cout << "  3. show which employees possess a specific key" << endl;
      cout << "  4. add a key to an employee" << endl;
      cout << "  5. return a key by an employee" << endl;
      cout << "  6. save the current key status" << endl;
      cout << "  7. exit the program" << endl;

      cin >> option;
      // Option out of range commands
      while (!((option > 0) && (option < 8))) {
        cout << "Not a valid option. Please try again." << endl;
        cout << endl;
        cout << "Please select from the following options: " << endl;
        cout << "  1. show all employees and their keys" << endl;
        cout << "  2. show the keys an employee possesses" << endl;
        cout << "  3. show which employees possess a specific key" << endl;
        cout << "  4. add a key to an employee" << endl;
        cout << "  5. return a key by an employee" << endl;
        cout << "  6. save the current key status" << endl;
        cout << "  7. exit the program" << endl;
        cin >> option;
      }
      // Option 1 commands
      if (option == 1) {
        for (int i = 0; i < numEmpl; ++i) {
          cout << "Name: " << emplArray[i].name << endl;
          cout << "Keys possessed: ";
          for (int j = 0; j < emplArray[i].nKeysPossessed; ++j) {
            cout << emplArray[i].keys[j] << " ";
          }
          cout << endl;
        }
        cout << endl;
      }
      // Option 2 commands
      else if (option == 2) {
        bool found = false;
        string emplName;
        
        cout << "Please enter employee's name: ";
        cin.ignore();
        getline(cin, emplName);

        for (int i = 0; i < numEmpl; ++i) {
          if (emplArray[i].name == emplName) {
            found = true;
            cout << emplName << " possess the following keys: ";
            for (int j = 0; j < emplArray[i].nKeysPossessed; ++j) {
              cout << emplArray[i].keys[j] << " ";
            }
            cout << endl;
          }
        }

        if (found == false) {
          cout << "Cannot find the specified employee!" << endl;
        }
        cout << endl;
      }
      // Option 3 commands
      else if (option == 3) {
        string selectKeyArr[numEmpl];
        int arrIdx = 0;
        string selectKey;
        bool keyFound = false;
        
        cout << "Please enter a key: ";
        cin >> selectKey;

        for (int i = 0; i < numEmpl; ++i) {
          for (int j = 0; j < emplArray[i].nKeysPossessed; ++j) {
            if (emplArray[i].keys[j] == selectKey) {
              keyFound = true;
              selectKeyArr[arrIdx] = emplArray[i].name;
              arrIdx ++;
            }
          }
        }
        if (keyFound == true) {
          for (int i = 0; i < arrIdx; ++i) {
            cout << selectKeyArr[i] << ", ";
          }
          cout << "possess this key." << endl;
        }
        else {
          cout << "No one possesses this key." << endl;
        }
        cout << endl;
      }
      // Option 4 commands
      else if (option == 4) {
        string targetEmpl;
        string newKey;
        
        cout << "Please enter employee's name: ";
        cin.ignore();
        getline(cin, targetEmpl); 

        cout << "Please enter a new key: ";
        cin >> newKey;

        if (addKeyForEmployee(emplArray, numEmpl, targetEmpl, newKey) == true) {
          cout << "Key added successfully." << endl;
        }
        cout << endl;
      }
      // Option 5 commands
      else if (option == 5) {
        string targetEmpl;
        string returnedKey;
        
        cout << "Please enter employee's name: ";
        cin.ignore();
        getline(cin, targetEmpl);

        cout << "Please enter the returned key: ";
        cin >> returnedKey;

        if (returnAKey(emplArray, numEmpl, targetEmpl, returnedKey) == true) {
          cout << "Key returned successfully." << endl;
        }
        cout << endl;
      }
      // Option 6 commands
      else if (option == 6) {
        string output_filename;

        cout << "Please enter output file name: " << endl;
        cin >> output_filename;

        writer(output_filename, emplArray, numEmpl);
        cout << endl;
      }
      // Option 7 commands
      else if (option == 7) {
        cout << "Thank you for using the system! Goodbye!" << endl;
        writer("keys_updated.txt", emplArray, numEmpl);
        exit(0);
      }
    }
  }
  
  return 0;
}
