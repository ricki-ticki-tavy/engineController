#include "tankPerepherialControllerStarter.h"

void setup() {
  tankController::controllerSetup();
}

void loop() {
  tankController::controllerLoop();                         
}
