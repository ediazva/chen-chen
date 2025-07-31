#include "app.h"
#include "logger.h"

int main() {
  App app;
  if(!app.init()) {
    logger::error("Error al iniciar el disco");
    return 1;
  }

  return app.run();
}