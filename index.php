<?php
    #include ("php/test.php");         // Savoir si le php est activé
    include ("php/selectGraphe.php"); // Charge le select des graphes du repertoier "graph/"
    include ("php/storeJson.php");    // Converti le form en un fichier JSON pour le C++
    include ("php/dispSolution.php"); // Converti un .dot en SVG
?> 

<!DOCTYPE html>
<html lang="fr">
<head>

    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title> Graphe - Coloration </title>
    <link rel="stylesheet" href="css/style.css">
    <script src="js/function.js" defer></script> <!-- Charge les functions js -->
    <script src="js/selectAlgo.js" defer></script> <!-- Charge le DOM js -->

    
</head>
<body>

    <a href="indexpond.php"><button class="button"> Version Pondérée </button></a>

    <h1 class="title">COLORATION DE GRAPHE</h1>

    <h1> Paramètres de la requète : </h1>

    <form method="post">
        <fieldset>
            <legend> Paramètres </legend>
            <p>
                <?php genereSelectFiles("graph/classic/"); ?> 
            </p>
            
            <p id="algo">
                <label>Choisir un algorithme :</label> <br>
                      
            </p>
            <p>
                <input class="button" type="submit">
                <input class="button" type="reset">
            </p>
        </fieldset>
    </form>   

<?php

    if ($_SERVER["REQUEST_METHOD"] == "POST") {

        // pour rajouter la ligne "pond" => 'on' et savoir s'il faut utiliser une matrice d'adjacence
        $g = vertedge($_POST["listegraphe"]);
        $bool = (($g["vertexCount"] * 3) > $g["edgeCount"]); // formule pour activé liste d'adjacence
        if ($bool) {
            formToJSON(false, false);
        }
        else {
            formToJSON(false, true);
        }

        echo '<div class="contain">';
        echo '<div class="bloc" style="margin-right: 7%;">';

        echo '<h1 style="text-align: center">Visualisation du Graphe :</h1>';
        if (isset($_POST["listegraphe"])) dimacsToSvgFile($_POST["listegraphe"]);

        echo '</div><div style="text-align: center" class="bloc">';

        chdir("cpp/imp");
        viderDossier('pasapas/'); // reset les fichier du pas à pas avant de lancer le c++
        //$exec1 = shell_exec("g++ -o main.o main.cpp coloration.cpp graph.cpp graphListMat.cpp solver.cpp 2>&1");
        $exec2 = exec("main.o 2>&1");      

        echo "<h1 style=\"text-align: center;\">Visualisation du graphe coloré :</h1>";
        echo dotToSvg(file_get_contents("solution.dot"));
        echo '<br><a href="pasapas.php"><button class="button">Voir pas à pas</button></a>';

        echo "</div></div>";
        chdir("../../");
        echo "<pre style='text-align : center;'>  \n $exec2 \n </pre>";
    }
    
?>


</body>
</html>
