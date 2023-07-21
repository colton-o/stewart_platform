#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

// setting period of motor to 50htz
#define PERIOD (USEC_PER_SEC / 50U)
#define STEP 20
#define MINPULSE 500
#define MAXPULSE 2500
#define MIDPULSE 1500
#define SLEEP_TIME_S 1
#define SERVO_NUM 6
#define RECEIVE_TIMEOUT 100
#define RX_BUFFER_SIZE 1000

static uint8_t rx_buffer[RX_BUFFER_SIZE] = {0};
static uint8_t tx_buffer[] = "we doin uart \n\r";
const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

typedef struct {
  const struct pwm_dt_spec name;
  uint32_t pulse;
  int direction;
} servo;

servo servos[SERVO_NUM] = {{PWM_DT_SPEC_GET(DT_ALIAS(alpha)), MIDPULSE, 1},
                           {PWM_DT_SPEC_GET(DT_ALIAS(beta)), MIDPULSE, 1},
                           {PWM_DT_SPEC_GET(DT_ALIAS(gamma)), MIDPULSE, 1},
                           {PWM_DT_SPEC_GET(DT_ALIAS(delta)), MIDPULSE, 1},
                           {PWM_DT_SPEC_GET(DT_ALIAS(epsilon)), MIDPULSE, 1},
                           {PWM_DT_SPEC_GET(DT_ALIAS(zeta)), MIDPULSE, 1}};

static const struct gpio_dt_spec btn_01 =
    GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static const struct gpio_dt_spec btn_02 =
    GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
static const struct gpio_dt_spec joy_north =
    GPIO_DT_SPEC_GET(DT_ALIAS(north), gpios);

static const struct gpio_dt_spec joy_south =
    GPIO_DT_SPEC_GET(DT_ALIAS(south), gpios);

static const struct gpio_dt_spec joy_east =
    GPIO_DT_SPEC_GET(DT_ALIAS(east), gpios);

static const struct gpio_dt_spec joy_west =
    GPIO_DT_SPEC_GET(DT_ALIAS(west), gpios);

void set_Servos(servo *servos) {
  for (int i = 0; i < SERVO_NUM; i++) {
    if (servos[i].pulse > MAXPULSE)
      servos[i].pulse = MAXPULSE;
    else if (servos[i].pulse < MINPULSE)
      servos[i].pulse = MINPULSE;
    pwm_set(servos[i].name.dev, servos[i].name.channel, PWM_USEC(PERIOD),
            PWM_USEC(servos[i].pulse), 0);
    printk("%d = %d \n", i, servos[i].pulse);
  }
}

void up() { 
    for (int i = 0; i < 6; i++){
        if(servos[i].pulse != MIDPULSE){
            if(i%2 == 0)
                servos[i].pulse += STEP;
            else
                servos[i].pulse -= STEP;
        }
        set_Servos(servos);
    }
}
void down() {
  printk("south test \n");
  for (int i = 0; i < 6; i += 2){
    if (servos[i].pulse != MINPULSE)
        servos[i].pulse -= STEP/2;
  }
 
  for (int i = 1; i < 6; i += 2){
    if(servos[i].pulse != MAXPULSE)
        servos[i].pulse += STEP/2;
  }
  set_Servos(servos);
}
void north(){
    servos[1].pulse += STEP;
    servos[2].pulse += STEP;
    servos[3].pulse -= STEP;
    servos[4].pulse -= STEP;
    set_Servos(servos);
}
void south(){
    servos[1].pulse -= STEP;
    servos[2].pulse -= STEP;
    servos[3].pulse += STEP;
    servos[4].pulse += STEP;
    set_Servos(servos);
}
void east(){
    servos[0].pulse -= STEP;
    servos[5].pulse -= STEP;
    set_Servos(servos);
}
void west(){
    servos[0].pulse += STEP;
    servos[5].pulse += STEP;
    set_Servos(servos);
}
uint32_t angle_to_pulse(uint8_t angle) {
    float angle_pct = ((float)angle / 180.0f);
    uint32_t pulse = MINPULSE + (angle_pct * (MAXPULSE - MINPULSE));
    return pulse;
}

static void callback_uart(const struct device *dev, struct uart_event *evt,
                          void *user_data) {
    switch (evt->type) {
        case UART_RX_RDY:
            if ((evt->data.rx.len) == 1) {
                switch (evt->data.rx.buf[evt->data.rx.offset]) {
                    case 'w':
                        for (int i = 0; i < 6; i++)
                            servos[i].pulse = MIDPULSE;
                        set_Servos(servos);
                        break;
                    case 's':
                        for (int i = 0; i < 6; i += 2)
                            servos[i].pulse = MINPULSE;
                        for (int i = 1; i < 6; i += 2)
                            servos[i].pulse = MAXPULSE;
                        set_Servos(servos);
                        break;

                    case 'a':
                        servos[3].pulse += STEP;
                        servos[4].pulse += STEP;
                        set_Servos(servos);
                        break;

                    case 'd':
                        servos[3].pulse -= STEP;
                        servos[4].pulse -= STEP;
                        set_Servos(servos);
                        break;

                    case '1':
                        servos[0].pulse += STEP;
                        set_Servos(servos);
                        break;
                    case '2':
                        servos[1].pulse += STEP;
                        set_Servos(servos);
                        break;
                    case '3':
                        servos[2].pulse += STEP;
                        set_Servos(servos);
                        break;
                    case '4':
                        servos[3].pulse += STEP;
                        set_Servos(servos);
                        break;
                    case '5':
                        servos[4].pulse += STEP;
                        set_Servos(servos);
                        break;
                    case '6':
                        servos[5].pulse += STEP;
                        set_Servos(servos);
                        break;

                    case '0':
                        for (int i = 0; i < 6; i++)
                            servos[i].pulse = MINPULSE;
                        set_Servos(servos);
                        break;
                    case '9':
                        for (int i = 0; i < 6; i++)
                            servos[i].pulse = MINPULSE;
                        set_Servos(servos);
                        break;
                    case '8':
                        for (int i = 0; i < 6; i++)
                            servos[i].pulse = MIDPULSE;
                        set_Servos(servos);
                        break;
                }
                printk("%d\n", evt->data.rx.buf[evt->data.rx.offset]);
            }
            break;
        case UART_RX_DISABLED:
            uart_rx_enable(dev, rx_buffer, sizeof rx_buffer, RECEIVE_TIMEOUT);
            break;
        default:
            break;
    }
}

void main(void) {
    printk("lets begin \n");
    set_Servos(servos);
    uart_callback_set(uart, callback_uart, NULL);
    uart_tx(uart, tx_buffer, sizeof(tx_buffer), SYS_FOREVER_MS);
    uart_rx_enable(uart, rx_buffer, sizeof(rx_buffer), RECEIVE_TIMEOUT);

    if (!device_is_ready(uart))
        printk("no uart device");
    else
        printk("we got uart");

    gpio_pin_configure_dt(&joy_north, GPIO_INPUT); 
    gpio_pin_configure_dt(&joy_south, GPIO_INPUT);
    gpio_pin_configure_dt(&joy_east, GPIO_INPUT);
    gpio_pin_configure_dt(&joy_west, GPIO_INPUT);
    gpio_pin_configure_dt(&btn_01, GPIO_INPUT);
    gpio_pin_configure_dt(&btn_02, GPIO_INPUT);

    while (1){
        if(gpio_pin_get_dt(&joy_north)){
            if(gpio_pin_get_dt(&btn_01))
                up();
            else
                north();
        }

        if(gpio_pin_get_dt(&joy_south)){
            if(gpio_pin_get_dt(&btn_01))
                down();
            else
                south();
        }
        if(gpio_pin_get_dt(&joy_west)){
            west();
            printk("west");
        }
        if(gpio_pin_get_dt(&joy_east)){
            east();
        }
    }
}



