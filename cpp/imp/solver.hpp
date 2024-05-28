#pragma once

#include "coloration.hpp"
#include "graph.hpp"

// Check if the coloration is valid.
// A coloration is valid if no two adjacent vertices have the same color and if the color
// of each vertex is between 1 and the number of colors used.
bool checker(const Graph &graph, const Coloration &coloration);

// Heuristic 1: color vertices in the order they appear in the graph.
Coloration solve_heuristic_1(Graph &graph);

// Heuristic 2: color vertices in the order they appear in the permutation.
// The permutation is given as a vector of vertices.
Coloration solve_heuristic_2(Graph &graph, const vector<vertex> &perm);

// Heuristic 3: color vertices in the order they appear in the permutation.
// Create the permutation by sorting the vertices by degree.
Coloration solve_heuristic_3(Graph &graph);

// Heuristic 4: color vertices if they can be added in the current color.
// Create the permutation by sorting the vertices by degree
// try to add the possible vertex to the color before adding a new color
Coloration solve_heuristic_4(Graph &graph);

// Heuristic 5: color vertices if they can be added in the current color.
// try to add the possible vertex to the color before adding a new color
// update the permutation after each color is full
Coloration solve_heuristic_5(Graph &graph);

// Heuristic 6: DSatur algorithm
Coloration solve_heuristic_6(Graph &graph);

// Heursitic max color :
Coloration solve_heuristicMAX(Graph & graph);



// Heurisitc for wvcp with ponderate nodes

// HC MA :
Coloration solve_hcma (Graph &graph, std::string filename, Coloration const & clr, int to, std::vector <int> poids);
Coloration solve_hcma_init (Graph &graph, std::string filename, std::string const & init, int to);


// HC PA :
Coloration solve_hcpa (Graph &graph, std::string filename, Coloration const & clr, int to, std::vector <int> poids);
Coloration solve_hcpa_init (Graph &graph, std::string filename, std::string const & init, int to);
Coloration solve_hcpa_ite_init (Graph &graph, std::string filename, std::string const & init, int to);



// COMP :
Coloration solve_compactage_init(Graph &graph, std::string filename, std::string const & init, int to);
Coloration solve_compactage (Graph &graph, std::string filename, Coloration & clr, int to, std::vector <int> poids);

// HCCO it:
Coloration solve_hcco_init (Graph &graph, std::string filename, std::string const & init, int to);
Coloration solve_hcco (Graph &graph, std::string filename, Coloration & clr, int to, std::vector <int> poids);


