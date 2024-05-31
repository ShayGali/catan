#pragma once
#include "../game_piece/resource.hpp"

enum class PlayerColor {
    RED,
    BLUE,
    YELLOW
};

class Player {
   private:
    PlayerColor color;
    int resourceCount[5];  // WOOD, CLAY, SHEEP, WHEAT, STONE

   public:
    Player(PlayerColor color);

    std::string get_color();
};