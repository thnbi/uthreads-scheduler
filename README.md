# Escalonamento de Threads em Nível de Usuário

Este projeto implementa um sistema de threads em nível de usuário em linguagem C, sem utilizar a biblioteca pthread. Todas as threads são gerenciadas dentro do próprio processo, utilizando a biblioteca `ucontext.h` para manipulação de contextos de execução.

O sistema cria 5 threads de usuário, cada uma com seu próprio contexto, pilha alocada, identificador e estado (pronta, executando, bloqueada ou finalizada). Essas threads não executam em paralelo real — elas alternam o uso da CPU, caracterizando concorrência por compartilhamento de tempo (time sharing).

A troca entre threads é preemptiva: ao iniciar, o programa cria um processo auxiliar via `fork()` que envia um sinal (`SIGUSR1`) ao processo principal a cada 1 segundo, simulando o fim de um quantum. Ao receber o sinal, o processo principal aciona o escalonador, que decide qual thread deve executar a seguir, e o dispatcher, que realiza a troca de contexto de fato.

A implementação separa claramente essas duas responsabilidades. O escalonador utiliza uma fila de prontos (ready queue) para gerenciar as threads aptas a executar, aplicando a política round-robin. O dispatcher salva o contexto da thread atual e restaura o da próxima utilizando `swapcontext()`.

Além do escalonamento, o projeto demonstra um problema clássico de concorrência: a condição de corrida sobre dados compartilhados. As threads acessam uma estrutura global comum e, sem nenhum mecanismo de controle, a interrupção no meio de uma operação causa inconsistências nos dados. A solução implementada utiliza um semáforo para proteger a seção crítica, bloqueando threads que tentam acessar o recurso compartilhado enquanto ele está em uso.

## Estrutura do projeto

```
include/          → headers (.h)
src/              → código-fonte (.c)
  main.c          → ponto de entrada e loop principal
  thread.c        → criação e limpeza de threads
  ready_queue.c   → fila de prontos (ready queue)
  scheduler.c     → escalonador round-robin
  dispatcher.c    → troca de contexto
  signal_handler.c→ tratamento de SIGUSR1
  semaphore.c     → semáforo para sincronização
  concurrency.c   → funções de trabalho e verificação
build/            → objetos compilados (gerado pelo make)
Makefile          → regras de compilação
```

## Compilação

```bash
make
```

## Execução

```bash
./scheduler            # modo unsafe (demonstra condição de corrida)
./scheduler --safe     # modo safe (com sincronização)
```

## Saída esperada

```
Modo: UNSAFE
[Thread 0] executando
--- troca ---
[Thread 1] executando
--- troca ---
[Thread 2] executando
...
Todas as threads finalizadas.
Resultado: INCORRETO (N de 1000 elementos divergem do esperado 5000)
```

Com `--safe`:

```
Resultado: CORRETO (todos os 1000 elementos = 5000)
```

## Limpeza

```bash
make clean
```
