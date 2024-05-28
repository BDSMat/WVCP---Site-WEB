#pragma once

#include "graph.hpp"

class GraphMat : public Graph {

  private:
  
    unsigned int _nb_edges;
    std::vector<std::vector<bool>> _matrix;

  public:
    GraphMat(const std::string &graph_file);
    GraphMat(gint nb_vertices, unsigned int threshold, unsigned int seed);

    ~GraphMat() = default;

    // Returns the number of vertices in the graph.
    gint nb_vertices() const override;

    // Returns the number of edges in the graph.
    gint nb_edges() const override;

    // Whether the vertices v1 and v2 are connected by an edge
    bool is_edge(vertex v1, vertex v2) const override;

    // Add an edge between the vertices v1 and v2.
    void add_edge(vertex v1, vertex v2) override;

    // Returns the degree of the vertex.
    gint degree(vertex v) const override;

    // Returns the list of neighbors of the vertex v1.
    std::vector<vertex> neighbors(vertex v) const override;

  protected:
    // Reset the graph to a new size nb_vertices.
    void reset(gint nb_vertices);
};

class GraphList : public Graph {

  private:
    unsigned int _nb_edges;
    std::vector<std::vector<vertex>> _neighbors;

  public:
    GraphList(const std::string &graph_file);
    GraphList(gint nb_vertices, unsigned int threshold, unsigned int seed);

    ~GraphList() = default;

    // Returns the number of vertices in the graph.
    gint nb_vertices() const override;

    // Returns the number of edges in the graph.
    gint nb_edges() const override;

    // Whether the vertices v1 and v2 are connected by an edge
    bool is_edge(vertex v1, vertex v2) const override;

    // Add an edge between the vertices v1 and v2.
    void add_edge(vertex v1, vertex v2) override;

    // Returns the degree of the vertex.
    gint degree(vertex v) const override;

    // Returns the list of neighbors of the vertex v1.
    std::vector<vertex> neighbors(vertex v) const override;

  protected:
    // Reset the graph to a new size nb_vertices.
    void reset(gint nb_vertices);
};
