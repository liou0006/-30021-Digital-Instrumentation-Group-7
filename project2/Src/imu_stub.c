/*
 * imu_stub.c
 *
 *  Created on: Nov 20, 2025
 *      Author: Mark
 */


#include "imu_stub.h"
#include <stdint.h>

// Helper: store int16_t in little-endian form
static inline void put_le16(uint8_t *buf, int16_t v)
{
    buf[0] = (uint8_t)(v & 0xFF);
    buf[1] = (uint8_t)((v >> 8) & 0xFF);
}

// Stub IMU value generator: call once per sample.
// Produces slowly changing values for all axes.
void imu_stub_next_packet(uint8_t out[IMU_PACKET_SIZE])
{
    static int16_t t = 0;   // simple time / step counter

    // advance "time"
    t++;
    if (t > 1000) {
        t = -1000;          // wrap so it oscillates between -1000 and 1000
    }

    // --- Fake sensor values ---

    // Accelerometer (just some patterns)
    int16_t ax = t;                 // ramp
    int16_t ay = t / 2;             // smaller ramp
    int16_t az = 1000 - t;          // inverse ramp

    // Gyro (pretend degrees)
    int16_t gx =  (int16_t)(t % 360);
    int16_t gy =  (int16_t)(-t % 360);
    int16_t gz =  (int16_t)((t * 3) % 360);

    // Magnetometer
    int16_t mx = t / 4;
    int16_t my = -(t / 5);
    int16_t mz = t / 6;

    // --- Pack into binary packet (little-endian) ---

    put_le16(&out[ 0], ax);
    put_le16(&out[ 2], ay);
    put_le16(&out[ 4], az);

    put_le16(&out[ 6], gx);
    put_le16(&out[ 8], gy);
    put_le16(&out[10], gz);

    put_le16(&out[12], mx);
    put_le16(&out[14], my);
    put_le16(&out[16], mz);
}
