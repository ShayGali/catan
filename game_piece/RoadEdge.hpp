#pragma once
#include "../player/Player.hpp"
#include "LandVertex.hpp"

class LandVertex;  // Forward declaration

class RoadEdge {
   private:
    int id;
    Player* owner;
    vector<LandVertex*> adjacentVertices;
    vector<RoadEdge*> adjacentEdges;

   public:
    RoadEdge();
    RoadEdge(int id);

    void set_adjacent_vertex(LandVertex* vertex1, LandVertex* vertex2);

    void set_adjacent_edge(RoadEdge* edge1, RoadEdge* edge2, RoadEdge* edge3, RoadEdge* edge4);

    LandVertex* get_adjacent_vertex(int index);

    RoadEdge* get_adjacent_edge(int index);

    void set_owner(Player* player);

    Player* get_owner();

    int get_id() const;

    std::string get_color_code();
};