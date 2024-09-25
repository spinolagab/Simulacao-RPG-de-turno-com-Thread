#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Pcharacter.h"
#include "skills.h"
#include <pthread.h>
#include <semaphore.h>

// Mutex
pthread_mutex_t Turno_Atual;

// Semaforos
sem_t Turnos_Mago;
sem_t Turnos_Combatente;
sem_t Turnos_Boss;

// Dados do Boss
int HP_Boss = 400;
char* Efeito_Status[50];
int danoAtaque = 10;



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

PCharacter characterCreate(const char *nome,const char *classe ,int HP, skillPack habilidades){
  PCharacter personagem;

  //personagem.nome = nome;
  strncpy(personagem.nome, nome, sizeof(personagem.nome)-1);
  personagem.nome[sizeof(personagem.nome)-1];

  //personagem.classe = classe;
  strncpy(personagem.classe, classe, sizeof(personagem.classe)-1);
  personagem.classe[sizeof(personagem.classe)-1];

  personagem.HP = HP;

  personagem.habilidades = habilidades;

}

void mostrarAcoes(PCharacter personagem){

  for (int i = 0; i < 4; i++){
    printf("%d: %s  Usos restantes:%d ", i, personagem.habilidades.pack[i].n_habilidade, personagem.habilidades.pack[i].disponibilidade[0]);
    if (i%2 == 1){
      printf("\n");
    }
  }

}

void* Acao_mago(PCharacter arg){
  int escolha;
  int critico = 1;
  // Enquanto o boss e o mago estiverem vivos o mago poderá agir
  while(HP_Boss != 0 && arg.HP != 0){
    sem_wait(&Turnos_Mago); // Espera o mago ter sua ação disponível

    // Travar a sessão crítica
    pthread_mutex_lock(&Turno_Atual);

    // Mostrar as ações que podem ser tomadas
    printf("Turno do mago. O que ele deve usar: \n ");
    mostrarAcoes(arg);
    printf("\n");

    scanf("%d", &escolha);
    




  }
}

int main(void){

  // Pack de skills para o combatente
  skill Espadada = createSkill("Espadada", "Cortante", 8, "Nenhum", 20), 
  Escudada = createSkill("Golpe de Escudo", "Impacto", 2, "Atordoar", 15), 
  Flechada = createSkill("Saraivada de Flechas", "Perfurante", 5, "Multiattack", 10),
  Restringir = createSkill("Restringir", "-", 0, "Agarrado", 10);

  skill conjunto1[4] = {Espadada, Escudada, Flechada, Restringir};

  skillPack Combatente = createSkillPack(conjunto1);

  // Pack de skills para o mago
  skill Cura = createSkill("Cura", "Vida", 20, "Cura", 10),
  FlechaElemental = createSkill("Flecha Flamejante", "Fogo", 5, "Queimaduras", 20), 
  RaioNecrotico = createSkill("Raio Necrótico", "Necrotico", 15, "Alquebrado", 10),
  Bruxaria = createSkill("Bruxaria", "Necrotico", 0, "Amaldicoado", 15);

  skill conjunto2[4] = {Cura, FlechaElemental, RaioNecrotico, Bruxaria};

  skillPack Mago = createSkillPack(conjunto2);

  PCharacter Fulano = characterCreate("Fulano", "Mago", 60, Mago);
  PCharacter Siclano = characterCreate("Siclano", "Combatente", 180, Combatente);

  





  return 0;


}



