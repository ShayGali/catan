// @author: shay.gali@msmail.ariel.ac.il
#pragma once
#include "../player/Player.hpp"
#include "LandVertex.hpp"

class LandVertex;  // Forward declaration

/**
 * @brief RoadEdge class
 * This class represents the road edge in the game.
 * Each road edge has an id, owner, adjacent vertices and adjacent edges.
 */
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

    /**
     * @brief Get the color code of the owner of the road edge
     * @return std::string the color code of the owner
     */
    std::string get_color_code();
};