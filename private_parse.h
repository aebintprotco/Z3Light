/*
 * led_user.h
 *
 *  Created on: 2023年8月29日
 *      Author: YWJ
 */

#ifndef PRIVATE_PARSE_H_
#define PRIVATE_PARSE_H_



#include "app.h"
#include "led_user.h"



// 将 16 位整数转换为字节数组（小端序）
#define INT16_TO_LE_BYTES(val, bytes)  { (bytes)[0] = (uint8_t)((val) & 0xFF); (bytes)[1] = (uint8_t)(((val) >> 8) & 0xFF); }


// 将 val 转换成32位整数  转换为字节数组（小端序）
#define INT16_TO_LE_BYTES(val, bytes)  { (bytes)[0] = (uint8_t)((val) & 0xFF); (bytes)[1] = (uint8_t)(((val) >> 8) & 0xFF); }

// 将 小端序 字节数组转换为 16 位整数
#define LE_BYTES_TO_INT16(bytes)       ((uint16_t)((bytes)[0] | ((bytes)[1] << 8)))

// 将 32 位整数转换为字节数组（小端序）
#define INT32_TO_LE_BYTES(val, bytes)  { (bytes)[0] = (uint8_t)((val) & 0xFF); \
                                         (bytes)[1] = (uint8_t)(((val) >> 8) & 0xFF); \
                                         (bytes)[2] = (uint8_t)(((val) >> 16) & 0xFF); \
                                         (bytes)[3] = (uint8_t)(((val) >> 24) & 0xFF); }
// 将 小端序 字节数组转换为 32 位整数
#define LE_BYTES_TO_INT32(bytes)       ((uint32_t)((bytes)[0] | ((bytes)[1] << 8) | ((bytes)[2] << 16)) | ((bytes)[3] << 24))








extern void zigbee_Private_Trasmit_Parse(uint8_t endpoint,
                                  EmberAfClusterId cluster,
                                  EmberAfAttributeId attributeID,
                                  uint8_t *dataPtr,
                                  uint16_t readLength);






#endif /* PRIVATE_PARSE_H_ */
