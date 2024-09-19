#include "Pet.h"

#ifndef OWL_H
#define OWL_H

class Owl : public Pet {
public:
  Owl();
  void feed();
  void play();
  void talk();
  void hunt();
  void nextHour();
  void activateNightVision();
  void setTalonStr(int inputTalonStr);
  void saveProgress(int inputType, string filename) const;

private:
  int talonStrength;
};

#endif