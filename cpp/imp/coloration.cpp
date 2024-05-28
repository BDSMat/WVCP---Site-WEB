#include "coloration.hpp"

#include <fstream>
#include <iostream>

void Coloration::display() const {
    for (int color = 1; color <= nb_colors; color++) {
        cout << "color " << color << " for  vertices : ";
        for (vertex v = 0; v < colors.size(); v++) {
            if (colors[v] == color)
                cout << v << " ";
        }
        cout << endl;
    }
}

void Coloration::to_dot(const string &filename, const Graph &graph) const {
    std::ofstream file(filename);
    file << "graph {\n node[width=0.5,fixedsize=true,penwidth=10]\n";
    for (vertex v = 0; v < graph.nb_vertices(); ++v) {
        file << "\t" << v << "[shape=circle, color=" << dot_color_list[colors[v]]
             << "]\n";
    }
    for (vertex v1{0}; v1 < graph.nb_vertices(); ++v1) {
        for (vertex v2{v1 + 1}; v2 < graph.nb_vertices(); ++v2) {
            if (graph.is_edge(v1, v2)) {
                // if the two vertices have the same color, the edge is red
                if (colors[v1] == colors[v2]) {
                    file << "\t" << v1 << " -- " << v2 << " [color=red]\n";
                } else {
                    file << "\t" << v1 << " -- " << v2 << "\n";
                }
            }
        }
    }
    file << "}\n";
    file.close();
    std::string command;

    // remove the extension in the filename
    auto pos = filename.find_last_of(".");
    string name = filename.substr(0, pos);

    // call to GraphViz with different engines
    string extcall;
    extcall = "dot -Tpng " + filename + " -o " + name + ".png";

    // // if error with dot engine, try with neato engine
    // if (system((extcall).c_str()) != 0) {
    //     extcall = "neato -Tpng " + filename + " -o " + name + ".png";

    //     if (system((extcall).c_str()) != 0) {
    //         cerr
    //             << "Error: conversion from dot to png failed, install neato or dot engine"
    //             << endl;
    //     }
    // }
}





std::string Coloration::dot_color_list[] = {
    "#000000",
    "#ff4500",
    "#b22222",
    "#ee82ee",
    "#66cdaa",
    "#f06347",
    "#aac0e6",
    "#8b008b",
    "#696969",
    "#4169e1",
    "#f1e4b5",
    "#1febcd",
    "#98fb98",
    "#add8e6",
    "#483d8b",
    "#fdf5e6",
    "#cd5c5c",
    "#2f4f4f",
    "#87cefa",
    "#7fff00",
    "#c71585",
    "#f4a460",
    "#ffefd5",
    "#e0ffff",
    "#dc143c",
    "#ffe4e1",
    "#ff00ff",
    "#db7093",
    "#a0522d",
    "#e9967a",
    "#7b68ee",
    "#6b8e23",
    "#5f9ea0",
    "#00ced1",
    "#90ee90",
    "#f5f5dc",
    "#0000ff",
    "#ffa500",
    "#9370db",
    "#fffafa",
    "#00ffff",
    "#bc8f8f",
    "#fffaf0",
    "#d8bfd8",
    "#808000",
    "#ffc0cb",
    "#afeeee",
    "#00ff00",
    "#008b8b",
    "#ff00ff",
    "#fff0f5",
    "#87ceeb",
    "#9acd32",
    "#00ffff",
    "#a9a9a9",
    "#bdb76b",
    "#000000",
    "#f5deb3",
    "#f08080",
    "#f0ffff",
    "#ffd700",
    "#ffb6c1",
    "#00008b",
    "#b8860b",
    "#daa520",
    "#48d1cc",
    "#6a5acd",
    "#556b2f",
    "#d2b48c",
    "#1e90ff",
    "#4682b4",
    "#d2691e",
    "#ff69b4",
    "#f5fffa",
    "#8a2be2",
    "#f0f8ff",
    "#7cfc00",
    "#fa8072",
    "#fafad2",
    "#f8f8ff",
    "#adff2f",
    "#deb887",
    "#00bfff",
    "#ffdead",
    "#fffacd",
    "#fffff0",
    "#191970",
    "#ba55d3",
    "#8fbc8f",
    "#708090",
    "#32cd32",
    "#00ff7f",
    "#ffdab9",
    "#ff7f50",
    "#00fa9a",
    "#cd853f",
    "#0000cd",
    "#e6e6fa",
    "#7fffd4",
    "#b0e0e6",
    "#ff1493",
    "#006400",
    "#d3d3d3",
    "#778899",
    "#f0fff0",
    "#9400d3",
    "#3cb371",
    "#a52a2a",
    "#ff0000",
    "#2e8b57",
    "#ff8c00",
    "#008080",
    "#ffff00",
    "#dda0dd",
    "#fff5ee",
    "#8b0000",
    "#40e0d0",
    "#fff8dc",
    "#f0e68c",
    "#dcdcdc",
    "#20b2aa",
    "#eee8aa",
    "#ffa07a",
    "#4b0082",
    "#6495ed",
    "#c0c0c0",
    "#faebd7",
    "#ffe4c4",
    "#f5f5f5",
    "#228b22",
    "#da70d6",
    "#b0c4de",
    "#9932cc",
    "#8b4513",
    "#ffffe0",
    "#000080"
};
