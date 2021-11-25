#include "forecast.h"
#include "metaweather/metaweather.h"
#include <stdio.h>

void
forecast_print(FILE* output, MW_Forecasts* forecasts, MW_Location* location)
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

int
forecast_for_location(char* location_name)
{
  MW_Locations* locations = NULL;
  locations = mw_locations_alloc();
  if (locations == NULL) {
    fprintf(stderr, "Ошибка при выделении памяти.\n");
    return -1;
  }

  MW_Code find_locations_code = mw_locations_search(locations, location_name);
  if (find_locations_code != MW_OK) {
    fprintf(stderr, "Ошибка при поиске локации.\n");
    mw_locations_free(locations);
    return -2;
  }

  for (size_t i = 0; i < locations->size; i++) {

    MW_Forecasts* forecasts = NULL;
    forecasts = mw_forecasts_alloc();
    if (forecasts == NULL) {
      fprintf(stderr, "Ошибка при выделении памяти.\n");
      mw_locations_free(locations);
      return -3;
    }

    MW_Code forecasts_fetch_code = mw_forecasts_fetch(forecasts, locations->entries[i]->woeid);
    if (forecasts_fetch_code != MW_OK) {
      fprintf(stderr, "Ошибка при получении прогноза для локации.\n");
      mw_forecasts_free(forecasts);
      mw_locations_free(locations);
      return -4;
    }

    forecast_print(stdout, forecasts, locations->entries[i]);
    mw_forecasts_free(forecasts);
  }

  mw_locations_free(locations);
  return 0;
}
