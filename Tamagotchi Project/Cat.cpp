#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Cat.h"

Cat::Cat() {
  hunger = 10;
  sleepy = 10;
  happiness = 0;
  voiceVolume = 5;
  age = 1;
}

void Cat::feed() {
  hunger -= (1 + rand() % 5);
  sleepy += (1 + rand() % 5);
  happiness += (1 + rand() % 5);
  cout << name << " loved eating tuna. Yum! " << endl << endl;
}

void Cat::play() {
  hunger += (1 + rand() % 5);
  sleepy += (1 + rand() % 5);
  happiness += (1 + rand() % 5);
  cout << name << " loved playing with the colorful ribbons." << endl;
  cout << name << " is a little tired now..." << endl << endl;
}

void Cat::talk() {
  hunger += (1 + rand() % 3);
  sleepy += (1 + rand() % 3);
  happiness += (1 + rand() % 5);
  voiceVolume += (1 + rand() % 5);

  if (voiceVolume < 15) {
    cout << name << " has a beautiful singing voice. Listen to their meows!" << endl << endl;
  }
  else {
    cout << name << "'s voice is too loud, try another activity or risk scaring other kittens!" << endl << endl;
  }
  
}

void Cat::scratch() {
  sleepy += (1 + rand() % 3);
  happiness += (1 + rand() % 5);
  voiceVolume += (1 + rand() % 2);

  cout << "Scratching " << name << "'s ears makes them so happy. Listen to them purr!" << endl << endl;
}

void Cat::groom() {
  happiness += (1 + rand() % 2);
  voiceVolume += (1 + rand() % 2);

  cout << name << " needs to stay clean. " << name << "'s grooming session left their fur so shiny!" << endl << endl;
}

void Cat::nextHour() {
  age += (1 + rand() % 5);
  hunger += (1 + rand() % 3);
  sleepy -= (1 + rand() % 3);
  happiness += (1 + rand() % 3);
  voiceVolume += (1 + rand() % 2);

  cout << "In an hour your pet has grown so much! They are now " << age;
  cout << " years old!" << endl << endl;
}

void Cat::setVoiceVol(int inputVoiceVol) {
  voiceVolume = inputVoiceVol;
}

void Cat::saveProgress(int inputType, string filename) const {
  ofstream outFS;
  outFS.open(filename + ".txt");

  outFS << inputType << endl;
  outFS << name << endl;
  outFS << hunger << endl;
  outFS << sleepy << endl;
  outFS << happiness << endl;
  outFS << voiceVolume << endl;
  outFS << age;
  
  outFS.close();
}