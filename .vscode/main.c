#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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


// Mostrar ações do personagem escolhido
void mostrarAcoes(PCharacter personagem){

  for (int i = 0; i < 4; i++){
    printf("%d: %s  Usos restantes:%d ", i, personagem.habilidades.pack[i].n_habilidade, personagem.habilidades.pack[i].disponibilidade[0]);
    if (i%2 == 1){
      printf("\n");
    }
  }

}


// Ações do Mago (arg[0])
void* Acao_mago(PCharacter* arg){
  int escolha;
  int critico = 1;
  int dano_causado;
  int alvo;
  bool valido = false;
  // Enquanto o boss e o mago estiverem vivos o mago poderá agir
  while(HP_Boss != 0 && arg[0].HP != 0){
    sem_wait(&Turnos_Mago); // Espera o mago ter sua ação disponível

    // Travar a sessão crítica
    pthread_mutex_lock(&Turno_Atual);


    while(!valido){
      // Mostrar as ações que podem ser tomadas
      printf("Turno do mago. O que ele deve usar: \n ");
      mostrarAcoes(arg[0]);
      printf("\n");

      scanf("%d", &escolha);
      switch (escolha){
      
      case 0:
        // Se não tiver usos restantes da habilidade ela não poderá ser usada
        if(arg[0].habilidades.pack[0].disponibilidade[0] == 0){
          printf("Não há usos restantes dessa habilidae, use outra!\n");
          valido = true;
          break;
        }

        // Escolha de alvo sendo validada
        while (alvo != 0 && alvo != 1){
          printf("Escolha o alvo \n[0] Mago HP: %d/60    [1] Combatente HP: %d/180", arg[0].HP, arg[1].HP);
          scanf("%d", &alvo);

          if(alvo > 1 || alvo < 0){
            printf("Escolha um valor válido! Você não quer curar seu inimigo.\n");
          }
        }

        // Cura realizada
        printf("Alvo curado em 20 pontos. \n");
        //arg[alvo].HP += 20;
        setHP(arg[alvo], getHP(arg[alvo]));

        // Valida a escolha
        valido = true;
        
        break;
      
      case 1:
        // Se não tiver usos restantes não poderá usar a habilidade
        if(arg[0].habilidades.pack[1].disponibilidade[0] == 0){
          printf("Não há usos restantes dessa habilidae, use outra!\n");
          break;
        }

        // Verificar se acertou o ataque (70% de chance de acerto)
        if((rand()%99)+1 <= 30){
          printf("O ataque não acertou!\n");
          break;
        }

        /*TODO: funções da habilidade de dano do mago*/

        break;



      default:
        printf("Escolha inválida. Insira apenas números no intervalo [0 - 3].\n");
        break;
      }
    }

    pthread_mutex_unlock(&Turno_Atual);


  }
  if(arg[0].HP == 0)
    printf("O Mago caiu! \n");

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
  skill Cura = createSkill("Curar", "Vida", 20, "Cura", 10),
  FlechaElemental = createSkill("Flecha Flamejante", "Fogo", 5, "Queimaduras", 20), 
  RaioNecrotico = createSkill("Raio Necrótico", "Necrotico", 15, "Alquebrado", 10),
  Bruxaria = createSkill("Bruxaria", "Necrotico", 0, "Amaldicoado", 15);

  skill conjunto2[4] = {Cura, FlechaElemental, RaioNecrotico, Bruxaria};

  skillPack Mago = createSkillPack(conjunto2);

  PCharacter Fulano = characterCreate("Fulano", "Mago", 60, Mago);
  PCharacter Siclano = characterCreate("Siclano", "Combatente", 180, Combatente);

  





  return 0;


}



