#ifdef USE_SFML
#include "SFMLRenderer.h"
#include <stdexcept>

namespace si {

SFMLRenderer::SFMLRenderer(int w,int h)
    : _win{sf::VideoMode({unsigned(w*16),unsigned(h*16)}),
           "Space Invaders",sf::Style::Titlebar|sf::Style::Close},
      _rng{std::random_device{}()}
{
    _win.setVerticalSyncEnabled(true);
    if(!_font.openFromFile("/System/Library/Fonts/Monaco.ttf"))
        throw std::runtime_error("font");
    if(!_texPlayer.loadFromFile("assets/player.png") || !_texEnemy .loadFromFile("assets/enemy.png")  ||
       !_texBullet.loadFromFile("assets/bullet.png") || !_texBoom  .loadFromFile("assets/explosion.png"))
        throw std::runtime_error("PNG not found (run from build/ OR copy assets)");

    // звёздочки
    std::uniform_real_distribution<float> rx(0, w*16), ry(0, h*16), rs(20.f, 70.f);
    for (int i=0;i<120;++i) _stars.push_back({ {rx(_rng), ry(_rng)}, rs(_rng) });}

const sf::Texture& SFMLRenderer::texFor(char c) const
{
    switch (c) {
        case 'A': return _texPlayer;
        case 'V': return _texEnemy;
        case '*': return _texBoom;
        default : return _texBullet;}}

void SFMLRenderer::drawStars(float dt)
{
    sf::RectangleShape dot({2.f,2.f});
    dot.setFillColor(sf::Color(180,180,255));

    for (auto& s : _stars) {
        s.pos.y += s.speed * dt;
        if (s.pos.y > _win.getSize().y) s.pos.y = 0;
        dot.setPosition(s.pos);
        _win.draw(dot);}}

// рамочка
void SFMLRenderer::drawFrame(const std::vector<std::unique_ptr<Entity>>& objs,
    const Player& pl,int score,int lives,
    int wave,bool paused)
{
static sf::Clock clk;
float dt=clk.restart().asSeconds();
_win.clear(sf::Color::Black);
drawStars(dt);

for(auto& e:objs) if(e->alive()){
sf::Sprite s{texFor(e->symbol())};
s.setScale({0.5f,0.5f});
s.setPosition({float(e->x()*16),float(e->y()*16)});
_win.draw(s,(e->symbol()=='|'||e->symbol()=='!')?
sf::BlendAdd:sf::RenderStates::Default);}

sf::Sprite ps{_texPlayer};
ps.setScale({0.5f,0.5f});
ps.setPosition({float(pl.x()*16),float(pl.y()*16)});
_win.draw(ps);

sf::Text hud(_font,
"Score:"+std::to_string(score)+
"  Lives:"+std::to_string(lives)+
"  Wave:"+std::to_string(wave)+
(paused?"  [PAUSE]":""),18);
hud.setFillColor(sf::Color::White);
_win.draw(hud);

_win.display();}

void SFMLRenderer::pollEvents(bool& run,bool& pause)
{
while(auto ev=_win.pollEvent()){
if(ev->is<sf::Event::Closed>()){ run=false; _win.close(); }
else if(ev->is<sf::Event::KeyPressed>()){
auto k = ev->getIf<sf::Event::KeyPressed>()->code;
if(k==sf::Keyboard::Key::Escape || k==sf::Keyboard::Key::Q){
run=false; _win.close();
} else if(k==sf::Keyboard::Key::P){
pause=!pause;}}}}}
#endif