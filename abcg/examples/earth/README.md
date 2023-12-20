# Earth

## Link do Projeto

### [Clique aqui!](https://pedrotunin.github.io/CG/earth/)

## Integrantes

**Nome:** Pedro de Souza Tunin

**RA:** 11201811959

## Sobre

A aplicação apresenta um modelo do planeta Terra com iluminação e textura.

Foi utilizado como base o código dos projetos viewer.

## Trackball

- Para movimentar o trackball utilize o mouse clicando com o botão esquerdo do mouse e arrastando.
- Também é possivel aumentar ou diminuir a velocidade de rotação utilizando as teclas A, D, <- e ->


Para isso funcionar foram adicionas as seguintes linhas na função **Window::onEvent**:
```cpp
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_trackBallModel.setVelocity(m_trackBallModel.getVelocity() - 0.01f);
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_trackBallModel.setVelocity(m_trackBallModel.getVelocity() + 0.01f);
  }
```

Além disso, uma nova função foi criada na classe **TrackBall**:
```cpp
float TrackBall::getVelocity() { 
  return m_velocity; 
}
```

## Terra

Foi utilizado um modelo de uma esfera obtido na internet, juntamente com seu arquivo .mtl correspondente.

Para deixar a rotação parecida com a da Terra, o eixo inicial do trackball foi ajustado para as coordenadas {0, 1, 0}, com o seguinte trecho de codigo da função **Window::onCreate**:

```cpp
// Initial trackball spin
m_trackBallModel.setAxis(glm::normalize(glm::vec3(0, 1, 0)));
m_trackBallModel.setVelocity(0.1f);
```

Dessa forma, o planeta roda ao longo do seu eixo Y.

## Espaço

Para dar o aspecto de que o planeta está no espaço, foi implementado um skybox semelhante ao do projeto viewer6. Para os 6 lados do cubo, foi utilizada a mesma imagem. Para obter a maior qualidade possivel, utilizei uma imagem de 2048x2048 pixels.

## Textura e iluminação

Para a textura, quase nada foi alterado dos projetos viewer, apenas o mapping mode e a textura em si. O mapping mode foi definido como esférico, já  que o modelo se trata de uma esfera, servindo perfeitamente para o proposito.

Retirei a escolha do mapping mode da interface e o coloquei na definição da classe **Window**:
```cpp
// Mapping mode
// 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
int m_mappingMode{2};
```

Em relação à iluminação, a implementação não mudou muito dos projetos viewer, apenas os coeficientes de iluminação e materias mudaram para melhor atender as necessidades do projeto.

Os valores foram obtidos empiricamente por meio de testes. Segue trecho na definição da classe **Window**:

```cpp
// Light and material properties
glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
glm::vec4 m_Ia{1.0f};
glm::vec4 m_Id{1.0f};
glm::vec4 m_Is{1.0f};
glm::vec4 m_Ka{0.101f, 0.101f, 0.101f, 1.0f};
glm::vec4 m_Kd{0.722f, 0.722f, 0.722f, 1.0f};
glm::vec4 m_Ks{0.624f, 0.624f, 0.624f, 1.0f};
float m_shininess{10.0f};
```

## Interface

Para deixar o projeto final mais apresentável, todas as interfaces foram retiradas e a janela foi definida para ocupar toda a tela.

## Licença

Este projeto está licenciado sob a licenca do MIT. Consulte o arquivo LICENSE para mais detalhes.