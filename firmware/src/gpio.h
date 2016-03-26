
#ifndef _GPIO_H_
#define _GPIO_H_

#include <platform_config.h>
#include <stdbool.h>

typedef enum {
  FEELER_LEFT,
  FEELER_RIGHT
} Feeler;

HAL_StatusTypeDef gpio_setup();

extern void onFeeler(Feeler feeler, bool active);

#endif
