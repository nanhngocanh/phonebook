project(demo-sqlite
        LANGUAGES C)
find_package(PkgConfig REQUIRED)
cmake_minimum_required(VERSION 2.6)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_library(sqlite3 STATIC sqlite/sqlite3.c)
include_directories(sqlite)


include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER} -rdynamic)

add_executable(insert insert.c)
add_executable(contact contact-main.c)
foreach (example insert contact)
target_link_libraries(${example} sqlite3 pthread dl)


target_link_libraries(contact ${GTK3_LIBRARIES})
endforeach(example)