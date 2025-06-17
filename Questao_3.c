#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LEITURAS_PARA_0_SENSOR 2000

typedef enum { TIPO_INT, TIPO_FLOAT, TIPO_STRING, TIPO_BOOL } TipoDado;

typedef struct {
    char nome[50];
    TipoDado tipo;
} Sensor;

// Converte string "DD/MM/AAAA HH:MM:SS" para time_t
time_t string_para_timestamp(const char *data_str) {
    struct tm t = {0};
    if (sscanf(data_str, "%d/%d/%d %d:%d:%d",
               &t.tm_mday, &t.tm_mon, &t.tm_year,
               &t.tm_hour, &t.tm_min, &t.tm_sec) != 6) {
        return -1;
    }

    t.tm_year -= 1900;
    t.tm_mon -= 1;
    t.tm_isdst = -1;

    return mktime(&t);
}

// Gerador de uma string aleatória
void gerar_string_aleatoria(char *str, int tamanho) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < tamanho - 1; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        str[i] = charset[key];
    }
    str[tamanho - 1] = '\0';
}

// Converte a string para tipo com base nos nomes fornecidos (CONJ_Q, etc)
TipoDado string_para_tipo(const char *tipo_str) {
    if (strcmp(tipo_str, "CONJ_Z") == 0) return TIPO_INT;
    if (strcmp(tipo_str, "CONJ_Q") == 0) return TIPO_FLOAT;
    if (strcmp(tipo_str, "TEXTO") == 0) return TIPO_STRING;
    if (strcmp(tipo_str, "BINARIO") == 0) return TIPO_BOOL;
    
    printf("Tipo de dado invalido: %s\n", tipo_str);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <inicio> <fim> <sensor:tipo> [...]\n", argv[0]);
        printf("Exemplo: %s \"16/06/2025 00:00:00\" \"17/06/2025 00:00:00\" TEMP:CONJ_Z PRES:CONJ_Q TEXTO:TEXTO\n", argv[0]);
        return 1;
    }

    time_t inicio = string_para_timestamp(argv[1]);
    time_t fim = string_para_timestamp(argv[2]);

    if (inicio == -1 || fim == -1 || fim <= inicio) {
        printf("Erro: datas invalidas ou intervalo incorreto.\n");
        return 1;
    }

    int num_sensores = argc - 3;
    Sensor sensores[num_sensores];

    for (int i = 0; i < num_sensores; i++) {
        char *arg = argv[i + 3];
        char *sep = strchr(arg, ':');
        if (!sep) {
            printf("Formato invalido: %s. Use SENSOR:TIPO\n", arg);
            return 1;
        }

        *sep = '\0';
        strncpy(sensores[i].nome, arg, sizeof(sensores[i].nome));
        sensores[i].tipo = string_para_tipo(sep + 1);
    }

    FILE *arquivo = fopen("dados_liquidos.txt", "w");
    if (!arquivo) {
        perror("Erro ao criar arquivo");
        return 1;
    }

    srand((unsigned int)time(NULL));
    long intervalo = (long)(fim - inicio);

    for (int i = 0; i < num_sensores; i++) {
        for (int j = 0; j < LEITURAS_PARA_0_SENSOR; j++) {
            time_t ts = inicio + (rand() % intervalo);
            switch (sensores[i].tipo) {
                case TIPO_INT: {
                    int valor = rand() % 10000;
                    fprintf(arquivo, "%ld %s %d\n", ts, sensores[i].nome, valor);
                    break;
                }
                case TIPO_FLOAT: {
                    float valor = (float)(rand() % 10000) / 100.0f;
                    fprintf(arquivo, "%ld %s %.2f\n", ts, sensores[i].nome, valor);
                    break;
                }
                case TIPO_STRING: {
                    char texto[9];
                    gerar_string_aleatoria(texto, 9);
                    fprintf(arquivo, "%ld %s %s\n", ts, sensores[i].nome, texto);
                    break;
                }
                case TIPO_BOOL: {
                    int b = rand() % 2;
                    fprintf(arquivo, "%ld %s %s\n", ts, sensores[i].nome, b ? "true" : "false");
                    break;
                }
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo 'dados_liquidos.txt' gerado com sucesso!\n");
    return 0;
}
