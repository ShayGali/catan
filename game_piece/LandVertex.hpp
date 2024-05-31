#pragma once
#include <algorithm>
#include <utility>

#include "../player/Player.hpp"
#include "RoadEdge.hpp"
#include "resource.hpp"

class RoadEdge;  // Forward declaration

using std::pair;

class LandVertex {
   private:
    int id;
    Player* owner;
    bool isCity;
    LandVertex* adjacentVertices[3];
    RoadEdge* adjacentEdges[3];
    pair<resource, int> resources[3];  // Changed the variable name from 'resource' to 'resources'

   public:
    LandVertex() = default;
    LandVertex(int id) : id(id) {
        this->owner = nullptr;
        this->isCity = false;
    }

    void set_owner(Player* player) {
        this->owner = player;
    }

    void set_adjacent_vertex(LandVertex* vertex1, LandVertex* vertex2, LandVertex* vertex3);

    void set_adjacent_edge(RoadEdge* edge1, RoadEdge* edge2, RoadEdge* edge3);

    void set_resource(resource res1, int num1, resource res2, int num2, resource res3, int num3);

    int get_id() {
        return this->id;
    }

    void print_status();
};