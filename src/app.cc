#include "app.h"

#include "config.h"
#include "logger.h"

using namespace std::literals;

bool App::init() {
  if(!initDisk())
    return false;

  return true;
}

bool App::initDisk() {
  logger::info("Cargando disco");
  _megatron.load_disk(CONFIG_DISK_NAME, CONFIG_DISK_NFRAMES, CONFIG_DISK_EVICTION_POLICY);
  if(!_megatron.is_loaded()) {
    logger::info("No existe el disco. Intentando crearlo...");
    if(!createDisk()) {
      logger::error("No se pudo crear el disco");
      return false;
    } else {
      logger::info("Se creó el disco correctamente");
    }
  }

  _megatron.dump();

  logger::info("Se cargó el disco correctamente");

  return true;
}

bool App::createDisk() {
  _megatron.new_disk(
    CONFIG_DISK_NAME,
    CONFIG_DISK_SURFACES,
    CONFIG_DISK_TRACKS,
    CONFIG_DISK_SECTORS,
    CONFIG_DISK_BYTES,
    CONFIG_DISK_SECTOR_BLOCKS,
    CONFIG_DISK_NFRAMES,
    CONFIG_DISK_EVICTION_POLICY);

  if(!_megatron.is_loaded()) {
    logger::error("No se pudo crear el disco");
    return false;
  }
  
  bool res = createBoardsTable();
  res &= createThreadsTable();
  res &= createPostsTable();

  return res;
}

bool App::createBoardsTable() {
  auto metadata = std::vector{
    std::pair{"id"s, "SMALLINT"s},
    std::pair{"codigo"s, "VARCHAR(10)"s},
    std::pair{"nombre"s, "VARCHAR(20)"s},
    std::pair{"descripcion"s, "VARCHAR(250)"s},
  };
  if(!_megatron.create_table("Boards", metadata)) {
    logger::error("No se pudo crear la tabla Boards");
    return false;
  }

  _megatron.load_CSV("csv/board.csv", "Boards");

  return true;
}

bool App::createThreadsTable() {
  auto metadata = std::vector{
    std::pair{"id"s, "BIGINT"s},
    std::pair{"id_board"s, "SMALLINT"s},
    std::pair{"nombre"s, "VARCHAR(250)"s},
    std::pair{"fecha"s, "BIGINT"s},
  };
  if(!_megatron.create_table("Threads", metadata)) {
    logger::error("No se pudo crear la tabla Threads");
    return false;
  }

  _megatron.load_CSV("csv/thread.csv", "Threads");

  return true;
}

bool App::createPostsTable() {
  auto metadata = std::vector{
    std::pair{"id"s, "BIGINT"s},
    std::pair{"id_thread"s, "BIGINT"s},
    std::pair{"contenido"s, "VARCHAR(250)"s},
    std::pair{"fecha"s, "BIGINT"s},
    std::pair{"path"s, "VARCHAR(250)"s},
  };

  if(!_megatron.create_table("Posts", metadata)) {
    logger::error("No se pudo crear la tabla Posts");
    return false; 
  }

  _megatron.load_CSV("csv/post.csv", "Posts");

  return true;
}

int App::run() {

  return 0;
}