<?php
    #include ("php/test.php");         // Savoir si le php est activé
    include ("php/selectGraphe.php"); // Charge le select des graphes du repertoier "graph/"
    include ("php/storeJson.php");    // Converti le form en un fichier JSON pour le C++
    include ("php/dispSolution.php"); // Converti un .dot en SVG
?> 

<!DOCTYPE html>
<html lang="fr">
    <head>

        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title> Graphe Pondéré - Coloration </title>
        <link rel="stylesheet" type="text/css" href="css/style.css">
        <script src="js/function.js" defer></script> <!-- Charge les functions js -->
        <script src="js/selectAlgo.js" defer></script> <!-- Charge le DOM js -->
        <script src="js/paramAlgo.js" defer></script> <!-- Charge le DOM js -->

    </head>

    <body>

        <a href="index.php"><button class="button"> Version Classique </button></a>

        <h1 class="title">COLORATION DE GRAPHE PONDÉRÉ</h1>

        <h1> Paramètres de la requète : </h1>

        <form method="post">
            <fieldset>
                <legend class="rectangle"> Paramètres </legend>
                <p class="box">
                    <?php genereSelectFiles("graph/weighted/"); ?> 
                </p>
                
                <p id="algo">
                    <label>Choisir un algorithme :</label> <br>
                    <div id="lol">

                    </div>
                    
                        
                </p>
                <p id="param">
                    
                    <label>Timeout</label>

                    <input class="range" type="range" id="quantite" name="to" step="500" min="1000" max="20000" value="1000">
                    <output class="range" id="valeur" for="quantite">1000ms</output>
                    <script>
                        var slider = document.getElementById("quantite");
                        var output = document.getElementById("valeur");
                        output.innerHTML = slider.value + "ms"; // Affiche la valeur par défaut du slider

                        // Mise à jour de la valeur en temps réel à chaque fois que le curseur est déplacé
                        slider.oninput = function() {
                            output.innerHTML = this.value + "ms";
                        }
                    </script>

                </p>
                <p>
                    
                    <div class="checkbox-wrapper-2">
                        <label for="vue">Vue par couleur</label>
                        <input class="sc-gJwTLC ikxBAC" type="checkbox" name="vue"> 
                    </div>  
                
                    <br>

                    <input class="button" type="submit">
                    <input class="button" type="reset">
                </p>
            </fieldset>
        </form>   
        <div style="text-align:center;">

            <?php
                if ($_SERVER["REQUEST_METHOD"] == "POST") {

                    // Pour rajouter la ligne "pond" => 'on' et savoir s'il faut utiliser une matrice d'adjacence
                    $g = vertedge($_POST["listegraphe"]);
                    $bool = (($g["vertexCount"] * 3) > $g["edgeCount"]); // Formule pour activer liste d'adjacence
                    if ($bool) {
                        formToJSON(true, false);
                    }
                    else {
                        formToJSON(true, true);
                    }

                    echo '<div class="contain">';
                    echo '<div class="bloc" style="margin-right: 7%;">';
                    

                    chdir("cpp/imp");
                    viderDossier('pasapas/'); // Reset les fichier du pas à pas avant de lancer le c++
                    //$exec1 = shell_exec("g++ -o main.o main.cpp coloration.cpp graph.cpp graphListMat.cpp solver.cpp 2>&1");
                    $exec2 = exec("main.o 2>&1");   
                    
                    echo '<h1 style="text-align: center">Visualisation du Graphe Coloré Pondéré :</h1>';

                    if (isset($_POST["vue"])) 
                        echo dotPondToSvgRect($_POST["listegraphe"], "solutionInit.dot", false, -1);
                    else
                        echo dotPondToSvg($_POST["listegraphe"], "solutionInit.dot");
                    
                    echo '</div><div style="text-align: center" class="bloc">';

                    echo "<h1 style=\"text-align: center;\">Visualisation du Graphe Coloré Amélioré :</h1>";

                    if (isset($_POST["vue"]))
                        echo dotPondToSvgRect($_POST["listegraphe"], "solution.dot", false, -1);
                    else
                        echo dotPondToSvg($_POST["listegraphe"], "solution.dot");
                    echo '<br><a href="pasapaspond.php?graphe=' . $_POST["listegraphe"] . '"><button class="button">Voir pas à pas</button></a>';

                    echo "</div></div>";
                    chdir("../../");
                    echo "<pre> \n $exec2 \n </pre>";
                }
            ?>

            </div>
</body>
</html>
