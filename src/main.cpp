#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void task1(void *args __attribute((unused))) {
	for (;;) {
		gpio_toggle(GPIOC, GPIO14);
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

static void task2(void *args __attribute((unused))) {
	for (;;) {
		gpio_toggle(GPIOC, GPIO13);
		vTaskDelay(pdMS_TO_TICKS(800));
	}
}

int main(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13 | GPIO14);

    xTaskCreate(task1,"LED1",100,NULL,configMAX_PRIORITIES-1,NULL);
	xTaskCreate(task2,"LED2",100,NULL,configMAX_PRIORITIES-1,NULL);
	vTaskStartScheduler();
	for (;;);

	return 0;
}