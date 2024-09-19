#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <strings.h>
#include <sys/wait.h>
using namespace std;

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

// This function does all the calculations and adds all the necessary information to a string (cpuOutput) that will be the one being returned to the client
string cpuCalculations(string cpuInfo) {
  char givenCPU;
  double worstCase, period;
  double utilizCount = 0;
  ostringstream correctedNum;
  string correctedNumStr;
  vector<pair<char, double>> task_and_worstCase;
  vector<pair<char, double>> task_and_period;
  string cpuOutput = "";

  //This part removes the last character in the string since the char is used to identify what iteration of cpu it is (cpunum)
  string cpuNum = cpuInfo.substr(cpuInfo.size() - 1);;
  cpuInfo.pop_back();

  istringstream iss(cpuInfo);
  while (iss >> givenCPU >> worstCase >> period) {
    task_and_worstCase.push_back(make_pair(givenCPU, worstCase));
    task_and_period.push_back(make_pair(givenCPU, period));
  }

  cpuOutput = "CPU " + cpuNum + "\n";

  // This section accumulates and formates the task scheduling information
  // Using a vector of pairs the information can be easily accessed so that it can be relationed with each CPU
  cpuOutput = cpuOutput + "Task scheduling information: ";
  for (int i = 0; i < task_and_worstCase.size(); i++) {
    cpuOutput = cpuOutput + task_and_worstCase[i].first + " (WCET: " + to_string(static_cast<int>(task_and_worstCase[i].second)) + ", Period: " + to_string(static_cast<int>(task_and_period[i].second)) + ")";
    if (i != task_and_worstCase.size() - 1) {
      cpuOutput = cpuOutput + ", ";
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
  cpuOutput = cpuOutput + "\nTask set utilization: " + correctedNumStr + "\n";

  // The hyperperiod is found by calculating the LCM of the periods within the task
  int hyperPeriod = lcm(task_and_period);
  cpuOutput = cpuOutput + "Hyperperiod: " + to_string(hyperPeriod) + "\n";

  // This section calculates the possibility of the task set being executed by Rate Monotonic
  // If the utilization count is greater than 1 then task is not schedulable
  // If the utilization count is less than 1 but greater than the formula then its unknown
  // Otherwise the else statement will call the cpuScheduler function that will calculate the scheduling order of the CPU tasks
  cpuOutput = cpuOutput + "Rate Monotonic Algorithm execution for CPU" + cpuNum + ":\n";
  double numTasks = task_and_period.size();
  double formula = numTasks * (pow(2,1/numTasks) - 1);
  if (utilizCount > 1) {
    cpuOutput = cpuOutput + "The task set is not schedulable\n";
  }
  else if ((utilizCount <= 1) && (utilizCount > formula)) {
    cpuOutput = cpuOutput + "Task set schedulability is unknown\n";
  }
  else {
    sort(task_and_period.begin(), task_and_period.end(), priorityDefiner);
    cpuOutput = cpuOutput + "Scheduling Diagram for CPU " + cpuNum + ": " + cpuScheduler(task_and_worstCase, task_and_period, hyperPeriod) + "\n";
  }

  return cpuOutput;
}

// Fireman function is sourced from Professor Rincon's Unit 2 Folder - fireman.cpp file
// It is created to prevent zoombie processes that might occur when calling fork()
void fireman(int) {
   while (waitpid(-1, nullptr, WNOHANG) > 0) {}
}

int main(int argc, char *argv[]) {

  /* ===== Main() code comes from Professor Rincon's Unit 2 Folder - server.cpp file ===== */
  int sockfd, newsockfd, portno, clilen;
  struct sockaddr_in serv_addr, cli_addr;

  // Check the commandline arguments
  if (argc != 2) {
      cerr << "Port not provided" << endl;
      exit(0);
   }

  // Create the socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
      cerr << "Error opening socket" << endl;
      exit(0);
   }

  // Populate the sockaddr_in structure
  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = stoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  // Bind the socket with the sockaddr_in structure
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
      cerr << "Error binding" << endl;
      exit(0);
   }

  // Set the max number of concurrent connections
  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  // Calling sigchld for fireman function and then entering the infinite while loop
  signal(SIGCHLD, fireman);
  while (true) {
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
    if (fork() == 0) {
      close(sockfd);
      // Accept a new connection
      if (newsockfd < 0) {
          cerr << "Error accepting new connections" << endl;
          exit(0);
       }
      int n, msgSize = 0;
      n = read(newsockfd, &msgSize, sizeof(int));
      if (n < 0) {
          cerr << "Error reading from socket" << endl;
          exit(0);
       }
      char *tempBuffer = new char[msgSize + 1];
      bzero(tempBuffer, msgSize + 1);
      n = read(newsockfd, tempBuffer, msgSize + 1);
      if (n < 0) {
          cerr << "Error reading from socket" << endl;
          exit(0);
       }
      string buffer = tempBuffer;
      delete[] tempBuffer;
      // Do work on string given by the client
      buffer = cpuCalculations(buffer);
      msgSize = buffer.size();
      n = write(newsockfd, &msgSize, sizeof(int));
      if (n < 0) {
          cerr << "Error writing to socket" << endl;
          exit(0);
       }
      n = write(newsockfd, buffer.c_str(), msgSize);
      if (n < 0) {
          cerr << "Error writing to socket" << endl;
          exit(0);
       }

      close(newsockfd);
      _exit(0);
    }
  }

  close(newsockfd);
  return 0;
}
