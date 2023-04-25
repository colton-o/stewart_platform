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

void set_Servos(servo *servos) {
  printk("Settign Servos");
  for (int i = 0; i < SERVO_NUM; i++) {
    pwm_set(servos[i].name.dev, servos[i].name.channel, PWM_USEC(PERIOD),
            PWM_USEC(servos[i].pulse), 0);
  }
}
uint32_t angle_to_pulse(uint8_t angle) {
  float angle_pct = ((float)angle / 90.0f);
  uint32_t pulse = MINPULSE + (angle_pct * (MAXPULSE - MINPULSE));
  return pulse;
}

void main(void) {
  printk("lets begin \n");
  while (1) {
    printk("PWM device cycle\n");

    if (servos[0].pulse == MAXPULSE)
      servos[0].pulse = MINPULSE;
    else
      servos[0].pulse = MAXPULSE;

    if (servos[1].pulse == angle_to_pulse(45))
      servos[1].pulse = angle_to_pulse(20);
    else
      servos[1].pulse = angle_to_pulse(45);

    if (servos[2].pulse == angle_to_pulse(80))
      servos[2].pulse = angle_to_pulse(40);
    else
      servos[2].pulse = angle_to_pulse(80);

    if (servos[3].pulse == angle_to_pulse(25))
      servos[3].pulse = angle_to_pulse(50);
    else
      servos[3].pulse = angle_to_pulse(25);

    if (servos[4].pulse == angle_to_pulse(30))
      servos[4].pulse = angle_to_pulse(90);
    else
      servos[4].pulse = angle_to_pulse(30);

    if (servos[5].pulse == angle_to_pulse(45))
      servos[5].pulse = angle_to_pulse(40);
    else
      servos[5].pulse = angle_to_pulse(45);

    set_Servos(servos);
    k_sleep(K_SECONDS(SLEEP_TIME_S));
  }
}
