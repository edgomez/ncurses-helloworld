cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

project(ncurses-helloworld
    VERSION 0.0.1
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Curses dependency
# ---------------------------------------------------------------------------

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

# Define a curses library to link against as the module doesn't do it by
# itself

add_library(Curses::Curses INTERFACE IMPORTED GLOBAL)
target_link_libraries(Curses::Curses
    INTERFACE
        ${CURSES_LIBRARIES}
)
target_include_directories(Curses::Curses
    INTERFACE
        ${CURSES_INCLUDE_DIRECTORIES}
)
target_compile_options(Curses::Curses
    INTERFACE
        ${CURSES_CFLAGS}
)

# ---------------------------------------------------------------------------
# The helloworld program
# ---------------------------------------------------------------------------

add_executable(ncurses-helloworld
    "src/main.cpp"
)
target_link_libraries(ncurses-helloworld
    PRIVATE
        Curses::Curses
)
target_compile_features(ncurses-helloworld
    PUBLIC
        cxx_std_11
)
target_compile_options(ncurses-helloworld
    PRIVATE
        "$<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Werror>"
)
