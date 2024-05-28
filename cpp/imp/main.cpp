#include <iostream>
#include <fstream>
#include <filesystem>

#include "../lib/json/single_include/nlohmann/json.hpp"

#include "solver.hpp"
#include "graphListMat.hpp"



void colorationNotPond (std::string filename, std::string algo, bool use_list) {

    filename = "../../" + filename;
    
    bool generation_dot_file = true;

    Graph * graph;

    if (use_list) {
        graph = new GraphList (filename.c_str());
    } else {
        graph = new GraphMat (filename.c_str());
    }
    
    if (generation_dot_file) {
        graph->import_dimacs_file(filename);
    }   

    auto sol = Coloration(graph->nb_vertices());
    if (algo == "Coloration MAX") {
        sol = solve_heuristicMAX(*graph);
    } else if (algo == "Glouton") {
        sol = solve_heuristic_1(*graph);
    } else if (algo == "Trié par couleur") {
        sol = solve_heuristic_3(*graph);
    } else if (algo == "Permutation couleur") {
        sol = solve_heuristic_4(*graph);
    } else if (algo == "Permutation MAJ couleur") {
        sol = solve_heuristic_5(*graph);
    } else if (algo == "DSatur") {
        sol = solve_heuristic_6(*graph);
    }

    std::cout << "Colorié avec " << algo << std::endl;

    if (generation_dot_file) {
        sol.to_dot("solution.dot", *graph);
    }

}

void colorationPond(std::string filename, std::string algo, std::string const & init, std::string to, bool use_list) {

    filename = "../../" + filename;
    
    bool generation_dot_file = true;

    
    Graph * graph;

    if (use_list) {
        graph = new GraphList (filename.c_str());
    } else {
        graph = new GraphMat (filename.c_str());
    }
    

    if (generation_dot_file) {
        graph->import_dimacs_file(filename);
    }

    filename = filename + ".w";
    auto sol = Coloration (graph->nb_vertices());
    if (algo == "Hill-climbing Meilleur Améliorant") {
        sol = solve_hcma_init (*graph, filename, init, stoi(to));
    } else if (algo == "Hill-climbing Premier Améliorant") {
        sol = solve_hcpa_init (*graph, filename, init, stoi(to));
    } else if (algo == "Compactage Flat") {
        sol = solve_compactage_init(*graph, filename, init, stoi(to));
    } else if (algo == "RLI HCPA") {
        sol = solve_hcpa_ite_init (*graph, filename, "Permutation MAJ couleur", stoi(to));
    } else if (algo == "RLI HCCO") {
        sol = solve_hcco_init (*graph, filename, "Glouton", stoi(to));
    }
    
    if (generation_dot_file) {
        sol.to_dot("solution.dot", *graph);
    }

}


int main() {

    // Lis et stocke le json dans objet_json
    std::ifstream fichier("../../json/parametres.json");
    std::string contenu_json((std::istreambuf_iterator<char>(fichier)), std::istreambuf_iterator<char>());
    nlohmann::json objet_json;
    try {
        objet_json = nlohmann::json::parse(contenu_json);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Erreur lors de l'analyse du JSON : " << e.what() << std::endl;
        return 1;
    }
    
    // Trie dans les fonctions en fonction des paramètres
    if (objet_json["pond"] != "on") {
        if (objet_json["listAdj"] != "on") colorationNotPond (objet_json["listegraphe"], objet_json["listealgo"], false);
        else colorationNotPond (objet_json["listegraphe"], objet_json["listealgo"], true); 
        
    } else {
        if (objet_json["listAdj"] != "on") colorationPond (objet_json["listegraphe"], objet_json["listealgo"], objet_json["inithc"], objet_json["to"], false);
        else colorationPond (objet_json["listegraphe"], objet_json["listealgo"], objet_json["inithc"], objet_json["to"], true);
    }

    return 0;
}
