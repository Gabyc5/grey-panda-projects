#include "Pet.h"

#ifndef FISH_H
#define FISH_H

class Fish : public Pet {
public:
  Fish();
  void feed();
  void play();
  void talk();
  void medicine();
  void activateFins();
  void decorateFins(); 
  bool gameStatus();
  void displayLevels();
  void nextHour();
  void setHealth(int inputHealth);
  void saveProgress(int inputType, string filename) const;
private:
  int health;
};

#endif