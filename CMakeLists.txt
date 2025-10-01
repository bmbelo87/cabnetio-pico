cmake_minimum_required(VERSION 3.13)

set(PICO_SDK_FETCH_FROM_GIT ON)
include(pico_sdk_import.cmake)

project(cabinetio_pico)

pico_sdk_init()

add_executable(cabinetio_pico
    main.c
    )

target_sources(cabinetio_pico PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        )

# In addition to pico_stdlib required for common PicoSDK functionality, add dependency on tinyusb_device
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(cabinetio_pico
                      PUBLIC
                      pico_stdlib
                      pico_multicore
                      hardware_pio
                      hardware_dma
                      hardware_irq
                      hardware_uart)

# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
#target_compile_definitions(piuio_pico PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)

pico_add_extra_outputs(cabinetio_pico)
pico_enable_stdio_usb(cabinetio_pico 1)
pico_enable_stdio_uart(cabinetio_pico 1)
