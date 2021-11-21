#include "config.h"
#include "metaweather.h"
#include "forecast.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>



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

    forecast_print(stdout, forecasts, locations->entries[i]);

    mw_forecasts_free(forecasts);
  }

  mw_locations_free(locations);
  curl_global_cleanup();
  return 0;
}
