#include "LandVertex.hpp"

#include <iostream>

LandVertex::LandVertex(int id) : id(id) {
    this->owner = nullptr;
    this->isCity = false;
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

void LandVertex::print_status() {
    std::cout << "Vertex ID: " << this->id << std::endl;
    std::cout << "resources: " << std::endl;
    for (int i = 0; i < 3; i++) {
        if (this->resources[i].first != resource::NONE) {
            std::cout << "\t" << this->resources[i].first.get_emoji() << " : " << this->resources[i].second << std::endl;
        }
    }

    if (this->owner != nullptr) {
        std::cout << "Owner: " << this->owner->get_color() << std::endl;
    } else {
        std::cout << "No owner" << std::endl;
    }
}

LandVertex* LandVertex::get_adjacent_vertex(int index) {
    return this->adjacentVertices[index];
}

RoadEdge* LandVertex::get_adjacent_edge(int index) {
    return this->adjacentEdges[index];
}

pair<resource, int>* LandVertex::get_resources() {
    return this->resources;
}

int LandVertex::get_id() {
    return this->id;
}

Player* LandVertex::get_owner() {
    return this->owner;
}

void LandVertex::set_owner(Player* player) {
    this->owner = player;
}

bool LandVertex::get_isCity() {
    return this->isCity;
}

void LandVertex::upgrade_to_city() {
    this->isCity = true;
}