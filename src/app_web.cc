#include "app.h"

#include <board.hpp>
#include <crow.h>

std::string create_index_html(ResultSet&& boards) {
  const std::string top =
R"(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Boards - Chen Chen</title>
  <link href="/static/board.css" rel="stylesheet">
</head>
<body>
  <h1>ChenChen Boards</h1>
  <div class="container">
)";
  const std::string bottom =
R"(  </div>
</body>
</html>
)";
  
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

  return top + content + bottom;
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
  const std::string top =  std::format(
R"(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Board - {0} - {1}</title>
  <link href="/static/thread.css" rel="stylesheet">
</head>
<body>
  <h1>{0} - {1}</h1>
)",
SQL_type_to_string(board.registers[0].values[1]),
SQL_type_to_string(board.registers[0].values[2]));

  const std::string bottom = 
R"(</body>
</html>
)";
  
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

  return top + content + bottom;
}

std::string create_post_html(ResultSet&& board, ResultSet&& thread, ResultSet&& posts) {
  const std::string top =  std::format(
R"(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>{0} - Thread #{1}</title>
  <link href="/static/post.css" rel="stylesheet">
</head>
<body>
  <h1>{0} - Thread #{1}: {2}</h1>
)",
SQL_type_to_string(board.registers[0].values[1]),
SQL_type_to_string(thread.registers[0].values[0]),
SQL_type_to_string(thread.registers[0].values[2]));

  const std::string bottom = 
R"(</body>
</html>
)";

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

  return top + content + bottom;
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
  ([&board](int id_board) {
    crow::response res;
    res.code = 200;
    res.set_header("Content-Type", "text/html");
    res.body = create_board_html(board.get_board_on_id(id_board), board.get_threads_from_board(id_board));
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