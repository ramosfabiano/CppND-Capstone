# boost
find_package(Boost REQUIRED)

set(HTTP_SERVER_EXECUTABLE_NAME http_server)

add_executable(${HTTP_SERVER_EXECUTABLE_NAME} main.cpp )
target_include_directories(${HTTP_SERVER_EXECUTABLE_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/lib/http_server ${PROJECT_SOURCE_DIR}/src/lib/logger)
target_link_libraries(${HTTP_SERVER_EXECUTABLE_NAME} PUBLIC http_server_lib pthread boost_program_options)
set_target_properties(${HTTP_SERVER_EXECUTABLE_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

install (TARGETS ${HTTP_SERVER_EXECUTABLE_NAME} DESTINATION ./)
