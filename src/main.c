#include <stdio.h>
#include "boards/pico.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "mpu-9250.h"
#include "kiss_fftr.h"

#define NSAMP 1000

int main() {

    // Enable UART so we can print status output
    stdio_init_all();


    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    mpu9250_init(i2c_default, PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN);
    mpu_set_sensors(INV_XYZ_ACCEL);

    short accel[3];
    unsigned long timestamp;
    long buf[3][NSAMP];
    kiss_fft_scalar fft_in[3][NSAMP]; // kiss_fft_scalar is a float
    kiss_fft_cpx fft_out[3][NSAMP];
    kiss_fftr_cfg cfg = kiss_fftr_alloc(NSAMP,false,0,0);

    while (true) {
        for (int i=0; i<NSAMP; i++) {
            mpu_get_accel_reg(accel, &timestamp);

            buf[0][i] = (accel[0] >> 1) + 32768;
            buf[1][i] = (accel[1] >> 1) + 32768;
            buf[2][i] = (accel[2] >> 1) + 32768;
        }

        for (int i=0; i<1; i++) {
            uint64_t sum = 0;
            for (int j=0;j<NSAMP;j++) {sum+=buf[i][j];}
            float avg = (float)sum/NSAMP;
            for (int j=0;j<NSAMP;j++) {
                    fft_in[i][j] = (float) (buf[i][j]-avg) / 10000;
            }

            // compute fast fourier transform
            kiss_fftr(cfg , fft_in[i], fft_out[i]);

            // compute power and calculate max freq component
            float max_power = 0;
            int max_idx = 0;
            // any frequency bin over NSAMP/2 is aliased (nyquist sampling theorum)
            for (int j = 0; j < NSAMP/2; j++) {
                float power = fft_out[i][j].r*fft_out[i][j].r+fft_out[i][j].i*fft_out[i][j].i;
                if (power>max_power) {
                    max_power=power;
                    max_idx = j;
                }
                printf("%i,%f\r\n", j*4, power);
            }



            if (max_power > 1) {
//                printf("Axis %i: Max power: %.1f - Freq-Bin: %i Hz\n", i, max_power, max_idx*4);
            }
        }
    }

    return 0;

}
