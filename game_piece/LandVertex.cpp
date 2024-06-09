#include "LandVertex.hpp"

#include <iostream>

LandVertex::LandVertex()
    : id(-1), owner(nullptr), isCity(false) {}
    
LandVertex::LandVertex(int id)
    : id(id), owner(nullptr), isCity(false), adjacentVertices(3, nullptr), adjacentEdges(3, nullptr), resources(3) {
}

void LandVertex::set_adjacent_vertex(LandVertex* vertex1, LandVertex* vertex2, LandVertex* vertex3) {
    this->adjacentVertices[0] = vertex1;
    this->adjacentVertices[1] = vertex2;
    this->adjacentVertices[2] = vertex3;
}

void LandVertex::set_adjacent_edge(RoadEdge* edge1, RoadEdge* edge2, RoadEdge* edge3) {
    this->adjacentEdges[0] = edge1;
    this->adjacentEdges[1] = edge2;
    this->adjacentEdges[2] = edge3;
}

void LandVertex::set_resource(resource res1, int num1, resource res2, int num2, resource res3, int num3) {
    this->resources[0] = std::make_pair(res1, num1);
    this->resources[1] = std::make_pair(res2, num2);
    this->resources[2] = std::make_pair(res3, num3);
}

LandVertex* LandVertex::get_adjacent_vertex(int index) {
    return this->adjacentVertices[index];
}

RoadEdge* LandVertex::get_adjacent_edge(int index) {
    return this->adjacentEdges[index];
}

vector<pair<resource, int>> LandVertex::get_resources() {
    return this->resources;
}

int LandVertex::get_id() const {
    return this->id;
}

Player* LandVertex::get_owner() {
    return this->owner;
}

void LandVertex::set_owner(Player* player) {
    this->owner = player;
}

bool LandVertex::is_contains_city() const {
    return this->isCity;
}

void LandVertex::upgrade_to_city() {
    this->isCity = true;
}

std::string LandVertex::get_settlement_string() {
    if (this->owner == nullptr) {
        // empty vertex emoji
        return "E";
    }
    std::string player_color_code = this->owner->get_color_code();
    std::string RESET_COLOR = "\033[0m";

    string res = player_color_code;

    if (this->isCity) {
        res += "🏙" + RESET_COLOR;
    } else {
        res += "🏘" + RESET_COLOR;
    }

    return res;
}