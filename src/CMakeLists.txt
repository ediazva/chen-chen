set(BIN_DIR ${CMAKE_BINARY_DIR}/bin/)
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")

add_executable(chenchen main.cc)
target_link_libraries(chenchen Crow::Crow)

file(MAKE_DIRECTORY ${BIN_DIR})
add_custom_command(
  TARGET chenchen
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:chenchen> ${BIN_DIR}
)