#ifndef __PCHARACTER_H__
#define __PCHARACTER_H__


#include "skills.h"

typedef struct PCharacter {
  // Limitando o tamanho dos nomes e classe
  const char nome[50];
  const char classe[50];
  int HP;
  skillPack habilidades;

}PCharacter;

// Cosntrutor
PCharacter characterCreate(const char *nome,const char *classe ,int HP, skillPack habilidades);

// Setter
void setHP(PCharacter character, int value);

// Getters
char* getName(PCharacter character);
char* getClasse(PCharacter character);
int getHP(PCharacter character);
skillPack getPack(PCharacter character);

#endif