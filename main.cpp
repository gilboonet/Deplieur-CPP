//---------------------------------------------------------
// DEPLIAGE D'UN VOLUME version C++
// 1°) Lecture du fichier (format OBJ) contenant le volume
// 2°) Extraction des points (FAIT) et des faces (FAIT)
// 3°) Création des triangles 3d et 2d (FAIT)
// 4°) Calcul du voisinage de chaque face (FAIT)
// 5°) Calcul des coplanéités (FAIT)
// 6°) Numérotation des arêtes (FAIT)
// 7°) Découpage en classes (FAIT)
// 8°) Dépliage automatique sans chevauchement (FAIT)
//---------------------------------------------------------
// Gilbert Duval 2023-03-29 - 2023-04-23
// https://raw.githubusercontent.com/vincentlaucsb/svg/master/src/svg.hpp
// C++20

#include "donnees.h"

int main (int argc, char** argv) {
    // Le volume est lu depuis un fichier au format .OBJ
    if (argc > 0) {
        Donnees donnees (argv[1]); // Creation du dépliage

        donnees.depliage ();
        donnees.cree_SVG ();

        std::ofstream sauveDat("donnees.dat");
        donnees.affiche_facettes (std::cout);
        donnees.affiche_facettes (sauveDat);
        sauveDat.close();

        donnees.affiche_depl ();
        return 0;
    }
}
