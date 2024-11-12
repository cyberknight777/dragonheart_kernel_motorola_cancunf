#ifndef _MOT_CONFIG_H
#define  _MOT_CONFIG_H
#include "precomp.h"
#define ARRAY_VALUE_MAX  64

typedef enum  {
    WIFI_CFG_INDEX,
    TXPOWERCTRL_CFG_INDEX
} WIFI_CFG_ENUM;

/**
 * get the moto config file name
 * name: for save file name
 * index: WIFI_CFG_INDEX for get wifi.cfg name.
 *        TXPOWERCTRL_CFG_INDEX for get txpowerctrl.cfg name.
 */
void get_moto_config_file_name(char* name, WIFI_CFG_ENUM index);
#endif