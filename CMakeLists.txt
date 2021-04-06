cmake_minimum_required(VERSION 3.0)

project(roboclaw_test)

add_library(roboclaw library/roboclaw.c)

install(TARGETS roboclaw DESTINATION lib)
install(FILES include/roboclaw.h DESTINATION include)
install(FILES include/conio.h DESTINATION include)


add_executable(${PROJECT_NAME} examples/jeevan.cpp)

target_link_libraries(${PROJECT_NAME} roboclaw)
