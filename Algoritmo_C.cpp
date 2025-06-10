#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>

#define TAM_TITULO 100
#define TAM_AUTOR 50

/*
    Projeto: Cadastro de Livro para biblioteca
    Matéria: Algoritmo e Estrutura de Dados

      Anotação 1: Tipo de fila utilizado: Fila Linear Dinâmica (Lista Encadeada Simples).
    - Não tem tamanho fixo, cresce e diminui conforme a necessidade.
    - Inserção (Enfilerado) no final da fila é O(1) com um ponteiro 'tras'.
    - Remoção (Desenfilerar) do início da fila é O(1) com um ponteiro 'frente'.
    - Remoção por posição (arbitrária) é O(n) pois exige percorrer a lista.
    - Vantagens: Flexibilidade de tamanho, inserção/remoção no início/fim são eficientes.
    - Desvantagens: Acesso a elementos por índice é O(n), maior consumo de memória por nó (devido ao ponteiro).
*/

// Anotação 2: Estrutura de um livro
typedef struct {
    char titulo[TAM_TITULO];
    char autor[TAM_AUTOR];
    char dataRegistro[20];
} Livro;

// Anotação 3: Estrutura de um Nó da Lista Encadeada
typedef struct NoLivro {
    Livro dadosLivro;
    struct NoLivro *proximo; 
} NoLivro;

// Anotação 4: Fila de livros (agora baseada em lista encadeada) Uso de ponteiros para o ptimeiro nó da fila, ultimo nó e controle dos números de livros 

typedef struct {
    NoLivro *frente; 
    NoLivro *tras;   
    int tamanho;   
} FilaLivros;

// --- Protótipos das Funções ---
void inicializarFila(FilaLivros *fila); 
int filaVazia(FilaLivros *fila);        
void pegarDataHora(char *dataHora);    
void tirarEnter(char *texto);          
void adicionarLivro(FilaLivros *fila); 
void listarLivros(FilaLivros *fila);   
void removerLivro(FilaLivros *fila);  
void salvarLivrosEmArquivo(FilaLivros *fila); 

// --- Função Principal: O coração do Sistema de Biblioteca ---
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

// --- Implementação das Funções para Lista Encadeada ---

// Anotação 5: Inicialização da fila
void inicializarFila(FilaLivros *fila) {
    fila->frente = NULL;
    fila->tras = NULL;
    fila->tamanho = 0;
}

// Anotação 6: Verifica se a fila está vazia (para lista encadeada)
int filaVazia(FilaLivros *fila) {
    return fila->frente == NULL;
}

// Anotação 7: Pega a data e hora atual para registro
void pegarDataHora(char *dataHora) {
    time_t agora = time(NULL);
    struct tm *tm_info = localtime(&agora);
    strftime(dataHora, 20, "%d/%m/%Y %H:%M", tm_info);
}

// Anotação 8: Remove o '\n' do final da string
void tirarEnter(char *texto) {
    int len = strlen(texto);
    if (len > 0 && texto[len - 1] == '\n') {
        texto[len - 1] = '\0';
    }
}

// Anotação 9: Adiciona um novo livro à fila (enqueue em lista encadeada)
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

// Anotação 10: Lista todos os livros na fila
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

// Anotação 11: Esse trecho é utilizado para Remover um livro da fila por posição
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
    // Anotação 12: Libera a memória do nó removido
    free(noRemover); 
    fila->tamanho--;
    printf("Livro removido com sucesso!\n");
}

// Anotação 13: Nova Função, Salva os livros no arquivo livros.txt
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
