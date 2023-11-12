# Space Invaders

## Link do Projeto

### [Clique aqui!](https://pedrotunin.github.io/CG/spaceinvaders/)

## Integrantes

**Nome:** Pedro de Souza Tunin

**RA:** 11201811959

## Detalhes

A aplicação é uma **tentativa** de cópia do clássico Space Invaders.

Foi utilizado como base o código do jogo Asteroids, disponibilizado nas aulas.

### Elementos

#### Nave do Jogador
![Nave do Jogador com os vértices](https://cdn.discordapp.com/attachments/561391859411451916/904495767937490964/unknown.png)

- Foram utilizados 9 vértices, formando 5 triângulos
- Os vértices foram distribuídos como na imagem acima (pontos brancos) e os triângulos destacados pelas linhas.

#### Naves Inimigas
![Nave Inimiga com os vértices](https://cdn.discordapp.com/attachments/561391859411451916/904500076620173362/unknown.png)

 - Foram utilizados 9 vértices, formando 3 triângulos.
 - Os vértices foram distribuídos como na imagem acima (pontos brancos), e os triângulos destacados (verde, azul, rosa).

#### Tiros

A implementação dos tiros da nave não mudou com o projeto Asteroids, somente a velocidade e a quantidade de tiros criados foi alterada (de 2 para 1).

#### Contador de inimigos

Ao iniciar um novo jogo (execução da função restart da classe Window), a variável **m_gameData.m_enemies_left** é inicializada com a quantidade inicial de inimigos.

A cada vez que é detectada uma colisão de um tiro com um inimigo (função checkCollisions da classe Window), o contador é decrescido em 1.

Para efetivamente mostrar na tela o conteúdo dessa variável, foi criado um campo de texto na função paintUI da classe Window. Além disso, foi criada uma lógica para somente mostrar o campo quando o estado do jogo for "Playing", ou seja, só quando o jogo estiver sendo executado.

### Movimentação

#### Nave do Jogador

Assim como no jogo original, só é possível realizar a movimentação horizontal.

A cada vez que o jogador pressiona um dos botões de movimento lateral (A, D, <-, ->), a posição X da nave é alterada (função update da classe Ship). Além disso, foram implementadas restrições para que a nave não saia da tela.

#### Naves Inimigas

As naves inimigas se movimentam de cima para baixo e de um lado para o outro. A cada vez que as naves das extremidades chegam em um lado da tela, a posição Y de todas é decrescida.

Além disso, a velocidade das naves é aumentada à medida que a quantidade de naves diminui. Para isso foi utilizada a seguinte função:

f(x) = 1 + log<sub>2</sub><sup>QTD_INICIAL_INIMIGOS/x</sup>

Onde **x** é quantidade atual de inimigos.

No gráfico abaixo, é a função para QTD_INICIAL_INIMIGOS = 50. A função funcionou para o que eu precisava, pois aumenta a dificuldade aos poucos no decorrer do jogo, e no final aumenta consideravelmente.ls 

![Gráfico da função f](https://cdn.discordapp.com/attachments/715747590578110486/1163653613399330917/image.png)
