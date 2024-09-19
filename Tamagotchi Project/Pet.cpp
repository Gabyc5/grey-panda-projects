#include <iostream>
#include <string>

using namespace std;

#include "Pet.h"

void Pet::setName(string inputName) {
  name = inputName;
}

string Pet::getName() {
  return name;
}

bool Pet::gameStatus() {
  bool endGame = false;
  
  if (hunger > 30 || sleepy > 35 || happiness < 0) {
    cout << "Your pet's levels have fallen too low..." << endl;
    cout << "YOU LOSE!!!" << endl;
    endGame = true;    
  }
  else if (happiness > 35) {
    cout << "Congratulations!!! Your pet has lived a wonder life.";
    cout << endl << "YOU WIN!!!" << endl;
    endGame = true;    
  }

  return endGame;
}

void Pet::displayLevels() {
  cout << name << "'s levels:" << endl << endl;
  cout << "Hunger: " << hunger << endl;
  cout << "Sleepiness: " << sleepy << endl;
  cout << "Happiness: " << happiness << endl << endl;  
}

void Pet::setAge(int inputAge) {
  age = inputAge;
}

void Pet::setHunger(int inputHunger) {
  hunger = inputHunger;
}

void Pet::setSleepy(int inputSleepy) {
  sleepy = inputSleepy;
}

void Pet::setHappiness(int inputHappiness) {
  happiness = inputHappiness;
}