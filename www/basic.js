function changerCouleur()
{
        //On récupère tout les input.
    var input1 = document.getElementsByClassName("test");
    
        //Comme le résultat est un array on est obliger de le parcourrir pour
        //Changer chaque input
    for (var i = 0; i < input1.length; i++) {
        if(input1[i].style.backgroundColor == "red")
            input1[i].style.backgroundColor = "orange";
        else
        input1[i].style.backgroundColor = "red"
    }
}