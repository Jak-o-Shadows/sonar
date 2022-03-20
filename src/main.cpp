#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "taskBlink.hpp"
#include "taskTransmit.hpp"

extern "C" void* __dso_handle = (void*) &__dso_handle;  // Magic to do with destructors (which we shall try to avoid in embedded) // TOOD: Move to different file


HeartbeatTask heartbeat;
TransmitTask transmit;

static void task2(void *args __attribute((unused))) {
	for (;;) {
		gpio_toggle(GPIOC, GPIO14);
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

int main(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO14);

	xTaskCreate(task2,"LED2",100,NULL,configMAX_PRIORITIES-1,NULL);
    heartbeat.start("heartbeat", configMINIMAL_STACK_SIZE, 1);
    transmit.start("transmit", configMINIMAL_STACK_SIZE, 2);
	vTaskStartScheduler();
	for (;;);

	return 0;
}