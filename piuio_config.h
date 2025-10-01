#ifndef PIUIO_PICO_PIUIO_CONFIG_H
#define PIUIO_PICO_PIUIO_CONFIG_H

static const uint8_t pinSwitch[4] = {
        // Player 1
        0, // Service   (2)
        1, // Test      (3)
        2,  // Coin     (4) 
        4 // Clear      (5)
};

static const uint8_t pinLEDCabinet[4] = {
        18,      // Halo L1
        19,      // Halo L2 // R1?
        21,      // Halo R1
        22       // Halo R2
};

#define NEON_PIN 20

// #define COIN_COUNTER 3

#endif //PIUIO_PICO_PIUIO_CONFIG_H
