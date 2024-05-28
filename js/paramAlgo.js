document.addEventListener("DOMContentLoaded", ()=> {
    document.querySelector("#listealgo").parentNode.appendChild(genererSelectInitHC());
    document.querySelector("#listealgo").addEventListener("click", (e) => {
        let element = document.querySelector("#algo");
        let liste = document.querySelector("#inithc");
        if ((e.target.value == "Hill-climbing Meilleur Améliorant") || (e.target.value == "Hill-climbing Premier Améliorant") || (e.target.value == "Compactage Flat")) {
            if (liste != undefined) {liste.remove();}
            element.appendChild(genererSelectInitHC());
        } else {
            if (liste != undefined) {liste.remove();}
            
        }
    });
});



