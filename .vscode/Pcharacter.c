#include "Pcharacter.h"
#include "skills.h"
#include <string.h>


// Construtor
PCharacter characterCreate(const char *nome,const char *classe ,int HP, skillPack habilidades){
  PCharacter personagem;

  //personagem.nome = nome;
  strncpy(personagem.nome, nome, sizeof(personagem.nome)-1);
  personagem.nome[sizeof(personagem.nome)-1] = '\0';

  //personagem.classe = classe;
  strncpy(personagem.classe, classe, sizeof(personagem.classe)-1);
  personagem.classe[sizeof(personagem.classe)-1] = '\0';

  personagem.HP = HP;

  personagem.habilidades = habilidades;

  return personagem;

}

ThreadData characterPack(PCharacter *personagens){
  ThreadData data;

  data.personagens[0] = personagens[0];
  data.personagens[1] = personagens[1];
  
  return data;
}

// Setter
void setHP(PCharacter *character, int value){
  character->HP = value;
}


// Getters
char* getName(PCharacter *character){
  return character->nome;
}

char* getClasse(PCharacter *character){
  return character->classe;
}

int getHP(PCharacter *character){
  return character->HP;
}

skillPack getPack(PCharacter *character){
  return character->habilidades;
}
