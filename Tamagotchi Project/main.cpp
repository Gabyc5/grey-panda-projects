#include <iostream>
#include <fstream>
using namespace std;

#include "Cat.h"
#include "Owl.h"
#include "Fish.h"


void introArt() {   

cout << ".__  .__  _____               _____ _________        __" << endl;       
cout << "|  | |__|/ ____\\____    _____/ ____\\ \\______ \\ _____/  |_  ______" << endl;   
cout << "|  | |  \\   __\\/ __ \\  /  _ \\   __\\   |   ___// __ \\   __\\/  ___/" << endl;   
cout << "|  |_|  ||  | \\  ___/ (  <_> )  |     |  |   \\  ___/|  |  \\___ \\" << endl;   
cout << "|____/__||__|  \\___  > \\____/|__|     |__|    \\___  >__| /____  >" << endl;   
cout << "                   \\/                             \\/          \\/" << endl << endl;
cout << "                                  +&-" << endl;
cout << "                               _.-^-._    .--." << endl;
cout << "                            .-'   _   '-. |__|" << endl;
cout << "                           /     |_|     \\|  |" << endl;
cout << "                          /               \\  |" << endl;
cout << "                         /|     _____     |\\ |" << endl;
cout << "                          |    |==|==|    |  |" << endl;
cout << "      |---|---|---|---|---|    |--|--|    |  |" << endl;
cout << "      |---|---|---|---|---|    |==|==|    |  |" << endl;
cout << "     ^jgs^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl << endl;
}
void catArt() {
  cout << "*************************" << endl;
  cout << "*************************" << endl << endl;
  cout << "       /\\_____/\\" << endl;
  cout << "      /  o   o  \\ " << endl;
  cout << "     ( ==  ^  == ) " << endl;
  cout << "      )         ( " << endl;
  cout << "     (           ) " << endl;
  cout << "    (  (  ) (  )  ) " << endl;
  cout << "    (__(__)_(__)__)___/ " << endl << endl;
  cout << "*************************" << endl;
  cout << "*************************" << endl;
}
void owlArt() {
  cout << "*********************************" << endl;
  cout << "*********************************" << endl;
  cout << "           _________" << endl;
  cout << "          /_  ___   \\" << endl;
  cout << "         /0 \\/0  \\   \\" << endl;
  cout << "         \\__/\\___/   /" << endl;
  cout << "          \\_\\/______/" << endl;
  cout << "          /     /\\\\\\\\\\" << endl;
  cout << "         |     |\\\\\\\\\\\\" << endl;
  cout << "          \\       \\\\\\\\\\\\\\" << endl;
  cout << "           \\______/\\\\\\\\" << endl;
  cout << "      _______ ||_||_______" << endl;
  cout << "     (______(((_(((______(@)" << endl << endl;
  cout << "*********************************" << endl;
  cout << "*********************************" << endl;
}
void fishArt() {
  cout << "******************************" << endl;
  cout << "******************************" << endl;
  cout << endl;
  cout << "         o                 o" << endl;
  cout << "                    o" << endl;
  cout << "           o   ______      o" << endl;
  cout << "             _/  (   \\_" << endl;
  cout << "   _       _/  (       \\_  O" << endl;
  cout << "  | \\_   _/  (   (    0  \\" << endl;
  cout << "  |== \\_/  (   (          |" << endl;
  cout << "  |=== _ (   (   (        <" << endl;
  cout << "  |==_/ \\_ (   (          |" << endl;
  cout << "  |_/     \\_ (   (    \\__/" << endl;
  cout << "            \\_ (      _/" << endl;
  cout << "              |  |___/" << endl;
  cout << "             /__/" << endl << endl;
  cout << "******************************" << endl;
  cout << "******************************" << endl;
}

void catMenu(Cat* givenCat, int type) {
  int option = 0, rounds = 0;
  string givenFileName;
  
  while (option != 7) {
    if (rounds != 0 && rounds % 5 == 0) {
      givenCat->nextHour();
    }
    if (rounds != 0 && rounds % 2 == 0) {
      if (givenCat->gameStatus() == true) {
        exit(0);
      }
    }
    
    cout << "Please select from the following options: " << endl;
    cout << "  1. Feed" << endl;
    cout << "  2. Play" << endl;
    cout << "  3. Talk" << endl;
    cout << "  4. Cleaning session" << endl;
    cout << "  5. Scratch " << givenCat->getName() << "'s ears" << endl;
    cout << "  6. Display " << givenCat->getName() << "'s levels" << endl;
    cout << "  7. End game and save " << givenCat->getName() << "'s progress" << endl;
    
    cin >> option;
    while (!((option > 0) && (option < 8))) {
      cout << "Not a valid option. Please try again." << endl << endl;
      cin >> option;
    }
    rounds++;
    cout << endl;

    if (option == 1) {
      givenCat->feed();
    }
    else if (option == 2) {
      givenCat->play();
    }
    else if (option == 3) {
      givenCat->talk();
    }
    else if (option == 4) {
      givenCat->groom();
    }
    else if (option == 5) {
      givenCat->scratch();
    }
    else if (option == 6) {
      givenCat->displayLevels();
    }
    else if (option == 7) {
      cout << "Please enter a filename to save " << givenCat->getName() << " to:" << endl;
      cin >> givenFileName;
      cout << endl << "Thank you for playing! Goodbye!" << endl;
      givenCat->saveProgress(type, givenFileName);
    }
  }
}
void owlMenu(Owl* givenOwl, int type) {
  int option = 0, rounds = 0;
  string givenFileName;
  
  while (option != 7) {
    if (rounds != 0 && rounds % 5 == 0) {
      givenOwl->nextHour();
    }
    if (rounds != 0 && rounds % 2 == 0) {
      if (givenOwl->gameStatus() == true) {
        exit(0);
      }
    }
    
    cout << "Please select from the following options: " << endl;
    cout << "  1. Feed" << endl;
    cout << "  2. Play" << endl;
    cout << "  3. Talk" << endl;
    cout << "  4. Go hunting" << endl;
    cout << "  5. Activate night vision" << endl;
    cout << "  6. Display " << givenOwl->getName() << "'s levels" << endl;
    cout << "  7. End game and save " << givenOwl->getName() << "'s progress" << endl;
    
    cin >> option;
    while (!((option > 0) && (option < 8))) {
      cout << "Not a valid option. Please try again." << endl << endl;
      cin >> option;
    }
    rounds++;
    cout << endl;
    
    if (option == 1) {
      givenOwl->feed();
    }
    else if (option == 2) {
      givenOwl->play();
    }
    else if (option == 3) {
      givenOwl->talk();
    }
    else if (option == 4) {
      givenOwl->hunt();
    }
    else if (option == 5) {
      givenOwl->activateNightVision();
    }
    else if (option == 6) {
      givenOwl->displayLevels();
    }
    else if (option == 7) {
      cout << "Please enter a filename to save " << givenOwl->getName() << " to:" << endl;
      cin >> givenFileName;
      cout << endl << "Thank you for playing! Goodbye!" << endl;
      givenOwl->saveProgress(type, givenFileName);
    }
  }
}
void fishMenu(Fish* givenFish, int type) {
  int option = 0, rounds = 0;
  string givenFileName;
  
  while (option != 8) {
    if (rounds != 0 && rounds % 5 == 0) {
      givenFish->nextHour();
    }
    if (rounds != 0 && rounds % 2 == 0) {
      if (givenFish->gameStatus() == true) {
        exit(0);
      }
    }
    
    cout << "Please select from the following options: " << endl;
    cout << "  1. Feed" << endl;
    cout << "  2. Play" << endl;
    cout << "  3. Talk" << endl;
    cout << "  4. Give " << givenFish->getName() << " medicine" << endl;
    cout << "  5. Activate bionic fins" << endl;
    cout << "  6. Decorate fins" << endl;
    cout << "  7. Show " << givenFish->getName() << "'s levels" << endl;
    cout << "  8. End game and save " << givenFish->getName() << "'s progress" << endl;
    
    cin >> option;
    while (!((option > 0) && (option < 9))) {
      cout << "Not a valid option. Please try again." << endl << endl;
      cin >> option;
    }
    rounds++;
    cout << endl;
    
    if (option == 1) {
      givenFish->feed();
    }
    else if (option == 2) {
      givenFish->play();
    }
    else if (option == 3) {
      givenFish->talk();
    }
    else if (option == 4) {
      givenFish->medicine();
    }
    else if (option == 5) {
      givenFish->activateFins();
    }
    else if (option == 6) {
      givenFish->decorateFins();
    }
    else if (option == 7) {
      givenFish->displayLevels();
    }
    else if (option == 8) {
      cout << "Please enter a filename to save " << givenFish->getName() << " to:" << endl;
      cin >> givenFileName;
      cout << endl << "Thank you for playing! Goodbye!" << endl;
      givenFish->saveProgress(type, givenFileName);
    }
  }  
}

int Welcome() {
  int creation, petType;
  
  introArt();                  
  cout << " _______" << endl;                 
  cout << "(_  _  _)_____ ____  _   _" << endl;      
  cout << "| ||_|| | ___ |  _ \\| | | |" << endl;     
  cout << "| |   | | ____| | | | |_| |" << endl;     
  cout << "|_|   |_|_____)_| |_|_____/" << endl << endl;
  cout << "Select a number:" << endl;
  cout << "(1) Create new pet" << endl << "(2) Load saved pet" << endl << endl;

  cin >> creation;
  cout << endl;
  if (creation == 1) {
    cout << "Select your pet:" << endl;
    cout << "(1) Cat" << endl << "(2) Owl" << endl;
    cout << "(3) Fish" << endl << endl;
    cin >> petType;
    cout << endl;
    if (petType == 1) {
      catArt();
    }
    else if (petType == 2) {
      owlArt();
    }
    else {
      fishArt();
    }
    return petType;
  }
  else {    
    return 0;
  }
  
}

int main() {
  string givenNm, givenFileName;
  int option = 0, rounds = 0;
  int petTypeResponse = Welcome();

  // If Loading Saved Pet
  if (petTypeResponse == 0) {
    string savedFileName;
    int option = 0;
    int savedType, savedHunger, savedSleepy, savedHappiness, savedAge;
    string savedName;
    ifstream inFS;
      
    cout << "Please enter saved filename: " << endl;
    cin >> savedFileName;
    cout << endl;
    inFS.open(savedFileName + ".txt");
      
    while (inFS.is_open() == false) {
      cout << "File does not exist, reenter filename:" << endl;
      cin >> savedFileName;
      cout << endl;
      inFS.open(savedFileName + ".txt");
    }
      
    inFS >> savedType;
      
    if (savedType == 1) {
      Cat* cat = new Cat();
      int savedVoiceVol;
        
      inFS >> savedName >> savedHunger >> savedSleepy >> savedHappiness >> savedVoiceVol >> savedAge;
      cat->setName(savedName);
      cat->setHunger(savedHunger);
      cat->setSleepy(savedSleepy);
      cat->setHappiness(savedHappiness);
      cat->setVoiceVol(savedVoiceVol);
      cat->setAge(savedAge);

      cout << "Your pet: " << cat->getName() << endl << endl;
      catArt();
      cout << endl;
      catMenu(cat, savedType);
    }
      
    else if (savedType == 2) {
      Owl* owl = new Owl();
      int savedTalonStr;
      
      inFS >> savedName >> savedHunger >> savedSleepy >> savedHappiness >> savedTalonStr >> savedAge;
      owl->setName(savedName);
      owl->setHunger(savedHunger);
      owl->setSleepy(savedSleepy);
      owl->setHappiness(savedHappiness);
      owl->setTalonStr(savedTalonStr);
      owl->setAge(savedAge);
      
      cout << "Your pet: " << owl->getName() << endl << endl;
      owlArt();
      cout << endl;
      owlMenu(owl, savedType);
    }
      
    else if (savedType == 3) {
      Fish* fish = new Fish();
      int savedHealth;
      
      inFS >> savedName >> savedHunger >> savedSleepy >> savedHappiness >> savedHealth >> savedAge;
      fish->setName(savedName);
      fish->setHunger(savedHunger);
      fish->setSleepy(savedSleepy);
      fish->setHappiness(savedHappiness);
      fish->setHealth(savedHealth);
      fish->setAge(savedAge);

      cout << "Your pet: " << fish->getName() << endl << endl;
      fishArt();
      cout << endl;
      fishMenu(fish, savedType);
    }
      
    inFS.close(); 
  }

  // If Creating New Pet
  else {
    cout << endl << "What's your pet's name? " << endl;
    cin >> givenNm;
    cout << endl;
    
    if (petTypeResponse == 1) {
      Cat* cat = new Cat();
      cat->setName(givenNm);
      catMenu(cat, petTypeResponse);
      delete cat;
    }
    else if (petTypeResponse == 2) {
      Owl* owl = new Owl();
      owl->setName(givenNm);
      owlMenu(owl, petTypeResponse);
      delete owl;
    }
    else if (petTypeResponse == 3) {
      Fish* fish = new Fish();
      fish->setName(givenNm);
      fishMenu(fish, petTypeResponse);
      delete fish;
    }
  }
  
  
  return 0;
}
