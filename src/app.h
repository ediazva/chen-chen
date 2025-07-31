#pragma once
#include <megatron.hpp>

class App {
  Megatron _megatron;

  bool initDisk();
  bool createDisk();
  bool createBoardsTable();
  bool createPostsTable();
  bool createThreadsTable();
public:
  bool init();

  int run();
};