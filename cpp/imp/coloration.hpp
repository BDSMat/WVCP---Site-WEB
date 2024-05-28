#pragma once

#include "graphDefs.hpp"

#include "graph.hpp"

struct Coloration {
    
    vector<int> colors;
    int nb_colors;

    Coloration(int nb_vertices) : colors(nb_vertices, 0), nb_colors(0) {
    }

    int &operator[](vertex v) {
        return colors[v];
    }

    int operator[](vertex v) const {
        return colors[v];
    }

    void display() const;

    void to_dot(const string &filename, const Graph &graph) const;

    // list of colors for dot output
    static string dot_color_list[];
};
