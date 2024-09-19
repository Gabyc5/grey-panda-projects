#include "Pet.h"

#ifndef CAT_H
#define CAT_H

class Cat : public Pet {
public:
  Cat();
  void feed();
  void play();
  void talk();
  void groom();
  void scratch();
  void nextHour();
  void setVoiceVol(int inputVoiceVol);
  void saveProgress(int inputType, string filename) const;
private:
  int voiceVolume;
};

#endif