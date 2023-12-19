// This file is generated by ZCL Advanced Platform generator. Please don't edit manually.

// This file contains the tokens for nonSingletons stored in flash

// Identifier tags for tokens
// Creator for attribute: on/off, endpoint: 1
#define CREATOR_ON_OFF_1 0xB000
#define NVM3KEY_ON_OFF_1 (NVM3KEY_DOMAIN_ZIGBEE | 0xB000)
// Creator for attribute: start up on off, endpoint: 1
#define CREATOR_START_UP_ON_OFF_1 0xB001
#define NVM3KEY_START_UP_ON_OFF_1 (NVM3KEY_DOMAIN_ZIGBEE | 0xB001)
// Creator for attribute: current level, endpoint: 1
#define CREATOR_CURRENT_LEVEL_1 0xB002
#define NVM3KEY_CURRENT_LEVEL_1 (NVM3KEY_DOMAIN_ZIGBEE | 0xB002)
// Creator for attribute: start up current level, endpoint: 1
#define CREATOR_START_UP_CURRENT_LEVEL_1 0xB003
#define NVM3KEY_START_UP_CURRENT_LEVEL_1 (NVM3KEY_DOMAIN_ZIGBEE | 0xB003)
// Creator for attribute: color temperature, endpoint: 1
#define CREATOR_COLOR_CONTROL_COLOR_TEMPERATURE_1 0xB004
#define NVM3KEY_COLOR_CONTROL_COLOR_TEMPERATURE_1 (NVM3KEY_DOMAIN_ZIGBEE | 0xB004)
// Creator for attribute: start up color temperature mireds, endpoint: 1
#define CREATOR_START_UP_COLOR_TEMPERATURE_MIREDS_1 0xB005
#define NVM3KEY_START_UP_COLOR_TEMPERATURE_MIREDS_1 (NVM3KEY_DOMAIN_ZIGBEE | 0xB005)
// Creator for attribute: on/off, endpoint: 2
#define CREATOR_ON_OFF_2 0xB006
#define NVM3KEY_ON_OFF_2 (NVM3KEY_DOMAIN_ZIGBEE | 0xB006)
// Creator for attribute: start up on off, endpoint: 2
#define CREATOR_START_UP_ON_OFF_2 0xB007
#define NVM3KEY_START_UP_ON_OFF_2 (NVM3KEY_DOMAIN_ZIGBEE | 0xB007)
// Creator for attribute: current level, endpoint: 2
#define CREATOR_CURRENT_LEVEL_2 0xB008
#define NVM3KEY_CURRENT_LEVEL_2 (NVM3KEY_DOMAIN_ZIGBEE | 0xB008)
// Creator for attribute: start up current level, endpoint: 2
#define CREATOR_START_UP_CURRENT_LEVEL_2 0xB009
#define NVM3KEY_START_UP_CURRENT_LEVEL_2 (NVM3KEY_DOMAIN_ZIGBEE | 0xB009)
// Creator for attribute: color temperature, endpoint: 2
#define CREATOR_COLOR_CONTROL_COLOR_TEMPERATURE_2 0xB00A
#define NVM3KEY_COLOR_CONTROL_COLOR_TEMPERATURE_2 (NVM3KEY_DOMAIN_ZIGBEE | 0xB00A)
// Creator for attribute: start up color temperature mireds, endpoint: 2
#define CREATOR_START_UP_COLOR_TEMPERATURE_MIREDS_2 0xB00B
#define NVM3KEY_START_UP_COLOR_TEMPERATURE_MIREDS_2 (NVM3KEY_DOMAIN_ZIGBEE | 0xB00B)


// Types for the tokens
#ifdef DEFINETYPES
typedef uint8_t tokType_on_off;
        typedef uint8_t tokType_start_up_on_off;
        typedef uint8_t tokType_current_level;
        typedef uint8_t tokType_start_up_current_level;
        typedef uint16_t tokType_color_control_color_temperature;
        typedef uint16_t tokType_start_up_color_temperature_mireds;
        #endif // DEFINETYPES

// Actual token definitions
#ifdef DEFINETOKENS

DEFINE_BASIC_TOKEN(ON_OFF_1, tokType_on_off, 0x01)
DEFINE_BASIC_TOKEN(START_UP_ON_OFF_1, tokType_start_up_on_off, 0xFF)
DEFINE_BASIC_TOKEN(CURRENT_LEVEL_1, tokType_current_level, 0x64)
DEFINE_BASIC_TOKEN(START_UP_CURRENT_LEVEL_1, tokType_start_up_current_level, 0x33)
DEFINE_BASIC_TOKEN(COLOR_CONTROL_COLOR_TEMPERATURE_1, tokType_color_control_color_temperature, 0x00FA)
DEFINE_BASIC_TOKEN(START_UP_COLOR_TEMPERATURE_MIREDS_1, tokType_start_up_color_temperature_mireds, 0x4444)
DEFINE_BASIC_TOKEN(ON_OFF_2, tokType_on_off, 0x01)
DEFINE_BASIC_TOKEN(START_UP_ON_OFF_2, tokType_start_up_on_off, 0xFF)
DEFINE_BASIC_TOKEN(CURRENT_LEVEL_2, tokType_current_level, 0x64)
DEFINE_BASIC_TOKEN(START_UP_CURRENT_LEVEL_2, tokType_start_up_current_level, 0x33)
DEFINE_BASIC_TOKEN(COLOR_CONTROL_COLOR_TEMPERATURE_2, tokType_color_control_color_temperature, 0x00FA)
DEFINE_BASIC_TOKEN(START_UP_COLOR_TEMPERATURE_MIREDS_2, tokType_start_up_color_temperature_mireds, 0x4444)
#endif // DEFINETOKENS

// Macro snippet that loads all the attributes from tokens
#define GENERATED_TOKEN_LOADER(endpoint) do { \
  uint8_t ptr[2]; \
  uint8_t curNetwork = emberGetCurrentNetwork(); \
  uint8_t epNetwork; \
  epNetwork = emberAfNetworkIndexFromEndpoint(1); \
  if(1 == (endpoint) || (EMBER_BROADCAST_ENDPOINT == (endpoint) && epNetwork == curNetwork)) { \
    halCommonGetToken((tokType_on_off *)ptr, TOKEN_ON_OFF_1); \
    emberAfWriteServerAttribute(1, ZCL_ON_OFF_CLUSTER_ID, ZCL_ON_OFF_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_BOOLEAN_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_start_up_on_off *)ptr, TOKEN_START_UP_ON_OFF_1); \
    emberAfWriteServerAttribute(1, ZCL_ON_OFF_CLUSTER_ID, ZCL_START_UP_ON_OFF_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_ENUM8_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_current_level *)ptr, TOKEN_CURRENT_LEVEL_1); \
    emberAfWriteServerAttribute(1, ZCL_LEVEL_CONTROL_CLUSTER_ID, ZCL_CURRENT_LEVEL_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_start_up_current_level *)ptr, TOKEN_START_UP_CURRENT_LEVEL_1); \
    emberAfWriteServerAttribute(1, ZCL_LEVEL_CONTROL_CLUSTER_ID, ZCL_START_UP_CURRENT_LEVEL_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_color_control_color_temperature *)ptr, TOKEN_COLOR_CONTROL_COLOR_TEMPERATURE_1); \
    emberAfWriteServerAttribute(1, ZCL_COLOR_CONTROL_CLUSTER_ID, ZCL_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT16U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_start_up_color_temperature_mireds *)ptr, TOKEN_START_UP_COLOR_TEMPERATURE_MIREDS_1); \
    emberAfWriteServerAttribute(1, ZCL_COLOR_CONTROL_CLUSTER_ID, ZCL_START_UP_COLOR_TEMPERATURE_MIREDS_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT16U_ATTRIBUTE_TYPE); \
  } \
  epNetwork = emberAfNetworkIndexFromEndpoint(2); \
  if(2 == (endpoint) || (EMBER_BROADCAST_ENDPOINT == (endpoint) && epNetwork == curNetwork)) { \
    halCommonGetToken((tokType_on_off *)ptr, TOKEN_ON_OFF_2); \
    emberAfWriteServerAttribute(2, ZCL_ON_OFF_CLUSTER_ID, ZCL_ON_OFF_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_BOOLEAN_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_start_up_on_off *)ptr, TOKEN_START_UP_ON_OFF_2); \
    emberAfWriteServerAttribute(2, ZCL_ON_OFF_CLUSTER_ID, ZCL_START_UP_ON_OFF_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_ENUM8_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_current_level *)ptr, TOKEN_CURRENT_LEVEL_2); \
    emberAfWriteServerAttribute(2, ZCL_LEVEL_CONTROL_CLUSTER_ID, ZCL_CURRENT_LEVEL_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_start_up_current_level *)ptr, TOKEN_START_UP_CURRENT_LEVEL_2); \
    emberAfWriteServerAttribute(2, ZCL_LEVEL_CONTROL_CLUSTER_ID, ZCL_START_UP_CURRENT_LEVEL_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_color_control_color_temperature *)ptr, TOKEN_COLOR_CONTROL_COLOR_TEMPERATURE_2); \
    emberAfWriteServerAttribute(2, ZCL_COLOR_CONTROL_CLUSTER_ID, ZCL_COLOR_CONTROL_COLOR_TEMPERATURE_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT16U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_start_up_color_temperature_mireds *)ptr, TOKEN_START_UP_COLOR_TEMPERATURE_MIREDS_2); \
    emberAfWriteServerAttribute(2, ZCL_COLOR_CONTROL_CLUSTER_ID, ZCL_START_UP_COLOR_TEMPERATURE_MIREDS_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT16U_ATTRIBUTE_TYPE); \
  } \
} while (false)

// Macro snippet that saves the attribute to token
#define GENERATED_TOKEN_SAVER do { \
  uint8_t allZeroData[2]; \
  MEMSET(allZeroData, 0, 2);  \
  if ( data == NULL ) { data = allZeroData; } \
if ( 1 == endpoint ) { \
    if ( 0x0006 == clusterId ) { \
if ( 0x0000 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_ON_OFF_1, data); \
if ( 0x4003 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_START_UP_ON_OFF_1, data); \
    } \
    else if ( 0x0008 == clusterId ) { \
if ( 0x0000 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_CURRENT_LEVEL_1, data); \
if ( 0x4000 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_START_UP_CURRENT_LEVEL_1, data); \
    } \
    else if ( 0x0300 == clusterId ) { \
if ( 0x0007 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_COLOR_CONTROL_COLOR_TEMPERATURE_1, data); \
if ( 0x4010 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_START_UP_COLOR_TEMPERATURE_MIREDS_1, data); \
    } \
} \
if ( 2 == endpoint ) { \
    if ( 0x0006 == clusterId ) { \
if ( 0x0000 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_ON_OFF_2, data); \
if ( 0x4003 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_START_UP_ON_OFF_2, data); \
    } \
    else if ( 0x0008 == clusterId ) { \
if ( 0x0000 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_CURRENT_LEVEL_2, data); \
if ( 0x4000 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_START_UP_CURRENT_LEVEL_2, data); \
    } \
    else if ( 0x0300 == clusterId ) { \
if ( 0x0007 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_COLOR_CONTROL_COLOR_TEMPERATURE_2, data); \
if ( 0x4010 == metadata->attributeId && 0x0000 == emberAfGetMfgCode(metadata) && !emberAfAttributeIsClient(metadata) ) \
          halCommonSetToken(TOKEN_START_UP_COLOR_TEMPERATURE_MIREDS_2, data); \
    } \
} \
} while (false)