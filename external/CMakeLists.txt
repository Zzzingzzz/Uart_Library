add_library(external external.h)

add_subdirectory(serial)

target_link_libraries(external PUBLIC serial)
