#define __USE_GNU 1 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <semaphore.h>

#define N_THREADS 4
#define N_RECURSOS 1
#define N_USOS 2

sem_t sem_recursos;
char recursos_em_uso[N_THREADS];

/* Função da thread que executa sem_post */
void* f_thread(void *v) {
  int id = (int) v;
  int i;
  
  for (i = 0; i < N_USOS; i++) {
    sem_wait(&sem_recursos);
    printf("Thread %d está usando um recurso.\n", id);     
    sleep(random() % 3);
    printf("Thread %d vai liberar.\n", id);
    sem_post(&sem_recursos);    
  }
  return NULL;
}

int main() {
  pthread_t thr[N_THREADS];
  int i;
  
  sem_init(&sem_recursos, 0, N_RECURSOS);

  for (i = 0; i < N_THREADS; i++)
    pthread_create(&thr[i], NULL, f_thread, (void*) i);

  for (i = 0; i < N_THREADS; i++)
    pthread_join(thr[i], NULL); 

  sem_destroy(&sem_recursos);
  
  return 0;
}

