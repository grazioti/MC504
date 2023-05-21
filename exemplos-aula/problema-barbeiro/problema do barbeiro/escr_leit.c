#define __USE_GNU 1 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <semaphore.h>

#define N_ESCRITORES 1
#define N_ESCRITAS_POR_ESCRITOR 2
#define N_LEITORES 1
#define N_LEITURAS_POR_LEITOR 2

int painel;
sem_t sem_escrita;
sem_t sem_leitura;

void* f_escrita(void *v) {
  int id = (int) v;
  int i;
  
  for (i = 0; i < N_ESCRITAS_POR_ESCRITOR; i++) {
    sem_wait(&sem_escrita);
    printf("Escritor %d escreveu %d\n", id, id); 
    sem_post(&sem_leitura);
    sleep(random() % 3);  
  }
  return NULL;
}

void* f_leitura(void *v) {
  int id = (int) v;
  int i;
  
  for (i = 0; i < N_LEITURAS_POR_LEITOR; i++) {
    sem_wait(&sem_leitura);
    printf("Leitor %d leu %d\n", id, id); 
    sem_post(&sem_escrita);
    sleep(random() % 3);  
  }
  return NULL;
}

int main() {

  pthread_t thr_escritores[N_ESCRITORES],
    thr_leitores[N_LEITORES];
  int i;
  
  sem_init(&sem_escrita, 0, 1);
  sem_init(&sem_leitura, 0, 0);

  for (i = 0; i < N_ESCRITORES; i++)
    pthread_create(&thr_escritores[i], NULL, f_escrita, (void*) i);

  for (i = 0; i < N_LEITORES; i++)
    pthread_create(&thr_leitores[i], NULL, f_leitura, (void*) i);  

  for (i = 0; i < N_ESCRITORES; i++)  
    pthread_join(thr_escritores[i], NULL);

  for (i = 0; i < N_LEITORES; i++)  
    pthread_join(thr_leitores[i], NULL);  

  sem_destroy(&sem_escrita);
  sem_destroy(&sem_leitura);
  
  return 0;
}

