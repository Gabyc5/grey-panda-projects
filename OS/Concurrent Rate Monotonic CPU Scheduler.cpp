#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <pthread.h>
using namespace std;

// Structure that was changed to accomodate PA3 requrements of having a single memory for all the threads
struct argInfo {
  int numCPU;
  string cpuTasks;
  int* count;
  pthread_mutex_t* mutex;
  pthread_mutex_t* hyperspace_mutex;
  pthread_cond_t* condition;

  argInfo() = default;
  argInfo(int given_cpu, const string& given_line, int* turn, pthread_mutex_t* bsem, pthread_mutex_t* bsem2, pthread_cond_t* cond) {
    numCPU = given_cpu;
    cpuTasks = given_line;
    count = turn;
    mutex = bsem;
    hyperspace_mutex = bsem2;
    condition = cond;
  }
};

// Struct specifically for collecting information relevant 
// for each task as it relates to the scheduling diagram
struct Task {
  char letter;
  int period;
  int worstCase;
  int startTime;
  int totalRunTime;
  int remainingUnits;
  bool shouldRun;
  Task(char givenLetter, double givenPeriod, double givenWorstCase, double givenHyperP);
};

Task::Task(char givenLetter, double givenPeriod, double givenWorstCase, double givenHyperP) {
  letter = givenLetter;
  period = givenPeriod;
  worstCase = givenWorstCase;
  startTime = period;
  totalRunTime = worstCase * (givenHyperP/period);
  remainingUnits = 0;
  shouldRun = true;
}

// Code from GeeksforGeeks (https://www.geeksforgeeks.org/program-to-find-lcm-of-two-numbers/)
int gcd(int num1, int num2) {
  if (num2 == 0) {
    return num1;
  }
  return gcd(num2, num1 % num2);
}

// Code from GeeksforGeeks (https://www.geeksforgeeks.org/program-to-find-lcm-of-two-numbers/)
int lcm(vector<pair<char, double>> values) {
  int ans = values[0].second;
  for (int i = 1; i < values.size(); i++) {
    int num1 = ans;
    int num2 = values[i].second;
    int gcd_val = gcd(num1, num2);
    ans = (ans * values[i].second) / gcd_val;
  }
  return ans;
}

// Custom comparison to sort tasks by priority and alphabetical order
bool priorityDefiner(pair<char, double>& leftVal, pair<char, double>& rightVal) {
  if (leftVal.second < rightVal.second) {
    return true;
  }
  else if (leftVal.second == rightVal.second) {
    return (leftVal.first < rightVal.first);
  }
  else {
    return false;
  }
}

// Additional function to find worstCase based on task letter Id
// This function is more like a bridge, connecting the sorted tasks and periods with the respective worstCase time
double findWC(char cpuLetter, vector<pair<char, double>> tasknWorstCase) {
  for (int i = 0; i < tasknWorstCase.size(); i++) {
    if (tasknWorstCase[i].first == cpuLetter) {
      return tasknWorstCase[i].second;
    }
  }
  return 0;
}

// Rate Monotonic Algorithm
string cpuScheduler(vector<pair<char, double>>& tasknWorstCase, vector<pair<char, double>>& tasknPeriod, int hyperP) {
  vector<Task> tasks;
  string cpuSchedule = "";
  int currTime = 0;
  bool interrupt = false;

  // Look through vector of pairs, creating a new Task object for each task, getting all the relevant information ready
  for (int i = 0; i < tasknPeriod.size(); i++) {
    tasks.emplace_back(tasknPeriod[i].first, tasknPeriod[i].second, findWC(tasknPeriod[i].first, tasknWorstCase), hyperP);
  }

  // Initialize variable currStartTime to task[0].startTime (Very Important!!) that will keep track of what 
  // the next start time of the corresponding tasks is. A tasks startTime starts out as the period of the task
  // and it is incremented by the period each time the task is called 
  int currStartTime = tasks[0].startTime;

  // This the main loop, which loops until the currTime (starts at 0) reaches the hyperperiod
  // The second loop checks each task (sorted by priority) and compares the current time to the startTime of each task
  // If the current time is equal to any of the tasks start time (that means the time has reached the instance of an important period) then that task interrupts
  // Else if the current time is not equal to it then it executes based on what type of task it is, is it a previously interrupted task, a regular task, etc.?
  // These if statemnts update the according values correctly and keep track of when the next start time is coming allowing everything to execute based on priority
  while (currTime < hyperP) {
    for (int i = 0; i < tasks.size(); i++) {
      if (currTime == tasks[i].startTime) {
        cpuSchedule = cpuSchedule + tasks[i].letter + "(" + to_string(tasks[i].worstCase) + "), ";
        currTime += tasks[i].worstCase;
        tasks[i].totalRunTime -= tasks[i].worstCase;
        tasks[i].shouldRun = true;
        tasks[i].startTime += tasks[i].period;

        bool pendingDeadline;
        for (int z = 0; z < tasks.size(); z++) {
          if ((tasks[z].startTime > currStartTime) && (tasks[z].startTime < tasks[i].startTime)) {
            currStartTime = tasks[z].startTime;
            tasks[i].shouldRun = false;
            pendingDeadline = false;
            break;
          }
          else {
            pendingDeadline = true;
          }
        }
        if (pendingDeadline) {
          currStartTime = tasks[i].startTime;
        }

        for (int z = 0; z < tasks.size(); z++) {
          if (tasks[z].startTime < currStartTime) {
            tasks[z].shouldRun = true;
            tasks[z].startTime += tasks[z].period;
          }
        }
      }
      else if (currTime < currStartTime) {
        if (tasks[i].totalRunTime > 0 && tasks[i].shouldRun == true && tasks[i].remainingUnits == 0) {
          if (tasks[i].worstCase + currTime > currStartTime) {
            int semiRunTime = currStartTime - currTime;
            currTime += semiRunTime;
            tasks[i].totalRunTime -= semiRunTime;
            tasks[i].remainingUnits = tasks[i].worstCase - semiRunTime;
            cpuSchedule = cpuSchedule + tasks[i].letter + "(" + to_string(semiRunTime) + "), ";
            interrupt = true;
          }
          else {
            cpuSchedule = cpuSchedule + tasks[i].letter + "(" + to_string(static_cast<int>(tasks[i].worstCase)) + "), ";
            currTime += tasks[i].worstCase;
            tasks[i].shouldRun = false;
            tasks[i].totalRunTime -= tasks[i].worstCase;
          }
        }
        else if (interrupt == true) {
          if (tasks[i].remainingUnits > 0) {
            int remTime = min(tasks[i].remainingUnits, currStartTime - currTime);
            cpuSchedule = cpuSchedule + tasks[i].letter + "(" + to_string(tasks[i].remainingUnits) + "), ";
            currTime += remTime;
            tasks[i].totalRunTime -= remTime;
            tasks[i].remainingUnits -= remTime;
            if (tasks[i].remainingUnits == 0) {
              tasks[i].shouldRun = false;
              interrupt = false;
            }
          }
        }
        else {
          bool lowerPriorityReady = false;
          for (int j = i+1; j < tasks.size(); j++) {
            if (tasks[j].shouldRun) {
              lowerPriorityReady = true;
              break;
            }
          }
          if (lowerPriorityReady) {continue; }
          else {
            cpuSchedule = cpuSchedule + "Idle(";
            cpuSchedule = cpuSchedule + to_string(currStartTime - currTime) + "), ";
            currTime += (currStartTime - currTime);
            break;
          }
        }
      }
    }
  }

  // This removes the trailing coma and space at the end of the concatenated string
  cpuSchedule = cpuSchedule.substr(0, cpuSchedule.size() - 2);

  return cpuSchedule;
}

// This is the child thread
void* cpuInformation(void* ptr) {
  argInfo* arg = (argInfo*) ptr;
  int numCPU = arg->numCPU;
  string cpuTasks = arg->cpuTasks;
  int* turn = arg->count;
  pthread_mutex_t* mutex = arg->mutex;
  pthread_mutex_t* hyperspace_mutex = arg->hyperspace_mutex;
  pthread_cond_t* condition = arg->condition;

  pthread_mutex_unlock(hyperspace_mutex);

  char givenCPU;
  double worstCase, period;
  double utilizCount = 0;
  ostringstream correctedNum;
  string correctedNumStr;
  vector<pair<char, double>> task_and_worstCase;
  vector<pair<char, double>> task_and_period;
  string cpuOutput = "";

  istringstream iss(arg->cpuTasks);
  while (iss >> givenCPU >> worstCase >> period) {
    task_and_worstCase.push_back(make_pair(givenCPU, worstCase));
    task_and_period.push_back(make_pair(givenCPU, period));
  }

  cpuOutput += "CPU " + to_string(numCPU) + "\n";

  // This section accumulates and formates the task scheduling information
  // Using a vector of pairs the information can be easily accessed so that it can be relationed with each CPU
  cpuOutput += "Task scheduling information: ";
  for (int i = 0; i < task_and_worstCase.size(); i++) {
    cpuOutput += string(1, task_and_worstCase[i].first) + " (WCET: " + to_string(static_cast<int>(task_and_worstCase[i].second)) + ", Period: " + to_string(static_cast<int>(task_and_period[i].second)) + ")";
    if (i != task_and_worstCase.size() - 1) {
      cpuOutput += ", ";
    }
  }

  // This section focuses on calculating the task set utilization by keeping count of the worst case 
  // execution time being divided by the period of the task. The sum of those differences is being calculated
  // In order to achieve a number with only two decimal points a ostringstream is used
  for (int i = 0; i < task_and_period.size(); i++) {
    utilizCount += (task_and_worstCase[i].second / task_and_period[i].second);
  }
  correctedNum << fixed << setprecision(2) << utilizCount;
  correctedNumStr = correctedNum.str();
  cpuOutput += "\nTask set utilization: " + correctedNumStr + "\n";

  // The hyperperiod is found by calculating the LCM of the periods within the task
  // Code from GeeksforGeeks (https://www.geeksforgeeks.org/program-to-find-lcm-of-two-numbers/)
  int hyperPeriod = lcm(task_and_period);
  cpuOutput += "Hyperperiod: " + to_string(hyperPeriod) + "\n";

  // This section calculates the possibility of the task set being executed by Rate Monotonic
  cpuOutput += "Rate Monotonic Algorithm execution for CPU" + to_string(numCPU) + ":\n";
  double numTasks = task_and_period.size();
  double formula = numTasks * (pow(2,1/numTasks) - 1);
  // If the utilization count is greater than 1 then task is not schedulable
  if (utilizCount > 1) {
    cpuOutput += "The task set is not schedulable\n";
  }
  // If the utilization count is less than 1 but greater than the formula then its unknown
  else if ((utilizCount <= 1) && (utilizCount > formula)) {
    cpuOutput += "Task set schedulability is unknown\n";
  }
  // Otherwise the else statement will call the cpuScheduler function that will 
  // calculate the scheduling order of the CPU tasks
  else {
    sort(task_and_period.begin(), task_and_period.end(), priorityDefiner);
    cpuOutput += "Scheduling Diagram for CPU " + to_string(numCPU) + ": " + cpuScheduler(task_and_worstCase, task_and_period, hyperPeriod) + "\n";
  }

  pthread_mutex_lock(mutex);
  while (numCPU != *turn) {
    pthread_cond_wait(condition, mutex);
  }
  pthread_mutex_unlock(mutex);

  cout << cpuOutput << endl;

  pthread_mutex_lock(mutex);
  *turn += 1;
  pthread_cond_broadcast(condition);
  pthread_mutex_unlock(mutex);

  return nullptr;
}



int main() {
  vector<string> inputs;
  string line;

  pthread_cond_t condition;
  pthread_mutex_t mutex, hyperspace_mutex;
  pthread_cond_init(&condition, NULL);
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&hyperspace_mutex, NULL);

  while (getline(cin, line)) {
    if (line == "end") {
      break;
    }
    inputs.push_back(line);
  }

  int turn = 1;
  vector<pthread_t> threads(inputs.size());
  argInfo args;

  for (int i = 0; i < inputs.size(); i++) {
    pthread_mutex_lock(&hyperspace_mutex);
    args = argInfo(i+1, inputs[i], &turn, &mutex, &hyperspace_mutex, &condition);
    pthread_create(&threads[i], nullptr, cpuInformation, &args);
  }

  for (int i = 0; i < inputs.size(); i++) {
    pthread_join(threads[i], nullptr);
  }

  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&hyperspace_mutex);
  pthread_cond_destroy(&condition);

  return 0;
}