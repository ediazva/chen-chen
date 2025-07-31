set(BIN_DIR ${CMAKE_BINARY_DIR}/bin/)
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")

add_executable(chenchen
  main.cc
  app.cc)
target_link_libraries(chenchen megatron disk_manager buffer_manager)

file(MAKE_DIRECTORY ${BIN_DIR})
add_custom_command(
  TARGET chenchen
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:chenchen> ${BIN_DIR}
)

add_custom_command(
    TARGET chenchen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BIN_DIR}/csv
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/csv
        ${BIN_DIR}/csv
)