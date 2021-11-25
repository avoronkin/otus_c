#include "config.h"
#include "forecast.h"
#include "metaweather/metaweather.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char** argv)
{
  cfg config = cf_create(argc, argv);

  MW_Locations* locations = NULL;
  locations = mw_locations_alloc();
  if (locations == NULL) {
    fprintf(stderr, "Ошибка при выделении памяти.\n");
    return -1;
  }

  MW_Code find_locations_code = mw_locations_search(locations, config.location_name);
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
