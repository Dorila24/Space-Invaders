#ifndef RENDERER_H
#define RENDERER_H
#include <curses.h>
#include <vector>
#include <memory>
#include "Entity.h"
#include "Player.h"
namespace si {
    class Renderer {
    public:
        Renderer(int width, int height);
        ~Renderer();
        template <typename Container>
        void drawFrame(const Container& objects, const Player& player, int score, int lives, int wave, bool paused);
        int width()  const noexcept { return _w; }
        int height() const noexcept { return _h; }

    private:
        int _w, _h;};

    template <typename Container>
    void Renderer::drawFrame(const Container& objs, const Player& player, int score, int lives, int wave, bool paused)
    {
        erase();                    // очистка
        box(stdscr, 0, 0);          // рамка

        // HUD-строка
        mvprintw(0, 2, " Score: %d  Lives: %d  Wave: %d ", score, lives, wave);
        if (paused)
            mvprintw(0, _w - 10, "[PAUSE]");

        for (const auto& ptr : objs) {
            if (!ptr->alive()) continue;
            short col = 3;
            char drawCh = ptr->symbol();
            if (drawCh == 'A') { col = 1; drawCh = '^'; }
            else if (drawCh == 'V') { col = 2; drawCh = 'v'; }
            else if (drawCh == '*') { col = 4; drawCh = '*'; }

            attron(COLOR_PAIR(col));
            mvaddch(ptr->y()+1, ptr->x()+1, drawCh);
            attroff(COLOR_PAIR(col));


        }

        // 5. отдельно рисуем игрока (его нет в _objects)
        mvaddch(player.y() + 1, player.x() + 1, player.symbol());

        refresh();                  // 6. вывод на экран
    }

} // namespace si

#endif // RENDERER_H
