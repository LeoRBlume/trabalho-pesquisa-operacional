/*
* Integrantes: 
* 
* Leonardo Franco
* Leonardo Ribeiro Blume
* Luiz Felipe Silva Santos
* Pedro Henrique Nakata
* Pedro Leandro Andrade Cruz
* Weslley Richard
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#define TAM_X 40
#define TAM_Y 40
#defi/*
* Integrantes: 
* 
* Leonardo Franco
* Leonardo Ribeiro Blume
* Luiz Felipe Silva Santos
* Pedro Henrique Nakata
* Pedro Leandro Andrade Cruz
* Weslley Richard
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#define TAM_X 40
#define TAM_Y 40
#define vazio -9000

typedef int lst_info;
typedef struct lst_no *lst_ptr;

typedef struct
{
    char origem[31];
    char destino[31];
    lst_info custo;
} rotas;

struct lst_no
{
    rotas conjunto;
    lst_ptr prox;
};

// Inicializa uma lista
void lst_iniciar(lst_ptr *);

// Insere um novo nó na k-ésima posição da lista
bool lst_inserir(lst_ptr *, int, rotas);

// Determina o número de nós de uma lista
int lst_tamanho(lst_ptr);

// Busca qual país é o nó transbordo
bool find_transbordo(lst_ptr, char *, bool);

// Pega as origens e armazena e um vetor sem corrigir
void origens(char[][31], lst_ptr, char[], bool);

// Pega os destinos e armazena e um vetor sem corrigir
void destinos(char[][31], lst_ptr, char[], bool);

// Faz a tabela de custo unitario
void tabelaCustoUnitario(rotas[TAM_X][TAM_Y], rotas[], int i, char[][31], char[][31], int[], int[]);

// Transforma uma lista encadeada em um array
void listTransform(lst_ptr, rotas[]);

// Exibe a materia de custo unitario
void printTCU(rotas[TAM_X][TAM_Y], int[], int[], char[][31], char[][31]);

// Funcao que testa se a oferta e demanda estao balanceadas,
// se nao estiverem pergunta se vai querer colocar os custos do
// no ficticio = 0 ou passar por arquivo de texto
int testaBalanceamento(lst_ptr, int[], int[], char[][31], char[][31], int, int, bool);

// Identifica as rotas inexistentes e atribui o maior custo x 10 a elas
int rotaInexistente(lst_ptr *, char[][31], char[][31], int);

// Arruma a ordem da tabela de custos unitarios
void arrumarTCU(rotas[TAM_X][TAM_Y], char *, char[][31], char[][31], int[], int[]);

// Funcao que calcula a solucao inicial atraves do Metodo do Canto Noroeste
float cantoNoroeste(rotas[TAM_X][TAM_Y], int[], int[], char[][31], char[][31], bool);

// Funcao que calcula a solucao inicial atraves do Metodo do Menor Custo
float menorCusto(rotas[TAM_X][TAM_Y], int[], int[], char[][31], char[][31], bool);

// Funcao que procura uma solucao otima atraves do metodo de menor custo
void metodoMultiplicadores(rotas[TAM_X][TAM_Y], int, int, int[*][*], int[], int[], char[][31], char[][31]);

// Funcao que exibe a tabela ja preenchida com os valores das ofertas e demandas
void exibirTabela(int, int, int[*][*], int[], int[], char[][31], char[][31]);

int main()
{
    /*-----------Declaração de variaveis-----------*/

    char resp = 'n', transbordo[30];
    int i = 0, j = 0, sumDemandas = 0, sumOfertas = 0, indiceD = 0, indiceO = 0;
    // Lista encadeada
    lst_ptr l;
    // Struct do tipo rota, que possui origem, destino e custo da rota
    rotas rota;
    // Tabela de custo unitario
    rotas tcu[TAM_X][TAM_Y];
    // Variavel que alterna qual a solucao inicial o metodo dos multiplicadores vai utilizar
    bool flagMenor = false;
    // Variavel que armazena se existe ou nao transbordo
    bool t = false;

    char orig[TAM_X][31];
    char dest[TAM_X][31];
    int ofertas[TAM_X];
    int demandas[TAM_X];
    int demandasAux[TAM_X];
    int demandasAux2[TAM_X];

    /*---------------------Fim ---------------------*/

    lst_iniciar(&l);

    printf("\n---------------------------------------\n");
    printf("-   Trabalho de Pesquisa Operacional  -");
    printf("\n---------------------------------------\n");

    do
    {
        printf("\nOrigem: ");
        scanf(" %30[^\n]", rota.origem);
        printf("Destino: ");
        scanf(" %30[^\n]", rota.destino);
        printf("Custo: ");
        scanf("%d", &rota.custo);
        printf("Mais rotas? (S/N): ");
        getchar();
        resp = getchar();

        if (resp != 'n' && resp != 'N' && resp != 's' && resp != 'S')
        {
            fprintf(stderr, "Erro: Entrada invalida");
            return 1;
        }
        // Inserindo as rotas na lista encadeada
        lst_inserir(&l, i, rota);
        i++; // Contando numero de nos da lista

    } while (resp != 'n' && resp != 'N');

    // Se t for verdadeiro, a funcao encontrou o transbordo
    t = find_transbordo(l, transbordo, t);

    if (t == true)
    {
        //Inserindo o custo da viagem do no transbordo para ele mesmo (0)
        rota.custo = 0;
        strcpy(rota.origem, transbordo);
        strcpy(rota.destino, transbordo);
        lst_inserir(&l, i, rota);
        i++;
        // Inserindo o transbordo no vetor de origens
        origens(orig, l, transbordo, t);
        // Inserindo o transbordo no vetor de destinos
        destinos(dest, l, transbordo, t);
    }
    else
    {
        strcpy(transbordo, "naotemtransbordo");
        origens(orig, l, transbordo, t);
        destinos(dest, l, transbordo, t);
    }
    // Vetor conjunto de rotas
    rotas conjunto[i + 3];

    j = 0;

    printf("\nOrigens\n");
    while (strlen(orig[j]) > 0)
    {
        if (strcasecmp(orig[j], transbordo) != 0)
        {
            printf("oferta %s: ", orig[j]);
            scanf("%d", &ofertas[j]);
            sumOfertas += ofertas[j];
        }
        j++;
    }
    indiceO = j - 1;

    j = 0;
    printf("\nDestinos\n");
    while (strlen(dest[j]) > 0)
    {
        if (strcasecmp(dest[j], transbordo) != 0)
        {
            printf("demanda %s: ", dest[j]);
            scanf("%d", &demandas[j]);
            sumDemandas += demandas[j];
        }
        j++;
    }
    indiceD = j - 1;

    // Testando o maior entre a soma das ofertas e das demandas
    // a maior será a oferta e demanda do transbordo
    if (t == true)
    {
        if (sumDemandas > sumOfertas)
        {
            demandas[indiceD] = sumDemandas;
            ofertas[indiceO] = sumDemandas;
        }
        else
        {
            demandas[indiceD] = sumOfertas;
            ofertas[indiceO] = sumOfertas;
        }
    }
    // I nas duas funcoes esta recendo o indice do ultimo no inserido na lista encadeada
    i = rotaInexistente(&l, orig, dest, i);
    i = testaBalanceamento(l, ofertas, demandas, orig, dest, j, i, t);

    listTransform(l, conjunto);
    tabelaCustoUnitario(tcu, conjunto, i, orig, dest, ofertas, demandas);

    arrumarTCU(tcu, transbordo, orig, dest, ofertas, demandas);
    system("cls");
    printTCU(tcu, ofertas, demandas, orig, dest);

    j = 0;

    printf("\nSolucao Inicial Canto Noroeste\n");
    float custoTotalCN = cantoNoroeste(tcu, ofertas, demandas, orig, dest, false);

    printf("\nSolucao Inicial Menor Custo\n");
    float custoTotalMN = menorCusto(tcu, ofertas, demandas, orig, dest, false);

    // Compara qual solucao inicial tem menor custo, pois
    // essa sera a que a funcao metodoMultiplicador ira
    // pegar para calcular a solucao otima
    if (custoTotalMN <= custoTotalCN)
    {
        printf("\nMelhor solucao inicial: Metodo do Menor Custo\n");
        menorCusto(tcu, ofertas, demandas, orig, dest, true);
    }
    else
    {
        printf("\nMelhor solucao inicial: Metodo do Canto Noroeste\n");
        cantoNoroeste(tcu, ofertas, demandas, orig, dest, true);
    }
    free(l);
    system("pause");
    return 0;
}

void exibirTabela(int lin, int col, int matriz[lin][col], int ofertas[], int demandas[], char dest[][31], char orig[][31])
{
    int k = 0, i = 0, j = 0;

    printf("\n%-10.30s|", "Origem");
    // Percorrendo os destinos
    while (strlen(dest[k]) > 0)
    {
        // Printando os destinos
        printf(" %10.30s |", dest[k]);
        k++;
    }
    printf("%10.30s |", "Oferta");
    printf("\n--------------------------------------------------------------");

    // Percorrendo as origens
    while (strlen(orig[i]) > 0)
    {
        // Printando as origens
        printf("\n%-10.30s|", orig[i]);
        for (j = 0; j < col; j++)
        {
            // Testamos se a celula da matriz possui 0, pois eles serao substituidos por
            // espacos em branco na hora de exibir a tabela
            if (matriz[i][j] == 0 || matriz[i][j] == vazio)
            {
                printf("%12.30s|", " ");
            }
            else
            {
                // Se nao, imprime o valor contido na celula
                printf(" %10d |", matriz[i][j]);
            }
        }
        printf("%10d |", ofertas[i]);
        i++;
        j = 0;
    }
    k = 0;
    printf("\n%-10.30s|", "Demanda");
    while (strlen(dest[k]) > 0)
    {
        printf(" %10d |", demandas[k]);
        k++;
    }
}

void metodoMultiplicadores(rotas tcu[TAM_X][TAM_Y], int lin, int col, int matriz[lin][col], int ofertas[], int demandas[], char orig[][31], char dest[][31])
{
    int i = 0, j = 0, menorOp = 0, menorOpLin = 0, menorOpCol = 0, interacao = 1;
    int cellOcup = 0, K1Linha = 0, K1Coluna = 0, K2Linha = 0, K2Coluna = 0, AddVal, AddLin, AddCol, k = 0, m = 0;
    bool lock1 = false, lock2 = false; // Forcar o permanecimento num loop
    int Y[col];
    int X[lin];
    int MatrizCusto[lin][col];
    int estatistica[lin][col];
    float custoTotal = 0;
    X[0] = 0;

    while (1)
    {
        i = 0, j = 0, menorOp = 0, menorOpLin = 0, menorOpCol = 0;
        cellOcup = 0, K1Linha = 0, K1Coluna = 0, K2Linha = 0, K2Coluna = 0, AddVal = 0, AddLin = 0, AddCol = 0, k = 0;
        custoTotal = 0;

        //preencher o MatrizCusto[][] com os custo das celulas ocupadas, e contar as celulas ocupadas
        for (i = 0; i < lin; i++)
        {
            for (j = 0; j < col; j++)
            {
                if (matriz[i][j] > 0)
                {
                    // Salva os custos das celulas preenchidas
                    MatrizCusto[i][j] = tcu[i][j].custo;
                    cellOcup++;
                }
                else
                {
                    matriz[i][j] = vazio;
                    MatrizCusto[i][j] = vazio;
                }
            }
        }

        //Fazer o programa repetir esse trecho ate o lock1 == false
        lock1 = true;
        while (lock1)
        {
            //Ver se m > cellOcup, e caso sim, adicionar nova variavel
            m = lin + col - 1;
            AddVal = 9000;
            if (m > cellOcup)
            {
                for (i = 0; i < lin; i++)
                {
                    for (j = 0; j < col; j++)
                    {
                        //Variavel precisar ser:
                        //De uma celula vazia.
                        //Nao pode ser marcada, matrizCusto == vazio-1.
                        //Menor custo da tabela, e acima de 0.
                        if (matriz[i][j] < 0 && (MatrizCusto[i][j] != (vazio - 1)) && (AddVal > tcu[i][j].custo && tcu[i][j].custo > 0))
                        {
                            AddVal = tcu[i][j].custo;
                            AddLin = i;
                            AddCol = j;
                        }
                    }
                }
                MatrizCusto[AddLin][AddCol] = AddVal;
                matriz[AddLin][AddCol] = 0;
            }

            //preencher o estatistica[][] com vazio
            for (i = 0; i < lin; i++)
            {
                for (j = 0; j < col; j++)
                {
                    estatistica[i][j] = vazio;
                }
            }
            //preencher o X e Y com vazio
            for (i = 0; i < lin; i++)
            {
                X[i] = vazio;
            }
            for (j = 0; j < col; j++)
            {
                Y[j] = vazio;
            }
            // Zerar um valor dos aux
            X[0] = 0;

            i = 0;
            j = 0;

            int Xpreenchidos = 0;
            int Ypreenchidos = 0;

            // Enquanto as variasveis X e Y nao estiverem preenchidas
            while (Xpreenchidos < lin || Ypreenchidos < col)
            {
                Xpreenchidos = 0;
                Ypreenchidos = 0;

                // Verificando se as variaveis X e Y ainda estao vazias
                for (i = 0; i < lin; i++)
                {
                    if (X[i] != vazio)
                    {
                        Xpreenchidos++;
                    }
                }
                i = 0;

                for (j = 0; j < col; j++)
                {
                    if (Y[i] != vazio)
                    {
                        Ypreenchidos++;
                    }
                }
                j = 0;
                // Calculando as variaveis X e Y
                while (i < lin)
                {
                    while (j < col)
                    {
                        if (matriz[i][j] != vazio)
                        {
                            if (cellOcup < lin + col - 1)
                            {
                                if (X[i] == vazio && Y[j] == vazio)
                                {
                                    X[i] = 0;
                                    Y[j] = MatrizCusto[i][j] - X[i];
                                }
                                else if (X[i] == vazio)
                                {
                                    X[i] = MatrizCusto[i][j] - Y[j];
                                }
                                else
                                {
                                    Y[j] = MatrizCusto[i][j] - X[i];
                                }
                            }
                            else
                            {
                                if (X[i] == vazio && Y[j] != vazio)
                                {
                                    X[i] = MatrizCusto[i][j] - Y[j];
                                }
                                else if (X[i] != vazio && Y[j] == vazio)
                                {
                                    Y[j] = MatrizCusto[i][j] - X[i];
                                }
                            }
                        }
                        j++;
                    }
                    j = 0;
                    i++;
                }
                i = 0;
            }

            i = 0;
            j = 0;

            //Achar menor custo de oportunidade, e inserir em menorOp
            menorOp = 0;
            for (i = 0; i < lin; i++)
            {
                for (j = 0; j < col; j++)
                {
                    if (matriz[i][j] == vazio)
                    {
                        // Preenchendo a matriz estatistica atraves da formula Cij - Xi + Yi
                        estatistica[i][j] = tcu[i][j].custo - (X[i] + Y[j]);
                        if (estatistica[i][j] < menorOp)
                        {
                            // Pegando o valor menor valor
                            menorOp = estatistica[i][j];
                            menorOpLin = i;
                            menorOpCol = j;
                        }
                    }
                }
            }

            //Caso a menor oportunidade seja >= 0, significa que a solucao atual esta otimizada.
            //E a otimizacao sera parada
            if (menorOp >= 0)
            {
                if (menorOp == 0)
                    printf("\n\nA otimizacao terminou. \nE existe a possibilidade de uma alternativa solucao\n\n  ");
                else
                    printf("\n\nA otimizacao terminou.\n\n  ");

                return;
            }

            //Fazer caminho (loop quadrado), podendo apenas escolher celulas ocupadas, e celula desocupada escolhida
            i = 0;
            j = 0;
            lock2 = true;
            while (lock2)
            {
                //Procura uma celula ocupada, e maior que 0, na vertical a menorOp
                //e depois, procura uma celula ocupada, e maior que 0, na horizontal a menorOp,
                //e uma celula ocupada a diagonal de menorOp
                while (i < lin)
                {
                    if (matriz[i][menorOpCol] > 0)
                    {
                        K1Linha = i;
                        K1Coluna = menorOpCol;
                        break;
                    }
                    i++;
                }
                j = 0;
                while (j < col && i < lin)
                {
                    if (matriz[menorOpLin][j] > 0 && matriz[i][j] != vazio)
                    {
                        K2Linha = menorOpLin;
                        K2Coluna = j;
                        lock1 = false;
                        lock2 = false;
                        break;
                    }
                    j++;
                }
                i++;
                if (i > lin + 1)
                {
                    MatrizCusto[AddLin][AddCol] = vazio - 1;
                    matriz[AddLin][AddCol] = vazio;
                    lock2 = false;
                }
            }
        }

        //Pegar k, para somar e subtrair das celulas K
        if (matriz[K1Linha][K1Coluna] <= matriz[K2Linha][K2Coluna])
            k = matriz[K1Linha][K1Coluna];
        else
            k = matriz[K2Linha][K2Coluna];

        for (int h = 0; h < lin; h++)
        {
            for (int l = 0; l < col; l++)
            {
                if (h == K1Linha && l == K1Coluna)
                {
                    matriz[h][l] -= k;
                }
                else if (h == K2Linha && l == K2Coluna)
                {
                    matriz[h][l] -= k;
                }
                else if (h == K1Linha && l == K2Coluna)
                    matriz[h][l] += k;

                else if (h == menorOpLin && l == menorOpCol)
                    matriz[h][l] = k;
            }
        }
        printf("\n\nInteracao %d\n", interacao);
        
        //Na matriz[][] fazer com que os valores == vazio, virem 0
        for (i = 0; i < lin; i++)
        {
            for (j = 0; j < col; j++)
            {
                if (matriz[i][j] == vazio)
                    matriz[i][j] = 0;
            }
        }
        exibirTabela(lin, col, matriz, ofertas, demandas, dest, orig);

        // Calculo do Custo Total
        for (int h = 0; h < lin; h++)
        {
            for (int l = 0; l < col; l++)
            {
                if (matriz[h][l] >= 0)
                    // Calculo do Custo Total
                    custoTotal += matriz[h][l] * tcu[h][l].custo;
            }
        }
        printf("\n\nCusto: %.2f", custoTotal);
        interacao++;
    }
}

float menorCusto(rotas tcu[TAM_X][TAM_Y], int ofertas[], int demandas[], char orig[][31], char dest[][31], bool flag)
{
    int i = 0, j = 0, k = 0, lin = 0, col = 0, menor = 100000, sumDemanda = 1, cont = 0;
    int mX = 0, mY = 0;
    float custoTotal = 0.0;

    while (ofertas[i] > 0)
    {
        j++; // Contando o numero de elementos
        i++;
    }
    i = 0;

    // "Clonando" o vetor de ofertas
    int auxOfertas[j];
    for (i = 0; i < j; i++)
    {
        auxOfertas[i] = ofertas[i];
    }
    j = 0;
    i = 0;
    while (demandas[i] > 0)
    {
        j++; //contando o numero de elementos
        i++;
    }
    // "Clonando" o vetor de demandas
    int auxDemandas[j];
    i = 0;
    for (i = 0; i < j; i++)
    {
        auxDemandas[i] = demandas[i];
    }
    j = 0;
    i = 0;

    // Percorrendo a tabela de custos unitarios contando
    // o numero de linhas e colunas para criar as matrizes
    // que serao utilizadas
    while (strlen(tcu[i][j].origem) > 0)
    {
        col = 0;
        while (strlen(tcu[i][j].destino) > 0)
        {
            col++;
            j++;
        }
        j = 0;
        i++;
        lin++;
    }
    // A matriz de tcu (tabela de custos unitarios) é do tipo struct rota,
    // porem, para ficar mais facil de trabalhar, clonamos os custos
    // para uma matriz do tipo int
    int custos[lin][col];

    // Matriz que sera preenchida com a solucao inicial
    int matriz[lin][col];

    for (i = 0; i < lin; i++)
    {
        for (j = 0; j < col; j++)
        {
            // Preenchendo a matriz com 0, para nao dar conflito na hora de exibi-la
            matriz[i][j] = vazio;
        }
    }

    i = 0;
    j = 0;
    for (i = 0; i < lin; i++)
    {
        for (j = 0; j < col; j++)
        {
            // Clonando os custos da tabela tcu
            custos[i][j] = tcu[i][j].custo;
        }
    }
    i = 0;
    j = 0;

    // Continuara calculando a solucao inicial enquanto a soma das demandas forem != 0
    while (sumDemanda != 0)
    {
        sumDemanda = 0;
        menor = 100000;
        // For que percorre a matriz procurando o menor valor e salvando numa variavel
        // se a linha e coluna nao estiverem zeradas
        for (i = 0; i < lin; i++)
        {
            for (j = 0; j < col; j++)
            {
                if (menor > custos[i][j] && (auxOfertas[i] != 0 && auxDemandas[j] != 0))
                {
                    menor = custos[i][j];
                    // Salvando os indices do menor valor
                    mX = i;
                    mY = j;
                }
            }
        }
        // Se a oferta da linha do menor valor for menor que a demanda da coluna de menor valor
        if (auxOfertas[mX] < auxDemandas[mY])
        {
            // Coloca a oferta na matriz
            matriz[mX][mY] = auxOfertas[mX];
            // Multiplica o custo com a oferta e soma na variavel que armazena o custo total
            custoTotal += matriz[mX][mY] * custos[mX][mY];
            // Demanda - oferta
            auxDemandas[mY] -= auxOfertas[mX];
            // Zera a oferta
            auxOfertas[mX] = 0;
        }
        // Se a oferta e demanda forem iguais
        else if (auxOfertas[mX] == auxDemandas[mY])
        {
            // Salva a oferta
            matriz[mX][mY] = auxOfertas[mX];
            // Multiplica o custo com a oferta e soma na variavel que armazena o custo total
            custoTotal += matriz[mX][mY] * custos[mX][mY];
            // Zera tanto a demanda quanto a oferta
            auxOfertas[mX] = 0;
            auxDemandas[mY] = 0;
        }
        else // Se nao a demanda da coluna do menor valor e menor que a oferta da linha de menor valor
        {
            // Coloca a demanda na matriz
            matriz[mX][mY] = auxDemandas[mY];
            // Multiplica o custo com a demanda e soma na variavel que armazena o custo total
            custoTotal += matriz[mX][mY] * custos[mX][mY];
            // Oferta - demanda
            auxOfertas[mX] -= auxDemandas[mY];
            // Zera a demanda
            auxDemandas[mY] = 0;
        }
        i = 0;
        while (i < col)
        {
            // Somando as demandas para saber se todas demandas estao zeradas
            sumDemanda += auxDemandas[i];
            i++;
        }
        i = 0;
        j = 0;
    }
    i = 0;
    j = 0;
    if (!flag)
    {
        exibirTabela(lin, col, matriz, ofertas, demandas, dest, orig);
        printf("\n\nCusto Inicial: %.2f\n", custoTotal);
    }
    else
        metodoMultiplicadores(tcu, lin, col, matriz, ofertas, demandas, orig, dest);

    return custoTotal;
}

float cantoNoroeste(rotas tcu[TAM_X][TAM_Y], int ofertas[], int demandas[], char orig[][31], char dest[][31], bool flag)
{
    int i = 0, j = 0, k = 0, lin = 0, col = 0;
    float custoTotal = 0.0;

    j = 0;

    while (ofertas[i] > 0)
    {
        j++; //contando o numero de elementos
        i++;
    }
    i = 0;
    // "Clonando" o vetor de ofertas
    int auxOfertas[j];
    for (i = 0; i < j; i++)
    {
        auxOfertas[i] = ofertas[i];
    }
    j = 0;
    i = 0;
    while (demandas[i] > 0)
    {
        j++; //contando o numero de elementos
        i++;
    }
    // "Clonando" o vetor de demandas
    int auxDemandas[j];
    i = 0;
    for (i = 0; i < j; i++)
    {
        auxDemandas[i] = demandas[i];
    }
    j = 0;
    i = 0;

    // Percorrendo a tabela de custos unitarios contando
    // o numero de linhas e colunas para criar as matrizes
    // que serao utilizadas
    while (strlen(tcu[i][j].origem) > 0)
    {
        col = 0;
        while (strlen(tcu[i][j].destino) > 0)
        {
            col++;
            j++;
        }
        j = 0;
        i++;
        lin++;
    }
    // A matriz de tcu (tabela de custos unitarios) é do tipo struct rota,
    // porem, para ficar mais facil de trabalhar, clonamos os custos
    // para uma matriz do tipo int
    int custos[lin][col];

    // Matriz que sera preenchida com a solucao inicial
    int matriz[lin][col];

    for (i = 0; i < lin; i++)
    {
        for (j = 0; j < col; j++)
        {
            // Preenchendo a matriz com 0, para nao dar conflito na hora de exibi-la
            matriz[i][j] = vazio;
        }
    }

    i = 0;
    j = 0;
    for (i = 0; i < lin; i++)
    {
        for (j = 0; j < col; j++)
        {
            // Clonando os custos da tabela tcu
            custos[i][j] = tcu[i][j].custo;
        }
    }
    i = 0;
    j = 0;
    // Enquanto a ultima celula da matriz for diferente da ultima demanda do
    // vetor de demandas ou diferente da ultima oferta do vetor de ofertas
    // continuara fazendo os calculos
    while (matriz[lin - 1][col - 1] != ofertas[lin - 1] || matriz[lin - 1][col - 1] != demandas[col - 1])
    {
        // Se a ultima celula da matriz for igual a ultima demanda do vetor
        // de demandas ou igual a ultima oferta do vetor de ofertas dara break no while
        if (matriz[lin - 1][col - 1] == ofertas[lin - 1] || matriz[lin - 1][col - 1] == demandas[col - 1])
        {
            break;
        }
        if (auxOfertas[i] < auxDemandas[j]) // Se a oferta for menor que a demanda
        {
            // Demanda - oferta
            auxDemandas[j] -= auxOfertas[i];
            // Coloca a oferta na matriz
            matriz[i][j] = auxOfertas[i];
            // Multiplica o custo com a oferta e soma na variavel que armazena o custo total
            custoTotal += matriz[i][j] * custos[i][j];
            i++;
        }
        else // Senao demanda e menor que a oferta
        {
            // Oferta - demanda
            auxOfertas[i] -= auxDemandas[j];
            // Coloca a demanda na matriz
            matriz[i][j] = auxDemandas[j];
            // Multiplica o custo com a demanda e soma na variavel que armazena o custo total
            custoTotal += matriz[i][j] * custos[i][j];
            j++;
        }
    }
    i = 0;
    j = 0;
    if (!flag)
    {
        exibirTabela(lin, col, matriz, ofertas, demandas, dest, orig);
        printf("\n\nCusto Inicial: %.2f\n\n", custoTotal);
    }
    else
        metodoMultiplicadores(tcu, lin, col, matriz, ofertas, demandas, orig, dest);

    return custoTotal;
}

int rotaInexistente(lst_ptr *l2, char orig[][31], char dest[][31], int tam)
{
    int j = 0, i = 0, cont = 0, maiorCusto = 0;
    lst_ptr aux = *l2;
    lst_ptr l = *l2;
    rotas rota;

    // Procura o maior custo inserido na lista encadeada de rotas
    while (l != NULL)
    {
        if (maiorCusto < l->conjunto.custo)
        {
            maiorCusto = l->conjunto.custo;
        }
        l = l->prox;
    }
    l = aux;
    // Multiplica o maior custo por 10, ja que sera o custo da rota inexistente
    maiorCusto = maiorCusto * 10;

    while (strlen(orig[j]) > 0)
    {
        while (strlen(dest[i]) > 0)
        {
            while (l != NULL)
            {
                // Se encontrou uma rota que ja existe da break e deixa cont = 0
                if (strcasecmp(orig[j], l->conjunto.origem) == 0 && strcasecmp(dest[i], l->conjunto.destino) == 0)
                {
                    cont = 0;
                    break;
                }
                else // Se nao, significa que a rota e inexistente e cont = 1
                {
                    cont = 1;
                }
                l = l->prox;
            }
            if (cont == 1)
            {
                // Insere a origem, destino e o custo da rota inexistente
                // na struct do tipo rota
                strcpy(rota.origem, orig[j]);
                strcpy(rota.destino, dest[i]);
                rota.custo = maiorCusto;
                // Insere a rota na lista encadeada
                lst_inserir(l2, tam, rota);
                tam++; // Para sabermos qual foi o ultimo indice da lista
                cont = 0;
            }
            i++;
        }
        l = aux;
        i = 0;
        j++;
    }

    return tam;
}

int testaBalanceamento(lst_ptr l, int ofertas[], int demandas[], char orig[][31], char dest[][31], int n, int tam, bool t)
{
    FILE *arq;
    rotas rota;
    int sumOfertas = 0, sumDemandas = 0, j = 0, auxIndice = 0;
    int resp;
    //Se flag = 0 significa q o ficticio fica na origem, se flag = 1 significa q fica no destino
    int flag = 2;
    char s[10];
    bool flagResp = false;

    while (ofertas[j] > 0)
    {
        sumOfertas += ofertas[j];
        j++;
    }

    j = 0;
    while (demandas[j] > 0)
    {
        sumDemandas += demandas[j];
        j++;
    }

    j = 0;

    if (sumDemandas == sumOfertas)
    {
        flag = 2;
    }
    else
    {
        // Se a soma das demandas for maior que das ofertas
        // insere o no ficticio no vetor de origens
        if (sumDemandas > sumOfertas)
        {
            origens(orig, l, "Outros", t);
            // Adiciona a diferenca das demandas e ofertas no vetor de ofertas
            ofertas[n] = sumDemandas - sumOfertas;
            flag = 0; // Avisando que o no ficticio esta na origem
        }
        // Se nao, insere o no ficticio no vetor de destinos
        else
        {
            destinos(dest, l, "Outros", t);
            // Adiciona a diferenca ofertas e demandas no vetor de demandas
            demandas[n] = sumOfertas - sumDemandas;
            flag = 1; // Avisando que o no ficticio esta no destino
        }
    }

    j = 0;
    if (flag == 0) // Se estiver na origem
    {
        do
        {
            printf("\n\nDetectamos que o programa nao esta balanceado.\n[1] Preencher o custo com 0\n[2] Preencher o custo atraves de um arquivo de texto\n");
            printf("[3] Preencher o custo manualmente\n");
            printf("Informe sua resposta: ");
            scanf("%d", &resp);
            // Significa que escolheu preencher os custos do no ficticio com 0
            if (resp == 1)
            {
                flagResp = true;
                // Inserindo as rotas (Outros -> destino) na lista
                while (strlen(orig[j]) > 0 && strcasecmp("Outros", orig[j]) != 0)
                {
                    strcpy(rota.origem, "Outros");
                    strcpy(rota.destino, dest[j]);
                    rota.custo = 0;
                    lst_inserir(&l, tam, rota);
                    tam++; // Para sabermos qual foi o ultimo indice da lista
                    j++;
                }
            }
            // Significa que escolheu preencher os custos do no ficticio atraves de um arquivo
            else if (resp == 2)
            {
                flagResp = true;
                printf("\nCrie um arquivo no mesmo local do programa chamado "
                       "entrada.txt"
                       " e informe os custos do no ficticio na ordem abaixo.\n\n");
                printf("Ordem do arquivo de entrada: \n");
                while (strlen(dest[auxIndice]) > 0)
                {
                    printf("Outros com %s\n", dest[auxIndice]);
                    auxIndice++;
                }
                printf("\n");

                printf("Pressione ENTER apos ter criado: ");
                getchar();
                getchar();
                // Testa se foi possivel abrir o arquivo de tipo texto para leitura
                if ((arq = fopen("entrada.txt", "r")) == NULL)
                {
                    fprintf(stderr, "Erro na abertura do arquivo ou arquivo nao encontrado!\n");
                }
                int cont = 0;
                j = 0;
                // Percorrendo o arquivo inserindo as rotas na lista encadeada
                while (fgets(s, sizeof s, arq) != NULL)
                {
                    strcpy(rota.origem, "Outros");
                    strcpy(rota.destino, dest[j]);
                    rota.custo = atoi(s);
                    lst_inserir(&l, tam, rota);
                    tam++; // Para sabermos qual foi o ultimo indice da lista
                    j++;
                }
            }
            // Significa que escolheu preencher os custos do no ficticio manualmente
            else if (resp == 3)
            {
                auxIndice = 0;
                flagResp = true;
                printf("\nInforme os custos do no ficticio: \n");
                while (strlen(dest[auxIndice]) > 0)
                {
                    printf("Rota: Outros com %s \n", dest[auxIndice]);
                    strcpy(rota.origem, "Outros");
                    strcpy(rota.destino, dest[auxIndice]);
                    printf("Informe o custo: ");
                    scanf("%d", &rota.custo);
                    lst_inserir(&l, tam, rota);
                    tam++; // Para sabermos qual foi o ultimo indice da lista
                    auxIndice++;
                    printf("\n");
                }
            }

        } while (!flagResp);
    }
    else if (flag == 1) // Se estiver no destino
    {
        do
        {
            printf("\n\nDetectamos que o programa nao esta balanceado.\n[1] Preencher o custo com 0\n[2] Preencher o custo atraves de um arquivo de texto\n");
            printf("[3] Preencher o custo manualmente\n");
            printf("Informe sua resposta: ");
            scanf("%d", &resp);
            if (resp == 1) // Significa que escolheu preencher os custos do no ficticio com 0
            {
                flagResp = true;
                // Inserindo as rotas (Origem -> Outros) na lista encadeada
                while (strlen(dest[j]) > 0 && strcasecmp("Outros", dest[j]) != 0)
                {
                    strcpy(rota.origem, orig[j]);
                    strcpy(rota.destino, "Outros");
                    rota.custo = 0;
                    lst_inserir(&l, tam, rota);
                    tam++; // Para sabermos qual foi o ultimo indice da lista
                    j++;
                }
            }
            // Significa que escolheu preencher os custos do no ficticio atraves de um arquivo
            else if (resp == 2)
            {
                flagResp = true;
                printf("\nCrie um arquivo no mesmo local do programa chamado "
                       "entrada.txt"
                       " e informe os custos do no ficticio na ordem abaixo.\n\n");
                printf("Ordem do arquivo de entrada: \n");
                while (strlen(orig[auxIndice]) > 0)
                {
                    printf("%s com outros\n", orig[auxIndice]);
                    auxIndice++;
                }
                printf("\n");

                printf("Pressione ENTER apos ter criado: ");
                getchar();
                getchar();
                // Testa se foi possivel abrir o arquivo de tipo texto para leitura
                if ((arq = fopen("entrada.txt", "r")) == NULL)
                {
                    fprintf(stderr, "Erro na abertura do arquivo ou arquivo nao encontrado!\n");
                }
                int cont = 0;
                j = 0;
                // Percorrendo o arquivo inserindo as rotas na lista encadeada
                while (fgets(s, sizeof s, arq) != NULL)
                {
                    strcpy(rota.origem, orig[j]);
                    strcpy(rota.destino, "Origem");
                    rota.custo = atoi(s);
                    lst_inserir(&l, tam, rota);
                    tam++; // Para sabermos qual foi o ultimo indice da lista
                    j++;
                }
            }
            // Significa que escolheu preencher os custos do no ficticio manualmente
            else if (resp == 3)
            {
                flagResp = true;
                auxIndice = 0;
                printf("\nInforme os custos do no ficticio: \n");
                while (strlen(orig[auxIndice]) > 0)
                {
                    printf("Rota: Outros com %s \n", orig[auxIndice]);
                    strcpy(rota.origem, orig[auxIndice]);
                    strcpy(rota.destino, "Outros");
                    printf("Informe o custo: ");
                    scanf("%d", &rota.custo);
                    lst_inserir(&l, tam, rota);
                    tam++; // Para sabermos qual foi o ultimo indice da lista
                    auxIndice++;
                    printf("\n");
                }
            }

        } while (!flagResp);
    }

    return tam;
}

void arrumarTCU(rotas tcu[TAM_X][TAM_Y], char transbordo[], char orig[][31], char dest[][31], int ofertas[], int demandas[])
{
    int i = 0, j = 0, k = 0, l = 0, m = 0, n = 0;
    rotas tcu2[TAM_X][TAM_Y];

    // Percorre a matriz de struct tcu colocando a ordem correta dos elementos na tcu2
    while (strlen(orig[i]) > 0)
    {
        while (strcasecmp(tcu[k][0].origem, orig[i]) != 0)
        {
            k++;
        }
        while (strlen(dest[j]) > 0)
        {
            while (strcasecmp(tcu[k][l].destino, dest[j]) != 0)
            {
                l++;
            }
            if (strcasecmp(tcu[k][l].origem, orig[i]) == 0 && strcasecmp(tcu[k][l].origem, orig[i]) == 0)
            {
                tcu2[m][n] = tcu[k][l];
                n++;
            }

            j++;
            l = 0;
        }
        n = 0;
        m++;
        i++;
        j = 0;
    }
    i = 0;
    j = 0;

    while (strlen(tcu2[i][0].origem) > 0)
    {
        while (strlen(tcu2[i][j].destino) > 0)
        {
            // Colocando a tcu2 arrumada de volta na tcu, para corrigi-la
            tcu[i][j] = tcu2[i][j];
            j++;
        }
        j = 0;
        i++;
    }
}

void listTransform(lst_ptr l, rotas conjunto[])
{
    int j = 0;
    // Percorre a lista salvando as rotas num vetor de rotas chamado conjunto
    // para uma melhor manipulacao dos dados
    while (l != NULL)
    {
        conjunto[j] = l->conjunto;
        l = l->prox;
        j++;
    }
    j--;
}

void tabelaCustoUnitario(rotas tcu[TAM_X][TAM_Y], rotas conjunto[], int n, char orig[][31], char dest[][31], int ofertas[], int demandas[])
{
    int j = 0, k = 0, l = 0;

    // Percorre o vetor de origens enquanto ele for diferente de vazio
    while (strlen(orig[j]) > 0)
    {
        // Percorre o conjunto onde armazena todas as rotas
        for (k = 0; k < n; k++)
        {
            // Verifica se a origem da rota e igual a origem esperada
            if (strcasecmp(conjunto[k].origem, orig[j]) == 0)
            {
                // Passando a rota para a matriz de rotas
                tcu[j][l] = conjunto[k];
                l++;
            }
        }
        j++;
        l = 0;
    }
}

void printTCU(rotas tcu[TAM_X][TAM_Y], int ofertas[], int demandas[], char orig[][31], char dest[][31])
{
    int i = 0, j = 0, k = 0, l = 0, aux = 10;

    //Pegar maior tamanho da variavel
    for (int tam = 0; strlen(orig[tam]) > 0; tam++)
    {
        aux = strlen(orig[tam]);
        if (aux >= l)
            l = aux + 1; //adicionar +1 para espaco da variavel
    }
    for (int tam = 0; strlen(dest[tam]) > 0; tam++)
    {
        aux = strlen(dest[tam]);
        if (aux >= l)
            l = aux + 1;
    }
    // Percorrendo os vetores e printando as linhas e colunas da tabela tcu
    printf("\nQuadro de Custos Unitarios\n");
    printf("\n%-10.30s|", "Origem");
    while (strlen(dest[k]) > 0)
    {
        printf(" %10.30s |", dest[k]);
        k++;
    }
    printf("%10.30s |", "Oferta");
    printf("\n--------------------------------------------------------------");
    while (strlen(tcu[i][0].origem) > 0)
    {
        printf("\n%-10.30s|", tcu[i][0].origem);
        while (strlen(tcu[i][j].origem) > 0)
        {
            printf(" %10d |", tcu[i][j].custo);
            j++;
        }
        printf("%10d |", ofertas[i]);
        i++;
        j = 0;
    }
    k = 0;
    printf("\n%-10.30s|", "Demanda");
    while (strlen(dest[k]) > 0)
    {
        printf(" %10d |", demandas[k]);
        k++;
    }
    printf("\n\n");
}

void origens(char orig[][31], lst_ptr l, char transbordo[], bool t)
{
    int indice = 0, m = 0;

    // Se a origem da rota do primeiro no da lista encadeada for diferente de nulo
    // e diferente do transbordo
    if (l->conjunto.origem != NULL && strcasecmp(transbordo, l->conjunto.origem) != 0)
    {
        // Copia a origem da rota para o vetor de origens
        strcpy(orig[indice], l->conjunto.origem);
    }
    l = l->prox;

    while (l != NULL)
    {
        indice = 0;
        m = 0;
        while (strcasecmp(orig[indice], "") != 0)
        {
            // Verifica se a origem do conjunto é igual a esperada e diferente do transbordo
            if (strcasecmp(orig[indice], l->conjunto.origem) == 0 && strcasecmp(transbordo, l->conjunto.origem) != 0)
            {
                m++;
            }
            indice++;
        }
        // Copia a origem da lista encadeada para o vetor de origens
        // caso m = 0 significa que a origem do conjunto nao esta contida no vetor origem
        // e copia para o vetor, caso contrario passa para o proximo no da lista
        if (m == 0 && strcasecmp(transbordo, l->conjunto.origem) != 0)
            strcpy(orig[indice], l->conjunto.origem);
        l = l->prox;
    }
    // Se a variavel t do tipo bool passada por parametro seja igual a verdadeiro,
    // significa que o programa encontrou um transbordo
    if (t == true)
    {
        // Copia o transbordo para o final do vetor de origens
        strcpy(orig[indice], transbordo);
    }
}

void destinos(char dest[][31], lst_ptr l, char transbordo[], bool t)
{
    int indice = 0, m = 0;

    // Se o destino da rota do primeiro no da lista encadeada for diferente de nulo
    // e diferente do transbordo
    if (l->conjunto.destino != NULL && strcasecmp(transbordo, l->conjunto.destino) != 0)
    {
        // Copia o destino da rota para o vetor de destinos
        strcpy(dest[indice], l->conjunto.destino);
    }
    l = l->prox;

    while (l != NULL)
    {
        indice = 0;
        m = 0;
        while (strlen(dest[indice]) > 0)
        {
            // Verifica se o destino do conjunto é igual ao esperado e diferente do transbordo
            if (strcasecmp(dest[indice], l->conjunto.destino) == 0 && strcasecmp(transbordo, l->conjunto.destino) != 0)
            {
                m++;
            }
            indice++;
        }
        // Copia o destino da lista encadeada para o vetor de destinos
        // caso m = 0, que significa que o destino do conjunto nao esta contido no vetor destinos,
        // e copia para o vetor, caso contrario passa para o proximo no da lista
        if (m == 0 && strcasecmp(transbordo, l->conjunto.destino) != 0)
            strcpy(dest[indice], l->conjunto.destino);
        l = l->prox;
    }
    // Se a variavel t do tipo bool passada por parametro seja igual a verdadeiro,
    // significa que o programa encontrou um transbordo
    if (t == true)
    {
        // Copia o transbordo para o final do vetor de origens
        strcpy(dest[indice], transbordo);
    }
}

bool find_transbordo(lst_ptr l, char *transbordo, bool t)
{
    lst_ptr aux = l;
    lst_ptr noInicial = l; // Salva o no inicial da lista

    // Percorre a lista e a lista auxiliar
    while (l != NULL)
    {
        while (aux != NULL)
        {
            // Verifica se existe algum pais que seja tanto destino quanto origem
            if (strcasecmp(l->conjunto.origem, aux->conjunto.destino) == 0)
            {
                strcpy(transbordo, aux->conjunto.destino);
                // t = verdadeiro, mostrando para o programa que existe um transbordo
                t = true;
            }
            aux = aux->prox;
        }
        // Faz a lista voltar para o inicio
        aux = noInicial;
        l = l->prox;
    }
    return t;
}

void lst_iniciar(lst_ptr *l)
{
    *l = NULL;
}

bool lst_inserir(lst_ptr *l, int k, rotas val)
{
    // Percorre de 0 ate a posicao desejada da lista
    if (k >= 0 && k <= lst_tamanho(*l))
    {
        lst_ptr n; // Declarando um no auxiliar

        // Tenta alocar a memoria e retorna falso caso nao consiga
        if ((n = (lst_ptr)malloc(sizeof(struct lst_no))) == NULL)
        {
            return false;
        }
        // Coloca a rota num no auxiliar
        n->conjunto = val;

        // Caso k = 0 a lista esta vazia e coloca a primeira posicao
        if (k == 0)
        {
            n->prox = *l;
            *l = n;
        }
        // Como a lista nao esta vazia, percorre a lista ate a posicao k
        // passada por parametro
        else
        {
            lst_ptr p = *l;
            for (int i = 0; i < k - 1; i++)
            {
                p = p->prox;
            }
            n->prox = p->prox;
            p->prox = n;
        }
        return true;
    }
    return false;
}

int lst_tamanho(lst_ptr l)
{
    int cont = 0;
    while (l != NULL)
    {
        // Contando o tanto de nos da lista
        cont++;
        l = l->prox;
    }
    return cont;
}