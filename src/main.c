#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/sys/printk.h>

// setting period of motor to 50htz
#define PERIOD (USEC_PER_SEC / 50U)

#define STEP 100
#define MINPULSE 1000
#define MAXPULSE 2000
#define MIDPULSE 1500

#define SLEEP_TIME_S .5

const struct pwm_dt_spec pwm_led = PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));
const struct pwm_dt_spec other = PWM_DT_SPEC_GET(DT_ALIAS(pwm_01));

void PWM_control(uint8_t *dir, uint32_t *pulse_width)
{
	if (*dir == 1U)
	{
		*pulse_width = 1500;
		*dir = 0;
	}
	else if (*dir == 0U)
	{
		*pulse_width = MINPULSE;
		*dir = 1;
	}
}

void get_degrees(uint8_t *degrees, uint32_t *pulse_width)
{
	*degrees = (uint8_t)((float)(*pulse_width - MINPULSE) /
						 (float)(MAXPULSE - MINPULSE) * 90.0);
}

void main(void)
{
	printk("lets begin \n");
	uint32_t pulse_width = MINPULSE;
	uint8_t dir = 0U;
	uint8_t degrees = 0U;
	while (1)
	{
		printk("PWM device cycle\n");

		pwm_set(pwm_led.dev, 0, PWM_USEC(PERIOD), PWM_USEC(MAXPULSE), 0);
		pwm_set(pwm_led.dev, 1, PWM_USEC(PERIOD), PWM_USEC(MINPULSE), 0);
		pwm_set(other.dev, 0, PWM_USEC(PERIOD), PWM_USEC(MAXPULSE), 0);
		get_degrees(&degrees, &pulse_width);

		printk("PWM pulse width: %d\n", pulse_width);
		printk("Degrees: %d\n", degrees);

		// PWM_control(&dir, &pulse_width);

		printk("\n");

		k_sleep(K_SECONDS(SLEEP_TIME_S));
	}
}
