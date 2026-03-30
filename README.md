# Escalonamento de Threads em Nível de Usuário

Este projeto implementa um sistema de threads em nível de usuário em linguagem C, sem utilizar a biblioteca pthread. Todas as threads são gerenciadas dentro do próprio processo, utilizando a biblioteca `ucontext.h` para manipulação de contextos de execução.

O sistema cria 5 threads de usuário, cada uma com seu próprio contexto, pilha alocada, identificador e estado (pronta, executando ou finalizada). Essas threads não executam em paralelo real — elas alternam o uso da CPU, caracterizando concorrência por compartilhamento de tempo (time sharing).

A troca entre threads é preemptiva: ao iniciar, o programa cria um processo auxiliar via `fork()` que envia um sinal (`SIGUSR1`) ao processo principal a cada 10ms, simulando o fim de um quantum. Ao receber o sinal, o processo principal aciona o escalonador, que decide qual thread deve executar a seguir, e o dispatcher, que realiza a troca de contexto de fato.

A implementação separa claramente essas duas responsabilidades. O escalonador aplica a política round-robin, escolhendo a próxima thread em ordem circular e ignorando threads que já finalizaram. O dispatcher salva o contexto da thread atual e restaura o da próxima utilizando `swapcontext()`.

Além do escalonamento, o projeto demonstra um problema clássico de concorrência: a condição de corrida sobre dados compartilhados. As threads acessam uma estrutura global comum e, sem nenhum mecanismo de controle, a interrupção no meio de uma operação causa inconsistências nos dados. A solução implementada bloqueia o sinal de preempção durante a seção crítica via `sigprocmask`, análogo a desabilitar interrupções em um kernel real.

## Estrutura do projeto

```
include/          → headers (.h)
src/              → código-fonte (.c)
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
Resultado: INCORRETO (N de 1000 elementos divergem do esperado 500000)
```

Com `--safe`:

```
Resultado: CORRETO (todos os 1000 elementos = 500000)
```

## Limpeza

```bash
make clean
```
