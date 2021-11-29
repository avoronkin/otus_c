#ifndef FORECAST_H_INCLUDED
#define FORECAST_H_INCLUDED

#include "metaweather/metaweather.h"
#include <stdio.h>

void
forecast_print(FILE* output, MW_Forecasts* forecasts, MW_Location* location);

int
forecast_for_location(char* location_name);

#endif // FORECAST_H_INCLUDED
