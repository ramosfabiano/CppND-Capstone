# boost
find_package(Boost REQUIRED)

set (HTTP_SERVER_LIBRARY_NAME http_server_lib)

add_library(${HTTP_SERVER_LIBRARY_NAME} STATIC httpserver.cpp serversocket.cpp requestsocket.cpp requesthandler.cpp getmethodhandler.cpp headmethodhandler.cpp)
target_include_directories(${HTTP_SERVER_LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/lib/common ${PROJECT_SOURCE_DIR}/src/lib/logger ${PROJECT_SOURCE_DIR}/src/lib/threadpool)
target_link_libraries(${HTTP_SERVER_LIBRARY_NAME} PUBLIC boost_filesystem)
set_target_properties (${HTTP_SERVER_LIBRARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})
set_target_properties(${HTTP_SERVER_LIBRARY_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

#install (TARGETS ${HTTP_SERVER_LIBRARY_NAME} DESTINATION ./)


