#include "hardware.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencmsis/core_cm3.h>

void uart_servos_setup(void) {
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


#define UART_STRATO USART3
#define RCC_UART_STRATO  RCC_USART3
void uart_strato_setup() {
    // This then enables the clock to the USART1 peripheral which is
    // attached inside the chip to the APB1 bus. Different peripherals
    // attach to different buses, and even some UARTS are attached to
    // APB1 and some to APB2, again the data sheet is useful here.
    rcc_periph_clock_enable(RCC_UART_STRATO);
    // MUST enable the GPIO clock in ADDITION to the USART clock
    rcc_periph_clock_enable(RCC_GPIOC);

    // On utilise PC10 pour Tx, PC11 pour Rx.
    gpio_mode_setup(GPIOC,
        GPIO_MODE_AF,
        GPIO_PUPD_NONE,
        GPIO10 | GPIO11);

    // Actual Alternate function number (in this case 2) is part
    // depenedent, CHECK THE DATA SHEET for the right number to use.
    gpio_set_af(GPIOC, GPIO_AF7, GPIO10 | GPIO11);

    // Set up USART/UART parameters using the libopencm3 helper functions
    usart_set_baudrate(UART_STRATO, 115200);
    usart_set_databits(UART_STRATO, 8);
    usart_set_stopbits(UART_STRATO, USART_STOPBITS_1);
    usart_set_mode(UART_STRATO, USART_MODE_TX);
    usart_set_parity(UART_STRATO, USART_PARITY_NONE);
    usart_set_flow_control(UART_STRATO, USART_FLOWCONTROL_NONE);
    usart_enable(UART_STRATO);

    // Enable interrupts from the USART
    nvic_enable_irq(NVIC_USART3_IRQ);

    // Specifically enable recieve interrupts
    usart_enable_rx_interrupt(UART_STRATO);
}

int UART_getc(unsigned char *c)
{}
void UART_send_message(char *buff, unsigned int buff_len) {
    console_puts(UART_STRATO, buff, buff_len);
}


static void gpio_setup(void)
{
    // Set GPIO12-15 (in GPIO port D) to 'output push-pull' for leds.
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT,
            GPIO_PUPD_NONE, GPIO12 | GPIO13 | GPIO14 | GPIO15);

    // Set GPIOE9, E11 for motors
    gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT,
            GPIO_PUPD_NONE, GPIO9 | GPIO11);

    gpio_clear(GPIOE, GPIO9 | GPIO11);

    // Setup GPIO pins for UART_STRATO transmit.
    // gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO2);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO2);

    // Setup UART_STRATO TX pin as alternate function.
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}

void init_PWM_mot_g() {
    gpio_mode_setup(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0 | GPIO2);
    gpio_set_output_options(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO0 | GPIO2);
    gpio_set_af(GPIOE, GPIO_AF1, GPIO0 | GPIO2);

    timer_reset(RCC_TIM1);
}

void init_hardware()
{
    clock_setup();
    uart_servos_setup();
    uart_strato_setup();
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
