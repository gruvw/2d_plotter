#include <Arduino.h>

extern "C" {
  #include "log.h"
}

void log_double(double d) {
    Serial.println(d);
}
