## 1° Explicando os arquivos

<br/>

> #### O .rar/.zip tem os seguintes arquivos:
> 
> - entrada.txt: O arquivo onde pode ser feita a leitura dos nós ficticios
> - entradaEnunciado.txt: Entrada de exemplo do trabalho, pode ser copiada e colada na leitura do programa
> - entradaExercicioPDF.txt: Entrada retirada de um dos PDFs das aulas, pode ser copiada e colada na leitura do programa
> - main.c: Arquivo com o codigo fonte do programa.
> - trabalhoPO.exe: É o executavel do **main.c** 

<br/>

## 2° Como rodar a aplicação
<br/>

1. Executar atraves do Explorador de Arquivos dando um duplo click no <strong>trabalhoPO.exe</strong> 

2. Atraves do VsCode pelo terminal digitando <em>./trabalhoPO.exe</em></br>Obs: Para abrir o terminal, basta pressionar CNTRL + '


<br>

## 3° Explicando a aplicação

Ao iniciar a aplicação, sera solicitada a entrada de dados. Caso o programa perceba que o problema não esta balanceado, ele solicitará uma interação do usuario com o programa.
<br/>
A interação é constituida por: <br><br>
[1] Preencher o custo com 0 <br>
[2] Preencher o custo atraves de um arquivo de texto <br/>
[3] Preencher o custo manualmente

Digitando 1: O programa irá preencher as variaveis com 0 <br>

Digitando 2: O programa irá ler os dados atraves de um arquivo que devera ser criado no mesmo local do executavel e tem que ser chamado de <em>entrada.txt</em> <br/>

Digitando 3: Voce irá digitar manualmente as entradas.

<br>

Feito isso, o programa irá calcular e exibir o **Quadro de custo unitario**, **A solução inicial pelo Método: Canto Noroeste e Menor Custo**, ira usar a melhor solução dentre os dois metodos e efetuar as interações para otimizar a solução atraves do metodo de Multiplicadores.rabalho de Pesquisa Operacional II
