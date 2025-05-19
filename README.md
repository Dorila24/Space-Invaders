# Space Invaders (C++17 • ncurses • SFML)

Учебная реализация классической аркады **Space Invaders** для терминала
(ncurses) с опциональным графическим режимом (SFML).
В проекте реализованы все требования к работе:

* **Полноценная ООП-архитектура** (Entity → Player/Enemy/Bullet/Explosion, класс‐координатор Game).
* Игровые волны, уровни сложности, жизни, очки, экраны «Game Over» / «Victory».
* **ncurses**: плавная анимация в терминале.
* **SFML** (по опции `-DUSE_SFML=OFF/ON`).
* 100 % кроссплатформенно; проверено на macOS (Apple Clang) и Linux (g++).

---

## Установка зависимостей (macOS)

```bash
brew update
brew install cmake ncurses sfml
```


## Сборка

```bash
cd SpaceInvaders
mkdir build && cd build
# Вариант 1. Терминальная версия
cmake .. -DUSE_SFML=OFF
make -j
# Вариант 2. Графическая версия 
cmake .. -DUSE_SFML=ON
make -j
```

## Запуск

```bash
./space_invaders
```

## Управление

```bash
Клавиша     Действие

← / →	    Перемещение корабля влево/вправо
SPACE	    Выстрел
P	        Пауза / продолжить
Q	        Выйти из игры
```
