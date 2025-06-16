#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_SENSORES 5
#define LEITURAS_POR_SENSOR 2000

const char *sensores[NUM_SENSORES] = {"TEMP", "PRES", "VIBR", "UMID", "FLUX"};

// Função para ler timestamp no formato DD/MM/AAAA HH:MM:SS
time_t ler_timestamp(const char *mensagem) {
    char buffer[30];
    struct tm t = {0};

    printf("%s (DD/MM/AAAA HH:MM:SS): ", mensagem);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // remove o \n

    if (sscanf(buffer, "%d/%d/%d %d:%d:%d",
               &t.tm_mday, &t.tm_mon, &t.tm_year,
               &t.tm_hour, &t.tm_min, &t.tm_sec) != 6) {
        printf("Formato invalido!\n");
        exit(1);
    }

    t.tm_year -= 1900;
    t.tm_mon -= 1;
    t.tm_isdst = -1;

    return mktime(&t);
}

// Função para gerar string aleatória (para tipo TEXTO)
void gerar_string_aleatoria(char *str, int tamanho) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < tamanho - 1; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        str[i] = charset[key];
    }
    str[tamanho - 1] = '\0';
}

int main() {
    printf("\n========== GERADOR DE DADOS ALEATORIOS ==========\n");
    printf("Escolha o tipo de dado que deseja gerar:\n");
    printf("1 - CONJ_Z (inteiro)\n");
    printf("2 - CONJ_Q (float)\n");
    printf("3 - TEXTO (string)\n");
    printf("4 - BINARIO (booleano)\n");
    printf("Digite a opcao desejada: ");

    int opcao = 0;
    if (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 4) {
        printf("Opcao invalida. Saindo...\n");
        return 1;
    }
    while (getchar() != '\n'); // limpar buffer stdin após scanf

    printf("\n==========================================\n");
    time_t inicio = ler_timestamp("Digite a data e hora de INICIO");
    printf("\n==========================================\n");
    time_t fim = ler_timestamp("Digite a data e hora de FIM");
    printf("\n==========================================\n");

    if (inicio == -1 || fim == -1 || fim <= inicio) {
        printf("Intervalo de datas invalido.\n");
        return 1;
    }

    FILE *arquivo = fopen("dados_liquidos.txt", "w");
    if (!arquivo) {
        perror("Erro ao criar arquivo");
        return 1;
    }

    srand((unsigned int)time(NULL));
    long intervalo = (long)(fim - inicio);

    for (int i = 0; i < NUM_SENSORES; i++) {
        for (int j = 0; j < LEITURAS_POR_SENSOR; j++) {
            time_t ts = inicio + (rand() % intervalo);

            switch(opcao) {
                case 1: { // CONJ_Z - inteiro
                    int valor = rand() % 10000; // valor inteiro 0-9999
                    fprintf(arquivo, "%ld %s %d\n", ts, sensores[i], valor);
                    break;
                }
                case 2: { // CONJ_Q - float
                    float valor = (float)(rand() % 10000) / 100.0f; // float com 2 casas decimais
                    fprintf(arquivo, "%ld %s %.2f\n", ts, sensores[i], valor);
                    break;
                }
                case 3: { // TEXTO - string aleatória de 8 caracteres
                    char texto[9];
                    gerar_string_aleatoria(texto, 9);
                    fprintf(arquivo, "%ld %s %s\n", ts, sensores[i], texto);
                    break;
                }
                case 4: { // BINARIO - booleano true/false
                    int bool_val = rand() % 2;
                    fprintf(arquivo, "%ld %s %s\n", ts, sensores[i], bool_val ? "true" : "false");
                    break;
                }
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo 'dados_liquidos_aleatorios.txt' gerado com sucesso!\n");
    return 0;
}
