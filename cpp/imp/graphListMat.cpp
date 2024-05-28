#include "graphListMat.hpp"


// MATRICE :

GraphMat::GraphMat(const std::string &filename) {
    import_dimacs_file(filename);
}

GraphMat::GraphMat(gint nb_vertices, unsigned int threshold, unsigned int seed) {
    generate_random(nb_vertices, threshold, seed);
}

gint GraphMat::nb_vertices() const {
    return static_cast<gint>(_matrix[0].size());
}

gint GraphMat::nb_edges() const {
    return _nb_edges;
}

bool GraphMat::is_edge(vertex v1, vertex v2) const {
    return _matrix[v1][v2];
}

void GraphMat::add_edge(vertex v1, vertex v2) {
    if (!_matrix[v1][v2]) {
        _matrix[v1][v2] = true;
        _matrix[v2][v1] = true;
        _nb_edges++;
    }
}

gint GraphMat::degree(vertex v) const {
    return static_cast<gint>(_matrix[v].size());
}

std::vector<vertex> GraphMat::neighbors(vertex v) const {
    std::vector<vertex> ENS;
    for (vertex vert(0); vert < nb_vertices(); vert++)
    {
        if (is_edge(v,vert)) ENS.push_back(vert);
    }
    return ENS;
}

void GraphMat::reset(gint nb_vertices) {
    _matrix.clear();
    _matrix.resize(nb_vertices, std::vector<bool>(nb_vertices, false));
    _nb_edges = 0;
}


// LISTE :

GraphList::GraphList(const std::string &filename) {
    import_dimacs_file(filename);
}

GraphList::GraphList(gint nb_vertices, unsigned int threshold, unsigned int seed) {
    generate_random(nb_vertices, threshold, seed);
}

gint GraphList::nb_vertices() const {
    return static_cast<gint>(_neighbors.size());
}

gint GraphList::nb_edges() const {
    return _nb_edges;
}

bool GraphList::is_edge(vertex v1, vertex v2) const {

    for (vertex vert(0); vert < _neighbors[v1].size(); vert++) {
        if (_neighbors[v1][vert]==v2) return true;
    }
    return false;
}

void GraphList::add_edge(vertex v1, vertex v2) {
    if (!is_edge(v1,v2)) {
        _neighbors[v1].push_back(v2);
        _neighbors[v2].push_back(v1);
        _nb_edges++;
    } 
}

gint GraphList::degree(vertex v) const {
    return static_cast<gint>(_neighbors[v].size());
}

std::vector<vertex> GraphList::neighbors(vertex v) const {
    return _neighbors[v];
}

void GraphList::reset(gint nb_vertices) {
    _neighbors.clear();
    _neighbors.resize(nb_vertices);
    _nb_edges = 0;
}
