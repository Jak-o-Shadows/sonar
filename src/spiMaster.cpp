#include "spiMaster.hpp"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>


void SpiMaster::setup(){

    // Using SPI2 on:
    //  * MISO: PB14
    //  * MOSI: PB15
    //  * SCK: PB13
    
    // Choosing to use chip select:
    //  PB12 (This is also NSS - so will work nicer)

    // Clocks
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_SPI2);
    rcc_periph_clock_enable(RCC_AFIO);

    // Pin Config
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO12 | GPIO13 | GPIO15);
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT,
                  GPIO_CNF_INPUT_FLOAT, GPIO14);

    // Setup SPI2
    spi_reset(SPI2);

    /* Set up SPI in Master mode with:
    * Clock baud rate: 1/64 of peripheral clock frequency
    * Clock polarity: Idle High
    * Clock phase: Data valid on 2nd clock pulse
    * Data frame format: 8-bit
    * Frame format: MSB First
    */
    spi_init_master(SPI2, SPI_CR1_BAUDRATE_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

    /*
    * Set NSS management to software.
    *
    * Note:
    * Setting nss high is very important, even if we are controlling the GPIO
    * ourselves this bit needs to be at least set to 1, otherwise the spi
    * peripheral will not send any data out.
    */
    spi_enable_software_slave_management(SPI2);
    spi_set_nss_high(SPI2);

    /* Enable SPI1 periph. */
    spi_enable(SPI2);

}

void SpiMaster::transfer_bytes(uint16_t *data, uint16_t numHalfWords){
    while( numHalfWords-- ){
        spi_send(SPI2, *data);
        data++;
        spi_read(SPI2);
    }
}
