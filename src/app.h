#pragma once
#include <megatron.hpp>

class App {
  Megatron _megatron;

  bool initDisk();
  bool createDisk();
  bool createTables();
public:
  bool init();

  int run();
};