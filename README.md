# Projeto de Interrupções e Controle de LEDs com a Raspberry Pi Pico W 2040

O projeto foi desenvolvido no contexto da **Unidade 4 do Capítulo 4 do EmbarcaTech (TIC37)**, Programa de Residência Tecnológica.

## Descrição

Este projeto utiliza a **Raspberry Pi Pico W 2040** para demonstrar o uso de interrupções, controle de LEDs e manipulação de uma matriz de LEDs endereçáveis WS2812. Ele é composto por dois botões que controlam a exibição de números de 0 a 9 em uma matriz de LEDs, um LED RGB que pisca a 5 Hz, e o tratamento de interrupções com debouncing via software. 

### Funcionalidades

- **LED Vermelho**: Pisca a uma taxa de 5 Hz.
- **Botão A**: Incrementa o número exibido na matriz de LEDs (0-9).
- **Botão B**: Decrementa o número exibido na matriz de LEDs (0-9).
- **Matriz de LEDs WS2812**: Exibe números de 0 a 9 em formato digital.
- **Debouncing de Botões**: Interrupções com tratamento de debouncing para evitar múltiplos disparos.

### Objetivos do Projeto

- Compreender e implementar interrupções em microcontroladores.
- Tratar o fenômeno de bouncing nos botões usando software.
- Controlar LEDs RGB e LEDs WS2812.
- Utilizar resistores de pull-up internos em botões de acionamento.

---

## Como Clonar e Testar o Código

### Pré-Requisitos

- **Raspberry Pi Pico W 2040** (ou equivalente)
- **VSCode** com extensões para desenvolvimento com a Raspberry Pi Pico
  - **C/C++ Extension** (para suporte a C e C++)
  - **Pico SDK** e **Pico Toolchain** configurados
- **Wokwi** (opcional para simulação online do código)

### Dependências

1. **Pico SDK**: Para compilar o código C para a Raspberry Pi Pico W 2040.
2. **Bibliotecas de Hardware**:
   - `pico/stdlib.h`: Para funções padrão.
   - `hardware/gpio.h`: Para manipulação de GPIOs.
   - `hardware/pio.h`: Para controle de pinos de entrada/saída.
   - `ws2812.pio.h`: Para controle do LED endereçável WS2812.

### Passo a Passo para Clonar e Rodar o Código

1. **Clonar o Repositório**:
   Abra o terminal e clone o repositório com o seguinte comando:

   ```bash
   git clone https://github.com/WeltonAM/u4_c4_t1_interruptores_leds_WS2812.git
   cd u4_c4_t1_interruptores_leds_WS2812

2. **Compilar e Rodar o Código no Pico W 2040**:
   - Abra o arquivo `ws2812.c` no VSCode.
   - Ajuste a intensidade dos LEDs da matriz conforme desejado até 255 (Sugestão de % 10 para usar direto na placa).
   - Configure o ambiente de desenvolvimento para a Raspberry Pi Pico W 2040 usando o Pico SDK e Pico Toolchain.
   - Garanta que a placa esteja em modo BOOTSEL.
   - Compile o código C para a Raspberry Pi Pico W 2040.
   - Execute o código C no Pico W 2040.
   - O código será compilado e enviado para o Pico W 2040.

### Vídeo de Demonstração

[![Vídeo de Demonstração](https://img.shields.io/badge/Assistir-Vídeo-blue)](https://drive.google.com/file/d/18xzR_nOx4u2zyOtpwkDaSLpqa9nVlOmI/view?usp=sharing)
