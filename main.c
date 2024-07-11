#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 20

typedef struct Carro {
    char marca[MAX_STR + 1];
    char modelo[MAX_STR + 1];
    int ano;
    int kilometragem;
    float preco;
    struct Carro *prox;
} Carro;

Carro* criarCarro(char *marca, char *modelo, int ano, int kilometragem, float preco) {
    Carro *novo = (Carro*)malloc(sizeof(Carro));
    strncpy(novo->marca, marca, MAX_STR);
    novo->marca[MAX_STR] = '\0';  // Garantir terminação correta
    strncpy(novo->modelo, modelo, MAX_STR);
    novo->modelo[MAX_STR] = '\0';  // Garantir terminação correta
    novo->ano = ano;
    novo->kilometragem = kilometragem;
    novo->preco = preco;
    novo->prox = NULL;
    return novo;
}

void inserirOrdenado(Carro **lista, Carro *novo) {
    if (*lista == NULL || (*lista)->preco > novo->preco) {
        novo->prox = *lista;
        *lista = novo;
    } else {
        Carro *atual = *lista;
        while (atual->prox != NULL && atual->prox->preco <= novo->preco) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
}

void carregarArquivo(Carro **lista) {
    FILE *file = fopen("dados.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    char marca[MAX_STR + 1], modelo[MAX_STR + 1];
    int ano, kilometragem;
    float preco;
    
    while (fscanf(file, "%20s %20[^\n] %d %d %f", marca, modelo, &ano, &kilometragem, &preco) != EOF) {
        Carro *novo = criarCarro(marca, modelo, ano, kilometragem, preco);
        inserirOrdenado(lista, novo);
    }
    
    fclose(file);
}

void salvarArquivo(Carro *lista) {
    FILE *file = fopen("dados.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    Carro *atual = lista;
    while (atual != NULL) {
        fprintf(file, "%s\n%s\n%d\n%d\n%.2f\n", atual->marca, atual->modelo, atual->ano, atual->kilometragem, atual->preco);
        atual = atual->prox;
    }
    
    fclose(file);
}

void exibirLista(Carro *lista) {
    Carro *atual = lista;
    while (atual != NULL) {
        printf("%s %s %d %d %.2f\n", atual->marca, atual->modelo, atual->ano, atual->kilometragem, atual->preco);
        atual = atual->prox;
    }
}

void exibirPorMarca(Carro *lista, char *marca) {
    Carro *atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->marca, marca) == 0) {
            printf("%s %s %d %d %.2f\n", atual->marca, atual->modelo, atual->ano, atual->kilometragem, atual->preco);
        }
        atual = atual->prox;
    }
}

void exibirPorPreco(Carro *lista, float minPreco, float maxPreco) {
    Carro *atual = lista;
    while (atual != NULL) {
        if (atual->preco >= minPreco && atual->preco <= maxPreco) {
            printf("%s %s %d %d %.2f\n", atual->marca, atual->modelo, atual->ano, atual->kilometragem, atual->preco);
        }
        atual = atual->prox;
    }
}

void inserirNovoRegistro(Carro **lista) {
    char marca[MAX_STR + 1], modelo[MAX_STR + 1];
    int ano, kilometragem;
    float preco;

    printf("Marca: ");
    fgets(marca, sizeof(marca), stdin);
    if (marca[strlen(marca) - 1] == '\n') {
        marca[strlen(marca) - 1] = '\0';  // Remover o '\n' do final
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);  // Limpar buffer de entrada
        printf("Opcao invalida\n");
        return;
    }
    if (strlen(marca) > MAX_STR) {
        printf("Opcao invalida\n");
        return;
    }

    printf("Modelo: ");
    fgets(modelo, sizeof(modelo), stdin);
    if (modelo[strlen(modelo) - 1] == '\n') {
        modelo[strlen(modelo) - 1] = '\0';  // Remover o '\n' do final
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);  // Limpar buffer de entrada
        printf("Opcao invalida\n");
        return;
    }
    if (strlen(modelo) > MAX_STR) {
        printf("Opcao invalida\n");
        return;
    }

    printf("Ano: ");
    scanf("%d", &ano);
    printf("Kilometragem: ");
    scanf("%d", &kilometragem);
    printf("Preco: ");
    scanf("%f", &preco);
    getchar();  // Para consumir o '\n' deixado pelo scanf

    Carro *novo = criarCarro(marca, modelo, ano, kilometragem, preco);
    inserirOrdenado(lista, novo);
    salvarArquivo(*lista);
}

void removerPorKilometragem(Carro **lista, int maxKilometragem) {
    Carro *atual = *lista;
    Carro *anterior = NULL;

    while (atual != NULL) {
        if (atual->kilometragem > maxKilometragem) {
            if (anterior == NULL) {
                *lista = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            Carro *temp = atual;
            atual = atual->prox;
            free(temp);
        } else {
            anterior = atual;
            atual = atual->prox;
        }
    }
    salvarArquivo(*lista);
}

void liberarLista(Carro *lista) {
    Carro *atual = lista;
    while (atual != NULL) {
        Carro *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}

int main() {
    Carro *lista = NULL;
    carregarArquivo(&lista);

    int opcao;
    do {
        printf("1. Exibir a lista completa de registros\n");
        printf("2. Exibir todos os registros de uma marca especifica\n");
        printf("3. Exibir todos os registros com preco entre valores fornecidos\n");
        printf("4. Inserir um novo registro\n");
        printf("5. Remover registros com kilometragem superior a um valor\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();  // Consumir o '\n' deixado pelo scanf

        switch (opcao) {
            case 1:
                exibirLista(lista);
                break;
            case 2: {
                char marca[MAX_STR + 1];
                printf("Digite a marca: ");
                fgets(marca, sizeof(marca), stdin);
                marca[strcspn(marca, "\n")] = '\0';  // Remover o '\n' do final
                exibirPorMarca(lista, marca);
                break;
            }
            case 3: {
                float minPreco, maxPreco;
                printf("Digite o preco minimo: ");
                scanf("%f", &minPreco);
                printf("Digite o preco maximo: ");
                scanf("%f", &maxPreco);
                exibirPorPreco(lista, minPreco, maxPreco);
                break;
            }
            case 4:
                inserirNovoRegistro(&lista);
                break;
            case 5: {
                int maxKilometragem;
                printf("Digite a kilometragem maxima: ");
                scanf("%d", &maxKilometragem);
                removerPorKilometragem(&lista, maxKilometragem);
                break;
            }
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 6);

    liberarLista(lista);
    return 0;
}