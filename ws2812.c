#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define LED_RED 13
#define LED_BLUE 11

#define BUTTON_A 5
#define BUTTON_B 6

#define WS2812_PIN 7
#define NUM_PIXELS 25

#define DEBOUNCE_TIME 200000 // 200ms de debounce

static volatile uint32_t last_time_A = 0;
static volatile uint32_t last_time_B = 0;
static volatile int num_displayed = 0;

PIO pio = pio0;
int sm = 0;
uint32_t led_buffer[NUM_PIXELS] = {0};

// Representações dos números de 0 a 9 na matriz 5x5
int num_map[10][NUM_PIXELS] = {
    {0, 1, 1, 1, 0, // 0
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0},

    {0, 1, 1, 1, 0, // 1
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 1, 1, 0, 0,
     0, 0, 1, 0, 0},

    {0, 1, 1, 1, 0, // 2
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0},

    {0, 1, 1, 1, 0, // 3
     0, 0, 0, 1, 0,
     0, 1, 1, 0, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0},

    {0, 1, 0, 0, 0, // 4
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0},

    {0, 1, 1, 1, 0, // 5
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0},

    {0, 1, 1, 1, 0, // 6
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0},

    {0, 1, 0, 0, 0, // 7
     0, 0, 0, 1, 0,
     0, 1, 0, 0, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0},

    {0, 1, 1, 1, 0, // 8
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0},

    {0, 1, 1, 1, 0, // 9
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0}};

void initPin()
{
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
}

// Envia pixel para os leds
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
}

// Converter cor RGB para formato WS2812 (GRB)
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Atualizar os LEDs da matriz
void set_leds_from_buffer()
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        put_pixel(led_buffer[i]);
    }
}

// Exibir número na matriz na cor azul
void display_number(int num)
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        led_buffer[i] = (num_map[num][i] == 1) ? urgb_u32(0, 0, 255) : urgb_u32(0, 0, 0);
    }
    set_leds_from_buffer();
}

// Handler de interrupção dos botões
void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (gpio == BUTTON_A && current_time - last_time_A > DEBOUNCE_TIME)
    {
        last_time_A = current_time;
        num_displayed = (num_displayed + 1) % 10;
    }
    if (gpio == BUTTON_B && current_time - last_time_B > DEBOUNCE_TIME)
    {
        last_time_B = current_time;
        num_displayed = (num_displayed - 1 + 10) % 10;
    }
    display_number(num_displayed);
}

int main()
{
    stdio_init_all();

    initPin();
    
    // Carrega o PIO para controlar os leds WS2812
    uint offset = pio_add_program(pio, &ws2812_program);
    // Inicialização do state machine para uso dos leds
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);

    // Disparo de interrupções para os botões (transição de alto para baixo) de forma mais rápida
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (1)
    {
        gpio_put(LED_RED, true); // Liga led vermelho
        sleep_ms(100);
        gpio_put(LED_RED, false);
        sleep_ms(100);
    }
    return 0;
}
