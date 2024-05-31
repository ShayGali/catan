#include "Player.hpp"

Player::Player(PlayerColor color) {
    this->color = color;
    for (int i = 0; i < 5; i++) {
        resourceCount[i] = 0;
    }
}

std::string Player::get_color() {
    switch (this->color) {
        case PlayerColor::RED:
            return "RED";
        case PlayerColor::BLUE:
            return "BLUE";
        case PlayerColor::YELLOW:
            return "YELLOW";
    }
}