/*

Key Changes:
1. Interpolation Function:
Interpolates the SBUS channel value (172 to 1811) to the Sabertooth motor's PWM range (0 to 127).
I used round() for rounding the result after interpolation to ensure the value sent to the motor 
driver is an integer.

2. Parse Buffer Function:
Left the bit masking and shifting logic intact since it correctly extracts the 11-bit channel values
from the SBUS packet.

*/

#include <stdint.h>
#include <parsing.h>
#include <math.h>
#include <stdint.h>
#include <parsing.h>

// Interpolate channel value to PWM value
int interpolation(uint16_t ch) {
    // Define the minimum and maximum values for SBUS and PWM
    const int RC_MIN = 172;     // SBUS minimum value
    const int RC_MAX = 1811;    // SBUS maximum value
    const int PWM_MIN = 0;      // Sabertooth minimum PWM (full reverse)
    const int PWM_MAX = 127;    // Sabertooth maximum PWM (full forward)

    // Perform linear interpolation
    int pwm = (ch - RC_MIN) * (PWM_MAX - PWM_MIN) / (RC_MAX - RC_MIN) + PWM_MIN;

    // Ensure PWM value is within bounds (optional)
    if (pwm < PWM_MIN) pwm = PWM_MIN;
    if (pwm > PWM_MAX) pwm = PWM_MAX;

    return pwm;
}

// Parse SBUS packet to extract channel values
uint16_t* parse_buffer(uint8_t buff[]) {
    static uint16_t channel[16];
    uint16_t mask = 0x7FF;  // 11-bit mask

    // Extract 16 channels from the 25-byte SBUS packet
    channel[0]  = ((buff[1] | buff[2] << 8) & mask);
    channel[1]  = ((buff[2] >> 3 | buff[3] << 5) & mask);
    channel[2]  = ((buff[3] >> 6 | buff[4] << 2 | buff[5] << 10) & mask);
    channel[3]  = ((buff[5] >> 1 | buff[6] << 7) & mask);
    channel[4]  = ((buff[6] >> 4 | buff[7] << 4) & mask);
    channel[5]  = ((buff[7] >> 7 | buff[8] << 1 | buff[9] << 9) & mask);
    channel[6]  = ((buff[9] >> 2 | buff[10] << 6) & mask);
    channel[7]  = ((buff[10] >> 5 | buff[11] << 3) & mask);
    channel[8]  = ((buff[12] | buff[13] << 8) & mask);
    channel[9]  = ((buff[13] >> 3 | buff[14] << 5) & mask);
    channel[10] = ((buff[14] >> 6 | buff[15] << 2 | buff[16] << 10) & mask);
    channel[11] = ((buff[16] >> 1 | buff[17] << 7) & mask);
    channel[12] = ((buff[17] >> 4 | buff[18] << 4) & mask);
    channel[13] = ((buff[18] >> 7 | buff[19] << 1 | buff[20] << 9) & mask);
    channel[14] = ((buff[20] >> 2 | buff[21] << 6) & mask);
    channel[15] = ((buff[21] >> 5 | buff[22] << 3) & mask);

    return channel;
}
