# C-speed
Jogo em C
Collecting workspace informationClaro! Aqui está um passo a passo para rodar o jogo descrito em jogo.c:

## Passo a Passo para Rodar o Jogo

1. **Pré-requisitos**
   - Instale o [Raylib](https://www.raylib.com/) (biblioteca gráfica usada pelo jogo).
   - Tenha um compilador C instalado (como GCC).

2. **Baixe ou crie as imagens**
   - Crie uma pasta chamada `assets` na mesma pasta do arquivo jogo.c.
   - Coloque dentro dela as imagens:
     - `player.png`
     - `obstaculo.png`
     - `arvore.png`
   - As imagens devem ter qualquer tamanho, mas recomenda-se 32x32 pixels.

3. **Compile o jogo**
   - No terminal, navegue até a pasta do projeto.
   - Compile usando:
     ```sh
     gcc jogo.c -o jogo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
     ```
     > No Windows, pode ser necessário ajustar as bibliotecas, por exemplo:

https://github.com/user-attachments/assets/9bf8d1ea-0daa-4eb4-8b01-c61814bbfed5


     ```sh
     gcc jogo.c -o jogo.exe -lraylib -lopengl32 -lgdi32 -lwinmm
     ```

4. **Execute o jogo**
   - No terminal, rode:
     ```sh
     ./jogo
     ```
     ou, no Windows:
     ```sh
     jogo.exe
     ```

5. **Jogue!**
   - Siga as instruções na tela para jogar.

---

Se precisar de mais detalhes sobre a instalação do Raylib, consulte a [documentação oficial](https://github.com/raysan5/raylib/wiki/Working-on-Windows).

Veja o código do jogo em jogo.c.
Collecting workspace informationAqui está o passo a passo para rodar o jogo em Linux:

## Passo a passo para rodar o C-speed no Linux

1. **Instale as dependências**
   - Instale o Raylib e o compilador GCC:
     ```sh
     sudo apt update
     sudo apt install gcc raylib-dev
     ```

2. **Crie a pasta de assets**
   - Crie uma pasta chamada `assets` na raiz do projeto.
   - Coloque dentro dela as imagens:
     - `player.png`
     - `obstaculo.png`
     - `arvore.png`

3. **Compile o jogo**
   - No terminal, dentro da pasta do projeto, execute:
     ```sh
     gcc jogo.c -o jogo -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
     ```

4. **Execute o jogo**
   - No terminal:
     ```sh
     ./jogo
     ```

Veja o código principal em jogo.c.
