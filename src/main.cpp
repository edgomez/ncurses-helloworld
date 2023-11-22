/* --------------------------------------------------------------------------
 * Copyright (C) 2023 Edouard Gomez
 * SPDX-License-Identifier: MIT
 *  ---------------------------------------------------------------------- */

#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <thread>

#include <ncurses.h>
#include <signal.h>

namespace
{
/** Application context */
struct ApplicationContext
{
    int w; /**< Width of the terminal */
    int h; /**< Height of the terminal */
};

/** Global application context.
 * 
 * This allows the signal handler to record the new window size when used
 * within a windowed terminal
*/
ApplicationContext g_app;

/** Screen Size change signal handler present before installing this app's one */
void (*previousHandleScreenResize)(int sig) = nullptr;

/** Screen Resize signal handler
 * @param sig Signal number being raised
*/
void handleScreenResize(int sig)
{
    if (previousHandleScreenResize)
    {
        previousHandleScreenResize(sig);
    }

    clear();
    refresh();
    getmaxyx(stdscr, g_app.h, g_app.w);
}
}

/** Main program entry point
 * @param argc number of arguments
 * @param argv array of program's arguments
 * @retval #EXIT_SUCCESS when everything went well
 * @retval #EXIT_FAILURE when an error occured
*/
extern "C" int main(int /* argc */, char** /* argv */)
{
    initscr();

    getmaxyx(stdscr, g_app.h, g_app.w);

    previousHandleScreenResize = signal(SIGWINCH, handleScreenResize);

    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    /** poor's man RAII :-) */
    struct CursesCleanExit
    {
        ~CursesCleanExit()
        {
            endwin();
        }
    };
    CursesCleanExit cursesCleanExit;

    std::chrono::time_point next = std::chrono::high_resolution_clock::now();
    bool exit = false;
    while (!exit)
    {
        next += std::chrono::milliseconds(250);

        int ch = getch();
        switch (ch)
        {
        case 'q':
            exit = true;
            break;
        default:
            break;
        }

        wborder(stdscr, 0, 0, 0, 0, 0, 0, 0, 0);

        static constexpr char hello[] = "Hello World !";
        static constexpr int helloLen = static_cast<int>(sizeof(hello));
        wmove(stdscr, g_app.h/2, std::max(0, (g_app.w - helloLen) /2 ));
        waddstr(stdscr, hello);

        static constexpr char quitMsg[] = "(Press 'q' to quit)";
        static constexpr int quitMsgLen = static_cast<int>(sizeof(quitMsg));
        wmove(stdscr, g_app.h/2 + 1, std::max(0, (g_app.w - quitMsgLen) /2 ));
        waddstr(stdscr, quitMsg);

        wrefresh(stdscr);

        std::this_thread::sleep_until(next);
    }
    
    return EXIT_SUCCESS;
}
