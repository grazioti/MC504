# Projeto Multithread

## Integrantes
Projeto desenvolvido na disciplina de Sistemas Operacionais (MC504), na Unicamp, durante o primeiro semestre de 2023, pelos seguintes alunos:
 * César Devens Grazioti - RA: 195641
 * João Miguel De Oliveira Guimarães - RA: 174358
 * Otavio Anovazzi - RA: 186331
 * Renan Luis Moraes De Sousa - RA: 243792

## Objetivo
O seguinte projeto tem o objetivo de ilustrar uma aplicação multithread utilizando semáforos e/ou mutex locks e variáveis de condição para sincronização. Em sua execução é ilustrado o estado global da aplicação e o processo da execução em passos por meio de uma animação obtida com a impressão de caracteres ASCII.

## Introdução
Para desenvolvimento do projeto, utlizamos o capítulo 5.8 do livro *The Little Book of Semaphores* de Allen B. Downey. Neste capítulo é discutido o Problema da Montanha Russa e suas variações, além de apresentar dicas e ideias para sua resolução. Abaixo segue a solução e respectiva vizualização proposta por nós.

## Vídeo com descrição do problema abordado
Inserir vídeo

# O Problema da Montanha Russa

## Descrição do problema

Suponha que haja N passageiros e um trem de montanha-russa. Os passageiros esperam repetidamente para passear na montanha-russa. O trem tem capacidade para C lugares, C < N, mas o parque não deixa o trem sair da estação até que esteja cheio.

## Detalhes adicionais

- Passageiros podem solicitar embarque e desembarque;
- O trem pode sinalizar três diferentes estados: embarque, andando ou desembarque;
- Os passageiros não podem embarcar enquanto o trem sinalizar1que o embarque está disponível;
- O trem não pode partir até que os C passageiros tenham embarcado.
- Os passageiros não podem desembarcar até que o trem inicie o período de desembarque;

# Implementação
Para a solução do problema, utilizamos N threads para os passageiros e uma thread para o carro. Definimos também um número N_RUNS de voltas que o carro executa. Isso é análogo ao parque fechar e os passageiros irem embora (threads morrem).

## Semáforos, mutex locks e/ou variáveis de condição
- mutex: protege os passageiros, contando o número de passageiros que
invocaram um embarque.
- mutex2: protege os passageiros, contando o número de passageiros que
invocaram um desembarque.
- mutexPrint = Semaphore(1)
- boarders: variável utilizada para armazenar o número de passageiros que embarcou no trem;
- unboarders: variável utilizada para armazenar o número de passageiros que desembarcou no trem;
- boardQueue = Semaphore (0)
- unboardQueue = Semaphore (0)
- allAboard: semáforo que indica quando o trem está completo com C passageiros.
- allAshore: semáforo que indica quando todos os C passageiros desembarcaram do trem.

## Thread do carro
A thread do carro fica em loop até que a condição de parada seja atingida: o número de voltas que o trem completou é exatamente igual a N_RUNS (hora de fechar o parque!). Inicialmente, ela posta que o embarque iniciou e espera até que todos os C passageiros estejam abordo. Posteriormente, podemos iniciar o passeio. Então, rodamos uma animação da montanha russa até o trem completar uma volta na tela. Quando ele volta a posição inicial, sabemos que ele completou uma volta e, portanto, devemos sinalizar que o desembarque iniciou. Esperamos que todos os passageiros estejam em terra e o processo se reinicia.

~~~c
void* f_car(void *v) {
    int runs = 0;
    //printf("The roller coaster has opened, let's go!\n");
    while(runs < N_RUNS) {
        //printf("Boarding time!\n");
        for (int i = 0; i < CAPACITY; i++) {
            sem_post(&boardQueue);
        }
        sem_wait(&allAboard);

        sem_wait(&mutexPrint);
        //printf("Start of the tour.\n");
        move_train();
        //printf("End of the tour.\n");
        sem_post(&mutexPrint);

        //printf("Unboarding time!\n");
        for (int i = 0; i < CAPACITY; i++) {
            sem_post(&unboardQueue);
        }
        sem_wait(&allAshore);

        runs++;
    }
    //printf("It was good, but it's over! Time to close the park.\n");
    return NULL;
}
~~~

## Thread dos passageiros
Cada passageiro possui uma thread que é associada a um identificador id. Eles esperam até que o embarque se inicie (seja postado pelo carro) e também esperam sua vez por meio da sinalização do *mutex*. Uma vez conseguido esse acesso, o passageiro embarca no trem, é removido da fila e executado um print desse estado da aplicação. Se este for o último passageiro a embarcar no trem, é sinalizado que o carro está completo com C passageiros. Nesse instante, os passageiros que embarcaram estão num tour pela montanha russa e aguardam até que seja postada a liberação de desembarque. Além disso, é esperado que sua vez do desembarque ocorra, através do *mutex2*. Consequentemente, a medida que um passageiro desembarca, mostramos o estado atual da aplicação e reinserimos na fila de embarque. Quando o último passageiro desembarca, é sinalizado que o carro está totalmente vazio e pronto para um novo passeio. 

~~~c
void* f_passenger(void *v) {
    while(1) {
        int id = *(int*) v;
        int time = random() % 3;

        sleep(time);

        sem_wait(&boardQueue);

        sem_wait(&mutex);
        int pos = boarders;
        listaVagoes[pos].pessoa = id;
        boarders += 1;

        sem_wait(&mutexPrint);
        //printf("Passenger %d boarded the car.\n", id);
        remove_person(&l,id);
        //entra no carro
        render_line(l);
        adicionar_trem_buffer(listaVagoes,0);
        print_on_screen(true);

        sem_post(&mutexPrint);

        if (boarders == CAPACITY) {
            sem_post(&allAboard);
            //printf("The car is complete with %d passengers.\n", CAPACITY);
            
            boarders = 0;
        }
        sem_post(&mutex);

        sem_wait(&unboardQueue);
        
        
        sem_wait(&mutexPrint);

        //printf("Passenger %d unboarded the car.\n", id);
        //Sai do carro
        listaVagoes[pos].pessoa = EMPTY;
        add_person(&l,id);
        render_line(l);
        adicionar_trem_buffer(listaVagoes,0);
        print_on_screen(true);

        sem_post(&mutexPrint);

        sem_wait(&mutex2);
        unboarders += 1;
        if (unboarders == CAPACITY) {
            sem_post(&allAshore);
            //printf("The %d passengers disembarked from the car.\n", CAPACITY);
            unboarders = 0;
        }
        sem_post(&mutex2);
    }
    return NULL;
}
~~~

## Animação

# Executando na sua máquina
Nesse tópico iremos ensinar um passo a passo de como baixar, configurar e executar o projeto no seu computador.

1. Acesse o link do repositório https://github.com/grazioti/MC504 contendo o projeto. Faça um clone para seu computador utilizando o comando `git clone https://github.com/grazioti/MC504` ou faça um **Download Zip** dos arquivos;
2. Abra em seu sistema de arquivos a pasta **projetomultithread**;
3. Segurando o botão Shift, clique com o botão direito do mouse nessa pasta e selecione "Abrir o shell do Linux aqui";
4. Clique com o botão direito na janela do shell e selecione "Propriedades";
5. Na aba **Layout**, busque por **Tamanho da Janela** e ajuste para que ela fique com Largura = 160 px e Altura = 40 px. Isso é de extrema importância para evitar erros na visualização da animação.
6. Com o shell aberto no diretório **projetomultithread**, execute o comando `make clean`.
7. Em seguida, faça `make`.
8. Que tal dar um passeio de montanha russa? Basta executar `./rollercoaster` para ver a animação do trabalho!