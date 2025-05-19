#ifndef ENTITY_H
#define ENTITY_H
#include <cstdint>

namespace si {
    /*виртуалочка*/
    class Entity {
    public:
        Entity(int x, int y, char symbol, uint8_t width = 1, uint8_t height = 1)
            : _x{x}, _y{y}, _symbol{symbol}, _w{width}, _h{height}, _alive{true} {}

        virtual ~Entity() = default;

        // Геттеры
        int x()      const noexcept { return _x;      }
        int y()      const noexcept { return _y;      }
        uint8_t w()  const noexcept { return _w;      }
        uint8_t h()  const noexcept { return _h;      }
        char symbol()const noexcept { return _symbol; }
        bool alive() const noexcept { return _alive;  }

        // Сеты
        void setPos(int x, int y) noexcept { _x = x; _y = y; }
        void kill()            noexcept { _alive = false;    }

        virtual void update(double dt) = 0;

    protected:
        int _x, _y;             //спрайтики
        char _symbol;           //терминальные
        uint8_t _w, _h;
        bool _alive;};}

#endif