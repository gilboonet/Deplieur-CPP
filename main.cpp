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

int main(int argc, char** argv) {
//-----> pour créer un nouveau dépliage <fichier.dep> à partir de <fichier.obj>
// deplie fichier.obj [fichier.dat fichier.svg]

//-----> pour créer un gabarit fichier <fichier.svg> à partir du dépliage <fichier.dep>
// deplie fichier.dep fichier.svg

    // Le volume est lu depuis un fichier au format .OBJ
    if(argc > 1) {
        std::string ext = argv[1];
        std::string obj, dat, svg;
        bool ok = false;

        if(ext.ends_with(".obj")) {
            obj = argv[1];
            if(argc == 2) {
                ext = ext.substr(0, ext.size() -4);
                dat = ext + ".dat";
                svg = ext + ".svg";
                ok = true;
            } else if(argc > 3) {
                ext = argv[2];
                if(ext.ends_with(".dat")) {
                    dat = argv[2];
                    ext = argv[3];
                    if(ext.ends_with(".svg")) {
                        svg = argv[3];
                        ok = true;
                    }
                } else if(ext.ends_with(".svg")) {
                    svg = argv[2];
                    ext = argv[3];
                    if(ext.ends_with(".dat")) {
                        dat = argv[3];
                        ok = true;
                    }
                }
            }
            if(!ok) {
                std::cout << "ERREUR DE PARAMETRE (MODE CREATION)" << std::endl;
                std::cout << "Entrez un fichier .obj pour le volume" << std::endl;
                std::cout << "Et optionnellement :" << std::endl;
                std::cout << "  un fichier .dat pour les données" << std::endl;
                std::cout << "  et un fichier .svg pour le gabarit." << std::endl;
                return 1;
            }

            Donnees donnees(obj, dat, svg); // Creation du dépliage

            donnees.depliage();
            donnees.cree_SVG(svg);

            donnees.affiche_depl(std::cout);

            std::ofstream sauveDat(dat);
            donnees.affiche_depl(sauveDat);
            sauveDat.close();

        } else if(ext.ends_with(".dat")) {
            dat = argv[1];
            if(argc == 2) {
                ext = ext.substr(0, ext.size() -4);
                svg = ext + ".svg";
                ok = true;
            } else if(argc > 3) {
                ext = argv[2];
                if (ext.ends_with(".svg")) {
                    svg = argv[2];
                    ok = true;
                }
            }

            if(!ok) {
                std::cout << "ERREUR DE PARAMETRE" << std::endl;
                std::cout << "Entrez un fichier .dat pour les données" << std::endl;
                std::cout << "Et optionnellement :" << std::endl;
                std::cout << "un fichier .svg pour le gabarit." << std::endl;
                return 1;
            }

            std::ifstream fDAT(dat);
            if (fDAT.good()) {
                getline(fDAT, obj);
            }

            Donnees donnees(obj, dat, svg);

            donnees.affiche_facettes(std::cout);
        }
        return 0;
    }
}
