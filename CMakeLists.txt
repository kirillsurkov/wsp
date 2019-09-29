cmake_minimum_required(VERSION 3.10)

project(gameserver)

set(BOOST_INCLUDE_PATH /home/kirill/work/libs/boost)
set(BOOST_LIB_PATH /home/kirill/work/libs/boost/stage/lib)

set(V8_INCLUDE_PATH /home/kirill/work/libs/v8/include)
set(V8_LIB_PATH /home/kirill/work/libs/v8/out.gn/x64.release)

link_directories(${BOOST_LIB_PATH})
link_directories(${V8_LIB_PATH})

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")
add_executable(server ${SOURCES} ${HEADERS})

target_compile_definitions(server PRIVATE -DNDEBUG -DBT_USE_DOUBLE_PRECISION)
target_include_directories(server SYSTEM PRIVATE ${BOOST_INCLUDE_PATH})
target_include_directories(server SYSTEM PRIVATE ${V8_INCLUDE_PATH})
target_include_directories(server PRIVATE ./include)
target_link_libraries(server pthread BulletCollision BulletDynamics LinearMath v8 v8_libplatform)
target_compile_options(server PRIVATE -Wall -Wextra -pedantic --std=c++1z)
