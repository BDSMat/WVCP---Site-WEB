# Projet de Visualisation d'Algorithmes de Coloration de Graphes

## Introduction
Ce projet consiste en une application web permettant de visualiser le déroulement d'algorithmes de coloration de graphes. Vous pourrez observer comment ces algorithmes attribuent des couleurs aux sommets d'un graphe de manière efficace.

## Installation

### Prérequis
Il faut une application de serveur local de type Appaache 2.0.
Nous avons utiliser XAMPP.

Avant de commencer, assurez-vous d'avoir la version suivante de g++ installée :
```
g++.exe (MinGW-W64 x86_64-ucrt-mcf-seh, built by Brecht Sanders) 13.2.0
```
Et de compiler et d'exécuté sur une machine Windows.

### Compilation du code
1. Placez-vous dans le dossier `cpp/imp/`.
2. Exécutez la commande suivante pour compiler les fichiers source :
```
g++ -o main.o main.cpp coloration.cpp graph.cpp graphListMat.cpp solver.cpp 2>&1
```
3. Revenez à la racine du projet, puis lancer "index.php" avec le serveur local.
