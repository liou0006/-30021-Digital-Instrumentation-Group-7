#ifndef IMU_STUB_H
#define IMU_STUB_H
#include <stdint.h>

#define IMU_PACKET_SIZE 20   // 9 x int16_t = 18 bytes

// Fill 'out' with the next IMU telemetry packet (raw binary, little-endian)
void imu_stub_next_packet(uint8_t out[IMU_PACKET_SIZE]);

#endif // IMU_STUB_H