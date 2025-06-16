# Processador de Dados de Sensores ## PRIMEIRO CÓDIGO

Este projeto em C realiza a leitura, classificação e análise de registros de sensores armazenados em um arquivo texto. A aplicação identifica dinamicamente o tipo de dado (inteiro, racional, booleano ou string) de cada sensor e gera arquivos de saída organizados por sensor, contendo os dados ordenados por decrescente.

## 🧠 Funcionalidades

- Leitura de até 10.000 registros de sensores.
- Identificação automática do tipo de dado de cada sensor.
- Ordenação dos dados por decrescente.
- Geração de arquivos individuais para cada sensor.
- Interface simples via terminal para seleção das opções de análise.

## 🛠️ Tecnologias utilizadas

- Linguagem C (padrão C99 ou superior)
- Entrada e saída padrão de arquivos (`stdio.h`)
- Manipulação de strings e tipos (`string.h`, `ctype.h`)

## 📂 Estrutura dos Dados de Entrada

O programa espera um arquivo chamado `dados.txt` no mesmo diretório do executável, contendo linhas no seguinte formato:

<timestamp> <id_sensor> <valor>

**Exemplo:**
1623945600 sensor_temp 23.5
1623945601 sensor_umid 45
1623945602 sensor_estado true


## 🧪 Tipos de Dados Suportados

- **Inteiros** (ex: `45`)
- **Racionais** (ex: `23.5`)
- **Booleanos** (`true` ou `false`)
- **Strings** (qualquer outro valor textual com até 16 caracteres)

## Como usar

1. **Compile o programa** (em sistemas Linux/windows):

2. ** Não pode esquece de coloca o arquivo "dados.txt" no mesmo repositorio, é aparti desse dados brutos que o código começa a funcionar.

3. Escolha uma das opções do menu interativo:

1 → Processar todos os sensores.

2 → Processar um sensor específico (informe o ID).

3 → Encerrar o programa.

4. Depois de escolher uma opção sera gerado um novo arquivos.
**Se caso a opção numero 2 ser a escolhida, tem que informa o nome do senso.
** Em ambas escolhas sejam elas a 1 ou 2, vai gerar um arquivo txt com o nome do senso.


Autor
Victor Hugo

================================================================================================================================================
================================================================================================================================================

# Buscador de Leitura Mais Próxima de um Sensor ## SEGUNDO CÓDIGO

Este programa em C permite que o usuário informe um sensor e uma data/hora específica, e em seguida retorna a leitura mais próxima desse momento com base nos dados armazenados em um arquivo `.txt` correspondente ao sensor.

## 🧠 Funcionalidades

- Entrada de nome do sensor e data/hora via terminal.
- Conversão da data/hora fornecida para timestamp.
- Leitura de dados de arquivo com base no sensor informado.
- Ordenação eficiente dos dados por tempo (`qsort`).
- Busca da leitura mais próxima utilizando **busca decrescente**.
- Exibição da leitura encontrada (timestamp formatado e valor).

## 🛠️ Tecnologias Utilizadas

- Linguagem C
- Bibliotecas padrão:
  - `stdio.h` (entrada/saída)
  - `stdlib.h` (funções auxiliares e `qsort`)
  - `string.h` (manipulação de strings)
  - `time.h` (manipulação de datas e horários)

## 📂 Estrutura dos Dados de Entrada

O programa espera que exista um **arquivo com o nome do sensor** informado, contendo linhas no seguinte formato:

<timestamp> <id_sensor> <valor>

## ▶️ Como usar

1. **Compile o programa** (em sistemas Linux/windows):

2. Com o terminal aberto, escolha um senso do qual deseja consulta as informações.

3. Passe um data e um horario inical e um final.
** Para teste recomendo escolher a seguinte data e hora E SENSO: TEMP 14/05/2025 16:40:35  

⚙️ Algoritmo
° Ordenação dos dados com qsort (baseado no timestamp).

° Busca eficiente com busca binária adaptada para encontrar o timestamp mais próximo do informado.

 Autor
Victor Hugo

================================================================================================================================================
================================================================================================================================================

# 📄 Gerador de Dados Aleatórios para Sensores ## TERCEIRO CÓDIGO

Este programa em C gera um arquivo com 2000 leituras aleatórias para **cada um dos cinco sensores** definidos (`TEMP`, `PRES`, `VIBR`, `UMID`, `FLUX`) dentro de um intervalo de tempo informado pelo usuário. As leituras são gravadas em um arquivo de saída chamado `dados_liquidos.txt`.

---

## 🧠 Funcionalidades

- Solicita ao usuário:
  - solicita ao usuario que escolha como ele deve ordena pelo o tipo
   - Data e hora de início (`DD/MM/AAAA HH:MM:SS`)
  - Data e hora de fim (`DD/MM/AAAA HH:MM:SS`)
- Para cada sensor:
  - Gera 2000 leituras com timestamp aleatório dentro do intervalo
  - Atribui valores de leitura entre 0.00 e 99.99 (com duas casas decimais)
- Salva todas as leituras no arquivo `dados_liquidos.txt` no seguinte formato:



##  Como usar

1. **Compile o programa** (em sistemas Linux/windows):

2. Com o terminal aberto, escolha qual o tipo que você deseja

3. Passe um data e um horario inical e um final.
** Para teste recomendo escolher a seguinte data e hora: INICAL 15/05/2025 15:40:35  FINAL 15/06/2025 15:40:30

4. Com isso vai ser gerado um arquivo txt com o nome "dados_liquidos_aleatorios.txt", dentro dele será apresentado
10 mil leituras aleatórias, sendo 2000 de cada senso.

 Autor
Victor Hugo

====================================================================================================================================
====================================================================================================================================