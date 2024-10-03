#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Pcharacter.h"
#include "skills.h"
#include <pthread.h>
#include <semaphore.h>


typedef struct ThreadData{
  PCharacter *personagens;
  int numPersonagens;
} ThreadData;


// Mutex
pthread_mutex_t Turno_Atual;

// Semaforos
sem_t Turnos_Mago;
sem_t Turnos_Combatente;
sem_t Turnos_Boss;

// Dados do Boss
char* nome_boss = "Beltrano";
int HP_Boss = 400;
char Efeito_Status[50];
int duracao = 0;
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


// Ações do Mago (personagem[0])
void* Acao_mago(void* arg){
  int escolha, critico = 1, dano_causado, alvo, roll;
  bool valido = false;
  ThreadData *data = (ThreadData*)arg;
  PCharacter *personagem = data->personagens;

  // Enquanto o boss e o mago estiverem vivos o mago poderá agir
  while(HP_Boss != 0 && personagem[0].HP != 0){
    sem_wait(&Turnos_Mago); // Espera o mago ter sua ação disponível

    // Travar a sessão crítica
    pthread_mutex_lock(&Turno_Atual);


    while(!valido){
      // Mostrar as ações que podem ser tomadas
      printf("Turno do mago. O que ele deve usar: \n ");
      roll = (rand()%99)+1;
      mostrarAcoes(personagem[0]);
      printf("\n");

      scanf("%d", &escolha);
      switch (escolha){
      
      case 0:
        // Se não tiver usos restantes da habilidade ela não poderá ser usada
        if(personagem[0].habilidades.pack[0].disponibilidade[0] == 0){
          printf("Não há usos restantes dessa habilidade, use outra!\n");
          valido = true;
          break;
        }
        personagem[0].habilidades.pack[0].disponibilidade[0] -= 1;
        // Escolha de alvo sendo validada
        while (alvo != 0 && alvo != 1){
          printf("Escolha o alvo \n[0] Mago HP: %d/60    [1] Combatente HP: %d/180", personagem[0].HP, personagem[1].HP);
          scanf("%d", &alvo);

          if(alvo > 1 || alvo < 0){
            printf("Escolha um valor válido! Você não quer curar seu inimigo.\n");
          }
        }

        // Cura realizada
        printf("Alvo curado em 20 pontos. \n");
        //personagem[alvo].HP += 20;
        personagem[alvo].HP += 20;

        // Valida a escolha
        valido = true;
        
        break;
     // Flecha flamejante 
      case 1:
        // Se não tiver usos restantes não poderá usar a habilidade
        if(personagem[0].habilidades.pack[1].disponibilidade[0] == 0){
          printf("Não há usos restantes dessa habilidade, use outra!\n");
          break;
        }

        personagem[0].habilidades.pack[1].disponibilidade[0] -= 1;
        
        // Verificar se acertou o ataque (70% de chance de acerto)
        if(roll <= 30){
          printf("O ataque não acertou!\n");
          break;
        }

        if (roll >= 90){
          critico = 2;
          printf("Acerto crítico!\n");
        }

        dano_causado = personagem[0].habilidades.pack[1].dano * critico;

        HP_Boss -= dano_causado;

        // Calcular a chance de aplicar queimaduras (50%)
        if(Efeito_Status[0] == '\0'){
          if((rand()%99)+1 > 50){
            strcpy(Efeito_Status, personagem[0].habilidades.pack[1].efeito);
            printf("O alvo foi queimado!\n");
            // Deixar o efeito aplicado por até 5 rodadas (causando 5 de dano por rodada)
            duracao = 2+(rand()%4) ;
          }
        }

        // Valida a escolha e reseta o crítico
        valido = true;
        critico = 1;
        break;

      case 2:
        // Se não tiver usos restantes não poderá usar a habilidade
        if(personagem[0].habilidades.pack[2].disponibilidade[0] == 0){
          printf("Não há usos restantes dessa habilidade, use outra!\n");
          break;
        }
        
        personagem[0].habilidades.pack[2].disponibilidade[0] -= 1;

        // Verificar se acertou o ataque (55% de chance de acerto)
        if(roll <= 45){
          printf("O ataque não acertou!\n");
          break;
        }

        if (roll >= 80){
          critico = 4;
          printf("Acerto crítico!\n");
        }

        dano_causado = personagem[0].habilidades.pack[2].dano * critico;

        HP_Boss -= dano_causado;

        // Calcular a chance de aplicar queimaduras (65%)
        if(Efeito_Status[0] == '\0'){
          if((rand()%99)+1 > 65){
            strcpy(Efeito_Status, personagem[0].habilidades.pack[2].efeito);
            printf("A ferida está necrosando!\n");
            // Deixar o efeito aplicado por até 6 rodadas (causando 6 de dano por rodada)
            duracao = 2+(rand()%4) ;
          }
        }

        // Valida a escolha e reseta o crítico
        valido = true;
        critico = 1;
        break;

      case 3:
        // Se não tiver usos restantes não poderá usar a habilidade
        if(personagem[0].habilidades.pack[3].disponibilidade[0] == 0){
          printf("Não há usos restantes dessa habilidade, use outra!\n");
          break;
        }
        
        personagem[0].habilidades.pack[3].disponibilidade[0] -= 1;
        // Verificar se acertou o ataque (50% de chance de acerto)
        if(roll <= 50){
          printf("A maldição falhou!\n");
          break;
        }

        if(Efeito_Status[0] == '\0'){
          if((rand()%99)+1 > 65){
            strcpy(Efeito_Status, personagem[0].habilidades.pack[2].efeito);
            printf("A ferida está necrosando!\n");
            // Deixar o efeito aplicado por até 6 rodadas (causando 6 de dano por rodada)
            duracao = 2+(rand()%4) ;
          }
        } else{
          printf("O alvo já está sob algum efeito!");
        }

        // Valida a escolha e reseta o crítico
        valido = true;
        critico = 1;

        break;

      default:
        printf("Escolha inválida. Insira apenas números no intervalo [0 - 3].\n");
        break;
      }
    }

    pthread_mutex_unlock(&Turno_Atual);

    valido = false;

  }
  if(personagem[0].HP == 0)
    printf("O Mago caiu! \n");

}

void* Acao_combatente(void* arg){
  int escolha, critico = 1, dano_causado, alvo, roll;
  bool valido = false;
  ThreadData *data = (ThreadData*)arg;
  PCharacter *personagem = data->personagens;

  // Enquanto o boss e o combatente estiverem vivos o combatente poderá agir
  while(HP_Boss != 0 && personagem[1].HP != 0){
    sem_wait(&Turnos_Combatente); // Espera o combatente ter sua ação disponível

    // Travar a sessão crítica
    pthread_mutex_lock(&Turno_Atual);


    while(!valido){
      // Mostrar as ações que podem ser tomadas
      printf("Turno do combatente. O que ele deve usar: \n ");
      roll = (rand()%99)+1;
      mostrarAcoes(personagem[1]);
      printf("\n");

      scanf("%d", &escolha);
      switch (escolha){
      
      case 0:
        // Se não tiver usos restantes da habilidade ela não poderá ser usada
        if(personagem[1].habilidades.pack[0].disponibilidade[0] == 0){
          printf("Não há usos restantes dessa habilidade, use outra!\n");
          valido = true;
          break;
        }
        personagem[1].habilidades.pack[0].disponibilidade[0] -= 1;

        // Verificar se acertou o ataque (70% de chance de acerto)
        if(roll <= 30){
          printf("O ataque não acertou!\n");
          break;
        }

        if (roll >= 90){
          critico = 2;
          printf("Acerto crítico!\n");
        }

        dano_causado = personagem[1].habilidades.pack[1].dano * critico;

        HP_Boss -= dano_causado;

        // Valida a escolha
        valido = true;
        
        break;

      case 1:
        // Se não tiver usos restantes não poderá usar a habilidade
        if(personagem[1].habilidades.pack[1].disponibilidade[0] == 0){
          printf("Não há usos restantes dessa habilidade, use outra!\n");
          break;
        }

        personagem[1].habilidades.pack[1].disponibilidade[0] -= 1;
        
        // Verificar se acertou o ataque (70% de chance de acerto)
        if(roll <= 30){
          printf("O ataque não acertou!\n");
          break;
        }

        if (roll >= 90){
          critico = 2;
          printf("Acerto crítico!\n");
        }

        dano_causado = personagem[1].habilidades.pack[1].dano * critico;

        HP_Boss -= dano_causado;

        // Calcular a chance de atordoar (50%)
        if(Efeito_Status[0] == '\0'){
          if((rand()%99)+1 > 50){
            strcpy(Efeito_Status, personagem[1].habilidades.pack[1].efeito);
            printf("%s está atordoado!\n",nome_boss);
            // Deixar o efeito aplicado por até 5 rodadas (causando 5 de dano por rodada)
            duracao = 2+(rand()%4) ;
          }
        }

        // Valida a escolha e reseta o crítico
        valido = true;
        critico = 1;
        break;

      case 2:
        // Se não tiver usos restantes não poderá usar a habilidade
        if(personagem[1].habilidades.pack[2].disponibilidade[0] == 0){
          printf("Não há usos restantes dessa habilidade, use outra!\n");
          break;
        }
        
        personagem[1].habilidades.pack[2].disponibilidade[0] -= 1;

        // Verificar se acertou o ataque (65% de chance de acerto)
        if(roll <= 35){
          printf("O ataque não acertou!\n");
          break;
        }

        if (roll >= 80){
          critico = 4;
          printf("Acerto crítico!\n");
        }

        dano_causado = personagem[1].habilidades.pack[2].dano * critico;

        HP_Boss -= dano_causado;

        // Calcular a chance de aplicar multi-attack (65%)
        if(Efeito_Status[0] == '\0'){
          if((rand()%99)+1 > 65){
            strcpy(Efeito_Status, personagem[1].habilidades.pack[2].efeito);
            printf("Prepare-se para um multi-attack!\n");
            // Continue atacando por até 6 rodadas (causando 5 de dano por rodada)
            duracao = 2+(rand()%4) ;
          }
        }

        // Valida a escolha e reseta o crítico
        valido = true;
        critico = 1;
        break;

      case 3:
        // Se não tiver usos restantes não poderá usar a habilidade
        if(personagem[1].habilidades.pack[3].disponibilidade[0] == 0){
          printf("Não há usos restantes dessa habilidade, use outra!\n");
          break;
        }
        
        personagem[1].habilidades.pack[3].disponibilidade[0] -= 1;
        // Verificar se restringiu o ataque do boss (50% de chance de acerto)
        if(roll <= 50){
          printf("Restringir falhou!\n");
          break;
        }

        // Restrição realizada
        printf("Defesa será aumentada nesse turno! \n");
        danoAtaque/2;

        // Valida a escolha
        valido = true;
        
        break;

      default:
        printf("Escolha inválida. Insira apenas números no intervalo [0 - 3].\n");
        break;
      }
    }

    pthread_mutex_unlock(&Turno_Atual);

    valido = false;

  }
  if(personagem[1].HP == 0)
    printf("O Combatente caiu! \n");

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
  FlechaElemental = createSkill("Flecha Flamejante", "Fogo", 10, "Queimaduras", 20), 
  RaioNecrotico = createSkill("Raio Necrótico", "Necrotico", 15, "Necrose", 10),
  Bruxaria = createSkill("Bruxaria", "Necrotico", 0, "Amaldicoado", 15);

  skill conjunto2[4] = {Cura, FlechaElemental, RaioNecrotico, Bruxaria};

  skillPack Mago = createSkillPack(conjunto2);

  PCharacter Fulano = characterCreate("Fulano", "Mago", 60, Mago);
  PCharacter Siclano = characterCreate("Siclano", "Combatente", 180, Combatente);

  





  return 0;

}
