#include <iostream>
#include <fstream>
#include <string>

#include "Owl.h"

Owl::Owl() {
  hunger = 10;
  sleepy = 10;
  happiness = 0;
  talonStrength = 10;
  age = 1;
}

void Owl::feed() {
  hunger -= (1 + rand() % 5);
  sleepy += (1 + rand() % 5);
  happiness += (1 + rand() % 5);
  cout << name << " loved eating his rodent friends. Yum! " << endl;
  cout << "ps... " << name << " would love to go hunting later!" << endl << endl;
}

void Owl::play() {
  hunger += (1 + rand() % 5);
  sleepy += (1 + rand() % 5);
  happiness += (1 + rand() % 5);
  talonStrength -= (1 + rand() % 3);

  if (talonStrength < 25) {
    cout << name << " loved playing tag as they flew!" << endl;
    cout << name << " is a little tired now..." << endl << endl;
  }
  else {
    cout << name << "'s talons are too strong, maybe try more relaxing activity!" << endl << endl;
  }
}

void Owl::talk() {
  hunger += (1 + rand() % 5);
  sleepy += (1 + rand() % 6);
  talonStrength -= (1 + rand() % 5);
  happiness += (1 + rand() % 5);

  cout << name << " cood all night long! Your owl loves to sing!" << endl << endl;
}

void Owl::hunt() {
  hunger -= (1 + rand() % 5);
  sleepy += (1 + rand() % 4);
  happiness += (1 + rand() % 5);
  talonStrength += (1 + rand() % 4);

  cout << name << " loves using their strong wings to chase prey. " << name << " is an excellent predator!" << endl << endl;
}

void Owl::nextHour() {
  age += (1 + rand() % 5);
  hunger += (1 + rand() % 3);
  sleepy -= (1 + rand() % 3);
  happiness += (1 + rand() % 3);
  talonStrength += (1 + rand() % 3);

  cout << "In an hour your pet has grown so much! They are now " << age;
  cout << " years old!" << endl << endl;
}
  
void Owl::activateNightVision() {
  sleepy -= (1 + rand() % 4);
  happiness += (1 + rand() % 5);

  cout << "Your pet " << name << " has the super ability to activate night vision. " << name << " can see every detail and speck around them!" << endl << endl;
}

void Owl::setTalonStr(int inputTalonL) {
  talonStrength = inputTalonL;
}

void Owl::saveProgress(int inputType, string filename) const {
  ofstream outFS;
  outFS.open(filename + ".txt");

  outFS << inputType << endl;
  outFS << name << endl;
  outFS << hunger << endl;
  outFS << sleepy << endl;
  outFS << happiness << endl;
  outFS << talonStrength << endl;
  outFS << age;
  
  outFS.close();
}