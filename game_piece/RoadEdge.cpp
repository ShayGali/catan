#include "RoadEdge.hpp"
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