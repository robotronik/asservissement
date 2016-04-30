#include "hardware.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencmsis/core_cm3.h>


void clock_setup(void)
{
    // TODO on en a besoin ?
    //rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    /* Enable GPIOD clock for LED & USARTs. */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOE);

    /* Enable clocks for USART2. */
    rcc_periph_clock_enable(RCC_USART2);
}

void usart_setup(void)
{
    /* Setup USART2 parameters. */
    usart_set_baudrate(USART2, 1000000);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

    /* half duplex cuz wooo */
    USART_CR3(USART2) |= USART_CR3_HDSEL;
    USART_CR2(USART2) &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
    USART_CR3(USART2) &= ~(USART_CR3_SCEN | USART_CR3_IREN);

    /* Finally enable the USART. */
    usart_enable(USART2);
}

static void gpio_setup(void)
{
    /* Set GPIO12-15 (in GPIO port D) to 'output push-pull' for leds. */
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT,
            GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    // Set GPIOE9, E11 for motors
    gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT,
            GPIO_PUPD_NONE, GPIO9 | GPIO11);

    gpio_clear(GPIOE, GPIO9 | GPIO11);

    /* Setup GPIO pins for USART2 transmit. */
    /* gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO2); */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO2);

    /* Setup USART2 TX pin as alternate function. */
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}

void init_PWM_mot_g() {
}

void init_hardware()
{
    clock_setup();
    usart_setup();
     gpio_setup();
}

void set_PWM_moteur_D(int PWM)
{
    (void) PWM;
}

void set_PWM_moteur_G(int PWM)
{
    (void) PWM;
}

long int get_nbr_tick_D()
{
    return 0;
}

long int get_nbr_tick_G()
{
    return 0;
}

void attente_synchro()
{
}

void motors_stop()
{
    // Attention, on ne redémarre pas après !
    // Mise en haute impédance des sorties moteurs
    gpio_set(GPIOE, GPIO9 | GPIO11);
}

void motors_on() {
    gpio_clear(GPIOE, GPIO9 | GPIO11);

}


void allumer_del()
{
    gpio_set(GPIOD, GPIO12 | GPIO14);
}

void eteindre_del()
{
    gpio_clear(GPIOD, GPIO12 | GPIO14);
}

//fonction pour compatibilité avec la simulation sur PC
int arret()
{
    return 0;
}
