/**
 * @file wifi.h
 * @brief 连接wifi
 * @author RedlightASl (dddbbbdd@foxmail.com)
 * @version 1.0
 * @date 2021-11-29
 * 
 * @copyright Copyright (c) 2021  RedlightASl
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-11-29 <td>1.0     <td>wangh     <td>Content
 * </table>
 */
#ifndef __WIFI_H
#define __WIFI_H
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "sdkconfig.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define EXAMPLE_ESP_WIFI_SSID      "_OurEDA_OurFi"
#define EXAMPLE_ESP_WIFI_PASS      "OurEDA2021"
#define EXAMPLE_ESP_MAXIMUM_RETRY  5

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void app_wifi(void);
#endif