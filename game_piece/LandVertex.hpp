#pragma once
#include <algorithm>
#include <utility>

#include "../player/Player.hpp"
#include "RoadEdge.hpp"
#include "resource.hpp"

class RoadEdge;  // Forward declaration
class Player;    // Forward declaration

using std::pair;

/**
 * @brief A class representing a vertex on the land.
 * will store the resources that the vertex can produce, the adjacent vertices and edges, and the owner of the vertex.
 */
class LandVertex {
   private:
    int id;
    Player* owner;
    bool isCity;
    vector<LandVertex*> adjacentVertices;
    vector<RoadEdge*> adjacentEdges;
    vector<pair<resource, int>> resources;

   public:
    /**
     * @brief dummy default constructor so we can create a vector of LandVertex in fixed size
     */
    LandVertex();
    LandVertex(int id);

    void set_adjacent_vertex(LandVertex* vertex1, LandVertex* vertex2, LandVertex* vertex3);

    void set_adjacent_edge(RoadEdge* edge1, RoadEdge* edge2, RoadEdge* edge3);

    void set_resource(resource res1, int num1, resource res2, int num2, resource res3, int num3);

    LandVertex* get_adjacent_vertex(int index);

    RoadEdge* get_adjacent_edge(int index);

    vector<pair<resource, int>> get_resources();

    int get_id() const;

    Player* get_owner();
    void set_owner(Player* player);

    /**
     * @brief check if the vertex is a city
     */
    bool is_contains_city() const;
    /**
     * @brief make the vertex a city
     */
    void upgrade_to_city();

    /**
     * @brief get the string representation of the settlement.
     * E - empty, 🏘 - settlement, 🏙 - city.
     * the emoji is colored according to the player color
     */
    std::string get_settlement_string();
};