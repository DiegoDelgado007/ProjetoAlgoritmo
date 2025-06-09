#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>

#define TAM_TITULO 100
#define TAM_AUTOR 50

/*
    Projeto: Cadastro de Livro para biblioteca
    Mat�ria: Algoritmo e Estrutura de Dados

    Anota��o Parte 1: Tipo de fila utilizado: Fila Linear Din�mica (Lista Encadeada Simples).
    - N�o tem tamanho fixo, cresce e diminui conforme a necessidade.
    - Inser��o (Enfilerado) no final da fila � O(1) com um ponteiro 'tras'.
    - Remo��o (Desenfilerar) do in�cio da fila � O(1) com um ponteiro 'frente'.
    - Remo��o por posi��o (arbitr�ria) � O(n) pois exige percorrer a lista.
    - Vantagens: Flexibilidade de tamanho, inser��o/remo��o no in�cio/fim s�o eficientes.
    - Desvantagens: Acesso a elementos por �ndice � O(n), maior consumo de mem�ria por n� (devido ao ponteiro).
*/

// Anota��o Parte 2: Estrutura de um livro
typedef struct {
    char titulo[TAM_TITULO];
    char autor[TAM_AUTOR];
    char dataRegistro[20];
} Livro;

// Nova Anota��o: Estrutura de um N� da Lista Encadeada
typedef struct NoLivro {
    Livro dadosLivro;
    struct NoLivro *proximo; // Ponteiro para o pr�ximo n� na fila
} NoLivro;

// Anota��o Parte 3: Fila de livros (agora baseada em lista encadeada)
typedef struct {
    NoLivro *frente; // Ponteiro para o primeiro n� da fila
    NoLivro *tras;   // Ponteiro para o �ltimo n� da fila
    int tamanho;     // Para controle f�cil do n�mero de livros
} FilaLivros;

// --- Prot�tipos das Fun��es ---
void inicializarFila(FilaLivros *fila); // Anota��o Parte 4: Inicializa��o da fila
int filaVazia(FilaLivros *fila);        // Anota��o Parte 5: Verifica se a fila est� vazia
void pegarDataHora(char *dataHora);    // Anota��o Parte 7: Pega a data e hora atual para registro
void tirarEnter(char *texto);          // Anota��o Parte 8: Remove o '\n' do final da string
void adicionarLivro(FilaLivros *fila); // Anota��o Parte 9: Adiciona um novo livro � fila
void listarLivros(FilaLivros *fila);   // Anota��o Parte 10: Lista todos os livros na fila
void removerLivro(FilaLivros *fila);   // Anota��o Parte 11: Remove um livro da fila por posi��o
void salvarLivrosEmArquivo(FilaLivros *fila); // Novo: Salvar livros no .txt

// --- Fun��o Principal: O cora��o do Sistema de Biblioteca ---
int main() {
    FilaLivros fila;
    inicializarFila(&fila);

    int opcao;

    do {
        printf("\n=== SISTEMA DE BIBLIOTECA ===\n");
        printf("1 - Adicionar Novo Livro\n");
        printf("2 - Listar Livros\n");
        printf("3 - Remover Livro por Posicao\n");
        printf("4 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                adicionarLivro(&fila);
                salvarLivrosEmArquivo(&fila);
                break;
            case 2:
                listarLivros(&fila);
                break;
            case 3:
                removerLivro(&fila);
                salvarLivrosEmArquivo(&fila);
                break;
            case 4:
                printf("Saindo do sistema da biblioteca...\n");

                salvarLivrosEmArquivo(&fila); // Salvar ao sair

                while (!filaVazia(&fila)) {
                    NoLivro *temp = fila.frente;
                    fila.frente = fila.frente->proximo;
                    free(temp);
                }
                printf("Memoria liberada. Ate mais!\n");
                break;
            default:
                printf("Opcao invalida! Por favor, tente novamente.\n");
        }

    } while (opcao != 4);

    return 0;
}

// --- Implementa��o das Fun��es para Lista Encadeada ---

// Anota��o Parte 4: Inicializa��o da fila
void inicializarFila(FilaLivros *fila) {
    fila->frente = NULL;
    fila->tras = NULL;
    fila->tamanho = 0;
}

// Anota��o Parte 5: Verifica se a fila est� vazia (para lista encadeada)
int filaVazia(FilaLivros *fila) {
    return fila->frente == NULL;
}

// Anota��o Parte 7: Pega a data e hora atual para registro
void pegarDataHora(char *dataHora) {
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    strftime(dataHora, 20, "%d/%m/%Y %H:%M", tm_info);
}

// Anota��o Parte 8: Remove o '\n' do final da string
void tirarEnter(char *texto) {
    int len = strlen(texto);
    if (len > 0 && texto[len - 1] == '\n') {
        texto[len - 1] = '\0';
    }
}

// Anota��o Parte 9: Adiciona um novo livro � fila (enqueue em lista encadeada)
void adicionarLivro(FilaLivros *fila) {
    NoLivro *novoNo = (NoLivro *)malloc(sizeof(NoLivro));
    if (novoNo == NULL) {
        printf("Erro de alocacao de memoria. Nao foi possivel adicionar o livro.\n");
        return;
    }

    printf("\n--- Adicionar Novo Livro ---\n");
    printf("Titulo: ");
    fgets(novoNo->dadosLivro.titulo, TAM_TITULO, stdin);
    tirarEnter(novoNo->dadosLivro.titulo);

    printf("Autor: ");
    fgets(novoNo->dadosLivro.autor, TAM_AUTOR, stdin);
    tirarEnter(novoNo->dadosLivro.autor);

    pegarDataHora(novoNo->dadosLivro.dataRegistro);
    novoNo->proximo = NULL;

    if (filaVazia(fila)) {
        fila->frente = novoNo;
        fila->tras = novoNo;
    } else {
        fila->tras->proximo = novoNo;
        fila->tras = novoNo;
    }

    fila->tamanho++;
    printf("Livro adicionado com sucesso!\n");
}

// Anota��o Parte 10: Lista todos os livros na fila
void listarLivros(FilaLivros *fila) {
    if (filaVazia(fila)) {
        printf("Nenhum livro registrado na biblioteca.\n");
        return;
    }

    printf("\n=== Livros na Biblioteca (%d livro(s)) ===\n", fila->tamanho);
    NoLivro *atual = fila->frente;
    int contador = 1;
    while (atual != NULL) {
        printf("%d. Titulo: %s, Autor: %s (Registro: %s)\n",
               contador, atual->dadosLivro.titulo, atual->dadosLivro.autor, atual->dadosLivro.dataRegistro);
        atual = atual->proximo;
        contador++;
    }
    printf("============================\n");
}

// Anota��o Parte 11: Esse trecho � utilizado para Remover um livro da fila por posi��o
void removerLivro(FilaLivros *fila) {
    if (filaVazia(fila)) {
        printf("Nenhum livro para remover da biblioteca.\n");
        return;
    }

    listarLivros(fila);

    int posRemover;
    printf("Digite o numero do livro que deseja remover: ");
    scanf("%d", &posRemover);
    getchar();

    if (posRemover < 1 || posRemover > fila->tamanho) {
        printf("Numero de livro invalido! Por favor, digite um numero entre 1 e %d.\n", fila->tamanho);
        return;
    }

    NoLivro *noRemover = NULL;

    if (posRemover == 1) {
        noRemover = fila->frente;
        fila->frente = fila->frente->proximo;

        if (fila->frente == NULL) {
            fila->tras = NULL;
        }
    } else {
        NoLivro *anterior = fila->frente;
        for (int i = 1; i < posRemover - 1; i++) {
            anterior = anterior->proximo;
        }
        noRemover = anterior->proximo;
        anterior->proximo = noRemover->proximo;

        if (noRemover == fila->tras) {
            fila->tras = anterior;
        }
    }

    free(noRemover); // Anota��o Parte 12: Libera a mem�ria do n� removido
    fila->tamanho--;
    printf("Livro removido com sucesso!\n");
}

// Nova Fun��o: Salva os livros no arquivo livros.txt
void salvarLivrosEmArquivo(FilaLivros *fila) {
    FILE *arquivo = fopen("livros.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }

    NoLivro *atual = fila->frente;
    int contador = 1;
    while (atual != NULL) {
        fprintf(arquivo, "%d. Titulo: %s, Autor: %s (Registro: %s)\n",
                contador, atual->dadosLivro.titulo, atual->dadosLivro.autor, atual->dadosLivro.dataRegistro);
        atual = atual->proximo;
        contador++;
    }

    fclose(arquivo);
    printf("Livros salvos no arquivo 'livros.txt'.\n");
}
