#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca pecas[TAM_FILA];
    int inicio;
    int fim;
    int total;
} FilaCircular;

typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

FilaCircular fila;
Pilha pilha;
int proximoId = 0;

void limparBuffer();
Peca gerarPeca();
void exibirEstado();

void inicializarFila();
int filaEstaVazia();
int filaEstaCheia();
void enqueue(Peca p);
Peca dequeue();

void inicializarPilha();
int pilhaEstaVazia();
int pilhaEstaCheia();
int push(Peca p);
Peca pop();

void jogarPeca();
void reservarPeca();
void usarPecaReservada();

int main() {
    srand(time(NULL));
    
    inicializarPilha();
    inicializarFila(); 

    int opcao = -1;

    while (opcao != 0) {
        exibirEstado();

        printf("Opcoes de Acao:\n");
        printf("1. Jogar peca\n");
        printf("2. Reservar peca\n");
        printf("3. Usar peca reservada\n");
        printf("0. Sair\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarPecaReservada();
                break;
            case 0:
                printf("\n--- Jogo Encerrado ---\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
        
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    }

    return 0;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    
    p.nome = tipos[rand() % 7];
    p.id = proximoId;
    proximoId++;
    
    return p;
}

void exibirEstado() {
    printf("\n\n--- ESTADO ATUAL DO JOGO ---\n");

    printf("Fila de pecas: ");
    if (filaEstaVazia()) {
        printf("[Vazia]");
    } else {
        for (int i = 0; i < fila.total; i++) {
            int indice = (fila.inicio + i) % TAM_FILA;
            printf("[%c %d] ", fila.pecas[indice].nome, fila.pecas[indice].id);
        }
    }
    printf("\n");

    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaEstaVazia()) {
        printf("[Vazia]");
    } else {
        for (int i = pilha.topo; i >= 0; i--) {
            printf("[%c %d] ", pilha.pecas[i].nome, pilha.pecas[i].id);
        }
    }
    printf("\n------------------------------\n\n");
}

void inicializarFila() {
    fila.inicio = 0;
    fila.fim = 0;
    fila.total = 0;
    
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(gerarPeca());
    }
}

int filaEstaVazia() {
    return (fila.total == 0);
}

int filaEstaCheia() {
    return (fila.total == TAM_FILA);
}

void enqueue(Peca p) {
    if (filaEstaCheia()) {
        return;
    }
    
    fila.pecas[fila.fim] = p;
    fila.fim = (fila.fim + 1) % TAM_FILA;
    fila.total++;
}

Peca dequeue() {
    Peca pVazia = {' ', -1};
    
    if (filaEstaVazia()) {
        return pVazia;
    }

    Peca p = fila.pecas[fila.inicio];
    fila.inicio = (fila.inicio + 1) % TAM_FILA;
    fila.total--;
    
    return p;
}

void inicializarPilha() {
    pilha.topo = -1;
}

int pilhaEstaVazia() {
    return (pilha.topo == -1);
}

int pilhaEstaCheia() {
    return (pilha.topo == TAM_PILHA - 1);
}

int push(Peca p) {
    if (pilhaEstaCheia()) {
        return 0;
    }
    
    pilha.topo++;
    pilha.pecas[pilha.topo] = p;
    return 1;
}

Peca pop() {
    Peca pVazia = {' ', -1};

    if (pilhaEstaVazia()) {
        return pVazia;
    }

    Peca p = pilha.pecas[pilha.topo];
    pilha.topo--;
    return p;
}

void jogarPeca() {
    Peca p = dequeue();
    printf("ACAO: Peca [%c %d] foi jogada.\n", p.nome, p.id);

    enqueue(gerarPeca());
}

void reservarPeca() {
    if (pilhaEstaCheia()) {
        printf("ACAO: Pilha de reserva esta cheia! Nao e possivel reservar.\n");
        return;
    }

    Peca p = dequeue();
    
    push(p);
    printf("ACAO: Peca [%c %d] foi movida para a reserva.\n", p.nome, p.id);

    enqueue(gerarPeca());
}

void usarPecaReservada() {
    Peca p = pop();

    if (p.id == -1) {
        printf("ACAO: Pilha de reserva esta vazia! Nao ha pecas para usar.\n");
    } else {
        printf("ACAO: Peca [%c %d] foi usada (removida da reserva).\n", p.nome, p.id);
    }
}
