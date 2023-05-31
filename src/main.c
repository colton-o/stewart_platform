#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

// setting period of motor to 50htz
#define PERIOD (USEC_PER_SEC / 50U)
#define STEP 100
#define MINPULSE 1000
#define MAXPULSE 2000
#define MIDPULSE 2000
#define SLEEP_TIME_S 1
#define SERVO_NUM 6

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
  printk("Settign Servos");
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

void main(void) {
  int step = 500;
  int dir = 1;
  printk("lets begin \n");
  set_Servos(servos);
  while (1) {
    printk("PWM device cycle\n");
    if (servos[0].pulse == MAXPULSE)
      dir = -1;
    if (servos[0].pulse == MINPULSE)
      dir = 1;

    for (int i = 0; i < 6; i++) {
      servos[i].pulse += step * dir;
    }

    k_sleep(K_SECONDS(1));
    set_Servos(servos);
  }
}
