#include "app.h"

#include <board.hpp>
#include <crow.h>

template<typename... Args>
std::string load_html(const std::string& path, Args&&... args) {
  std::ifstream file{"web/" + path};
  if(!file.is_open())
    return "ERROR";

  std::stringstream ss;
  ss << file.rdbuf();

  return std::vformat(ss.str(), std::make_format_args(args...));
}

std::string create_index_html(ResultSet&& boards) {
  std::string content;
  auto create_tag = [&content] (const RegisterEntry& entry) {
    content += std::format(
R"(    <a class="board-link" href="/board/{}">
      <div class="board">
        <div class="title-bar">{} - {}</div>
        {}
      </div>
    </a>)",
    SQL_type_to_string(entry.values[0]),
    SQL_type_to_string(entry.values[1]),
    SQL_type_to_string(entry.values[2]),
    SQL_type_to_string(entry.values[3]));
  };
  for(auto&& col : boards)
    create_tag(col);

  return load_html("index.html", content);
}

std::string convert_date_to_pretty_date(std::string&& date) {
  // 2025-07-29 0453
  date.insert(4, "-");
  date.insert(7, "-");
  date.insert(10, " ");
  date.insert(13, ":");
  
  return date;
}

std::string create_board_html(ResultSet&& board, ResultSet&& threads) {
  std::string content;
  auto create_tag = [&content, &board] (const RegisterEntry& entry) {
    content += std::format(
R"(  <div class="thread">
    <div class="thread-header">
      <a href="/board/{0}/thread/{1}"> >> {1}</a>
      <span class="thread-date">{2}</span>
    </div>
    <div class="thread-body">
      {3}
    </div>
  </div>)",
    SQL_type_to_string(board.registers[0].values[0]),
    SQL_type_to_string(entry.values[0]),
    convert_date_to_pretty_date(SQL_type_to_string(entry.values[3])),
    SQL_type_to_string(entry.values[2])
    );
  };

  for(auto&& col : threads)
    create_tag(col);

  return load_html("board.html",
    SQL_type_to_string(board.registers[0].values[1]),
    SQL_type_to_string(board.registers[0].values[2]),
    content);
}

std::string create_post_html(ResultSet&& board, ResultSet&& thread, ResultSet&& posts) {
  std::string content;
  auto create_tag = [&content, &board] (const RegisterEntry& entry) {
    std::string path = SQL_type_to_string(entry.values[4]);
    std::string img;
    if(!path.empty())
      img = std::format(R"(<img src="/static/{}">)", path);

    content += std::format(
R"(  <div class="post">
    <div class="post-header">Posted on: {}</div>
    <div class="post-body">
      {}
      {}
    </div>
  </div>)",
    convert_date_to_pretty_date(SQL_type_to_string(entry.values[3])),
    SQL_type_to_string(entry.values[2]),
    img
    );
  };

  for(auto&& col : posts)
    create_tag(col);

  return load_html("post.html",
    SQL_type_to_string(board.registers[0].values[1]),
    SQL_type_to_string(thread.registers[0].values[0]),
    SQL_type_to_string(thread.registers[0].values[2]),
    content);
}

int App::run() {
  Board board{_megatron};
  crow::SimpleApp app;

  CROW_ROUTE(app, "/")([&board] {
    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "text/html");
    res.body = create_index_html(board.get_all_boards());
    return res;
  });

  CROW_ROUTE(app, "/board/<int>")
  ([&board](const crow::request& req, int id_board) {
    auto order = req.url_params.get("order");
    ResultSet threads;
    if(!order || strlen(order) == 0) {
      threads = board.get_threads_from_board(id_board);
    } else {
      threads = board.get_threads_from_board_ordered_by_date(id_board, strcmp(order, "asc") == 0);
    }

    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "text/html");
    res.body = create_board_html(board.get_board_on_id(id_board), std::move(threads));
    return res;
  });

  CROW_ROUTE(app, "/board/<int>/thread/<int>")
  ([&board](int id_board, int id_thread){
    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "text/html");
    res.body = create_post_html(board.get_board_on_id(id_board), board.get_thread_on_id(id_thread), board.get_posts_from_thread(id_thread));
    return res;
  });

  app.port(8080).run();

  return 0;
}