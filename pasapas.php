<?php

    session_start(); // Démarre une nouvelle session ou reprend une session existante

    #include ("php/test.php");         // Savoir si le php est activé
    include ("php/selectGraphe.php"); // Charge le select des graphes du repertoier "graph/"
    include ("php/storeJson.php");    // Converti le form en un fichier JSON pour le C++
    include ("php/dispSolution.php"); // Converti un .dot en SVG

    if ((!isset($_SESSION['index'])) || (($_SERVER['REQUEST_METHOD'] === 'GET'))) {
        $_SESSION['index'] = 0; // Initialise l'index si ce n'est pas déjà fait
    }

?> 
<?php

chdir("cpp/imp");

$graphFiles = glob("pasapas/*.dot");

$totalFiles = count($graphFiles);
natsort($graphFiles);
$graphFiles = array_values($graphFiles);

// Bouton précédent
if (isset($_POST['previous']) && $_SESSION['index'] > 0) {
    $_SESSION['index']--;
}

// Bouton suivant
if (isset($_POST['next']) && $_SESSION['index'] < $totalFiles - 1) {
    $_SESSION['index']++;
}


$currentFile = $graphFiles[$_SESSION['index']];
?>

<!DOCTYPE html>
<html lang="fr">
<head>

    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title> Pas à pas pour Graphe Classique </title>
    <link rel="stylesheet" href="css/style.css">
    
</head>
<body>
    <div style="text-align : center;">
        <br>
        <h1> Visualisation pas à pas : </h1>
        <div class="ffm">        
            <form method="post">
                <?php echo "<p>Etape n°" . ($_SESSION['index']+1) . "/" . $totalFiles . "</p>"; ?>
                <input class="button" type="submit" name="previous" value="Précédent">
                <input class="button" type="submit" name="next" value="Suivant">
                <br>
            </form>
        </div>    
        <br>
        <div style="">
            <?php echo dotToSvg(file_get_contents($currentFile)); ?>
        </div>
        <br><a href="index.php"><button class="button">Retour</button></a>
        </div>

 <?php chdir("../../"); ?>





</body>
</html>
