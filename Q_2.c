#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINHAS 10000

typedef struct {
    time_t momentos;
    float valor;
} Leitura;

// Busca binária em vetor DECRESCENTE
int busca_binaria(Leitura leituras[], int n, time_t alvo) {
    int inicio = 0, fim = n - 1;
    int melhor_indice = -1;
    long menor_diferenca = -1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        long diferenca = labs(difftime(leituras[meio].momentos, alvo));

        if (menor_diferenca == -1 || diferenca < menor_diferenca) {
            menor_diferenca = diferenca;
            melhor_indice = meio;
        }

        if (leituras[meio].momentos > alvo) { // decrescente
            inicio = meio + 1;
        } else if (leituras[meio].momentos < alvo) {
            fim = meio - 1;
        } else {
            return meio; // Encontrado exatamente
        }
    }
    return melhor_indice;
}

int main() {
    char nome_sensor[50];
    char datahora[25];

    printf("\n==========================================\n");
    printf("Digite o nome do sensor (ex: TEMP): ");
    printf("\n==========================================\n");
    scanf("%49s", nome_sensor);
    getchar(); // Limpar buffer

    printf("\n==========================================\n");
    printf("Digite a data e hora (DD/MM/AAAA HH-MM-SS): ");
    printf("\n==========================================\n");
    fgets(datahora, sizeof(datahora), stdin);
    datahora[strcspn(datahora, "\n")] = 0;

    // Converte data/hora para time_t
    struct tm t;
    memset(&t, 0, sizeof(struct tm));
    sscanf(datahora, "%d/%d/%d %d-%d-%d",
           &t.tm_mday, &t.tm_mon, &t.tm_year,
           &t.tm_hour, &t.tm_min, &t.tm_sec);
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    time_t momento = mktime(&t);

    // Monta nome do arquivo
    char nome_arquivo[64];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", nome_sensor);

    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return 1;
    }

    Leitura leituras[MAX_LINHAS];
    int total = 0;

    while (fscanf(fp, "%ld %*s %f", &leituras[total].momentos, &leituras[total].valor) == 2) {
        if (total < MAX_LINHAS) {
            total++;
        }
    }

    fclose(fp);

    if (total == 0) {
        printf("Nenhuma leitura encontrada para o sensor %s.\n", nome_sensor);
        return 1;
    }

    // Não ordena, pois já está em ordem decrescente
    int indice = busca_binaria(leituras, total, momento);

    if (indice >= 0) {
        struct tm *tm_info = localtime(&leituras[indice].momentos);
        char buffer[25];
        strftime(buffer, sizeof(buffer), "%d/%m/%Y %H-%M-%S", tm_info);

        printf("\nLeitura mais proxima:\n");
        printf("Timestamp: %s\n", buffer);
        printf("Valor: %.2f\n", leituras[indice].valor);
    } else {
        printf("\nNenhuma leitura encontrada.\n");
    }

    return 0;
}
