#include "solver.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
#include <random> 
#include <sstream>
#include <chrono>
#include <thread>
#include <unordered_map>






bool checker(const Graph &graph, const Coloration &coloration) {
    for (vertex v1 = 0; v1 < graph.nb_vertices(); ++v1) {
        const int c_v1 = coloration[v1];
        if (c_v1 < 1 || c_v1 > coloration.nb_colors) {
            std::cout << "vertex " << v1 << " color " << c_v1 << " > nb_colors "
                      << coloration.nb_colors << "\n";
            return false;
        }
        for (auto neighbor : graph.neighbors(v1)) {
            if (coloration[neighbor] == c_v1) {
                std::cout << "vertex " << v1 << " color " << c_v1 << "vertex " << neighbor
                          << " color " << coloration[neighbor] << "\n";
                return false;
            }
        }
    }
    return true;
}

Coloration solve_heuristic_1(Graph &graph) {
    Coloration coloration(graph.nb_vertices());
    // for each vertex
    unsigned int nbsteps(0);
    coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
    nbsteps++;
    for (vertex v = 0; v < graph.nb_vertices(); ++v) {
        bool colored = false;
        int color = 1;
        // while the vertex is not colored and the color is less than the number of colors
        while (! colored && color < coloration.nb_colors) {
            bool color_used = false;
            // check if the color is used by a neighbor
            for (const auto n : graph.neighbors(v)) {
                if (coloration[n] == color) {
                    color_used = true;
                    break;
                }
            }
            // if the color is not used, color the vertex
            // otherwise, try the next color
            if (! color_used) {
                coloration[v] = color;
                colored = true;
            } else {
                ++color;
            }
        }
        // if the vertex is not colored, create a new color
        if (! colored) {
            ++coloration.nb_colors;
            coloration[v] = coloration.nb_colors;
        }
        coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
        nbsteps++;
    }
    return coloration;
}

Coloration solve_heuristic_2(Graph &graph, const vector<vertex> &perm) {
    Coloration coloration(graph.nb_vertices());
    unsigned int nbsteps(0);
    coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
    nbsteps++;
    for (const auto v : perm) {   
        bool colored = false;
        int color = 1;
        while (! colored && color < coloration.nb_colors) {
            bool color_used = false;
            for (const auto n : graph.neighbors(v)) {
                if (coloration[n] == color) {
                    color_used = true;
                    break;
                }
            }
            if (! color_used) {
                coloration[v] = color;
                colored = true;
            } else {
                ++color;
            }
            
        }
        if (! colored) {
            ++coloration.nb_colors;
            coloration[v] = coloration.nb_colors;
        }
        coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
        nbsteps++;
    }
    return coloration;
}

Coloration solve_heuristic_3(Graph &graph) {
    Coloration coloration(graph.nb_vertices());
    std::vector<vertex> perm(graph.nb_vertices());
    std::iota(perm.begin(), perm.end(), 0);
    std::vector<unsigned int> degree(graph.nb_vertices());
    for (const auto v : perm) {
        degree[v] = graph.degree(v);
    }

    std::stable_sort(perm.begin(), perm.end(), [&degree](vertex v1, vertex v2) {
        return degree[v1] > degree[v2];
    });

    return solve_heuristic_2(graph, perm);
}

Coloration solve_heuristic_4(Graph &graph) {
    Coloration coloration(graph.nb_vertices());
    std::vector<vertex> perm(graph.nb_vertices());
    std::iota(perm.begin(), perm.end(), 0);
    std::vector<unsigned int> degree(graph.nb_vertices());
    for (const auto v : perm) {
        degree[v] = graph.degree(v);
    }

    std::stable_sort(perm.begin(), perm.end(), [&degree](vertex v1, vertex v2) {
        return degree[v1] > degree[v2];
    });

    coloration.nb_colors = 0;

    unsigned int nbsteps(0);
    coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
    nbsteps++;

    while (! perm.empty()) {
        ++coloration.nb_colors;
        for (auto it = perm.begin(); it != perm.end();) {
            const auto v = *it;
            const auto neighbors = graph.neighbors(v);
            bool color_available = true;
            for (const auto n : neighbors) {
                if (coloration[n] == coloration.nb_colors) {
                    color_available = false;
                    break;
                }
            }
            if (! color_available) {
                ++it;
                continue;
            }
            coloration[v] = coloration.nb_colors;
            perm.erase(it);
            
            coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
            nbsteps++;
        }
    }
    return coloration;
}

Coloration solve_heuristic_5(Graph &graph) {
    Coloration coloration(graph.nb_vertices());
    std::vector<vertex> perm(graph.nb_vertices());
    std::iota(perm.begin(), perm.end(), 0);
    std::vector<unsigned int> degree(graph.nb_vertices());
    for (const auto v : perm) {
        degree[v] = graph.degree(v);
    }

    std::stable_sort(perm.begin(), perm.end(), [&degree](vertex v1, vertex v2) {
        return degree[v1] > degree[v2];
    });

    coloration.nb_colors = 0;

    unsigned int nbsteps(0);
    coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
    nbsteps++;

    while (! perm.empty()) {
        ++coloration.nb_colors;
        for (auto it = perm.begin(); it != perm.end();) {
            const auto v = *it;
            const auto neighbors = graph.neighbors(v);
            bool color_available = true;
            for (const auto n : neighbors) {
                if (coloration[n] == coloration.nb_colors) {
                    color_available = false;
                    break;
                }
            }
            if (! color_available) {
                ++it;
                continue;
            }
            coloration[v] = coloration.nb_colors;
            perm.erase(it);
            for (const auto n : neighbors) {
                --degree[n];
            }
            coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
            nbsteps++;
        }
        std::stable_sort(perm.begin(), perm.end(), [&degree](vertex v1, vertex v2) {
            return degree[v1] > degree[v2];
        });
    }
    return coloration;
}

// Struct used in conjunction with the DSatur priority queue
struct satItem {
    int sat;
    int deg;
    vertex vertex_;
};

struct maxSat {
    bool operator()(const satItem &lhs, const satItem &rhs) const {
        // Compares two satItems sat deg, then degree, then vertex label
        if (lhs.sat > rhs.sat)
            return true;
        if (lhs.sat < rhs.sat)
            return false;
        // if we are we know that lhs.sat == rhs.sat

        if (lhs.deg > rhs.deg)
            return true;
        if (lhs.deg < rhs.deg)
            return false;
        // if we are here we know that lhs.sat == rhs.sat and lhs.deg == rhs.deg.
        // Our choice can be arbitrary
        if (lhs.vertex_ > rhs.vertex_)
            return true;
        return false;
    }
};

// Heuristic 6: DSatur algorithm
Coloration solve_heuristic_6(Graph &graph) {
    Coloration coloration(graph.nb_vertices());
    vector<int> degrees(graph.nb_vertices());
    for (vertex v = 0; v < graph.nb_vertices(); ++v) {
        degrees[v] = graph.degree(v);
    }
    vector<set<int>> adjacent_colors(graph.nb_vertices(), set<int>());
    std::set<satItem, maxSat> queue;

    for (vertex v = 0; v < graph.nb_vertices(); ++v) {
        queue.emplace(
            satItem{static_cast<int>(adjacent_colors[v].size()), degrees[v], v});
    }

    coloration.nb_colors = 0;

    unsigned int nbsteps(0);
    coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
    nbsteps++;

    while (!queue.empty()) {
        auto maxPtr = queue.begin();
        const int v = (*maxPtr).vertex_;
        queue.erase(maxPtr);
        int color = -1;
        for (int color_ = 1; color_ < coloration.nb_colors; ++color_) {
            bool is_color_available = true;
            for (const int neighbor : graph.neighbors(v)) {
                if (coloration[neighbor] == color_) {
                    is_color_available = false;
                    break;
                }
            }
            if (is_color_available) {
                color = color_;
                break;
            }
        }
        if (color == -1) {
            ++coloration.nb_colors;
            color = coloration.nb_colors;
        }
        coloration[v] = color;
        
        for (auto neighbor : graph.neighbors(v)) {
            if (coloration[neighbor] != -1) {
                continue;
            }
            queue.erase({static_cast<int>(adjacent_colors[neighbor].size()),
                         degrees[neighbor],
                         neighbor});
            adjacent_colors[neighbor].insert(color);
            --degrees[neighbor];
            queue.emplace(satItem{static_cast<int>(adjacent_colors[neighbor].size()),
                                  degrees[neighbor],
                                  neighbor});
        }
        coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
        nbsteps++;
    }
    return coloration;
}


Coloration solve_heuristicMAX(Graph & graph) {
    Coloration coloration(graph.nb_vertices());
    unsigned int nbsteps(0);
    coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
    nbsteps++;
    for (unsigned int i(1); i < graph.nb_vertices()+1; i++) {
        coloration[i-1] = i;
        coloration.nb_colors++;
        coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dot", graph);
        nbsteps++;
    }
    coloration.nb_colors = graph.nb_vertices();
    return coloration;
}




// --------------------------------------- PONDERE : --------------------------------------- //


// FONCTIONS --

int nb_couleurs(Coloration coloration)
{
    std::set<int> colors;

    for (int color : coloration.colors) {
        colors.insert(color);
    }

    return colors.size();
}

void decalerCouleurs (Graph &graph, Coloration & coloration) {

    std::set <int> couleurs; // contient les couleurs uniques dans l'ordre 
    for (vertex vert(0); vert < graph.nb_vertices(); vert++) {
        couleurs.insert (coloration.colors[vert]);
    }
    
    std::unordered_map<int, int> valueToIndex;
    std::vector <int> result;
    int currentIndex = 1;
    for (const int& value : couleurs) {
        if (valueToIndex.find(value) == valueToIndex.end()) {
            valueToIndex[value] = currentIndex++;
        }
        result.push_back(valueToIndex[value]);
    }

    coloration.colors = result;
}

std::vector<int> couleurs_po(Coloration const & coloration, std::vector<int> poids)
{
    std::vector<int> total;
    for (int i = 0; i <= coloration.nb_colors; ++i)
    {
        int poid=0;
        for (int j = 0; j < poids.size(); ++j) 
        {
            if(i==coloration.colors[j])
            {
                if(poid<poids[j])
                {
                    poid=poids[j];
                }
            }
        }
        total.push_back(poid);
    }

    return total;
}

void stock(int nbr_couleurs,int indice,std::vector<int> po,int poids_meilleur_graph,int poids_graph_actuel)
{
    std::string filename="pasapas/step" + std::to_string(indice) + ".prm";
    std::ofstream file(filename);
    file << std::to_string(nbr_couleurs)+"\n";
    file << std::to_string(po[1]);
    for(size_t i=2;i<po.size();i++)
    {
        file << +","+std::to_string(po[i]);
    }
    file << "\n" + std::to_string(poids_meilleur_graph) + "," + std::to_string(poids_graph_actuel);
}

int poids_total(Coloration coloration, std::vector<int> poids) {
    int total = 0;
    std::vector<int> couleurs = coloration.colors;
    std::sort(couleurs.begin(), couleurs.end());
    couleurs.erase(std::unique(couleurs.begin(), couleurs.end()), couleurs.end());

    for (const int& couleur : couleurs) {
        int col = 0;
        for (size_t j = 0; j < coloration.colors.size(); ++j) {
            if (couleur == coloration.colors[j]) {
                if (col < poids[j]) {
                    col = poids[j];
                }
            }
        }
        total += col;
    }

    return total;
}

std::vector<int> aleatoire(int nbr)
{
    std::vector<int> vect;
    for(int i=0;i<nbr;i++)
    {
        vect.push_back(i);
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);

    std::shuffle(vect.begin(), vect.end(), rng);
    return vect;
}

std::vector<int> aleatoire_colo(int nbr)
{
    std::vector<int> vect;
    for(int i=1;i<nbr;i++)
    {
        vect.push_back(i);
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);

    std::shuffle(vect.begin(), vect.end(), rng);
    return vect;
}

Coloration rangement(Coloration &u, std::vector<int> poids) {
    /*std::cout << "plooooop : ";
    for(int i = 0; i < u.colors.size(); i++) {
        std::cout << u[i] << " : ";
    }   
    std::cout << std::endl;*/

    int nbCouleurs = nb_couleurs(u);
    std::vector<std::pair<int, int>> col(nbCouleurs + 1, {0, 0});
    
    for(int i = 1; i <= nbCouleurs; i++) {
        col[i].first = i;
        for(int j = 0; j < u.colors.size(); j++) {
            if(u.colors[j] == i && col[i].second < poids[j]) {
                col[i].second = poids[j];
            }
        }
    }

    std::sort(col.begin() + 1, col.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return a.second > b.second;
    });

    /*std::cout << "couleurs : ";
    for(int i = 0; i < u.colors.size(); i++) {
        std::cout << u[i] << " : ";
    }   
    std::cout << std::endl;*/

    std::cout << "pair : ";
    for(int i = 0; i < col.size(); i++) {
        std::cout << "(" << col[i].first << "," << col[i].second << "),";
    }   
    std::cout << std::endl;
    
    std::vector<std::pair<int, int>> som;
    for(int i = 0; i < u.colors.size(); i++) {
        som.emplace_back(i, u.colors[i]);
    }

    for(int i = 0; i < som.size(); i++) {
        for(int j = 1; j < col.size(); j++) {
            if(som[i].second == col[j].first) {
                u.colors[i] = j;
            }
        }
    }  

    /*std::cout << "plooooop : ";
    for(int i = 0; i < u.colors.size(); i++) {
        std::cout << u[i] << " : ";
    }   
    std::cout << std::endl;*/

    return u;
}

// -- FONCTIONS


                //  ------- HC MA : ------- // 

Coloration solve_hcma_init (Graph &graph, std::string filename, std::string const & init, int to) {

    // On charge le vecteur de poids & copie
    std::vector <int> poids;
    std::ifstream ficPoidscp (filename);
    std::ofstream copyPoids ("currentWeight.col.w");
    std::string line;
    while (getline(ficPoidscp, line)) {
        copyPoids << line << "\n";
        
    }
    copyPoids.close();
    ficPoidscp.close();
    std::ifstream ficPoids (filename);
    int entierPoids;
    while (ficPoids >> entierPoids) {
        poids.push_back (entierPoids);
    }
    ficPoids.close();

    // Choisit l'algorithme de coloration initiale
    auto sol = Coloration (graph.nb_vertices());
    if (init == "Coloration MAX") {    
        sol = solve_heuristicMAX (graph);
    } else if (init == "Glouton"){
        sol = solve_heuristic_1 (graph);
    } else if (init == "Trié par couleur") {
        sol = solve_heuristic_3 (graph);
    } else if (init == "Permutation couleur") {
        sol = solve_heuristic_4 (graph);
    } else if (init == "Permutation MAJ couleur") {
        sol = solve_heuristic_5 (graph);
    } else if (init == "DSatur") {
        sol = solve_heuristic_6 (graph);
    }

    return solve_hcma (graph, filename, sol, to, poids);
}

Coloration solve_hcma (Graph &graph, std::string filename, Coloration const & clr, int to, std::vector <int> poids) {
    // Solver Recherche Hill-Climbing en Meilleur Améliorant


    Coloration coloration = clr;
    coloration.to_dot("solutionInit.dot", graph);
   
    // Retourner la solution optimisée
    bool test=true;
    auto poid_total_av = poids_total(coloration,poids);

    unsigned int nbsteps(0);
    coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dotA", graph);
    stock(nb_couleurs(coloration),nbsteps,couleurs_po(coloration,poids),poid_total_av,poids_total(coloration,poids));
    nbsteps++;
    // Initialisation du chrono
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::milliseconds(to);

    // Coloration col_avant=coloration;
    // La boucle principale
    while((std::chrono::high_resolution_clock::now() < end))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Coloration col_avant=coloration;
        std::vector<int> alea = aleatoire(graph.nb_vertices());
        coloration.nb_colors=nb_couleurs(coloration);

        auto poid_total_avant = poids_total(coloration,poids);
        while(!alea.empty())
        {
            int sommet = alea.front(); 
            auto colora = aleatoire_colo(nb_couleurs(coloration));
            poid_total_avant = poids_total(coloration,poids);
            auto couleurs_initial = coloration.colors[sommet];
            bool test  = false;
            
            while(!colora.empty())
            {
                coloration.colors[sommet]=colora.front();
                coloration.nb_colors=nb_couleurs(coloration);

                auto poid_total_apres = poids_total(coloration,poids);
                
                if(poid_total_avant > poid_total_apres && coloration.colors[sommet]!=couleurs_initial && checker(graph,coloration))
                {
                    poid_total_avant=poid_total_apres;
                    couleurs_initial = colora.front();
                    test=true;  
                }
               
                colora.erase(colora.begin()); 
            }
            coloration[sommet] = couleurs_initial;
            coloration.nb_colors=nb_couleurs(coloration);
            alea.erase(alea.begin()); 
            if(test)
            {
                coloration = rangement(coloration,poids);
                coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dotA", graph);
                stock(nb_couleurs(coloration),nbsteps,couleurs_po(coloration,poids),poid_total_avant,poids_total(coloration,poids));
                nbsteps++;
            } 
        }   
        
        
    }
    
    std::cout << "Hill-Climbing MA Score Initial : " <<  poid_total_av <<", Score Amélioré : " << poids_total(coloration,poids)<<std::endl;

    return coloration;
}



                //  ------- HC PA : ------- // 

Coloration solve_hcpa_init (Graph &graph, std::string filename, std::string const & init, int to) {

    // On charge le vecteur de poids & copie
    std::vector <int> poids;
    std::ifstream ficPoidscp (filename);
    std::ofstream copyPoids ("currentWeight.col.w");
    std::string line;
    while (getline(ficPoidscp, line)) {
        copyPoids << line << "\n";
        
    }
    copyPoids.close();
    ficPoidscp.close();
    std::ifstream ficPoids (filename);
    int entierPoids;
    while (ficPoids >> entierPoids) {
        poids.push_back (entierPoids);
    }
    ficPoids.close();

    // Choisit l'algorithme de coloration initiale
    auto sol = Coloration (graph.nb_vertices());
    if (init == "Coloration MAX") {    
        sol = solve_heuristicMAX (graph);
    } else if (init == "Glouton"){
        sol = solve_heuristic_1 (graph);
    } else if (init == "Trié par couleur") {
        sol = solve_heuristic_3 (graph);
    } else if (init == "Permutation couleur") {
        sol = solve_heuristic_4 (graph);
    } else if (init == "Permutation MAJ couleur") {
        sol = solve_heuristic_5 (graph);
    } else if (init == "DSatur") {
        sol = solve_heuristic_6 (graph);
    }

    return solve_hcpa (graph, filename, sol, to, poids);
}

Coloration solve_hcpa (Graph &graph, std::string filename, Coloration const & clr, int to, std::vector <int> poids) {
    // Solver Recherche Hill-Climbing en Premier Améliorant

    Coloration coloration = clr;
    coloration.to_dot("solutionInit.dot", graph);
   
    // Retourner la solution optimisée
    bool test=true;
    auto poid_total_av = poids_total(coloration,poids);

    unsigned int nbsteps(0);
    coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dotA", graph);
    stock(nb_couleurs(coloration),nbsteps,couleurs_po(coloration,poids),poid_total_av,poids_total(coloration,poids));
    nbsteps++;
    // Initialisation du chrono
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::milliseconds(to);

    // Coloration col_avant=coloration;
    // La boucle principale
    while((std::chrono::high_resolution_clock::now() < end))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Coloration col_avant=coloration;
        std::vector<int> alea = aleatoire(graph.nb_vertices());
        coloration.nb_colors=nb_couleurs(coloration);

        auto poid_total_avant = poids_total(coloration,poids);
        while(!alea.empty())
        {
            int sommet = alea.front(); 
            auto colora = aleatoire_colo(nb_couleurs(coloration));
            poid_total_avant = poids_total(coloration,poids);
            auto couleurs_initial = coloration.colors[sommet];
            bool test  = false;
            
            while(!colora.empty())
            {
                coloration.colors[sommet]=colora.front();
                coloration.nb_colors=nb_couleurs(coloration);

                auto poid_total_apres = poids_total(coloration,poids);
                
                if(poid_total_avant > poid_total_apres && coloration.colors[sommet]!=couleurs_initial && checker(graph,coloration))
                {
                    poid_total_avant=poid_total_apres;
                    couleurs_initial = colora.front();
                    test=true;
                    break;
                }
               
                colora.erase(colora.begin()); 
            }
            coloration[sommet] = couleurs_initial;
            coloration.nb_colors=nb_couleurs(coloration);
            alea.erase(alea.begin()); 
            if(test)
            {
                coloration = rangement(coloration,poids);
                coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dotA", graph);
                stock(nb_couleurs(coloration),nbsteps,couleurs_po(coloration,poids),poid_total_avant,poids_total(coloration,poids));
                nbsteps++;
            } 
        }   
        
        
    }
    std::cout << "Hill-Climbing PA Score Initial : " <<  poid_total_av <<", Score Amélioré " << poids_total(coloration,poids)<<std::endl;

    return coloration;
}


                // ------- ITERE HCPA ------- //

std::vector<Coloration> solve_hcpa_ite (Graph &graph, std::string filename, Coloration const & clr, int to, std::vector <int> poids, int ite) {

    Coloration coloration = clr;
    coloration.to_dot("solutionInit.dot", graph);
   
    // Retourner la solution optimisée
    bool test=true;
    auto poid_total_av = poids_total(coloration,poids);

    unsigned int nbsteps(0);
    coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dotA", graph);
    stock(nb_couleurs(coloration),nbsteps,couleurs_po(coloration,poids),poid_total_av,poids_total(coloration,poids));
    nbsteps++;
    // Initialisation du chrono
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::milliseconds(to);

    // Coloration col_avant=coloration;
    // La boucle principale
    std::vector<Coloration> tete;
    while((std::chrono::high_resolution_clock::now() < end))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Coloration col_avant=coloration;
        std::vector<int> alea = aleatoire(graph.nb_vertices());
        coloration.nb_colors=nb_couleurs(coloration);
      
        while(!alea.empty())
        {
            int sommet = alea.front(); 
            auto colora = aleatoire_colo(nb_couleurs(coloration));
            auto poid_total_avant = poids_total(coloration,poids);
            auto couleurs_initial = coloration.colors[sommet];
            
            while(!colora.empty())
            {
                coloration.colors[sommet]=colora.front();
                coloration.nb_colors=nb_couleurs(coloration);

                auto poid_total_apres = poids_total(coloration,poids);
                
                if(poid_total_avant > poid_total_apres && coloration.colors[sommet]!=couleurs_initial && checker(graph,coloration))
                {
                    poid_total_avant=poid_total_apres;
                    couleurs_initial = colora.front();
                    coloration.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dotA", graph);
                    stock(nb_couleurs(coloration),nbsteps,couleurs_po(coloration,poids),poid_total_avant,poids_total(coloration,poids));
                    nbsteps++;
                    break;
                    
                }
                else
                {
                    if(poid_total_avant == poid_total_apres && coloration.colors[sommet]!=couleurs_initial && checker(graph,coloration) && tete.size()<ite)
                    {
                        tete.push_back(coloration);
                    }
                }
               
                colora.erase(colora.begin()); 
            }
            coloration[sommet] = couleurs_initial;
            coloration.nb_colors=nb_couleurs(coloration);
            alea.erase(alea.begin()); 
           
        }   
        
    }
    std::cout << "Hill-Climbing PA it : " <<  poid_total_av <<" poids total après : " << poids_total(coloration,poids)<<std::endl;
    tete.push_back(coloration);
    return tete;
}

Coloration solve_hcpa_ite_init (Graph &graph, std::string filename, std::string const & init, int to) 
{
    // On charge le vecteur de poids & copie
    std::vector <int> poids;
    std::ifstream ficPoidscp (filename);
    std::ofstream copyPoids ("currentWeight.col.w");
    std::string line;
    while (getline(ficPoidscp, line)) {
        copyPoids << line << "\n";
        
    }
    copyPoids.close();
    ficPoidscp.close();
    std::ifstream ficPoids (filename);
    int entierPoids;
    while (ficPoids >> entierPoids) {
        poids.push_back (entierPoids);
    }
    ficPoids.close();

    // Choisit l'algorithme de coloration initiale
    auto sol = Coloration (graph.nb_vertices());
    if (init == "Coloration MAX") {    
        sol = solve_heuristicMAX (graph);
    } else if (init == "Glouton"){
        sol = solve_heuristic_1 (graph);
    } else if (init == "Trié par couleur") {
        sol = solve_heuristic_3 (graph);
    } else if (init == "Permutation couleur") {
        sol = solve_heuristic_4 (graph);
    } else if (init == "Permutation MAJ couleur") {
        sol = solve_heuristic_5 (graph);
    } else if (init == "DSatur") {
        sol = solve_heuristic_6 (graph);
    }
    int nbr_ite =4;
    int poids_av=poids_total(sol,poids);
    auto t = solve_hcpa_ite (graph, filename ,sol, to, poids, nbr_ite);
    Coloration y = t.back();
    
    for(int i=0;i<t.size()-1;i++)
    {
        Coloration u = solve_hcpa (graph,filename,t[i],to,poids);
        if(poids_total(y,poids) > poids_total(u,poids))
        {
            y=u;
        }
    }
    std::cout << "Hill-Climbing PA itéré meilleur : " <<poids_total(y,poids)<<std::endl;
    return y;
}
                

                //  ------- COMPA : ------- //


// FONCTIONS : 

std::vector <vertex> t_1 (Graph &graph, Coloration t, int clr) {

    // renvoit tous les sommets de couleur "clr"
    std::vector <vertex> ENS; 
    for (vertex vert(0); vert < graph.nb_vertices(); vert++) {
        if (t[vert] == clr) ENS.push_back(vert);
    }
    return ENS;
}

int poids_colonne(Graph &graph,Coloration & t,int clr1)
{
    std::vector <vertex> J = t_1 (graph, t, clr1);
    int pd = 0;
    for (auto vert : J) {
        if(pd<t[vert])
        {
            pd=t[vert];
        }
    }
    return pd;

}

int upk (Graph &graph, Coloration t, vertex sommet) {

    // renvoit la couleur la plus à gauche qu'un sommet peut être bougé sans créé de conflits
    int max = t[sommet];
    Coloration tprime = t;
    for (int col(0); col < max; col++) {
        tprime[sommet] = col;
        if (checker(graph,tprime)) {
            max = col;
        }
        tprime = t;
    }
    return max;
    
}

int maxColor (Coloration t, std::vector <vertex> ENS) {

    // renvoit la couleur la plus à droite des sommets de ENS
    int max = t[0];
    for (vertex vert : ENS) {
        if (t[vert]>= max) max = t[vert];
    }
    return max;
}


// choisit le premier élément
int choose (std::vector <vertex> ENS) { return ENS[0]; }

std::vector<vertex> inter(const std::vector<vertex>& vec1, const std::vector<vertex>& vec2) {

    // renvoit l'intersection de ces deux ensembles
    std::vector<vertex> intersection;
    
    std::vector<vertex> vec1_sorted = vec1;
    std::vector<vertex> vec2_sorted = vec2;
    std::sort(vec1_sorted.begin(), vec1_sorted.end());
    std::sort(vec2_sorted.begin(), vec2_sorted.end());

    std::set_intersection(vec1_sorted.begin(), vec1_sorted.end(),
                          vec2_sorted.begin(), vec2_sorted.end(),
                          std::back_inserter(intersection));

    return intersection;
}

int minColor (Graph &graph, Coloration t, int i) {

    // renvoit la couleur la plus proche de i à sa droite
    std::vector <int> ENS; // ens de couleure
    for (vertex vert(0); vert < graph.nb_vertices(); vert++) {
        if (t[vert] >= i) {
            ENS.push_back (t[vert]);
        }
    }
    int min = t.nb_colors;
    for (auto clr : ENS) {
        if (clr < min) {
            min = clr;
        }
    }

    return min;
}

void swapColors (Graph &graph, Coloration & t, int clr1, int clr2) {
    std::vector <vertex> J = t_1 (graph, t, clr1);
    std::vector <vertex> I = t_1 (graph, t, clr2);

    for (auto vert : J) {
        t[vert] = clr2;
    }
    for (auto vert : I) {
        t[vert] = clr1;
    }
}



//// ALGOS :


Coloration solve_compactage_init (Graph &graph, std::string filename, std::string const & init, int to) {

    // On charge le vecteur de poids & copie
    std::vector <int> poids;
    std::ifstream ficPoidscp (filename);
    std::ofstream copyPoids ("currentWeight.col.w");
    std::string line;
    while (getline(ficPoidscp, line)) {
        copyPoids << line << "\n";
        
    }
    copyPoids.close();
    ficPoidscp.close();
    std::ifstream ficPoids (filename);
    int entierPoids;
    while (ficPoids >> entierPoids) {
        poids.push_back (entierPoids);
    }
    ficPoids.close();

    auto sol = Coloration(graph.nb_vertices());
    if (init == "Coloration MAX") {
        sol = solve_heuristicMAX(graph);
    } else if (init == "Glouton"){
        sol = solve_heuristic_1(graph);
    } else if (init == "Trié par couleur") {
        sol = solve_heuristic_3(graph);
    } else if (init == "Permutation couleur") {
        sol = solve_heuristic_4(graph);
    } else if (init == "Permutation MAJ couleur") {
        sol = solve_heuristic_5(graph);
    } else if (init == "DSatur") {
        sol = solve_heuristic_6(graph);
    }
    return solve_compactage(graph, filename, sol, to, poids);
}


Coloration solve_compactage (Graph &graph, std::string filename, Coloration & clr, int to, std::vector <int> poids) {
    // Initialise l'algorithme
    Coloration t = clr;
    t.to_dot ("solutionInit.dot", graph);
    auto poid_total_av = poids_total (t, poids);
    std::cout<<"test"<<std::endl;
    // Pour le pas à pas
    unsigned int nbsteps(0);
    t.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dotA", graph);
    stock(nb_couleurs(t),nbsteps,couleurs_po(t,poids),poid_total_av,poids_total(t,poids));
    nbsteps++;

    // Initialise le timeout 
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + std::chrono::milliseconds(to);
    
    // La boucle principale
    std::vector <vertex> I; // à bouger plus bas
    while(std::chrono::high_resolution_clock::now() < end)
    {
        int n = t.nb_colors;
        std::vector <vertex> U;
        for (vertex vert(0); vert < graph.nb_vertices(); vert++) {
            if (t[vert] > upk (graph, t, vert)) 
            {
                U.push_back (vert);
            }
        }

        while (!U.empty()) {
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            int i = maxColor (t, U);
            I = t_1(graph, t, i);
         
            do {
                vertex u = choose (inter(U,I));
                t[u] = upk(graph, t, u);
                auto it = std::find(U.begin(), U.end(), u);
                U.erase(it);

            

            } while (!inter(U,I).empty());
            while (i < n) {
                int j  = minColor (graph, t, i);
                swapColors (graph, t, j, i); // change les couleurs

            
                std::vector <vertex> S;
                for (vertex vert(0); vert < graph.nb_vertices(); vert++){
                    if ((t[vert]>i) && (inter(t_1(graph, t, i), graph.neighbors(vert)).empty())) {
                        S.push_back(vert);
                    }
                }
                while (!S.empty()) {
                   
                    vertex v = choose (S);
                    t[v] = i;
                

                    std::vector <vertex> Z;
                    for (vertex vert(0); vert < graph.nb_vertices(); vert++){
                        if ((t[vert]>i) && (inter(t_1(graph, t, i), graph.neighbors(vert)).empty())) {
                            Z.push_back(vert);
                        }
                    }
                    S = Z;
                }
                j = i + 1;
                while (j < n) {
                    int m = 0;
                    for (int l(j); l<=n; l++) {
                        for (int k(j); k <= l; k++) {
                            if (t_1(graph, t, k).empty()) m++;
                        }
                    }
                    if(poids_colonne(graph,t,j)==poids_colonne(graph,t,j+m))
                    {
                        swapColors (graph, t, j, j+m);
                    }
                    j += m+1;

                

                }
                n = t.nb_colors;
                i++;
            }
            t.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dotA", graph);
            stock(nb_couleurs(t),nbsteps,couleurs_po(t,poids),poid_total_av,poids_total(t,poids));
            nbsteps++;
        } 
    }
     // --- PAS A PAS
    t.to_dot("pasapas/step" + std::to_string(nbsteps) + ".dotA", graph);
    stock(nb_couleurs(t),nbsteps,couleurs_po(t,poids),poid_total_av,poids_total(t,poids));
    nbsteps++;
    // --- PAS A PAS
    std::cout<<"Compactage Ensembliste poids avant : "<<poid_total_av<<" poids total après : "<<poids_total(t, poids)<<std::endl;
    return t;
}


Coloration solve_hcco_init (Graph &graph, std::string filename, std::string const & init, int to) {
    // On charge le vecteur de poids & copie
    std::vector <int> poids;
    std::ifstream ficPoidscp (filename);
    std::ofstream copyPoids ("currentWeight.col.w");
    std::string line;
    while (getline(ficPoidscp, line)) {
        copyPoids << line << "\n";
        
    }
    copyPoids.close();
    ficPoidscp.close();
    std::ifstream ficPoids (filename);
    int entierPoids;
    while (ficPoids >> entierPoids) {
        poids.push_back (entierPoids);
    }
    ficPoids.close();

    auto sol = Coloration(graph.nb_vertices());
    if (init == "Coloration MAX") {
        sol = solve_heuristicMAX(graph);
    } else if (init == "Glouton"){
        sol = solve_heuristic_1(graph);
    } else if (init == "Trié par couleur") {
        sol = solve_heuristic_3(graph);
    } else if (init == "Permutation couleur") {
        sol = solve_heuristic_4(graph);
    } else if (init == "Permutation MAJ couleur") {
        sol = solve_heuristic_5(graph);
    } else if (init == "DSatur") {
        sol = solve_heuristic_6(graph);
    }
    return solve_hcco (graph, filename, sol, to, poids);
}

#include <filesystem>
Coloration solve_hcco (Graph &graph, std::string filename, Coloration & clr, int to, std::vector <int> poids) {
    int nbr_ite =4;
    int poids_av=poids_total(clr,poids);
    clr.nb_colors=nb_couleurs(clr);
    auto y = solve_hcpa(graph,filename,clr,to, poids);
    
    const std::string dossier = "pasapas";
    try {
        for (const auto& fichier : std::filesystem::directory_iterator(dossier)) {
            std::filesystem::remove(fichier);
        }
        std::cout << "Tous les fichiers ont été supprimés avec succès !" << std::endl;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Erreur lors de la suppression des fichiers : " << e.what() << std::endl;
    }

    auto z = rangement(y,poids);
    z.nb_colors=nb_couleurs(z);
    y = solve_compactage (graph,filename,z,to, poids);
    

    std::cout << "Hill-Climbing -> Compactage Score Initial : " <<poids_av<< ", Score Amélioré : "<<poids_total(y,poids)<<std::endl;
    return y;
}