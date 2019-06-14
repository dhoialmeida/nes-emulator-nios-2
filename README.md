# Emulador de NES
Emulador de NES desenvolvido para Nios II.

## Executando o emulador
### Na placa
Para rodar o emulador na placa, o projeto `src/emuleibe.ncf` deve ser aberto, compilado e carregado.

Em seguida, um breakpoint deve ser adicionado no início da função `execute`, e o programa deve ser executado. No momento em que o breakpoint for atingido, o emulador terá exibido um endereço de memória no terminal. A ROM desejada deve ser carregada neste endereço. Em seguida, a execução deve ser retomada.

### No computador
É possível rodar uma versão de teste do emulador em um computador. Para isso, é necessário instalar o `gcc` e a biblioteca gráfica SDL 2. O programa deve ser compilado com o seguinte comando no terminal, dentro da pasta `src/`:

`gcc -std=c99 -Wall -I/usr/include/SDL2 -D_REENTRANT -g **.c -lSDL2 -lSDL2main -O3 -o emulador`

O log de instruções pode ser desabilitado, causando um aumento considerável de performance. Para isso, `-D DISABLE_LOG` deve ser adicionado ao final da linha anterior.

Após compilado, o emulador pode ser executado da seguinte com o seguinte comando:

`./emulador caminho_da_rom`

## Resultados
Devido ao tempo limitado, o emulador não suporta áudio ou entrada de teclado. O jogo Donkey Kong mostra a tela inicial e entra no modo de demonstração, embora alguns glitches gráficos ocorram.
