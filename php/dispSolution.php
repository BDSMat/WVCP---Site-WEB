<?php
function dotToSvg($dotcontent) {
    // Parser pour extraire les informations des sommets et des arêtes
    $lines = explode("\n", $dotcontent);
    $vertices = [];
    $edges = [];
    foreach ($lines as $line) {
        if (preg_match('/(\d+)\[.*color=([^\]]+)\]/', $line, $matches)) {
            $vertices[$matches[1]] = $matches[2];
        } elseif (preg_match('/(\d+) -- (\d+)/', $line, $matches)) {
            $edges[] = [$matches[1], $matches[2]];
        }
    }

    // Calculer les positions des sommets pour un polygone équilatéral
    $nodeCount = count($vertices);
    $maxCanvasSize = 800; // Taille maximale du canevas (en pixels)
    $minNodeSize = 5; // Taille minimale des nœuds (en pixels)
    $maxNodeSize = 50; // Taille maximale des nœuds (en pixels)
    $minFontSize = 7; // Taille minimale de la police (en pixels)
    $maxFontSize = 40; // Taille maximale de la police (en pixels)

    $radius = min($maxCanvasSize / 2 - 50, $maxCanvasSize / 2 - 50); // Rayon du cercle (la moitié de la taille du canevas)
    $center_x = $maxCanvasSize / 2 ; // Centre du canevas (x)
    $center_y = $maxCanvasSize / 2 ; // Centre du canevas (y)
    $angleStep = (2 * M_PI) / $nodeCount; // Angle entre chaque sommet

    $nodePositions = [];
    foreach ($vertices as $node => $color) {
        $angle = $angleStep * $node;
        $x = $center_x + $radius * cos($angle);
        $y = $center_y + $radius * sin($angle);
        $nodePositions[$node] = ['x' => $x, 'y' => $y];
    }

    // Début du document SVG
    $svgContent = '<svg width="800" height="800" xmlns="http://www.w3.org/2000/svg">';

    // Ajoute les arêtes
    foreach ($edges as list($node1, $node2)) {
        $x1 = $nodePositions[$node1]['x'];
        $y1 = $nodePositions[$node1]['y'];
        $x2 = $nodePositions[$node2]['x'];
        $y2 = $nodePositions[$node2]['y'];
        $svgContent .= "<line x1=\"$x1\" y1=\"$y1\" x2=\"$x2\" y2=\"$y2\" stroke=\"black\" stroke-width=\"1\" opacity=\"0.50\" />" . PHP_EOL;
    }

    // Ajoute les sommets
    foreach ($vertices as $node => $color) {
        $angle = $angleStep * array_search($node, array_keys($vertices));
        $x = $center_x + $radius * cos($angle);
        $y = $center_y + $radius * sin($angle);
        $nodeSize = max($minNodeSize, $maxNodeSize - $nodeCount); // Ajuste la taille des nœuds
        $fontSize = max($minFontSize, $maxFontSize - $nodeCount); // Ajuste la taille de la police
        $svgContent .= "<circle cx=\"$x\" cy=\"$y\" r=\"$nodeSize\" stroke=\"black\" stroke-width=\"3\" fill=\"$color\" />" . PHP_EOL;
        $svgContent .= "<text x=\"$x\" y=\"$y\" font-family=\"Verdana\" font-size=\"$fontSize\" fill=\"white\" text-anchor=\"middle\" dominant-baseline=\"central\">".(($node)+1)."</text>" . PHP_EOL;
    }

    // Fin du document SVG
    $svgContent .= '</svg>';

    return $svgContent;
}

function dotPondToSvgRect($dotName, $init, bool $pap, int $ind) {
    $dotcontent = file_get_contents($init);
    $weights = file("../../" . $dotName . ".w", FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
    

    // Parser pour extraire les informations des sommets et des arêtes
    $lines = explode("\n", $dotcontent);
    $vertices = [];
    $edges = [];
    $vertexColors = []; // Nouveau tableau pour stocker les couleurs des sommets
    $nodePositionId = [];
    $coul = [];

    foreach ($lines as $line) {
        if (preg_match('/(\d+)\[.*color=([^\]]+)\]/', $line, $matches)) {
            $vertexId = $matches[1];
            $color = $matches[2];
            $coul[$matches[2]][] = $matches[1];
            $vertices[$vertexId] = $color;
            $vertexColors[$vertexId] = $color; // Ajout de la couleur au tableau des sommets

            if (!isset($nodePositionId[$color])) {
                $nodePositionId[$color] = 0; // Commence à la position 0
            }
            $nodePositionId[$color]++;
        } else if (preg_match('/(\d+) -- (\d+)/', $line, $matches)) {
            $edges[] = [$matches[1], $matches[2]];
        }
    }
    
    // print_r($vertexColors);
    // echo '<br>';
    // print_r($nodePositionId);
    // echo '<br>';
    // print_r($newArray);
    // echo '<br>';


    // Calculer les positions des sommets pour un polygone équilatéral
    $nodeCount = count($vertices);
    $maxCanvasSize = 800; // Taille maximale du canevas (en pixels)
    $minNodeSize = 10; // Taille minimale des nœuds (en pixels)
    $maxNodeSize = 35; // Taille maximale des nœuds (en pixels)
    $minFontSize = 10; // Taille minimale de la police (en pixels)
    $maxFontSize = 30; // Taille maximale de la police (en pixels)

    $center_x = $maxCanvasSize / 2 ; // Centre du canevas (x)
    $center_y = $maxCanvasSize / 2 ; // Centre du canevas (y)

    $newArray = [];
    $index = 0;

    foreach ($nodePositionId as $color => $value) {
        $newArray[$color] = [$index+1, $value];
        $index++;
    }


    $nbcoul = count($coul);
    $nodeSize = max($minNodeSize, $maxNodeSize - $nbcoul);
    $fontSize = max($minFontSize, $maxFontSize - $nbcoul);
    

    // Début du document SVG
    $svgContent = '<svg width="800" height="800" xmlns="http://www.w3.org/2000/svg">';
    
    // Affiche les rectangles + poids par couleurs
    if ($pap) {

        //print_r ($newArray);
        $graphParas = glob("pasapas/*.prm");
        natsort($graphParas);
        $currentPara = $graphParas[$ind];
        $lines = file($currentPara); 
        if (isset($lines[1])) {
            $currentColPoids = explode(",", trim($lines[1]));
        }
        $cmp = 0;
        foreach ($newArray as $color => $pos) {

            $x =  ($pos[0]  * ($maxCanvasSize / ($nbcoul + 1))) - $nodeSize;
            $maxValue = max(array_column($newArray, 1));
            $y = ($maxCanvasSize/($maxValue+1)) - $nodeSize*2; 
            $height = ($maxCanvasSize / ($maxValue+1)) * $maxValue;
                
            $svgContent .= "<text x=\"". ($x+$nodeSize) ."\" y=\"". ($y+$nodeSize/2) ."\" font-family=\"Verdana\" font-size=\"" . ($fontSize-2). "\" fill=\"$color\" text-anchor=\"middle\" dominant-baseline=\"central\">" . $currentColPoids[$cmp] . "</text>" . PHP_EOL;    
            $svgContent .= '<rect x="' . $x . '" y="' . $y . '" width="' . ($nodeSize*2) . '" height="' . ($height) . '" fill="none" stroke="'. $color .'" stroke-width="2px" />';
            $cmp++;
        }
    }
    // Affichage des couleurs et positions des nœuds
    foreach ($vertexColors as $id => $colors) {
        $x =  $newArray[$colors][0] * ($maxCanvasSize / ($nbcoul + 1));
        $y =  ($newArray[$colors][1]) * ($maxCanvasSize / (count($coul[$colors]) + 1));
        $newArray[$colors][1] -= 1;
        $nodePositions[$id] = ['x' => $x, 'y' => $y];
    }

    // Ajoute les arêtes
    foreach ($edges as list($node1, $node2)) {
        $x1 = $nodePositions[$node1]['x'];
        $y1 = $nodePositions[$node1]['y'];
        $x2 = $nodePositions[$node2]['x'];
        $y2 = $nodePositions[$node2]['y'];
        $svgContent .= "<line x1=\"$x1\" y1=\"$y1\" x2=\"$x2\" y2=\"$y2\" stroke=\"black\" stroke-width=\"1\" opacity=\"0.50\" />" . PHP_EOL;
    }

    // Ajoute les sommets
    // echo '<br>';
    // print_r($vertices);
    $cmp = 0;
    foreach ($vertices as $node => $color) {
        $x = $nodePositions[$node]['x'];
        $y = $nodePositions[$node]['y'];
        $nodeSize = max($minNodeSize, $maxNodeSize - $nbcoul); // Ajuste la taille des nœuds
        $fontSize = max($minFontSize, $maxFontSize - $nbcoul); // Ajuste la taille de la police
        $svgContent .= "<circle cx=\"$x\" cy=\"$y\" r=\"$nodeSize\" stroke=\"black\" stroke-width=\"3\" fill=\"$color\" />" . PHP_EOL;
        $svgContent .= "<text x=\"$x\" y=\"$y\" font-family=\"Verdana\" font-size=\"$fontSize\" fill=\"white\" text-anchor=\"middle\" dominant-baseline=\"central\">$node</text>" . PHP_EOL;
        //$x = $x + 10;
        $y = $y - ($nodeSize/2);
        $fontSize = $fontSize - ($nodeSize/3);
        $svgContent .= "<text x=\"$x\" y=\"$y\" font-family=\"Verdana\" font-size=\"" . $fontSize. "\" fill=\"white\" text-anchor=\"middle\" dominant-baseline=\"central\"> w:" . $weights[$cmp] . "</text>" . PHP_EOL;
        $cmp++;
    }

    // Fin du document SVG
    $svgContent .= '</svg>';

    return $svgContent;
}


function dotPondToSvg($dotName, $init) {

    $dotcontent = file_get_contents($init);
    $weights = file("../../" . $dotName . ".w", FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
    //print_r($weights);

    // Parser pour extraire les informations des sommets et des arêtes
    $lines = explode("\n", $dotcontent);
    $vertices = [];
    $edges = [];
    foreach ($lines as $line) {
        if (preg_match('/(\d+)\[.*color=([^\]]+)\]/', $line, $matches)) {
            $vertices[$matches[1]] = $matches[2];
        } elseif (preg_match('/(\d+) -- (\d+)/', $line, $matches)) {
            $edges[] = [$matches[1], $matches[2]];
        }
    }

    // Calculer les positions des sommets pour un polygone équilatéral
    $nodeCount = count($vertices);
    $maxCanvasSize = 800; // Taille maximale du canevas (en pixels)
    $minNodeSize = 5; // Taille minimale des nœuds (en pixels)
    $maxNodeSize = 50; // Taille maximale des nœuds (en pixels)
    $minFontSize = 7; // Taille minimale de la police (en pixels)
    $maxFontSize = 40; // Taille maximale de la police (en pixels)

    $radius = min($maxCanvasSize / 2 - 50, $maxCanvasSize / 2 - 50); // Rayon du cercle (la moitié de la taille du canevas)
    $center_x = $maxCanvasSize / 2 ; // Centre du canevas (x)
    $center_y = $maxCanvasSize / 2 ; // Centre du canevas (y)
    $angleStep = (2 * M_PI) / $nodeCount; // Angle entre chaque sommet

    $nodePositions = [];
    foreach ($vertices as $node => $color) {
        $angle = $angleStep * $node;
        $x = $center_x + $radius * cos($angle);
        $y = $center_y + $radius * sin($angle);
        $nodePositions[$node] = ['x' => $x, 'y' => $y];
    }

    // Début du document SVG
    $svgContent = '<svg width="800" height="800" xmlns="http://www.w3.org/2000/svg">';

    // Ajoute les arêtes
    foreach ($edges as list($node1, $node2)) {
        $x1 = $nodePositions[$node1]['x'];
        $y1 = $nodePositions[$node1]['y'];
        $x2 = $nodePositions[$node2]['x'];
        $y2 = $nodePositions[$node2]['y'];
        $svgContent .= "<line x1=\"$x1\" y1=\"$y1\" x2=\"$x2\" y2=\"$y2\" stroke=\"black\" stroke-width=\"1\" opacity=\"0.50\" />" . PHP_EOL;
    }

    // Ajoute les sommets
    $cmp = 0;
    foreach ($vertices as $node => $color) {
        $angle = $angleStep * array_search($node, array_keys($vertices));
        $x = $center_x + $radius * cos($angle);
        $y = $center_y + $radius * sin($angle);
        $nodeSize = max($minNodeSize, $maxNodeSize - $nodeCount); // Ajuste la taille des nœuds
        $fontSize = max($minFontSize, $maxFontSize - $nodeCount); // Ajuste la taille de la police
        $svgContent .= "<circle cx=\"$x\" cy=\"$y\" r=\"$nodeSize\" stroke=\"black\" stroke-width=\"3\" fill=\"$color\" />" . PHP_EOL;
        $svgContent .= "<text x=\"$x\" y=\"$y\" font-family=\"Verdana\" font-size=\"$fontSize\" fill=\"white\" text-anchor=\"middle\" dominant-baseline=\"central\">$node</text>" . PHP_EOL;
        //$x = $x + 10;
        $y = $y - 20;
        $fontSize = $fontSize-10;
        $svgContent .= "<text x=\"$x\" y=\"$y\" font-family=\"Verdana\" font-size=\"" . $fontSize. "\" fill=\"white\" text-anchor=\"middle\" dominant-baseline=\"central\"> w:" . $weights[$cmp] . "</text>" . PHP_EOL;
        $cmp++;
    }


    // Fin du document SVG
    $svgContent .= '</svg>';

    return $svgContent;
}


function viderDossier($dossier) {
    $fichiers = glob($dossier . '/*'); // Obtient tous les fichiers
    foreach($fichiers as $fichier) { // Itère à travers les fichiers
        if(is_file($fichier)) {
            unlink($fichier); // Supprime le fichier
        }
    }
}


?>