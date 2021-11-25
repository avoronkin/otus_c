#include "config.h"
#include "forecast.h"
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char** argv)
{
  cfg config = cf_create(argc, argv);

  return forecast_for_location(config.location_name);
}
