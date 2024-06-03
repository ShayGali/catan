#pragma once
#include <algorithm>
#include <utility>

#include "../player/Player.hpp"
#include "RoadEdge.hpp"
#include "resource.hpp"

class RoadEdge;  // Forward declaration
class Player;    // Forward declaration

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
    LandVertex(int id);


    void set_adjacent_vertex(LandVertex* vertex1, LandVertex* vertex2, LandVertex* vertex3);

    void set_adjacent_edge(RoadEdge* edge1, RoadEdge* edge2, RoadEdge* edge3);

    void set_resource(resource res1, int num1, resource res2, int num2, resource res3, int num3);

    LandVertex* get_adjacent_vertex(int index);

    RoadEdge* get_adjacent_edge(int index);

    pair<resource, int>* get_resources();

    int get_id();

    void print_status();

    Player* get_owner();
    void set_owner(Player* player);

    bool get_isCity();

    void upgrade_to_city();
};