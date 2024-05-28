#include <iostream>
#include <fstream>
#include "../lib/json/single_include/nlohmann/json.hpp"

#include "graphListMat.hpp"
#include "solver.hpp"

void colorationGlouton (std::string filename) {
    filename = "../../" + filename;
    GraphListMat g(filename.c_str());
    g.export_as_dimacs_file("index.txt");
}


int main() {

    std::ifstream fichier("json/parametres.json");
    std::string contenu_json((std::istreambuf_iterator<char>(fichier)), std::istreambuf_iterator<char>());
    nlohmann::json objet_json;
    try {
        objet_json = nlohmann::json::parse(contenu_json);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Erreur lors de l'analyse du JSON : " << e.what() << std::endl;
        return 1;
    }
    std::ofstream fic("index.txt");
    fic << "c Graphe : " << objet_json["listegraphe"];
    if(objet_json["listealgo"] == "glouton") {
        colorationGlouton (objet_json["listegraphe"]);
    }
    fic.close();
    return 0;
}
