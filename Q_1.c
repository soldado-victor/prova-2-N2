#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LIMITE_REGISTROS 10000
#define LIMITE_DISPOSITIVOS 100

typedef enum {
    TIPO_INTEIRO,
    TIPO_RACIONAL,
    TIPO_BOOLEANO,
    TIPO_STRING
} TipoDado;

typedef struct {
    long int momento;
    char identificador[50];
    TipoDado tipo;
    union {
        int inteiro;
        float racional;
        int booleano; // 0 = false, 1 = true
        char texto[17];
    } valor;
} Registro;

typedef struct {
    char nome[50];
    TipoDado tipo;
} SensorTipo;

SensorTipo sensores_tipos[LIMITE_DISPOSITIVOS];
int total_tipos = 0;

int sensor_existente(char lista[][50], int total, char *nome) {
    for (int i = 0; i < total; i++) {
        if (strcmp(lista[i], nome) == 0) {
            return 1;
        }
    }
    return 0;
}

TipoDado detectar_tipo(char *valor_str) {
    if (strcmp(valor_str, "true") == 0 || strcmp(valor_str, "false") == 0) {
        return TIPO_BOOLEANO;
    } else if (strchr(valor_str, '.') != NULL) {
        return TIPO_RACIONAL;
    } else if (isdigit(valor_str[0]) || (valor_str[0] == '-' && isdigit(valor_str[1]))) {
        return TIPO_INTEIRO;
    } else {
        return TIPO_STRING;
    }
}

TipoDado obter_tipo_sensor(char *nome) {
    for (int i = 0; i < total_tipos; i++) {
        if (strcmp(sensores_tipos[i].nome, nome) == 0) {
            return sensores_tipos[i].tipo;
        }
    }
    return -1;
}

void registrar_tipo_sensor(char *nome, TipoDado tipo) {
    strcpy(sensores_tipos[total_tipos].nome, nome);
    sensores_tipos[total_tipos].tipo = tipo;
    total_tipos++;
}

// ORDENAR EM ORDEM DECRESCENTE PELO TEMPO
void classificar_por_tempo(Registro *dados, int quantidade) {
    int i, j, indice_max;
    Registro temp;
    for (i = 0; i < quantidade - 1; i++) {
        indice_max = i;
        for (j = i + 1; j < quantidade; j++) {
            if (dados[j].momento > dados[indice_max].momento) { // ordem decrescente
                indice_max = j;
            }
        }
        if (indice_max != i) {
            temp = dados[i];
            dados[i] = dados[indice_max];
            dados[indice_max] = temp;
        }
    }
}

void analisar_sensor(Registro *dados, int total_registros, char *nome_sensor) {
    Registro filtrados[LIMITE_REGISTROS];
    int contador = 0;
    TipoDado tipo = obter_tipo_sensor(nome_sensor);

    for (int i = 0; i < total_registros; i++) {
        if (strcmp(dados[i].identificador, nome_sensor) == 0) {
            filtrados[contador++] = dados[i];
        }
    }

    if (contador == 0) {
        printf("Nenhum dado localizado para o sensor %s.\n", nome_sensor);
        return;
    }

    classificar_por_tempo(filtrados, contador);

    char arquivo_saida[60];
    snprintf(arquivo_saida, sizeof(arquivo_saida), "%s.txt", nome_sensor);

    FILE *saida = fopen(arquivo_saida, "w");
    if (saida == NULL) {
        printf("Falha ao criar o arquivo %s\n", arquivo_saida);
        return;
    }

    for (int i = 0; i < contador; i++) {
        fprintf(saida, "%ld %s ", filtrados[i].momento, filtrados[i].identificador);
        switch (tipo) {
            case TIPO_INTEIRO:
                fprintf(saida, "%d\n", filtrados[i].valor.inteiro);
                break;
            case TIPO_RACIONAL:
                fprintf(saida, "%.2f\n", filtrados[i].valor.racional);
                break;
            case TIPO_BOOLEANO:
                fprintf(saida, "%s\n", filtrados[i].valor.booleano ? "true" : "false");
                break;
            case TIPO_STRING:
                fprintf(saida, "%s\n", filtrados[i].valor.texto);
                break;
        }
    }
    fclose(saida);

    printf("Arquivo %s criado com sucesso!\n", arquivo_saida);
}

int main() {
    char *nome_arquivo = "dados.txt";

    FILE *entrada;
    Registro registros[LIMITE_REGISTROS];
    int total_registros = 0;

    char sensores_unicos[LIMITE_DISPOSITIVOS][50];
    int total_sensores = 0;

    entrada = fopen(nome_arquivo, "r");
    if (entrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada: %s\n", nome_arquivo);
        return 1;
    }

    char valor_str[100];
    char id_sensor[50];
    while (fscanf(entrada, "%ld %49s %99s", &registros[total_registros].momento, id_sensor, valor_str) == 3) {
        if (registros[total_registros].momento < 0) {
            printf("Timestamp invalido na linha %d. Ignorando registro.\n", total_registros + 1);
            continue;
        }

        strncpy(registros[total_registros].identificador, id_sensor, sizeof(registros[total_registros].identificador) - 1);
        registros[total_registros].identificador[sizeof(registros[total_registros].identificador) - 1] = '\0';

        TipoDado tipo = detectar_tipo(valor_str);

        if (!sensor_existente(sensores_unicos, total_sensores, registros[total_registros].identificador)) {
            strcpy(sensores_unicos[total_sensores++], registros[total_registros].identificador);
            registrar_tipo_sensor(registros[total_registros].identificador, tipo);
        }

        registros[total_registros].tipo = tipo;
        switch (tipo) {
            case TIPO_INTEIRO:
                registros[total_registros].valor.inteiro = atoi(valor_str);
                break;
            case TIPO_RACIONAL:
                registros[total_registros].valor.racional = atof(valor_str);
                break;
            case TIPO_BOOLEANO:
                registros[total_registros].valor.booleano = strcmp(valor_str, "true") == 0 ? 1 : 0;
                break;
            case TIPO_STRING:
                strncpy(registros[total_registros].valor.texto, valor_str, 16);
                registros[total_registros].valor.texto[16] = '\0';
                break;
        }
        total_registros++;
    }
    fclose(entrada);

    int escolha;
    do {
        printf("\n======== TABELA DE OPCOES ========\n");
        printf("1 - Processar TODOS os sensores\n");
        printf("2 - Processar UM sensor especifico\n");
        printf("3 - Encerrar programa\n");
        printf("================================\n");
        printf("Digite a opcao desejada: ");
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada invalida. Encerrando.\n");
            break;
        }
        while (getchar() != '\n'); // limpar buffer

        switch (escolha) {
            case 1:
                for (int i = 0; i < total_sensores; i++) {
                    analisar_sensor(registros, total_registros, sensores_unicos[i]);
                }
                break;

            case 2: {
                char sensor_especifico[50];
                printf("Informe o ID do sensor: ");
                scanf("%49s", sensor_especifico);
                analisar_sensor(registros, total_registros, sensor_especifico);
                break;
            }

            case 3:
                printf("Finalizando o sistema...\n");
                break;

            default:
                printf("Opcao invalida. Tente outra vez.\n");
        }

    } while (escolha != 3);

    return 0;
}