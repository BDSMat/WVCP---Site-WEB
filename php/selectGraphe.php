<?php

function dimacsToSvgFile(string $filename) {
    echo dimacsToSvg(file_get_contents($filename));
}

function dimacsToSvg($dimacsContent) {
    $lines = explode("\n", $dimacsContent);
    $nodes = [];
    $edges = [];

    foreach ($lines as $line) {
        $parts = preg_split('/\s+/', $line);
        if ($parts[0] === 'c') {
            continue;
        } elseif ($parts[0] === 'p') {
            // Cette ligne contient la déclaration du problème
            $numNodes = intval($parts[2]);
            $nodes = array_map(function ($i) { return strval($i + 1); }, range(0, $numNodes - 1));
        } elseif ($parts[0] === 'e') {
            // Cette ligne contient une arête
            $edges[] = [$parts[1], $parts[2]];
        }
    }

    // Calcul de la taille du graphique en fonction du nombre de sommets
    $maxCanvasSize = 800; // Taille maximale du canevas (en pixels)
    $minNodeSize = 5; // Taille minimale des nœuds (en pixels)
    $maxNodeSize = 50; // Taille maximale des nœuds (en pixels)
    $minFontSize = 7; // Taille minimale de la police (en pixels)
    $maxFontSize = 40; // Taille maximale de la police (en pixels)

    $radius = min($maxCanvasSize / 2 - 50, $maxCanvasSize / 2 - 50); // Rayon du cercle (la moitié de la taille du canevas)
    $center_x = $maxCanvasSize / 2 ; // Centre du canevas (x)
    $center_y = $maxCanvasSize / 2 ; // Centre du canevas (y)
    $angleStep = (2 * M_PI) / count($nodes); // Angle entre chaque sommet

    // Génère le contenu SVG
    $svgContent = '<svg width="' . $maxCanvasSize . '" height="' . $maxCanvasSize . '" xmlns="http://www.w3.org/2000/svg">';

    $nodePositions = []; // Stocke les positions des nœuds
    foreach ($nodes as $i => $node) { // Remplis les positions des noeuds
        $angle = $angleStep * $i;
        $x = $center_x + $radius * cos($angle);
        $y = $center_y + $radius * sin($angle);
        $nodePositions[$node] = ['x' => $x, 'y' => $y];
    }

    foreach ($edges as list($node1, $node2)) { // Ajoute les arêtes
        $x1 = $nodePositions[$node1]['x'];
        $y1 = $nodePositions[$node1]['y'];
        $x2 = $nodePositions[$node2]['x'];
        $y2 = $nodePositions[$node2]['y'];
        $svgContent .= "<line x1=\"$x1\" y1=\"$y1\" x2=\"$x2\" y2=\"$y2\" stroke=\"black\" stroke-width=\"1\" opacity=\"0.50\" />" . PHP_EOL;
    }

    foreach ($nodes as $node) { // Ajoute les sommets
        $angle = $angleStep * array_search($node, $nodes);
        $x = $center_x + $radius * cos($angle);
        $y = $center_y + $radius * sin($angle);
        $nodePositions[$node] = ['x' => $x, 'y' => $y];
        $nodeSize = max ($minNodeSize, $maxNodeSize - min ($maxNodeSize, count($nodes))); // Ajuste la taille des nœuds
        $fontSize = max ($minFontSize, $maxFontSize - min ($maxFontSize, count($nodes))); // Ajuste la taille de la police
        $svgContent .= "<circle cx=\"$x\" cy=\"$y\" r=\"$nodeSize\" stroke=\"black\" stroke-width=\"3\" fill=\"white\" />" . PHP_EOL;
        $svgContent .= "<text x=\"$x\" y=\"$y\" font-family=\"Verdana\" font-size=\"$fontSize\" fill=\"black\" text-anchor=\"middle\" dominant-baseline=\"central\">$node</text>" . PHP_EOL;
    }

    $svgContent .= '</svg>';
    return $svgContent;
}


function genereSelectFiles(string $dirname) {

    if (!is_dir($dirname)) {
        echo "le dossier est inexistant";
        return;
    }
    // Récupère la liste des fichiers .gr dans le répertoire
    $graphFiles = glob($dirname . "*.col");

    // Génère l'élément <select> avec les options
    echo '<label for="listeGraphe">Choisir un graphe :</label> <br>';
    echo '<select id="listeGraphe" name="listegraphe" required>';
    
    foreach ($graphFiles as $file) {
        echo '<option value="' . "$file" . '">' . "Graphe $file" . '</option>';
    }
    echo '</select>';
}


function vertedge($file_path) {
    $handle = fopen($file_path, "r");
    if ($handle) {
        while (($line = fgets($handle)) !== false) {
            if (strpos($line, "p") === 0) {
                $tokens = explode(" ", $line);
                $vertexCount = intval($tokens[2]);
                $edgeCount = intval($tokens[3]);
                break;
            }
        }
        fclose($handle);
        return ["vertexCount" => $vertexCount, "edgeCount" => $edgeCount];
    } else {
        return null; // Fichier non trouvé ou erreur lors de la lecture
    }
}

?>
