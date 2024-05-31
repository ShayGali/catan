#pragma once
#include "../player/Player.hpp"
#include "LandVertex.hpp"

class LandVertex;  // Forward declaration

class RoadEdge {
   private:
    int id;
    Player* owner;
    LandVertex* adjacentVertices[2];
    RoadEdge* adjacentEdges[4];

   public:
    RoadEdge() = default;
    RoadEdge(int id) : id(id) {
        this->owner = nullptr;
    }

    void set_adjacent_vertex(LandVertex* vertex1, LandVertex* vertex2);

    void set_adjacent_edge(RoadEdge* edge1, RoadEdge* edge2, RoadEdge* edge3, RoadEdge* edge4);

    int get_id() {
        return this->id;
    }
};