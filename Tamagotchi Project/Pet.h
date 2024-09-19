#ifndef PET_H
#define PET_H
using namespace std;

class Pet {
public:
  void setName(string inputName);
  void setHunger(int inputHunger);
  void setSleepy(int inputSleepy);
  void setHappiness(int inputHappiness);
  void setAge(int inputAge);
  string getName();
  virtual bool gameStatus();
  virtual void displayLevels();
  virtual void feed() = 0;
  virtual void play() = 0; 
  virtual void talk() = 0;
  virtual void saveProgress(int inputType, string filename) const = 0;
  virtual void nextHour() = 0;

protected:
  string name;
  int hunger;
  int sleepy;
  int happiness;
  int age;
};

#endif