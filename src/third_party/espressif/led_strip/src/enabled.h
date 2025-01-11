#pragma once

#include "sdkconfig.h"

#if CONFIG_IDF_TARGET_ESP32C2
#define FASTLED_ESP32_HAS_CLOCKLESS_SPI 1
#define FASTLED_ESP32_HAS_RMT 0
#define FASTLED_ESP32_HAS_RMT5 0
#elif CONFIG_IDF_TARGET_ESP32C3
#define FASTLED_ESP32_HAS_CLOCKLESS_SPI 1
#define FASTLED_ESP32_HAS_RMT 1
#define FASTLED_ESP32_HAS_RMT5 1
#elif CONFIG_IDF_TARGET_ESP32C6
#define FASTLED_ESP32_HAS_CLOCKLESS_SPI 1
#define FASTLED_ESP32_HAS_RMT 1
#define FASTLED_ESP32_HAS_RMT5 1
#elif CONFIG_IDF_TARGET_ESP32S2
#define FASTLED_ESP32_HAS_CLOCKLESS_SPI 1
#define FASTLED_ESP32_HAS_RMT 1
#define FASTLED_ESP32_HAS_RMT5 1
#elif CONFIG_IDF_TARGET_ESP32S3
#define FASTLED_ESP32_HAS_CLOCKLESS_SPI 1
#define FASTLED_ESP32_HAS_RMT 1
#define FASTLED_ESP32_HAS_RMT5 1
#elif CONFIG_IDF_TARGET_ESP32H2
#define FASTLED_ESP32_HAS_CLOCKLESS_SPI 1
#define FASTLED_ESP32_HAS_RMT 1
#define FASTLED_ESP32_HAS_RMT5 1
#elif CONFIG_IDF_TARGET_ESP32P4
#define FASTLED_ESP32_HAS_CLOCKLESS_SPI 1
#define FASTLED_ESP32_HAS_RMT 1
#define FASTLED_ESP32_HAS_RMT5 1
#elif CONFIG_IDF_TARGET_ESP8266
#define FASTLED_ESP32_HAS_CLOCKLESS_SPI 0
#define FASTLED_ESP32_HAS_RMT 0
#define FASTLED_ESP32_HAS_RMT5 0
#elif CONFIG_IDF_TARGET_ESP32
#define FASTLED_ESP32_HAS_CLOCKLESS_SPI 0
#define FASTLED_ESP32_HAS_RMT 1
#define FASTLED_ESP32_HAS_RMT5 0
#else
#warning "Unknown board, assuming support for clockless RMT5 and SPI chipsets."
#endif

#if FASTLED_ESP32_HAS_RMT5 && !__has_include("driver/rmt_types.h")
#warning "RMT5 driver because \"driver/rmt_types.h\" is missing, disabling automatic RMT5 support in FastLED."
#undef FASTLED_ESP32_HAS_RMT5
#define FASTLED_ESP32_HAS_RMT5 0
#endif

#if FASTLED_ESP32_HAS_RMT5 && !defined(FASTLED_RMT5)
#define FASTLED_RMT5 1
#else
#define FASTLED_RMT5 0
#endif
