# Atividade 1 - Space Invaders

## Link do projeto
#### [Clique aqui!](https://pedrotunin.github.io/CG/atividade1/)

## Integrantes
    Pedro de Souza Tunin;  RA: 11201811959 
    Leonardo Ryo Nakagawa; RA: 11201812233

## Sobre
O jogo é uma versão simplificada do clássico de Atari: Space Invaders.
Nele, o jogador controla uma nave que tem como objetivo derrotar todas as naves, antes que elas cheguem ao final da tela ou colidam contra a sua nave.

## Desenvolvimento
Utilizamos como base para o desenvolvimento o código do jogo Asteroids, disponinilizado em aula.

### Design das naves

#### Nave do Jogador
![Nave do Jogador com os vértices](https://cdn.discordapp.com/attachments/561391859411451916/904495767937490964/unknown.png)

- Foram utilizados 9 vértices, formando 5 triângulos
- Os vértices foram distribuídos como na imagem acima (pontos brancos) e os triângulos destacados pelas linhas.

#### Naves Inimigas
![Nave Iniimiga com os vértices](https://cdn.discordapp.com/attachments/561391859411451916/904500076620173362/unknown.png)

 - Foram utilizados 9 vértice, formando 3 triângulos.
 - Os vértices foram distribuídos como na imagem acima (pontos brancos), e os triângulos destacados (verde, azul, rosa).

A implementação dos tiros da nave não mudou com o projeto Asteroids, somente a velocidade e a quantidade de tiros criados foi alterada (de 2 para 1).

### Cor dos objetos

Para alterar a cor dos objetos, basta alterar o vetor de cores **m_color** (RGBA) correspondente ao objeto.

- Para a nave do jogador, basta alterar o **m_color** na função initializeGL da classe Ship.
- Para as naves inimigas, basta alterar o **m_color** na função createEnemy da classe Enemies.
- Para o tiro, basta alterar o **m_color** na função update da classe Bullets, ao criar um novo tiro.

### Contador de inimigos

Ao iniciar um novo jogo (execução da função restart da classe OpenGLWindow), a variável **m_gameData.m_enemies_left** é inicializada com a quantidade inicial de inimigos.

A cada vez que é detectada uma colisão de um tiro com um inimigo (função checkCollisions da classe OpenGLWindow), o contador é decrescido em 1.

Para efetivamente mostrar na tela o conteúdo dessa variável, foi criado um campo de texto na função paintUI da classe OpenGLWindow. Além disso, foi criada uma lógica para somente mostrar o campo quando o estado do jogo for "Playing", ou seja, só quando o jogo estiver sendo executado.

### Movimentação
Implementamos a movimentação da nave do jogador e das inimigas:

#### Nave do jogador
Foi restringido o movimento vertical. A cada vez que o jogador pressiona um dos botões de movimento lateral (A, D, <-, ->), a posição X da nave é alterada (função update da classe Ship). Além disso, restringimos o range para que a nave não saia da tela.

#### Naves Inimigas
As naves inimigas só se movimentam de cima para baixo, na mesma velocidade. Para que esse movimento aconteça, alteramos a posição Y de cada nave (função update da classe Enemies). Ainda, para que as naves sejam desenhadas com o mesmo espaçamento, foi criada uma lógica na função initializeGL da classe Enemies. A cada nova nave desenhada é acrescido um espaço entre ela e a próxima, além de dar um espaço vertical a cada 10 naves.

## Licença
MIT
