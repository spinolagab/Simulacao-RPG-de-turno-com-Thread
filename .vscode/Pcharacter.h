#ifndef __PCHARACTER_H__
#define __PCHARACTER_H__

#include "skills.h"

typedef struct PCharacter {
  // Limitando o tamanho dos nomes e classe
  char nome[50];
  char classe[50];
  int HP;
  skillPack habilidades;

}PCharacter;

typedef struct ThreadData{
  PCharacter personagens[2];
} ThreadData;

// Cosntrutor
PCharacter characterCreate(const char *nome,const char *classe ,int HP, skillPack habilidades);
ThreadData characterPack(PCharacter *personagens);

// Setter
void setHP(PCharacter *character, int value);

// Getters
char* getName(PCharacter *character);
char* getClasse(PCharacter *character);
int getHP(PCharacter *character);
skillPack getPack(PCharacter *character);

#endif