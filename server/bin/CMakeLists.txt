add_executable(MessangerServer main.cpp)

target_link_libraries(MessangerServer MessangerServerLib)
target_include_directories(MessangerServer PRIVATE ${PROJECT_SOURCE_DIR})
