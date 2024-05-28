function genererSelectNotPond() {
    const listeOptions = ["Glouton", "Trié par couleur", "Permutation couleur", "Permutation MAJ couleur", "DSatur"];
    selectElement = document.createElement("select");
    selectElement.name = "listealgo";
    selectElement.id = "listealgo";
    for (const option of listeOptions) {
        const optionElement = document.createElement("option");
        optionElement.value = option;
        optionElement.textContent = option;
        selectElement.appendChild(optionElement);
    }
    return selectElement;
}

function genererSelectInitHC() {
    const listeOptions = ["Coloration MAX", "Glouton", "Trié par couleur", "Permutation couleur", "Permutation MAJ couleur", "DSatur"];
    selectElement = document.createElement("select");
    selectElement.name = "inithc";
    selectElement.id = "inithc";
    for (const option of listeOptions) {
        const optionElement = document.createElement("option");
        optionElement.value = option;
        optionElement.textContent = option;
        selectElement.appendChild(optionElement);
    }
    return selectElement;
}

function genererSelectPond() {
    const listeOptions = ["Hill-climbing Meilleur Améliorant", "Hill-climbing Premier Améliorant", "Compactage Flat", "RLI HCPA", "RLI HCCO"]; // à add
    selectElement = document.createElement("select");
    selectElement.name = "listealgo";
    selectElement.id = "listealgo";
    for (const option of listeOptions) {
        const optionElement = document.createElement("option");
        optionElement.value = option;
        optionElement.textContent = option;
        selectElement.appendChild(optionElement);
    }
    return selectElement;
}

