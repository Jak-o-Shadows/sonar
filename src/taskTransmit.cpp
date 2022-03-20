#include "taskTransmit.hpp"

#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>



void TransmitTask::setup() {


    //////////// rcc Setup /////////////////////
	// Set AHB prescaler and APB prescaler
	//	Set no division, to give the most flexibility
	//	in that the timer can have a prescaler
	rcc_set_hpre(0);//RCC_CFGR_HPRE_NODIV);
	rcc_set_ppre1(0);//RCC_CFGR_PPRE_NODIV);

    rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_TIM2);


    //////////// GPIO Setup /////////////////////
    //  TIM2_CH2 = PA1
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO1);


    //////////// Timer Setup /////////////////////

	// Setup PWM timer for excitation frequency
    //  Want 40 kHz
    //  36 MHz -> 40 kHz = 900 clock ticks
    //  Can just do this with a prescaler of 1, and a PWM period of 900
	const uint8_t pwm_prescaler = 1;
	const uint16_t pwm_period = 900;
    const uint16_t pwm_width = 50;  // Short pulses only

	// Configure TIM 2, with channels 1, 2, 3 for PWM 1, 2, 3
	//	This does the pulse widths for each channel
	timer_disable_counter(TIM2);
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

	// ???????????
	timer_disable_preload(TIM2);
	// Set prescaler
	timer_set_prescaler(TIM2, pwm_prescaler);
	timer_set_period(TIM2, pwm_period);  // TODO: Determine why this doesn't quite work (set to 190k)
	// Enable continuous mode for repeat
	timer_continuous_mode(TIM2);

	// Configure the channels
	//	Put into PWM mode
	timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
	// Enable auto-preload
	timer_enable_oc_preload(TIM2, TIM_OC2);
	//	Put fast mode to speed up transitions
	timer_set_oc_fast_mode(TIM2, TIM_OC2);
	// Set specific OC values for duration
	timer_set_oc_value(TIM2, TIM_OC2, pwm_width);
	// Set it to actually do it
	timer_enable_oc_output(TIM2, TIM_OC2);

	// Start Both PWM channels
	timer_generate_event(TIM2, TIM_EGR_UG);
	timer_enable_counter(TIM2);
}

void TransmitTask::loop() {
    vTaskSuspend(NULL);  // Yield forever
}
