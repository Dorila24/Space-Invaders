//Точка входа
#include "Game.h"
#include <exception>
#include <iostream>

int main()
{
    try {
        si::Game game;
        game.run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 1;}
    return 0;}
