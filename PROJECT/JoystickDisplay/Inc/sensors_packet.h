/*
 * sensors_packet.h
 *
 *  Created on: Nov 25, 2025
 *      Author: markmalloy
 */

#ifndef SENSORS_PACKET_H_
#define SENSORS_PACKET_H_

void sensors_pack_raw(const lsm9ds1_raw_data_t *s, uint8_t out[20]);


#endif /* SENSORS_PACKET_H_ */
