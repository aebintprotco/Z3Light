/***************************************************************************//**
 * @brief Zigbee OTA Bootload Cluster Client Policy component configuration header.
 *\n*******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "../autogen/zap-config.h"
// <<< Use Configuration Wizard in Context Menu >>>

// <h>Zigbee OTA Bootload Cluster Client Policy configuration

// <o EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_IMAGE_TYPE_ID> Image Type ID <0-65535>
// <i> Default: 0
// <i> This is the device's OTA image identifier used for querying the OTA server about the next image to use for an upgrade.
#define EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_IMAGE_TYPE_ID   105

//#define XYZ_VERSION(major, minor, patch) ((major) << 6 | (minor) << 4 | (patch)) // 高位到低位-> xx yy zzzz ->01,010101 -> 1.1.5大版本 3.3.15)
// <o EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_FIRMWARE_VERSION> Firmware Version <1-4294967295>
// <i> Default: 1
// <i> This is the device's current firmware version, used when querying the OTA server about the next image to use for an upgrade.
#define EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_FIRMWARE_VERSION   VERSION      //00 x  00y  0000z   0x10 00 0000 代表1.0.0 2进制
                                              //这里的版本号每次都要和 zap-command.h 中的basic信息版本号同步
// <o EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_HARDWARE_VERSION> Hardware Version <0-65535>
// <i> Default: 0
// <i> Devices may have a hardware version that limits what images they can use.  OTA Images may be configured with minimum and maximum hardware versions that they are supported on.  If the device is not restricted by hardware version then this value should be 0xFFFF.
#define EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_HARDWARE_VERSION   XYZ_VERSION(1,0,0)       //00 x  00y  0000z   0x10 00 0000 代表1.0.0 2进制

// <q EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_EBL_VERIFICATION> Perform EBL Verification (SOC Only)
// <i> Default: TRUE
// <i> This uses the application bootloader routines to verify the EBL image after signature verification passes.
#define EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_EBL_VERIFICATION   1

// <q EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_INCLUDE_HARDWARE_VERSION> Include Hardware Version
// <i> Default: FALSE
// <i> This indicates that the current hardware version of the product should be included in the messages sent to the ZigBee OTA Cluster server.
#define EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_INCLUDE_HARDWARE_VERSION   0

// <q EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_DELETE_FAILED_DOWNLOADS> Delete Failed Downloads
// <i> Default: TRUE
// <i> This causes the device to delete any image (partial or complete) that has been downloaded but did not pass verification or when the server tells us to abort the download or upgrade.
#define EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_DELETE_FAILED_DOWNLOADS   1

// </h>

// <<< end of configuration section >>>
