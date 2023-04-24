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
#define MIDPULSE 1500
#define SLEEP_TIME_S 1
#define SERVO_NUM 6

typedef struct {
  const struct pwm_dt_spec name;
  uint32_t pulse;
} servo;

servo servos[SERVO_NUM] = {{PWM_DT_SPEC_GET(DT_ALIAS(alpha)), MINPULSE},
                           {PWM_DT_SPEC_GET(DT_ALIAS(beta)), MINPULSE},
                           {PWM_DT_SPEC_GET(DT_ALIAS(gamma)), MINPULSE},
                           {PWM_DT_SPEC_GET(DT_ALIAS(delta)), MINPULSE},
                           {PWM_DT_SPEC_GET(DT_ALIAS(epsilon)), MINPULSE},
                           {PWM_DT_SPEC_GET(DT_ALIAS(zeta)), MINPULSE}};

uint32_t angle_to_pulse(uint8_t angle) {
  float angle_pct = ((float)angle / 90.0f);
  uint32_t pulse = MINPULSE + (angle_pct * (MAXPULSE - MINPULSE));
  return pulse;
}

void main(void) {
  printk("lets begin \n");
  while (1) {
    printk("PWM device cycle\n");
    for (int i = 0; i < SERVO_NUM; i++) {
      int pulse = angle_to_pulse(40 + (i * 10));
      servos[i].pulse = pulse;
      pwm_set(servos[i].name.dev, servos[i].name.channel, PWM_USEC(PERIOD),
              servos[i].pulse, 0);
    }
    k_sleep(K_SECONDS(SLEEP_TIME_S));
  }
}
