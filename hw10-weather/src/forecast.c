// #include "metaweather.h"
// #include <stdio.h>
#include "forecast.h"

void
forecast_print(FILE* output, MW_forecasts* forecasts, MW_location* location)
{
  fprintf(output, "Дата       | Локация         | Прогноз       | Ветер | Ветер, м/с | Т мин.,°C  | Т макс.,°C \n");

  int i = 0;
  fprintf(output,
          "%-10s | %-15s | %-13s | %-5s | %-10f | %-10f | %-10f\n",
          forecasts->entries[i]->applicable_date,
          location->title,
          forecasts->entries[i]->weather_state_name,
          forecasts->entries[i]->wind_direction_compass,
          forecasts->entries[i]->wind_speed / 2.237, //перевод мили/час в метр/секунду
          forecasts->entries[i]->min_temp,
          forecasts->entries[i]->max_temp);
}
