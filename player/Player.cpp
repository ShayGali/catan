#include "Player.hpp"

#include <stdexcept>

#include "../Catan.hpp"

Player::Player(PlayerColor color) {
    this->color = color;
    for (int i = 0; i < 5; i++) {
        resourceCount[i] = 0;
    }
}

void Player::play_turn(Catan &game) {
    throw std::logic_error("Not implemented");
}

int Player::get_victory_points() {
    return victoryPoints;
}

void Player::add_victory_points(int points) {
    victoryPoints += points;
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

std::string Player::get_player_info() {
    std::string info = "Player color: " + get_color() + "\n";
    info += "Resources: \n";
    info += "Wood: " + std::to_string(resourceCount[0]) + "\n";
    info += "Clay: " + std::to_string(resourceCount[1]) + "\n";
    info += "Sheep: " + std::to_string(resourceCount[2]) + "\n";
    info += "Wheat: " + std::to_string(resourceCount[3]) + "\n";
    info += "Stone: " + std::to_string(resourceCount[4]) + "\n";
    return info;
}

int Player::get_resource_count(resource resource) {
    return resourceCount[static_cast<int>(resource)];
}

void Player::add_resource(resource resource, int count) {
    resourceCount[static_cast<int>(resource)] += count;
}

void Player::use_resource(resource resource, int count) {
    if (resourceCount[static_cast<int>(resource)] < count) {
        throw std::invalid_argument("Not enough resources");
    }
    resourceCount[static_cast<int>(resource)] -= count;
}