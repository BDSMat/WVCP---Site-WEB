#include "graph.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>

void Graph::display(ostream &f) const {
    f << "DISPLAY GRAPH" << endl;
    f << "nb of vertices " << nb_vertices() << endl;
    f << "nb of edges " << nb_edges() << endl;
    for (vertex v1 = 0; v1 < nb_vertices(); ++v1) {
        f << "neighbors of " << v1 << " : ";
        for (auto v2 : neighbors(v1)) {
            f << v2 << " ";
        }
        f << endl;
    }
    f << "END DISPLAY GRAPH" << endl;
}

void Graph::display_screen() const {
    display(cout);
}

void Graph::intersect_neighbors(vector<vertex> &vect, vertex v1) const {
    vector<vertex> neigh_v1 = neighbors(v1);
    set_intersection(
        vect.begin(), vect.end(), neigh_v1.begin(), neigh_v1.end(), back_inserter(vect));
}

void Graph::union_neighbors(vector<vertex> &vect, vertex v1) const {
    vector<vertex> neigh_v1 = neighbors(v1);
    set_union(
        vect.begin(), vect.end(), neigh_v1.begin(), neigh_v1.end(), back_inserter(vect));
}

void Graph::diff_neighbors(vector<vertex> &vect, vertex v1) const {
    vector<vertex> neigh_v1 = neighbors(v1);
    set_difference(
        vect.begin(), vect.end(), neigh_v1.begin(), neigh_v1.end(), back_inserter(vect));
}

void Graph::to_dot(const string &filename) const {
    ofstream file(filename);
    file << "graph {" << endl;
    for (vertex v = 0; v < nb_vertices(); ++v) {
        file << v << ";" << endl;
    }
    for (vertex v1 = 0; v1 < nb_vertices(); ++v1) {
        for (vertex v2 = v1; v2 < nb_vertices(); ++v2) {
            if (is_edge(v1, v2)) {
                file << v1 << " -- " << v2 << ";" << endl;
            }
        }
    }
    file << "}" << endl;
    file.close();

    // remove the extension in the filename
    auto pos = filename.find_last_of(".");
    string name = filename.substr(0, pos);

    // call to GraphViz with different engines
    string extcall;
    extcall = "dot -Tpng " + filename + " -o " + name + ".png";

    // if error with dot engine, try with neato engine
    // if (system((extcall).c_str()) != 0) {
    //     extcall = "neato -Tpng " + filename + " -o " + name + ".png";

    //     if (system((extcall).c_str()) != 0) {
    //         cerr
    //             << "Error: conversion from dot to png failed, install neato or dot engine"
    //             << endl;
    //     }
    // }
}

void Graph::export_as_dimacs_file(const string &filename) const {
    ofstream file(filename);
    file << "p edge " << nb_vertices() << " " << nb_edges() << endl;
    for (vertex v1 = 0; v1 < nb_vertices(); ++v1) {
        for (vertex v2 = v1; v2 < nb_vertices(); ++v2) {
            if (is_edge(v1, v2)) {
                file << "e " << v1 << " " << v2 << endl;
            }
        }
    }
}

void Graph::generate_random(gint nb_vertices,
                            unsigned int threshold_edges,
                            unsigned int seed) {
    mt19937 generator(seed);
    uniform_int_distribution<unsigned int> distribution(0, 100);

    reset(nb_vertices);
    for (vertex v1 = 0; v1 < nb_vertices; ++v1) {
        for (vertex v2 = v1 + 1; v2 < nb_vertices; ++v2) {
            if (distribution(generator) < threshold_edges) {
                add_edge(v1, v2);
            }
        }
    }
}

void Graph::import_dimacs_file(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Error: could not open file " + filename);
    }

    vertex nb_vertices;
    vertex nb_edges;
    vertex vertex1;
    vertex vertex2;

    string first;
    file >> first;
    while (!file.eof()) {
        if (first == "e") {
            file >> vertex1 >> vertex2;
            vertex1--;
            vertex2--;
            add_edge(vertex1, vertex2);
        } else if (first == "c") {
            getline(file, first);
        } else if (first == "p") {
            file >> first >> nb_vertices >> nb_edges;
            reset(nb_vertices);
        } else {
            throw runtime_error("Unknown character while reading file: " + first);
        }
        file >> first;
    }
}
