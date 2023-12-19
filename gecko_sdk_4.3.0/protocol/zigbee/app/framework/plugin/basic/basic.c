/***************************************************************************//**
 * @file
 * @brief Implementation for the Basic Server Cluster plugin.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "af.h"
#include "basic.h"
#include "app/framework/util/attribute-storage.h"
#if defined(SL_COMPONENT_CATALOG_PRESENT)
#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif
#ifdef SL_CATALOG_ZIGBEE_REPORTING_PRESENT
#include "reporting.h"
#endif // SL_CATALOG_ZIGBEE_REPORTING_PRESENT
#else // !SL_COMPONENT_CATALOG_PRESENT
#ifdef EMBER_AF_PLUGIN_REPORTING
#include "app/framework/plugin/reporting/reporting.h"
#define SL_CATALOG_ZIGBEE_REPORTING_PRESENT
#endif // EMBER_AF_PLUGIN_REPORTING
#endif // SL_COMPONENT_CATALOG_PRESENT


extern void zigbee_Private_Trasmit_Parse(uint8_t endpoint,
                                  EmberAfClusterId cluster,
                                  EmberAfAttributeId attributeID,
                                  uint8_t *dataPtr,
                                  uint16_t readLength);

bool emberAfBasicClusterResetToFactoryDefaultsCallback(void)
{
  emberAfBasicClusterPrintln("RX: ResetToFactoryDefaultsCallback");
  emberAfResetAttributes(emberAfCurrentEndpoint());
  emberAfPluginBasicResetToFactoryDefaultsCallback(emberAfCurrentEndpoint());
#ifdef SL_CATALOG_ZIGBEE_REPORTING_PRESENT
  sli_zigbee_af_reporting_get_last_value_all();
#endif // SL_CATALOG_ZIGBEE_REPORTING_PRESENT
  emberAfSendImmediateDefaultResponse(EMBER_ZCL_STATUS_SUCCESS);
  return true;
}

uint32_t emberAfBasicClusterServerCommandParse(sl_service_opcode_t opcode,
                                               sl_service_function_context_t *context)
{
  (void)opcode;
  bool wasHandled = false;
  EmberAfClusterCommand *cmd = (EmberAfClusterCommand *)context->data;
  if (!cmd->mfgSpecific && cmd->commandId == ZCL_RESET_TO_FACTORY_DEFAULTS_COMMAND_ID) {
    wasHandled = emberAfBasicClusterResetToFactoryDefaultsCallback();
  }else if(cmd->commandId == ZCL_BASIC_PRIVATE_TRASMIT_CMD_ID){  //私有协议部分

      uint8_t endpoint = cmd->apsFrame->sourceEndpoint; // 端点
      uint16_t clusterId = cmd->apsFrame->clusterId;    // 集群ID
    //  uint8_t sequenceNumber = cmd->seqNum;             // 序列号
      uint8_t commandId = cmd->commandId;               // 命令ID

 //     emberAfBasicClusterPrintln("basic  PRIVATE_TRASMIT  point: %x  sequenceNumber %x cmd->bufLen %x ", endpoint,sequenceNumber,cmd->bufLen);


      zigbee_Private_Trasmit_Parse(endpoint,
                            clusterId,
                            commandId,
                           (uint8_t *)&cmd->buffer[cmd->payloadStartIndex],
                           (cmd->bufLen-3));
      emberAfSendImmediateDefaultResponse(EMBER_ZCL_STATUS_SUCCESS);
      wasHandled = true;

  }




  /*
  // 提取消息的一些基本信息
  uint8_t endpoint = cmd->apsFrame->sourceEndpoint; // 端点
  uint16_t clusterId = cmd->apsFrame->clusterId;    // 集群ID
  uint8_t sequenceNumber = cmd->seqNum;             // 序列号
  uint8_t commandId = cmd->commandId;               // 命令ID

  // 打印消息信息
  emberAfBasicClusterPrintln("RX: len %d, ep %02X, clus 0x%04X (Cluster Name) FC %02X seq %02X cmd %02X",
                             cmd->bufLen, endpoint, clusterId, cmd->apsFrame->options, sequenceNumber, commandId);
*/
  // 提取并打印消息的 payload 部分
 /*
  emberAfBasicClusterPrint("payload[");
  for (int i = cmd->payloadStartIndex; i < cmd->bufLen; i++) {
      emberAfBasicClusterPrint(" %X", cmd->buffer[i]);
  }
  emberAfBasicClusterPrintln(" ]");
*/



  return ((wasHandled)
          ? EMBER_ZCL_STATUS_SUCCESS
          : EMBER_ZCL_STATUS_UNSUP_COMMAND);
}
