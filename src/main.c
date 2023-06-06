#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

// setting period of motor to 50htz
#define PERIOD (USEC_PER_SEC / 50U)
#define STEP 250
#define MINPULSE 1000
#define MAXPULSE 2000
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
servo servos[SERVO_NUM] = {{PWM_DT_SPEC_GET(DT_ALIAS(alpha)), MINPULSE, 1},
                           {PWM_DT_SPEC_GET(DT_ALIAS(beta)), MINPULSE, 1},
                           {PWM_DT_SPEC_GET(DT_ALIAS(gamma)), MINPULSE, 1},
                           {PWM_DT_SPEC_GET(DT_ALIAS(delta)), MINPULSE, 1},
                           {PWM_DT_SPEC_GET(DT_ALIAS(epsilon)), MINPULSE, 1},
                           {PWM_DT_SPEC_GET(DT_ALIAS(zeta)), MINPULSE, 1}};

void set_Servos(servo *servos) {
  printk("Settign Servos\n");
  for (int i = 0; i < SERVO_NUM; i++) {
    pwm_set(servos[i].name.dev, servos[i].name.channel, PWM_USEC(PERIOD),
            PWM_USEC(servos[i].pulse), 0);
  }
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
          servos[i].pulse += STEP;
        set_Servos(servos);
        break;
      case 's':
        for (int i = 0; i < 6; i++)
          servos[i].pulse -= STEP;
        set_Servos(servos);
        break;

      case 'a':
        for (int i = 0; i < 3; i++)
          servos[i].pulse += STEP;
        for (int i = 3; i < 6; i++)
          servos[i].pulse -= STEP;
        set_Servos(servos);
        break;

      case 'd':
        for (int i = 0; i < 3; i++)
          servos[i].pulse -= STEP;
        for (int i = 3; i < 6; i++)
          servos[i].pulse += STEP;
        set_Servos(servos);
        break;

      case 'q':
        break;
      case 'u':
        break;
      case 'j':
        break;
      case 'h':
        break;
      case 'k':
        break;
      case 'y':
        break;
      case 'i':
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
}
