#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Explosion.h"
#include "Renderer.h"
#ifdef USE_SFML
#include "SFMLRenderer.h"
#endif
#include "Input.h"

#include <vector>
#include <memory>
#include <chrono>

namespace si {

class Game {
public:
    explicit Game(int width = 40, int height = 25);
    void run();

private:
    void update(double dt);
    void spawnWave();
    void nextWave();
    void moveFleet(double dt);
    void handleCollisions();

    const int _w, _h;

    Player _player;
    std::vector<std::unique_ptr<Entity>> _objects;

    /* параметры армии */
    int    _level        {1};
    int    _dir          {1};
    int    _maxEnemies   {0};
    double _fleetX       {0.0};
    double _fleetSpeed   {2.0};
    double _shootTimer   {0.0};
    double _shootPeriod  {1.5};

    Renderer _renderer;
#ifdef USE_SFML
    SFMLRenderer _sfml;
#endif

    bool _running{true};
    bool _paused {false};
    std::chrono::steady_clock::time_point _last;};}
    
#endif
