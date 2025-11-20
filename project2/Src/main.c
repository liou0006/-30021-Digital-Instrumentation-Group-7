#include "stm32f30x.h"
#include "ultrasonic_sensor.h"
#include <stdint.h>
#include "openlog_sd.h"
#include "imu_stub.h"

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();
    init_uart(115200);
    delay(1000); // wait for openlog to boot

    int count = 0;
    uint8_t packet[IMU_PACKET_SIZE];

    while (count < 250) {
        // Get next IMU telemetry packet (18 bytes of raw binary)
        imu_stub_next_packet(packet);

        // Write it directly to SD via OpenLog
        openlog_writebytes(packet, IMU_PACKET_SIZE);

        count++;
        delay(5);   // a few ms between samples
    }

    while (1); // halt after done
}
