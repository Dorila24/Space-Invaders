#ifndef INPUT_H
#define INPUT_H

#include <optional>
#include <curses.h>

#ifdef USE_SFML
    #include <SFML/Window/Keyboard.hpp>
#endif

namespace si {

// Широкий выбор действий
enum class Action { None, MoveLeft, MoveRight, Shoot, Quit, Pause };

class Input {
public:
    static Action poll();
};}
#endif
