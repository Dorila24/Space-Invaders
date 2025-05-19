#include "Renderer.h"

namespace si {

Renderer::Renderer(int w, int h) : _w{w}, _h{h}
{
    initscr();
    start_color();
    use_default_colors();
    init_pair(1,COLOR_GREEN, -1);
    init_pair(2,COLOR_RED,   -1);
    init_pair(3,COLOR_YELLOW,-1);
    init_pair(4,COLOR_MAGENTA,-1);

    cbreak();             // делает getch() быстрым
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    if (LINES < h + 2 || COLS < w + 2) {
        endwin();
        throw std::runtime_error("Терминал слишком маленький.");}}

Renderer::~Renderer()
{
    endwin();}}
