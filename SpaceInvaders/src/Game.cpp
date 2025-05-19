#include "Game.h"
#include <algorithm>
#include <random>
#include <thread>
#include <cmath>

namespace si {


static bool overlap(const Entity& a, const Entity& b)
{
    return a.x()<b.x()+b.w() && a.x()+a.w()>b.x() && a.y()<b.y()+b.h() && a.y()+a.h()>b.y();}


Game::Game(int w,int h)
    : _w{w}, _h{h},
      _player{w/2, h-2},
      _renderer{w,h}
#ifdef USE_SFML
    , _sfml{w,h}
#endif
{
    spawnWave();
    _last = std::chrono::steady_clock::now();}


void Game::run()
{
    while (_running) {
        auto now  = std::chrono::steady_clock::now();
        double dt = std::chrono::duration<double>(now - _last).count();
        _last = now;

        Action act = Input::poll();
        if (act==Action::Quit) break;
        if (act==Action::Pause) _paused=!_paused;

        if (!_paused) {
            if (act==Action::MoveLeft)  _player.moveLeft();
            if (act==Action::MoveRight) _player.moveRight(_w-1);
            if (act==Action::Shoot && _player.canShoot())
                _objects.emplace_back(_player.shoot());

            update(dt);
            handleCollisions();}

        _renderer.drawFrame(_objects,_player,_player.score(),_player.lives(),_level,_paused);

#ifdef USE_SFML
        _sfml.pollEvents(_running,_paused);
        if (_sfml.isOpen())
            _sfml.drawFrame(_objects,_player,_player.score(),_player.lives(),_level,_paused);
#endif
        std::this_thread::sleep_for(std::chrono::milliseconds(8));}}


void Game::update(double dt)
{
    _player.update(dt);
    moveFleet(dt);

    for (auto& e: _objects)
        if (e->alive()) e->update(dt);

    _shootTimer += dt;
    if (_shootTimer >= _shootPeriod) {
        _shootTimer = 0.0;

        std::vector<Enemy*> alive;
        for (auto& p: _objects)
            if (auto* e=dynamic_cast<Enemy*>(p.get()); e && e->alive())
                alive.push_back(e);

        if (!alive.empty()) {
            int shooters = std::min(_level, int(alive.size()));
            static std::mt19937 rng{std::random_device{}()};
            std::shuffle(alive.begin(), alive.end(), rng);
            for (int i=0;i<shooters;++i)
                _objects.emplace_back(alive[i]->fire());}}

    /* если врагов не осталось */
    bool any = std::any_of(_objects.begin(),_objects.end(),
        [](auto& p){ return dynamic_cast<Enemy*>(p.get()) && p->alive(); });
    if (!any) nextWave();
}

/* движение нашествия */
void Game::moveFleet(double dt)
{
    int enemies = std::count_if(_objects.begin(),_objects.end(),
        [](auto& p){ return dynamic_cast<Enemy*>(p.get()) && p->alive(); });

    double speed = _fleetSpeed * (1.0 + (enemies?0.7*(1.0-double(enemies)/_maxEnemies):0));
    _fleetX += speed * dt * _dir;
    int dx = int(std::round(_fleetX));
    if (!dx) return;
    _fleetX -= dx;

    bool hitWall=false;
    for (auto& p:_objects)
        if (auto* e=dynamic_cast<Enemy*>(p.get()); e && e->alive()){
            int nx=e->x()+dx;
            if (nx<0 || nx>_w-1){ hitWall=true; break; }}

    if (hitWall){
        for (auto& p:_objects)
            if (auto* e=dynamic_cast<Enemy*>(p.get()); e && e->alive())
                e->setPos(e->x(), e->y()+1);
        _dir = -_dir;
    } else {
        for (auto& p:_objects)
            if (auto* e=dynamic_cast<Enemy*>(p.get()); e && e->alive())
                e->setPos(e->x()+dx, e->y());}}

/* волны */
void Game::spawnWave()
{
    _objects.clear();
    int cols = std::min(_w/3, 10+_level);
    int rows = 5;
    _maxEnemies = cols*rows;

    for (int r=0;r<rows;++r)
        for (int c=0;c<cols;++c)
            _objects.emplace_back(std::make_unique<Enemy>(c*3, 1+r*2));

    _fleetSpeed  = 1.2 + 0.15*_level;
    _shootPeriod = std::max(0.4, 1.5 - 0.1*_level);
    _shootTimer  = 0.0;
    _dir = 1;}
void Game::nextWave(){ ++_level; spawnWave(); }

/* колизии */
void Game::handleCollisions()
{
    /* пуля/пуля */
    for(auto& up:_objects){
        auto* a=dynamic_cast<Bullet*>(up.get());
        if(!a||!a->alive()||a->velocity()>=0) continue;
        for(auto& dn:_objects){
            auto* b=dynamic_cast<Bullet*>(dn.get());
            if(!b||!b->alive()||b->velocity()<=0) continue;
            if(std::abs(a->x()-b->x())<=0 && std::abs(a->y()-b->y())<=1){
                a->kill(); b->kill();
                _objects.emplace_back(std::make_unique<Explosion>(a->x(),a->y()));}}}

    /* пуля/враг */
    for(auto& p:_objects){
        auto* b=dynamic_cast<Bullet*>(p.get());
        if(!b||!b->alive()||b->velocity()>=0) continue;
        for(auto& t:_objects)
            if(auto* e=dynamic_cast<Enemy*>(t.get()); e&&e->alive()&&overlap(*b,*e)){
                e->kill(); b->kill();
                _player.addScore(100);
                _objects.emplace_back(std::make_unique<Explosion>(e->x(),e->y()));
                break;}}

    /* пуля/игрок */
    for(auto& p:_objects){
        auto* b=dynamic_cast<Bullet*>(p.get());
        if(!b||!b->alive()||b->velocity()<=0) continue;
        if(overlap(*b,_player)){
            b->kill();
            _player.hit();
            _objects.emplace_back(std::make_unique<Explosion>(_player.x(),_player.y()));
            if(_player.lives()<=0) _running=false;}}

    std::erase_if(_objects,[](auto& p){ return !p->alive(); });}}
