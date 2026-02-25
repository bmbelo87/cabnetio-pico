#include "device/usbd.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "piuio_structs.h"
#include "piuio_config.h"
#include "hardware/uart.h"
#include "pico/time.h"
#include "pico/stdlib.h"

#define UART_ID uart0
#define UART_TX_PIN 16
#define UART_RX_PIN 17
#define BAUD_RATE 115200

struct lampArray lamp;
uint8_t inputData[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void setup_uart() {
    uart_init(UART_ID, BAUD_RATE); // Configure UART
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // TX
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // RX
}

void setup_inputs() {
    for (int i = 0; i < 4; i++) {
        gpio_init(pinSwitch[i]);
        gpio_set_dir(pinSwitch[i], false);
        gpio_pull_up(pinSwitch[i]);
    }
}

void setup_outputs() {
    for (int i = 0; i < 4; i++) {
        gpio_init(pinLEDCabinet[i]);
        gpio_set_dir(pinLEDCabinet[i], true);
    }

    gpio_init(NEON_PIN);
    gpio_set_dir(NEON_PIN, true); // original

    // gpio_init(COIN_COUNTER);
    // gpio_set_dir(COIN_COUNTER, true);

}

// void led_on() {
//     gpio_init(25);
//     gpio_set_dir(25, GPIO_OUT);

//     // Liga o LED
//     gpio_put(25, 1);
// }

void blink_lights() {
    for (int i = 0; i < 4; i++) {
        gpio_put(pinLEDCabinet[i], 0);  
        gpio_put(pinLEDCabinet[i], 1);  
    }
        gpio_put(NEON_PIN, 0);
        gpio_put(NEON_PIN, 1);

    //     // Todas começam apagadas
    // for (int i = 0; i < 4; i++) {
    //     gpio_put(pinLEDCabinet[i], 1); // 1 = apagado (inversão de lógica)
    // }
    //     gpio_put(NEON_PIN, 0); // Apaga o neon também

    // // Repete duas vezes o ciclo
    //     for (int i = 0; i < 4; i++) {
    //         // Acende a luz atual
    //         gpio_put(pinLEDCabinet[i], 0);  
    //         sleep_ms(1000); // fica acesa por 1 segundos

    //         // Apaga a luz atual
    //         gpio_put(pinLEDCabinet[i], 1);
    //         sleep_ms(500); // pausa curta antes da próxima
    //     }
}

void update_lights_from_uart() {
    if (uart_is_readable(uart0)) {
        uint8_t cab_lights = uart_getc(uart0);

        for (int i = 0; i < 4; i++) {
            bool light_on = (cab_lights >> i) & 1;
            gpio_put(pinLEDCabinet[i], !light_on);  // Inverte a lógica aqui
        }

        bool neon_on = (cab_lights >> 4) & 1; // Bit 2 do segundo byte
        gpio_put(NEON_PIN, !neon_on); // ou !neon_on se inverter a lógica
    }
}

int main() {
    stdio_init_all();
    setup_uart();
    setup_inputs();
    setup_outputs();
    blink_lights();

    while (true) {

        bool service = !gpio_get(pinSwitch[0]);
        bool test = !gpio_get(pinSwitch[1]);
        bool coin = !gpio_get(pinSwitch[2]);
        bool clear = !gpio_get(pinSwitch[3]);

        // NÃO resetar tudo toda vez — só mudar se o botão está ou não pressionado
        if (coin)
            inputData[CABINET] &= ~(1 << 2); // Coin pressionado
        else
            inputData[CABINET] |= (1 << 2);  // Coin solto

        if (service)
            inputData[CABINET] &= ~(1 << 6);
        else
            inputData[CABINET] |= (1 << 6);

        if (test)
            inputData[CABINET] &= ~(1 << 1);
        else
            inputData[CABINET] |= (1 << 1);

        if (clear)
            inputData[CABINET] &= ~(1 << 7);
        else
            inputData[CABINET] |= (1 << 7);

        uart_write_blocking(UART_ID, &inputData[CABINET], 1);    

        update_lights_from_uart();
    }
}
