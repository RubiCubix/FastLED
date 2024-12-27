#pragma once


#include "esp_log.h"
#include "esp_check.h"
#include "fl/warn.h"

#define FASTLED_ASSERT(x, MSG)         \
    {                                  \
        if (!(x)) {                    \
            FASTLED_WARN(MSG);         \
            ESP_ERROR_CHECK(ESP_FAIL); \
        }                              \
    }