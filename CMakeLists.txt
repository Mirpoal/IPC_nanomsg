cmake_minimum_required(VERSION 3.12)
project(nnmsg)

set(CMAKE_CXX_STANDARD 11)

set(NANOMSG_PATH .)

aux_source_directory(${NANOMSG_PATH}/utils DIR_UTILS)
aux_source_directory(${NANOMSG_PATH}/messenger DIR_MESSENGER)
aux_source_directory(${NANOMSG_PATH}/base DIR_BASE)
aux_source_directory(${NANOMSG_PATH}/at_server DIR_AT_SERVER)
aux_source_directory(${NANOMSG_PATH}/system_manager DIR_SYSTEM_MANAGER)

include_directories(D:/cygwin64/usr/local/include/cjson D:/cygwin64/usr/local/include/nanomsg
        ${DIR_UTILS} ${DIR_MESSENGER} ${DIR_BASE} ${DIR_SYSTEM_MANAGER} ${DIR_AT_SERVER})
link_directories(D:/cygwin64/usr/local/lib)

add_definitions(-DDEBUG)

add_executable(${PROJECT_NAME} main.cpp ${DIR_UTILS} ${DIR_MESSENGER} ${DIR_BASE} ${DIR_SYSTEM_MANAGER} ${DIR_AT_SERVER})
target_link_libraries(${PROJECT_NAME} libcjson.a D:/cygwin64/usr/local/lib/libnanomsg.so)