macro(add_yaml target)
    find_package(yaml-cpp REQUIRED)
    target_link_libraries(${target} PRIVATE yaml-cpp)
endmacro()

macro(add_thread target)
    find_package(Threads REQUIRED)
    target_link_libraries(${target} PRIVATE Threads::Threads)
endmacro()


macro(add_slamtec name include_dirs)
    target_include_directories(${name} PUBLIC ${PROJECT_SOURCE_DIR}/slamtec/include/)
    link_directories(${PROJECT_SOURCE_DIR}/slamtec/share/)
    target_link_libraries(${name} PUBLIC ${PROJECT_SOURCE_DIR}/slamtec/share/libsl_lidar_sdk.a)
endmacro()

macro(add_boost target)
    find_package(Boost COMPONENTS thread system REQUIRED)
    target_link_libraries(${target} PUBLIC Boost::thread Boost::system)
endmacro()

