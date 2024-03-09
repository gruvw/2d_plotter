extern "C" {
    #include "src/main/start.h"
}

void setup() {
    Serial.begin(9600);
    start();
}

void loop() { }
