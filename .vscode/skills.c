#include "skills.h"
#include <string.h>

// Construtores
skill createSkill (char *n_habilidade, char *elemento, int dano, char *efeito, int disponibilidade){
  skill habilidade;

  //habilidade.n_habilidade = n_habilidade;

  strncpy(habilidade.n_habilidade, n_habilidade, sizeof(habilidade.n_habilidade)-1);
  habilidade.n_habilidade[sizeof(habilidade.n_habilidade)-1] = '\0';

  //habilidade.elemento = elemento;
  strncpy(habilidade.elemento, elemento, sizeof(habilidade.elemento)-1);
  habilidade.elemento[sizeof(habilidade.elemento)-1] = '\0';
  
  habilidade.dano = dano;
  
  //habilidade.efeito = efeito;
  strncpy(habilidade.efeito, efeito, sizeof(habilidade.efeito)-1);
  habilidade.efeito[sizeof(habilidade.efeito)-1] = '\0';

  habilidade.disponibilidade[1] = disponibilidade;
  habilidade.disponibilidade[0] = habilidade.disponibilidade[1];

  return habilidade;

}

skillPack createSkillPack(skill *habilidades){
  skillPack conjunto;

  for(int i = 0; i < 4; i++){
    conjunto.pack[i] = habilidades[i];
  }

  return conjunto;
}

// Getters
char* getNome(skill* habilidade){
  return habilidade->n_habilidade;
}

char* getElemento(skill* habilidade){
  return habilidade->elemento;
}

int getDano(skill* habilidade){
  return habilidade->dano;
}

char* getEfeito(skill* habilidade){
  return habilidade->efeito;
}

int getDisponibilidade(skill* habilidade){
  return habilidade->disponibilidade[0];
}

skill getSkillIndex(skillPack* pack, int index){
  return pack->pack[index];
}