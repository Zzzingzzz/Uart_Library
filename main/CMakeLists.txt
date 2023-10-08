file(GLOB_RECURSE main_srcs CONFIGURE_DEPENDS include/*.hpp sources/main.cpp)

add_executable(run ${main_srcs})

add_thread(run)

target_include_directories(run PUBLIC include)
target_link_libraries(run PUBLIC external)

add_executable(run2 include/main.hpp sources/main2.cpp)

target_include_directories(run2 PUBLIC include)
target_link_libraries(run2 PUBLIC external)