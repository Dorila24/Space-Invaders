//графика
#ifndef SFML_RENDERER_H
#define SFML_RENDERER_H
#ifdef USE_SFML
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <random>
#include "Entity.h"
#include "Player.h"
#include "Explosion.h"

namespace si {

class SFMLRenderer {
public:
    SFMLRenderer(int fieldW, int fieldH);

    void drawFrame(const std::vector<std::unique_ptr<Entity>>& objects, const Player& player, int score, int lives, int wave, bool paused);

    void pollEvents(bool& running, bool& pause);
    bool isOpen() const { return _win.isOpen(); }

private:
    const sf::Texture& texFor(char symbol) const;
    void drawStars(float dt);

    sf::RenderWindow _win;
    sf::Font _font;
    sf::Texture _texPlayer, _texEnemy, _texBullet, _texBoom;

    struct Star { sf::Vector2f pos; float speed; };
    std::vector<Star> _stars;
    std::mt19937 _rng;};}
#endif
#endif