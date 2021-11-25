#ifndef FORECAST_H_INCLUDED
#define FORECAST_H_INCLUDED 1

#include "metaweather/metaweather.h"
#include <stdio.h>

void
forecast_print(FILE* output, MW_Forecasts* forecasts, MW_Location* location);

#endif // FORECAST_H_INCLUDED
