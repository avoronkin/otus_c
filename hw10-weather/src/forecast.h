#ifndef FORECAST_H_INCLUDED
#define FORECAST_H_INCLUDED 1

#include "metaweather.h"
#include <stdio.h>

void
forecast_print(FILE* output, MW_forecasts* forecasts, MW_location* location);

#endif // FORECAST_H_INCLUDED
