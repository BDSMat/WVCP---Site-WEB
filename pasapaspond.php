<?php

    session_start(); // Démarre une nouvelle session ou reprend une session existante

    #include ("php/test.php");         // Savoir si le php est activé
    include ("php/selectGraphe.php"); // Charge le select des graphes du repertoier "graph/"
    include ("php/storeJson.php");    // Converti le form en un fichier JSON pour le C++
    include ("php/dispSolution.php"); // Converti un .dot en SVG

    if ((!isset($_SESSION['index'])) || (($_SERVER['REQUEST_METHOD'] === 'GET'))) {
        $_SESSION['index'] = 0; // Initialise l'index si ce n'est pas déjà fait
    }


    if (isset($_POST['enter'])) {
        $pageIndex = filter_input(INPUT_POST, 'page', FILTER_VALIDATE_INT);
        if ($pageIndex !== false) {
            $_SESSION['index'] = $pageIndex - 1; // Les index commencent à 0
        }
    }
?> 
<?php

chdir("cpp/imp");
$graphFiles = glob("pasapas/*.dotA");
$graphParas = glob("pasapas/*.prm");

$totalFiles = count($graphFiles);

natsort($graphFiles);
natsort($graphParas);
$graphFiles = array_values($graphFiles);

// Bouton précédent
if (isset($_POST['previous']) && $_SESSION['index'] > 0) {
    $_SESSION['index']--;
}

// Bouton suivant
if (isset($_POST['next']) && $_SESSION['index'] < $totalFiles - 1) {
    $_SESSION['index']++;
}
//print_r($graphFiles);
$currentFile = $graphFiles[$_SESSION['index']];
$currentPara = $graphParas[$_SESSION['index']];




// Crée les boutons de navigation

?>

<!DOCTYPE html>
<html lang="fr">
<head>

    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title> Pas à pas pour Graphe Pondéré </title>
    <link rel="stylesheet" href="css/style.css">
    
</head>
<body>
    <div style="text-align : center;">
        <br>
        <h1> Visualisation pas à pas : </h1>

        <div class="ffm">
            <form method="post"> 
                <p> <?php echo "Etape n° <input type='number' name='page' min='1' max='$totalFiles' size='5' value='". ($_SESSION['index'] + 1) ."'>"  . " / " . $totalFiles . "</p>";?>
                <?php
                    $lignes = file ($currentPara);
                    echo '<p>';
                    echo "Nombre de couleurs : " . $lignes[0];
                    echo "<br>";
                    echo "Poids pour chaque couleur : " . $lignes[1];
                    echo "<br>";
                    echo "Meilleur Poids à cet instant / Poids du graphe : " . $lignes[2];
                    echo '</p>';
                ?>
                
                <input class="button" type="submit" name="previous" value="Précédent">
                <input class="button" type="submit" name="enter" value="Entrer">
                <input class="button" type="submit" name="next" value="Suivant">

            </form>
            <br>
        </div>
        <br>
        <p style="margin-bottom: 25px; color: #2c7a83;"> 
            <?php 
                echo $_GET["graphe"];
            ?> 
        </p>
        <div style="">
            <?php echo dotPondToSvgRect("cpp/imp/currentWeight.col", $currentFile, true, $_SESSION['index']); ?>
        </div>
        <a href="indexpond.php"><button class="button">Retour</button></a>
    </div>


</body>
</html>

<?php chdir("../../");?>