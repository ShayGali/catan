#include "Catan.hpp"

Catan::Catan(Player player1, Player player2, Player player3) : players{player1, player2, player3} {
    init_game();
}

void Catan::init_game() {
    for (int i = 0; i < 54; i++) {
        vertices[i] = LandVertex(i);
    }

    for (int i = 0; i < 72; i++) {
        edges[i] = RoadEdge(i);
    }
    init_vertices();
    init_edges();
    init_board();
}

void Catan::init_vertices() {
    // first vertex row
    vertices[0].set_adjacent_vertex(&vertices[3], &vertices[4], nullptr);
    vertices[1].set_adjacent_vertex(&vertices[4], &vertices[5], nullptr);
    vertices[2].set_adjacent_vertex(&vertices[5], &vertices[6], nullptr);

    vertices[0].set_adjacent_edge(&edges[0], &edges[1], nullptr);
    vertices[1].set_adjacent_edge(&edges[2], &edges[3], nullptr);
    vertices[2].set_adjacent_edge(&edges[4], &edges[5], nullptr);

    // second vertex row
    vertices[3].set_adjacent_vertex(&vertices[0], &vertices[7], nullptr);
    vertices[4].set_adjacent_vertex(&vertices[0], &vertices[1], &vertices[8]);
    vertices[5].set_adjacent_vertex(&vertices[1], &vertices[2], &vertices[9]);
    vertices[6].set_adjacent_vertex(&vertices[2], &vertices[10], nullptr);

    vertices[3].set_adjacent_edge(&edges[0], &edges[6], nullptr);
    vertices[4].set_adjacent_edge(&edges[1], &edges[2], &edges[7]);
    vertices[5].set_adjacent_edge(&edges[3], &edges[4], &edges[8]);
    vertices[6].set_adjacent_edge(&edges[5], &edges[9], nullptr);

    // third vertex row
    vertices[7].set_adjacent_vertex(&vertices[3], &vertices[11], &vertices[12]);
    vertices[8].set_adjacent_vertex(&vertices[4], &vertices[12], &vertices[13]);
    vertices[9].set_adjacent_vertex(&vertices[5], &vertices[13], &vertices[14]);
    vertices[10].set_adjacent_vertex(&vertices[6], &vertices[14], &vertices[15]);

    vertices[7].set_adjacent_edge(&edges[6], &edges[10], &edges[11]);
    vertices[8].set_adjacent_edge(&edges[7], &edges[12], &edges[13]);
    vertices[9].set_adjacent_edge(&edges[8], &edges[14], &edges[15]);
    vertices[10].set_adjacent_edge(&edges[9], &edges[16], &edges[17]);

    // fourth vertex row
    vertices[11].set_adjacent_vertex(&vertices[7], &vertices[16], nullptr);
    vertices[12].set_adjacent_vertex(&vertices[7], &vertices[8], &vertices[17]);
    vertices[13].set_adjacent_vertex(&vertices[8], &vertices[9], &vertices[18]);
    vertices[14].set_adjacent_vertex(&vertices[9], &vertices[10], &vertices[19]);
    vertices[15].set_adjacent_vertex(&vertices[10], &vertices[20], nullptr);

    vertices[11].set_adjacent_edge(&edges[10], &edges[18], nullptr);
    vertices[12].set_adjacent_edge(&edges[11], &edges[12], &edges[19]);
    vertices[13].set_adjacent_edge(&edges[13], &edges[14], &edges[20]);
    vertices[14].set_adjacent_edge(&edges[15], &edges[16], &edges[21]);
    vertices[15].set_adjacent_edge(&edges[17], &edges[22], nullptr);

    // fifth vertex row
    vertices[16].set_adjacent_vertex(&vertices[11], &vertices[21], &vertices[22]);
    vertices[17].set_adjacent_vertex(&vertices[12], &vertices[22], &vertices[23]);
    vertices[18].set_adjacent_vertex(&vertices[13], &vertices[23], &vertices[24]);
    vertices[19].set_adjacent_vertex(&vertices[14], &vertices[24], &vertices[25]);
    vertices[20].set_adjacent_vertex(&vertices[15], &vertices[25], &vertices[26]);

    vertices[16].set_adjacent_edge(&edges[18], &edges[23], &edges[24]);
    vertices[17].set_adjacent_edge(&edges[19], &edges[25], &edges[26]);
    vertices[18].set_adjacent_edge(&edges[20], &edges[27], &edges[28]);
    vertices[19].set_adjacent_edge(&edges[21], &edges[29], &edges[30]);
    vertices[20].set_adjacent_edge(&edges[22], &edges[31], &edges[32]);

    // sixth vertex row
    vertices[21].set_adjacent_vertex(&vertices[16], &vertices[27], nullptr);
    vertices[22].set_adjacent_vertex(&vertices[16], &vertices[17], &vertices[28]);
    vertices[23].set_adjacent_vertex(&vertices[17], &vertices[18], &vertices[29]);
    vertices[24].set_adjacent_vertex(&vertices[18], &vertices[19], &vertices[30]);
    vertices[25].set_adjacent_vertex(&vertices[19], &vertices[20], &vertices[31]);
    vertices[26].set_adjacent_vertex(&vertices[20], &vertices[32], nullptr);

    vertices[21].set_adjacent_edge(&edges[23], &edges[33], nullptr);
    vertices[22].set_adjacent_edge(&edges[24], &edges[25], &edges[34]);
    vertices[23].set_adjacent_edge(&edges[26], &edges[27], &edges[35]);
    vertices[24].set_adjacent_edge(&edges[28], &edges[29], &edges[36]);
    vertices[25].set_adjacent_edge(&edges[30], &edges[31], &edges[37]);
    vertices[26].set_adjacent_edge(&edges[32], &edges[38], nullptr);

    // seventh vertex row
    vertices[27].set_adjacent_vertex(&vertices[21], &vertices[33], nullptr);
    vertices[28].set_adjacent_vertex(&vertices[22], &vertices[33], &vertices[34]);
    vertices[29].set_adjacent_vertex(&vertices[23], &vertices[34], &vertices[35]);
    vertices[30].set_adjacent_vertex(&vertices[24], &vertices[35], &vertices[36]);
    vertices[31].set_adjacent_vertex(&vertices[25], &vertices[36], &vertices[37]);
    vertices[32].set_adjacent_vertex(&vertices[26], &vertices[37], nullptr);

    vertices[27].set_adjacent_edge(&edges[33], &edges[39], nullptr);
    vertices[28].set_adjacent_edge(&edges[34], &edges[40], &edges[41]);
    vertices[29].set_adjacent_edge(&edges[35], &edges[42], &edges[43]);
    vertices[30].set_adjacent_edge(&edges[36], &edges[44], &edges[45]);
    vertices[31].set_adjacent_edge(&edges[37], &edges[46], &edges[47]);
    vertices[32].set_adjacent_edge(&edges[38], &edges[48], nullptr);

    // eighth vertex row
    vertices[33].set_adjacent_vertex(&vertices[27], &vertices[28], &vertices[38]);
    vertices[34].set_adjacent_vertex(&vertices[28], &vertices[29], &vertices[39]);
    vertices[35].set_adjacent_vertex(&vertices[29], &vertices[30], &vertices[40]);
    vertices[36].set_adjacent_vertex(&vertices[30], &vertices[31], &vertices[41]);
    vertices[37].set_adjacent_vertex(&vertices[31], &vertices[32], &vertices[42]);

    vertices[33].set_adjacent_edge(&edges[39], &edges[40], &edges[49]);
    vertices[34].set_adjacent_edge(&edges[41], &edges[42], &edges[50]);
    vertices[35].set_adjacent_edge(&edges[43], &edges[44], &edges[51]);
    vertices[36].set_adjacent_edge(&edges[45], &edges[46], &edges[52]);
    vertices[37].set_adjacent_edge(&edges[47], &edges[48], &edges[53]);

    // ninth vertex row
    vertices[38].set_adjacent_vertex(&vertices[33], &vertices[43], nullptr);
    vertices[39].set_adjacent_vertex(&vertices[34], &vertices[43], &vertices[44]);
    vertices[40].set_adjacent_vertex(&vertices[35], &vertices[44], &vertices[45]);
    vertices[41].set_adjacent_vertex(&vertices[36], &vertices[45], &vertices[46]);
    vertices[42].set_adjacent_vertex(&vertices[37], &vertices[46], nullptr);

    vertices[38].set_adjacent_edge(&edges[49], &edges[54], nullptr);
    vertices[39].set_adjacent_edge(&edges[50], &edges[55], &edges[56]);
    vertices[40].set_adjacent_edge(&edges[51], &edges[57], &edges[58]);
    vertices[41].set_adjacent_edge(&edges[52], &edges[59], &edges[60]);
    vertices[42].set_adjacent_edge(&edges[53], &edges[61], nullptr);

    // tenth vertex row
    vertices[43].set_adjacent_vertex(&vertices[38], &vertices[39], &vertices[47]);
    vertices[44].set_adjacent_vertex(&vertices[39], &vertices[40], &vertices[48]);
    vertices[45].set_adjacent_vertex(&vertices[40], &vertices[41], &vertices[49]);
    vertices[46].set_adjacent_vertex(&vertices[41], &vertices[42], &vertices[50]);

    vertices[43].set_adjacent_edge(&edges[54], &edges[55], &edges[62]);
    vertices[44].set_adjacent_edge(&edges[56], &edges[57], &edges[63]);
    vertices[45].set_adjacent_edge(&edges[58], &edges[59], &edges[64]);
    vertices[46].set_adjacent_edge(&edges[60], &edges[61], &edges[65]);

    // eleventh vertex row
    vertices[47].set_adjacent_vertex(&vertices[43], &vertices[51], nullptr);
    vertices[48].set_adjacent_vertex(&vertices[44], &vertices[51], &vertices[52]);
    vertices[49].set_adjacent_vertex(&vertices[45], &vertices[52], &vertices[53]);
    vertices[50].set_adjacent_vertex(&vertices[46], &vertices[53], nullptr);

    vertices[47].set_adjacent_edge(&edges[62], &edges[66], nullptr);
    vertices[48].set_adjacent_edge(&edges[63], &edges[67], &edges[68]);
    vertices[49].set_adjacent_edge(&edges[64], &edges[69], &edges[70]);
    vertices[50].set_adjacent_edge(&edges[65], &edges[71], nullptr);

    // twelfth vertex row
    vertices[51].set_adjacent_vertex(&vertices[47], &vertices[48], nullptr);
    vertices[52].set_adjacent_vertex(&vertices[48], &vertices[49], nullptr);
    vertices[53].set_adjacent_vertex(&vertices[49], &vertices[50], nullptr);

    vertices[51].set_adjacent_edge(&edges[66], &edges[67], nullptr);
    vertices[52].set_adjacent_edge(&edges[68], &edges[69], nullptr);
    vertices[53].set_adjacent_edge(&edges[70], &edges[71], nullptr);
}

void Catan::init_edges() {
    // first edge row
    edges[0].set_adjacent_edge(&edges[1], &edges[6], nullptr, nullptr);
    edges[1].set_adjacent_edge(&edges[0], &edges[2], &edges[7], nullptr);
    edges[2].set_adjacent_edge(&edges[1], &edges[7], &edges[3], nullptr);
    edges[3].set_adjacent_edge(&edges[2], &edges[8], &edges[4], nullptr);
    edges[4].set_adjacent_edge(&edges[3], &edges[8], &edges[5], nullptr);
    edges[5].set_adjacent_edge(&edges[4], &edges[9], nullptr, nullptr);

    edges[0].set_adjacent_vertex(&vertices[0], &vertices[3]);
    edges[1].set_adjacent_vertex(&vertices[0], &vertices[4]);
    edges[2].set_adjacent_vertex(&vertices[1], &vertices[4]);
    edges[3].set_adjacent_vertex(&vertices[1], &vertices[5]);
    edges[4].set_adjacent_vertex(&vertices[2], &vertices[5]);
    edges[5].set_adjacent_vertex(&vertices[2], &vertices[6]);

    // second edge row
    edges[6].set_adjacent_edge(&edges[0], &edges[10], &edges[11], nullptr);
    edges[7].set_adjacent_edge(&edges[1], &edges[2], &edges[12], &edges[13]);
    edges[8].set_adjacent_edge(&edges[3], &edges[4], &edges[14], &edges[15]);
    edges[9].set_adjacent_edge(&edges[5], &edges[16], &edges[17], nullptr);

    edges[6].set_adjacent_vertex(&vertices[3], &vertices[7]);
    edges[7].set_adjacent_vertex(&vertices[4], &vertices[8]);
    edges[8].set_adjacent_vertex(&vertices[5], &vertices[9]);
    edges[9].set_adjacent_vertex(&vertices[6], &vertices[10]);

    // third edge row
    edges[10].set_adjacent_edge(&edges[6], &edges[11], &edges[18], nullptr);
    edges[11].set_adjacent_edge(&edges[6], &edges[10], &edges[12], &edges[19]);
    edges[12].set_adjacent_edge(&edges[7], &edges[11], &edges[13], &edges[19]);
    edges[13].set_adjacent_edge(&edges[7], &edges[12], &edges[14], &edges[20]);
    edges[14].set_adjacent_edge(&edges[8], &edges[13], &edges[15], &edges[20]);
    edges[15].set_adjacent_edge(&edges[8], &edges[14], &edges[16], &edges[21]);
    edges[16].set_adjacent_edge(&edges[9], &edges[15], &edges[17], &edges[21]);
    edges[17].set_adjacent_edge(&edges[9], &edges[16], &edges[22], nullptr);

    edges[10].set_adjacent_vertex(&vertices[7], &vertices[11]);
    edges[11].set_adjacent_vertex(&vertices[7], &vertices[12]);
    edges[12].set_adjacent_vertex(&vertices[8], &vertices[12]);
    edges[13].set_adjacent_vertex(&vertices[8], &vertices[13]);
    edges[14].set_adjacent_vertex(&vertices[9], &vertices[13]);
    edges[15].set_adjacent_vertex(&vertices[9], &vertices[14]);
    edges[16].set_adjacent_vertex(&vertices[10], &vertices[14]);
    edges[17].set_adjacent_vertex(&vertices[10], &vertices[15]);

    // fourth edge row
    edges[18].set_adjacent_edge(&edges[10], &edges[23], &edges[24], nullptr);
    edges[19].set_adjacent_edge(&edges[11], &edges[12], &edges[25], &edges[26]);
    edges[20].set_adjacent_edge(&edges[13], &edges[14], &edges[27], &edges[28]);
    edges[21].set_adjacent_edge(&edges[15], &edges[16], &edges[29], &edges[30]);
    edges[22].set_adjacent_edge(&edges[17], &edges[31], &edges[32], nullptr);

    edges[18].set_adjacent_vertex(&vertices[11], &vertices[16]);
    edges[19].set_adjacent_vertex(&vertices[12], &vertices[17]);
    edges[20].set_adjacent_vertex(&vertices[13], &vertices[18]);
    edges[21].set_adjacent_vertex(&vertices[14], &vertices[19]);
    edges[22].set_adjacent_vertex(&vertices[15], &vertices[20]);

    // fifth edge row
    edges[23].set_adjacent_edge(&edges[18], &edges[24], &edges[33], nullptr);
    edges[24].set_adjacent_edge(&edges[18], &edges[23], &edges[25], &edges[34]);
    edges[25].set_adjacent_edge(&edges[19], &edges[24], &edges[26], &edges[34]);
    edges[26].set_adjacent_edge(&edges[19], &edges[25], &edges[27], &edges[35]);
    edges[27].set_adjacent_edge(&edges[20], &edges[26], &edges[28], &edges[35]);
    edges[28].set_adjacent_edge(&edges[20], &edges[27], &edges[29], &edges[36]);
    edges[29].set_adjacent_edge(&edges[21], &edges[28], &edges[30], &edges[36]);
    edges[30].set_adjacent_edge(&edges[21], &edges[29], &edges[31], &edges[37]);
    edges[31].set_adjacent_edge(&edges[22], &edges[30], &edges[32], &edges[37]);
    edges[32].set_adjacent_edge(&edges[22], &edges[31], &edges[38], nullptr);

    edges[23].set_adjacent_vertex(&vertices[16], &vertices[21]);
    edges[24].set_adjacent_vertex(&vertices[16], &vertices[22]);
    edges[25].set_adjacent_vertex(&vertices[17], &vertices[22]);
    edges[26].set_adjacent_vertex(&vertices[17], &vertices[23]);
    edges[27].set_adjacent_vertex(&vertices[18], &vertices[23]);
    edges[28].set_adjacent_vertex(&vertices[18], &vertices[24]);
    edges[29].set_adjacent_vertex(&vertices[19], &vertices[24]);
    edges[30].set_adjacent_vertex(&vertices[19], &vertices[25]);
    edges[31].set_adjacent_vertex(&vertices[20], &vertices[25]);
    edges[32].set_adjacent_vertex(&vertices[20], &vertices[26]);

    // sixth edge row
    edges[33].set_adjacent_edge(&edges[23], &edges[39], nullptr, nullptr);
    edges[34].set_adjacent_edge(&edges[24], &edges[25], &edges[40], &edges[41]);
    edges[35].set_adjacent_edge(&edges[26], &edges[27], &edges[42], &edges[43]);
    edges[36].set_adjacent_edge(&edges[28], &edges[29], &edges[44], &edges[45]);
    edges[37].set_adjacent_edge(&edges[30], &edges[31], &edges[46], &edges[47]);
    edges[38].set_adjacent_edge(&edges[32], &edges[48], nullptr, nullptr);

    edges[33].set_adjacent_vertex(&vertices[21], &vertices[27]);
    edges[34].set_adjacent_vertex(&vertices[22], &vertices[28]);
    edges[35].set_adjacent_vertex(&vertices[23], &vertices[29]);
    edges[36].set_adjacent_vertex(&vertices[24], &vertices[30]);
    edges[37].set_adjacent_vertex(&vertices[25], &vertices[31]);
    edges[38].set_adjacent_vertex(&vertices[26], &vertices[32]);

    // seventh edge row
    edges[39].set_adjacent_edge(&edges[33], &edges[40], &edges[49], nullptr);
    edges[40].set_adjacent_edge(&edges[34], &edges[39], &edges[41], &edges[49]);
    edges[41].set_adjacent_edge(&edges[34], &edges[40], &edges[42], &edges[50]);
    edges[42].set_adjacent_edge(&edges[35], &edges[41], &edges[43], &edges[50]);
    edges[43].set_adjacent_edge(&edges[35], &edges[42], &edges[44], &edges[51]);
    edges[44].set_adjacent_edge(&edges[36], &edges[43], &edges[45], &edges[51]);
    edges[45].set_adjacent_edge(&edges[36], &edges[44], &edges[46], &edges[52]);
    edges[46].set_adjacent_edge(&edges[37], &edges[45], &edges[47], &edges[52]);
    edges[47].set_adjacent_edge(&edges[37], &edges[46], &edges[48], &edges[53]);
    edges[48].set_adjacent_edge(&edges[38], &edges[47], &edges[53], nullptr);

    edges[39].set_adjacent_vertex(&vertices[27], &vertices[33]);
    edges[40].set_adjacent_vertex(&vertices[28], &vertices[33]);
    edges[41].set_adjacent_vertex(&vertices[28], &vertices[34]);
    edges[42].set_adjacent_vertex(&vertices[29], &vertices[34]);
    edges[43].set_adjacent_vertex(&vertices[29], &vertices[35]);
    edges[44].set_adjacent_vertex(&vertices[30], &vertices[35]);
    edges[45].set_adjacent_vertex(&vertices[30], &vertices[36]);
    edges[46].set_adjacent_vertex(&vertices[31], &vertices[36]);
    edges[47].set_adjacent_vertex(&vertices[31], &vertices[37]);
    edges[48].set_adjacent_vertex(&vertices[32], &vertices[37]);

    // eighth edge row
    edges[49].set_adjacent_edge(&edges[39], &edges[50], &edges[54], nullptr);
    edges[50].set_adjacent_edge(&edges[41], &edges[42], &edges[55], &edges[56]);
    edges[51].set_adjacent_edge(&edges[43], &edges[44], &edges[57], &edges[58]);
    edges[52].set_adjacent_edge(&edges[45], &edges[46], &edges[59], &edges[60]);
    edges[53].set_adjacent_edge(&edges[47], &edges[48], &edges[61], nullptr);

    edges[49].set_adjacent_vertex(&vertices[33], &vertices[38]);
    edges[50].set_adjacent_vertex(&vertices[34], &vertices[39]);
    edges[51].set_adjacent_vertex(&vertices[35], &vertices[40]);
    edges[52].set_adjacent_vertex(&vertices[36], &vertices[41]);
    edges[53].set_adjacent_vertex(&vertices[37], &vertices[42]);

    // ninth edge row
    edges[54].set_adjacent_edge(&edges[49], &edges[55], &edges[62], nullptr);
    edges[55].set_adjacent_edge(&edges[50], &edges[54], &edges[56], &edges[62]);
    edges[56].set_adjacent_edge(&edges[50], &edges[55], &edges[57], &edges[63]);
    edges[57].set_adjacent_edge(&edges[51], &edges[56], &edges[58], &edges[63]);
    edges[58].set_adjacent_edge(&edges[51], &edges[57], &edges[59], &edges[64]);
    edges[59].set_adjacent_edge(&edges[52], &edges[58], &edges[60], &edges[64]);
    edges[60].set_adjacent_edge(&edges[52], &edges[59], &edges[61], &edges[65]);
    edges[61].set_adjacent_edge(&edges[53], &edges[60], &edges[65], nullptr);

    edges[54].set_adjacent_vertex(&vertices[38], &vertices[43]);
    edges[55].set_adjacent_vertex(&vertices[39], &vertices[43]);
    edges[56].set_adjacent_vertex(&vertices[39], &vertices[44]);
    edges[57].set_adjacent_vertex(&vertices[40], &vertices[44]);
    edges[58].set_adjacent_vertex(&vertices[40], &vertices[45]);
    edges[59].set_adjacent_vertex(&vertices[41], &vertices[45]);
    edges[60].set_adjacent_vertex(&vertices[41], &vertices[46]);
    edges[61].set_adjacent_vertex(&vertices[42], &vertices[46]);

    // tenth edge row
    edges[62].set_adjacent_edge(&edges[54], &edges[55], &edges[66], nullptr);
    edges[63].set_adjacent_edge(&edges[56], &edges[57], &edges[67], &edges[68]);
    edges[64].set_adjacent_edge(&edges[58], &edges[59], &edges[69], &edges[70]);
    edges[65].set_adjacent_edge(&edges[60], &edges[61], &edges[71], nullptr);

    edges[62].set_adjacent_vertex(&vertices[43], &vertices[47]);
    edges[63].set_adjacent_vertex(&vertices[44], &vertices[48]);
    edges[64].set_adjacent_vertex(&vertices[45], &vertices[49]);
    edges[65].set_adjacent_vertex(&vertices[46], &vertices[50]);

    // eleventh edge row
    edges[66].set_adjacent_edge(&edges[62], &edges[67], nullptr, nullptr);
    edges[67].set_adjacent_edge(&edges[63], &edges[66], &edges[68], nullptr);
    edges[68].set_adjacent_edge(&edges[63], &edges[67], &edges[69], nullptr);
    edges[69].set_adjacent_edge(&edges[64], &edges[68], &edges[70], nullptr);
    edges[70].set_adjacent_edge(&edges[64], &edges[69], &edges[71], nullptr);
    edges[71].set_adjacent_edge(&edges[65], &edges[70], nullptr, nullptr);
}

void Catan::init_board() {
    // first row
    vertices[0].set_resource(resource::STONE, 10, resource::NONE, 0, resource::NONE, 0);
    vertices[1].set_resource(resource::SHEEP, 2, resource::NONE, 0, resource::NONE, 0);
    vertices[2].set_resource(resource::WOOD, 9, resource::NONE, 0, resource::NONE, 0);

    // second row
    vertices[3].set_resource(resource::STONE, 10, resource::NONE, 0, resource::NONE, 0);
    vertices[4].set_resource(resource::STONE, 10, resource::SHEEP, 2, resource::NONE, 0);
    vertices[5].set_resource(resource::SHEEP, 2, resource::WOOD, 9, resource::NONE, 0);
    vertices[6].set_resource(resource::WOOD, 9, resource::NONE, 0, resource::NONE, 0);

    // third row
    vertices[7].set_resource(resource::STONE, 10, resource::WHEAT, 12, resource::NONE, 0);
    vertices[8].set_resource(resource::STONE, 10, resource::SHEEP, 2, resource::CLAY, 6);
    vertices[9].set_resource(resource::SHEEP, 2, resource::WOOD, 9, resource::STONE, 4);
    vertices[10].set_resource(resource::WOOD, 9, resource::CLAY, 10, resource::NONE, 0);

    // TODO: Finish the rest of the board
}

void Catan::print_cell_status(int cell_id) {
    vertices[cell_id].print_status();
}