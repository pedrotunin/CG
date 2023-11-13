# Sistema Solar

## Link do Projeto

### [Clique aqui!](https://pedrotunin.github.io/CG/sistemasolar/)

## Integrantes

**Nome:** Pedro de Souza Tunin

**RA:** 11201811959

## Sobre

A aplicação é um modelo simplificado do sistema solar, onde é possível se movimentar entre os astros.

Foi utilizado como base o código do projeto LookAt, disponibilizado nas aulas.

## Movimentação

- Para se movimentar para frente e para trás utilize as teclas W e A ou as setas direcionais cima e baixo.
- Para se movimentar para os lados utilize as teclas Q e E.
- Para se movimentar para cima e para baixo utilize as teclas R e F.
- Para movimentar a câmera para os lados utilize as teclas A e D ou as setas direcionais esquerda e direita.

## Implementação
Foram implementadas as classes: window.cpp e camera.cpp

- **window.cpp:** Responsável por mostrar a aplicação na tela e desenhar os astros.
- **camera.cpp:** Responsável pela camera LookAt, com os métodos que possibilitam a movimentação da câmera.

### camera.cpp

A classe é quase idêntica ao projeto lookat. Foi adicionada a função para possibilitar a movimentação vertical. Segue o código:

```cpp
void Camera::climb(float speed) {
  // Compute up vector
  auto const up{glm::normalize(m_up)};

  // Move eye and center up (speed > 0) or down (speed < 0)
  m_eye += up * speed;
  m_at += up * speed;

  computeViewMatrix();
}
```

Para realizar a movimentação vertifical, primeiro obtemos o vetor correspondente a direção vertical (**up**). Em seguida, os vetores **m_eye** e **m_at** são acrescidos do vetor do movimento realizado (para cima ou para baixo), de acordo com o parâmetro **speed**. Se **speed** for maior do que 0, a câmera se move para cima, e se move para baixo caso **speed** for menor do que 0.

Ainda, a função Camera::computeProjectionMatrix foi alterada para permitir que a distância máxima de renderização fosse alterada de 5 unidades de distância para 25 unidades. Segue o código:

```cpp
void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projMatrix = glm::mat4(1.0f);
  auto const aspect{size.x / size.y};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 25.0f);
}
```

### window.cpp

Para realizar a movimentação vertifical, a função Window::onEvent() foi alterada para identificar quando as teclas R e F forem pressionadas ou liberadas.

```cpp
void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    
    ...

    if (event.key.keysym.sym == SDLK_r)
      m_climbSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_f)
      m_climbSpeed = -1.0f;
  }
  if (event.type == SDL_KEYUP) {

    ...

    if (event.key.keysym.sym == SDLK_r && m_climbSpeed > 0)
      m_climbSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_f && m_climbSpeed < 0)
      m_climbSpeed = 0.0f;
  }
}
```

Na função Window::onCreate() foi alterado o modelo 3D que será utilizado. Para representar um astro foi escolhido um modelo de uma esfera.

```cpp
// Load model
loadModelFromFile(assetsPath + "geosphere.obj");
```

Para desenhar os astros na tela utilizamos o seguinte trecho de código presente na função Window::onPaint. Segue código:

```cpp
// Draw Sun
glm::mat4 model{1.0f};
model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 0.0f));
model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
model = glm::scale(model, glm::vec3(0.8f));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);
```

O trecho foi repetido para que todos os astros fossem desenhados corretamente, alterando apenas a posição, escala e cor. O desenho de todos os astros vai da linha 187 até a linha 285 do arquivo window.cpp.

A função Window::onUpdate foi alterada de forma a inserir a atualização para quando ocorrer alguma movimentação vertical. Segue código:

```cpp
void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  m_camera.dolly(m_dollySpeed * deltaTime * 1.5f);
  m_camera.truck(m_truckSpeed * deltaTime * 1.5f);
  m_camera.pan(m_panSpeed * deltaTime * 1.5f);
  m_camera.climb(m_climbSpeed * deltaTime * 1.5f);
}
```

### lookat.vert

Para remover o efeito de intendidade de cor, a linha 15 do arquivo foi alterada para ser sempre igual a 1.

```cpp
float i = 1.0;
```

Dessa forma, a intensidade das cores sempre é máxima não importando a distância.

O código final ficou assim:

```cpp
#version 300 es

layout(location = 0) in vec3 inPosition;

uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec4 fragColor;

void main() {
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(inPosition, 1);

  float i = 1.0;
  fragColor = vec4(i, i, i, 1) * color;

  gl_Position = projMatrix * posEyeSpace;
}
```

## Licença

Este projeto está licenciado sob a licenca do MIT. Consulte o arquivo LICENSE para mais detalhes.