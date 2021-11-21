#include "config.h"
#include "metaweather.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

void
forecast_print(MW_forecasts* forecasts, MW_location* location)
{
  printf("Дата       | Локация         | Прогноз       | Ветер | Ветер, м/с | Т мин.,°C  | Т макс.,°C \n");

  int i = 0;
  printf("%-10s | %-15s | %-13s | %-5s | %-10f | %-10f | %-10f\n",
         forecasts->entries[i]->applicable_date,
         location->title,
         forecasts->entries[i]->weather_state_name,
         forecasts->entries[i]->wind_direction_compass,
         forecasts->entries[i]->wind_speed / 2.237,//перевод мили/час в метр/секунду
         forecasts->entries[i]->min_temp,
         forecasts->entries[i]->max_temp);
}

int
main(int argc, char** argv)
{
  // int err = 0;
  cfg config = cf_create(argc, argv);
  curl_global_init(CURL_GLOBAL_ALL);

  MW_locations* locations = NULL;
  MW_code locations_alloc_code = mw_locations_alloc(&locations);
  if (locations_alloc_code != MW_OK) {
    fprintf(stderr, "Ошибка при выделении памяти.\n");
    curl_global_cleanup();
    return -1;
  }

  MW_code find_locations_code = mw_locations_search(locations, config.location_name);
  if (find_locations_code != MW_OK) {
    fprintf(stderr, "Ошибка при поиске локации.\n");
    curl_global_cleanup();
    mw_locations_free(locations);
    return -2;
  }

  for (size_t i = 0; i < locations->size; i++) {

    MW_forecasts* forecasts = NULL;
    MW_code forecasts_alloc_code = mw_forecasts_alloc(&forecasts);
    if (forecasts_alloc_code != MW_OK) {
      fprintf(stderr, "Ошибка при выделении памяти.\n");
      mw_locations_free(locations);
      curl_global_cleanup();
      return -3;
    }

    MW_code forecasts_fetch_code = mw_forecasts_fetch(forecasts, locations->entries[i]->woeid);
    if (forecasts_fetch_code != MW_OK) {
      fprintf(stderr, "Ошибка при получении прогноза для локации.\n");
      mw_forecasts_free(forecasts);
      mw_locations_free(locations);
      curl_global_cleanup();
      return -4;
    }

    forecast_print(forecasts, locations->entries[i]);

    mw_forecasts_free(forecasts);
  }

  mw_locations_free(locations);
  curl_global_cleanup();
  return 0;
}
