#include "sensors.h"
#include <stdint.h>

static inline void put_le16(uint8_t *buf, int16_t v)
{
    buf[0] = (uint8_t)(v & 0xFF);
    buf[1] = (uint8_t)((v >> 8) & 0xFF);
}

void sensors_pack_raw(const lsm9ds1_raw_data_t *s, uint8_t out[20])
{
    put_le16(&out[ 0], s->ax);
    put_le16(&out[ 2], s->ay);
    put_le16(&out[ 4], s->az);

    put_le16(&out[ 6], s->gx);
    put_le16(&out[ 8], s->gy);
    put_le16(&out[10], s->gz);

    put_le16(&out[12], s->mx);
    put_le16(&out[14], s->my);
    put_le16(&out[16], s->mz);

    put_le16(&out[18], s->T);
}
