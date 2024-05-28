<?php
   function formToJSON(bool $pond, bool $list) {
    $data = [];

    $fileName = 'parametres.json';

    $directory = 'json/';

    $filePath = $directory . $fileName;

    if ($pond) $data["pond"] = "on";
    if ($list) $data["listAdj"] = "on";

    foreach ($_POST as $name => $val) {
        $data[$name] = $val;
    }

    if (!isset($data["inithc"])) $data["inithc"] = " ";
    $jsonData = json_encode($data, JSON_PRETTY_PRINT);

    file_put_contents($filePath, $jsonData);
}
?>