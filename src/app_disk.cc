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
  
  return createTables();
}

bool App::createTables() {
  std::vector<std::pair<std::string, std::string>>
    board_cols = {
        {"id", "SMALLINT"},
        {"codigo", "VARCHAR(10)"},
        {"nombre", "VARCHAR(20)"},
        {"descripcion", "VARCHAR(250)"},
    },
    thread_cols = {
        {"id", "BIGINT"},
        {"id_board", "SMALLINT"},
        {"nombre", "VARCHAR(250)"},
        {"fecha", "BIGINT"},
    },
    post_cols = {
        {"id", "BIGINT"},
        {"id_thread", "BIGINT"},
        {"contenido", "VARCHAR(250)"},
        {"fecha", "BIGINT"},
        {"path", "VARCHAR(250)"},
    };

  std::string board_str = "board", thread_str = "thread", post_str = "post";

  if(!_megatron.create_table(board_str, board_cols)) return false;
  if(!_megatron.create_table(thread_str, thread_cols)) return false;
  if(!_megatron.create_table(post_str, post_cols)) return false;

  _megatron.load_CSV("csv/board.csv",
                    board_str);

  _megatron.load_CSV("csv/thread.csv",
                    thread_str);

  _megatron.load_CSV("csv/post.csv",
                    post_str);

  _megatron.add_hash_to_table(board_str, board_cols[0].first);
  _megatron.add_hash_to_table(thread_str, thread_cols[0].first);
  _megatron.add_hash_to_table(post_str, post_cols[0].first);

  _megatron.add_index_to_table(thread_str, thread_cols[3].first);
  _megatron.add_index_to_table(post_str, post_cols[3].first);

  return true;
}