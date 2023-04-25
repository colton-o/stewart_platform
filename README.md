# stewart_platform
Using this project as a way of becoming familiar with zephyr and the nrf5340.

## goals

- use PWM signals and inverse kinimetics to make a stewart platform
- using Thread and Matter create wireless communication between the platform and mobile device
- create a custom pcb with a nrf5340 chip to make the platform

## Production
### Tools
6 servo motors
nrf5340 dev-kit

### Setting up devices
NRF5340 has 4 pwm generators each with four channels, so I mapped 6 signals to corrospond with six servos, by using 3 generators, with 2 channels each. 
`{
    mypwms{
        compatible = "pwm-leds";
        pwm_alpha: pwm_alpha{
            status = "okay";
		    pwms = <&pwm0 0 PWM_MSEC(20) PWM_POLARITY_INVERTED>;
	    }; 
        pwm_beta: pwm_beta{
            status = "okay";
		    pwms = <&pwm0 1 PWM_MSEC(20) PWM_POLARITY_INVERTED>;
	    };
        pwm_gamma: pwm_gamma{
            status = "okay";
		    pwms = <&pwm1 0 PWM_MSEC(20) PWM_POLARITY_INVERTED>;
	    };
        pwm_delta: pwm_delta{
            status = "okay";
		    pwms = <&pwm1 1 PWM_MSEC(20) PWM_POLARITY_INVERTED>;
	    };
        pwm_epsilon: pwm_epsilon{
            status = "okay";
		    pwms = <&pwm2 0 PWM_MSEC(20) PWM_POLARITY_INVERTED>;
	    };
        pwm_zeta: pwm_zeta{
            status = "okay";
		    pwms = <&pwm2 1 PWM_MSEC(20) PWM_POLARITY_INVERTED>;
	    };
    }; `

Then I outputted each PWM signal to pins 1.4 - 1.9 

```
&pwm0_default {
    group1 {
        psels = <NRF_PSEL(PWM_OUT0, 1, 4)>, <NRF_PSEL(PWM_OUT1, 1, 5)>;
    };
};

&pinctrl {
    pwm1_default: pwm1_default {
        group1 {
            psels = <NRF_PSEL(PWM_OUT0, 1, 6)>, <NRF_PSEL(PWM_OUT1, 1, 7)>;
        };
    };

    pwm2_default: pwm2_default {
        group1 {
            psels = <NRF_PSEL(PWM_OUT0, 1, 8)>, <NRF_PSEL(PWM_OUT1, 1, 9)>;
        };
    };
}; ```


### Testing Servos
