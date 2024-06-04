#include "RoadEdge.hpp"
RoadEdge::RoadEdge(int id) : id(id) {
    this->owner = nullptr;
}

void RoadEdge::set_adjacent_vertex(LandVertex* vertex1, LandVertex* vertex2) {
    adjacentVertices[0] = vertex1;
    adjacentVertices[1] = vertex2;
}

void RoadEdge::set_adjacent_edge(RoadEdge* edge1, RoadEdge* edge2, RoadEdge* edge3, RoadEdge* edge4) {
    adjacentEdges[0] = edge1;
    adjacentEdges[1] = edge2;
    adjacentEdges[2] = edge3;
    adjacentEdges[3] = edge4;
}

LandVertex* RoadEdge::get_adjacent_vertex(int index) {
    return this->adjacentVertices[index];
}

RoadEdge* RoadEdge::get_adjacent_edge(int index) {
    return this->adjacentEdges[index];
}

void RoadEdge::set_owner(Player* player) {
    this->owner = player;
}

Player* RoadEdge::get_owner() {
    return this->owner;
}
int RoadEdge::get_id() {
    return this->id;
}

std::string RoadEdge::get_color_code() {
    if (this->owner == nullptr) {
        return "\033[1;37m"; // White
    }
    return this->owner->get_color_code();
}