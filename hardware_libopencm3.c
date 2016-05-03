#include "hardware.h"
#include <debug.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencmsis/core_cm3.h>

static void uart_servos_setup(void) {
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


/*******************************************************************************
*                           UART
* *****************************************************************************/

#define UART_STRATO USART3
#define RCC_UART_STRATO  RCC_USART3

static void uart_strato_setup() {
    rcc_periph_clock_enable(RCC_UART_STRATO);
    rcc_periph_clock_enable(RCC_GPIOC);

    // On utilise PC10 pour Tx, PC11 pour Rx.
    gpio_mode_setup(GPIOC,
        GPIO_MODE_AF,
        GPIO_PUPD_NONE,
        GPIO10 | GPIO11);

    gpio_set_af(GPIOC, GPIO_AF7, GPIO10 | GPIO11);

    // Set up USART/UART parameters using the libopencm3 helper functions
    usart_set_baudrate(UART_STRATO, 115200);
    usart_set_databits(UART_STRATO, 8);
    usart_set_stopbits(UART_STRATO, USART_STOPBITS_1);
    usart_set_mode(UART_STRATO, USART_MODE_TX_RX);
    usart_set_parity(UART_STRATO, USART_PARITY_NONE);
    usart_set_flow_control(UART_STRATO, USART_FLOWCONTROL_NONE);
    usart_enable(UART_STRATO);

    // Enable interrupts from the USART
    nvic_enable_irq(NVIC_USART3_IRQ);

    // Specifically enable recieve interrupts
    usart_enable_rx_interrupt(UART_STRATO);
}

#define RxBufferSize 128    // Arbitrary buffer size
char RxBuffer[RxBufferSize];
volatile int RxBufferWrite; // Next place to store
volatile int RxBufferRead;  // Next place to read

void usart3_isr(void) {
    uint32_t    reg;
    int         i;
    do {
        reg = USART_SR(UART_STRATO);
        if (reg & USART_SR_RXNE) {
            RxBuffer[RxBufferWrite] = USART_DR(UART_STRATO);
            // Check for "overrun"
            i = (RxBufferWrite + 1) % RxBufferSize;
            if (i != RxBufferRead) {
                RxBufferWrite = i;
            }
        }
    } while ((reg & USART_SR_RXNE) != 0); // can read back-to-back interrupts
}

int UART_getc(unsigned char *c) {
    if (RxBufferRead != RxBufferWrite) {
        *c = RxBuffer[RxBufferRead];
        RxBufferRead = (RxBufferRead + 1) % RxBufferSize;
        return true;
    }
    return false;
}
void UART_send_message(char *buff, unsigned int buff_len) {
    console_puts(UART_STRATO, buff, buff_len);
}


/*******************************************************************************
*                           General purpose
* *****************************************************************************/


static void gpio_setup(void) {
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





/*******************************************************************************
*                           Encodeurs
* *****************************************************************************/
#define ENCODER_MAX (1<<16)

static unsigned int codeur_gauche_valeur;
static unsigned int codeur_droite_valeur;

static void init_timer_encodeur(unsigned int timer, unsigned int rcc, unsigned int nvic) {
    rcc_periph_clock_enable(rcc);
    timer_reset(timer);
    timer_set_period(timer, ENCODER_MAX-1);
    timer_set_counter(timer, ENCODER_MAX/2);
    // set encoder mode
    // x4 mode
    timer_slave_set_mode(timer, TIM_SMCR_SMS_EM3);
    timer_set_prescaler(timer, 0); // 0+1 = /1
    timer_ic_set_input(timer, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_input(timer, TIM_IC2, TIM_IC_IN_TI2);
    // enable counter
    timer_enable_counter(timer);
    // enable interrupts
    nvic_enable_irq(nvic);
    timer_enable_irq(timer, TIM_DIER_UIE);
}

static void init_timers_gpio() {
    // Gauche
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO1 | GPIO15);
    gpio_set_af(GPIOA, GPIO_AF1, GPIO1 | GPIO15);

    // Droite
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6);
    gpio_set_af(GPIOC, GPIO_AF2, GPIO6);
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5);
    gpio_set_af(GPIOB, GPIO_AF2, GPIO5);
}

static void init_encodeurs() {
    // Gauche
    init_timer_encodeur(TIM2, RCC_TIM2, NVIC_TIM2_IRQ);
    // Droite
    init_timer_encodeur(TIM3, RCC_TIM3, NVIC_TIM3_IRQ);

    // Les GPIOs
    init_timers_gpio();

    codeur_gauche_valeur = ENCODER_MAX/2;
    codeur_droite_valeur = ENCODER_MAX/2;
}

long int get_nbr_tick_G() {
    unsigned int current_value = timer_get_counter(TIM2);
    int delta_ticks = current_value - codeur_gauche_valeur;
    codeur_gauche_valeur = current_value;

    // check for overflow
    if (delta_ticks >  ENCODER_MAX/2)
        delta_ticks -= ENCODER_MAX;

    // underflow
    if (delta_ticks < -ENCODER_MAX/2)
        delta_ticks += ENCODER_MAX;

    return delta_ticks % ENCODER_MAX/2;
}

long int get_nbr_tick_D() {
    unsigned int current_value = timer_get_counter(TIM3);
    int delta_ticks = current_value - codeur_droite_valeur;
    codeur_droite_valeur = current_value;

    // check for overflow
    if (delta_ticks >  ENCODER_MAX/2)
        delta_ticks -= ENCODER_MAX;

    // underflow
    if (delta_ticks < -ENCODER_MAX/2)
        delta_ticks += ENCODER_MAX;

    return delta_ticks % ENCODER_MAX/2;
}

void tim2_isr() {
  timer_clear_flag(TIM2, TIM_DIER_UIE);
  debug(_ERROR_, "tim2 : overflow !\n\r");
}

void tim3_isr() {
  timer_clear_flag(TIM3, TIM_DIER_UIE);
  debug(_ERROR_, "tim3 : overflow !\n\r");
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
    init_alarms_and_delay();
    uart_servos_setup();
    uart_strato_setup();
    gpio_setup();
    init_encodeurs();
}

void set_PWM_moteur_D(int PWM)
{
    (void) PWM;
}

void set_PWM_moteur_G(int PWM)
{
    (void) PWM;
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

void allumer_autres_del()
{
    gpio_set(GPIOD, GPIO13 | GPIO15);
}

void eteindre_autres_del()
{
    gpio_clear(GPIOD, GPIO13 | GPIO15);
}
void toggle_autres_del()
{
    gpio_toggle(GPIOD, GPIO13 | GPIO15);
}

//fonction pour compatibilité avec la simulation sur PC
int arret()
{
    return 0;
}
