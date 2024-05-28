
document.addEventListener("DOMContentLoaded", () => {
    
        let element = document.querySelector("#algo");
        let select = genererSelectNotPond();

        if (document.title == "Graphe Pondéré - Coloration") {
            select = genererSelectPond();
        } 
        element.appendChild(select);
        

});





