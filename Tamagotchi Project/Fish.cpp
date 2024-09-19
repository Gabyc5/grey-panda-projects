#include <iostream>
#include <fstream>
#include <string>

#include "Fish.h"

Fish::Fish() {
  hunger = 10;
  sleepy = 10;
  happiness = 0;
  health = 10;
  age = 1;
}

void Fish::feed() {
  hunger -= (1 + rand() % 5);
  sleepy += (1 + rand() % 5);
  happiness += (1 + rand() % 5);
  health += (1 + rand() % 5);
  cout << name << " loved eating their flakes. Yum! " << endl << endl;
}

void Fish::play() {
  hunger += (1 + rand() % 5);
  sleepy += (1 + rand() % 5);
  happiness += (1 + rand() % 5);
  health -= (1 + rand() % 5);
  cout << name << " loved playing hide and seek in the treasure chest!" << endl;
  cout << name << " is a little tired now..." << endl << endl;
}

void Fish::talk() {
  hunger += (1 + rand() % 5);
  sleepy += (1 + rand() % 3);
  happiness += (1 + rand() % 5);

  cout << name << " can't speak because of the water, but they love blowing bubbles!" << endl << endl;
}

bool Fish::gameStatus() {
  bool endGame = false;
  
  if (hunger > 25 || sleepy > 35 || happiness < 0 || health < 0) {
    cout << "Your pet's levels have fallen too low..." << endl;
    cout << "YOU LOSE!!!" << endl;
    endGame = true;    
  }
  else if (hunger < 0 || happiness > 30 || health > 25) {
    cout << "Congratulations!!! Your pet has lived a wonder life.";
    cout << endl << "YOU WIN!!!" << endl;
    endGame = true;    
  }

  return endGame;
}

void Fish::displayLevels() {
  cout << name << "'s levels:" << endl << endl;
  cout << "Hunger: " << hunger << endl;
  cout << "Sleepiness: " << sleepy << endl;
  cout << "Happiness: " << happiness << endl;
  cout << "Health: " << health << endl << endl; 
}

void Fish::medicine() {
  hunger += (1 + rand() % 5);
  sleepy += (1 + rand() % 5);
  happiness -= (1 + rand() % 5);
  health -= (1 + rand() % 5);

  cout << "Oh no! Because fish are delicate in nature, giving them medicine to ensure their health is amazing!" << endl << endl;
}

void Fish::activateFins() {
  hunger += (1 + rand() % 5);
  sleepy += (1 + rand() % 5);
  happiness += (1 + rand() % 5);
  health -= (1 + rand() % 3);

  cout << name << " has activated their bionic fins. Watch them speed away around the tank!" << endl << endl;
}
  
void Fish::decorateFins() {
  sleepy -= (1 + rand() % 5);
  happiness += (1 + rand() % 5);

  cout << name << "'s fins have an explosion of glitter! Your fish is so happy with their beautiful fins..." << endl << endl;
}

void Fish::nextHour() {
  age += (1 + rand() % 6);
  hunger += (1 + rand() % 3);
  sleepy -= (1 + rand() % 3);
  happiness += (1 + rand() % 3);
  health += (1 + rand() % 2);

  cout << "In an hour your pet has grown so much! They are now " << age;
  cout << " years old!" << endl << endl;
}

void Fish::setHealth(int inputHealth) {
  health = inputHealth;
}

void Fish::saveProgress(int inputType, string filename) const {
  ofstream outFS;
  outFS.open(filename + ".txt");

  outFS << inputType << endl;
  outFS << name << endl;
  outFS << hunger << endl;
  outFS << sleepy << endl;
  outFS << happiness << endl;
  outFS << health << endl;
  outFS << age;
  
  outFS.close();
}