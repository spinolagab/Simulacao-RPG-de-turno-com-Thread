#ifndef __SKILL_H__
#define __SKILL_H__

typedef struct skill{
  // Limitando o tamanho máximo de nome
  char n_habilidade[50];
  char elemento[50];
  int dano;
  char efeito[50];
  int disponibilidade[2];
}skill;

typedef struct skillPack{
  
  skill pack[4];

}skillPack;


// Construtor
skill createSkill(char *n_habilidade, char *elemento, int dano, char *efeito, int disponibilidade);
skillPack createSkillPack(skill *habilidades);

// Getters
char* getNome(skill* skill);
char* getElemento(skill* skill);
int getDano(skill* skill);
char* getEfeito(skill* skill);
int getDisponibilidade(skill skill);
skill getSkillIndex(skillPack pack, int index);

#endif