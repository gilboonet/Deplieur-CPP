#ifndef DONNEES_H
#define DONNEES_H


#include <vector>
#include <array>
#include <string>
#include "vec3.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <ostream>

#include "vec2.h"
#include "vec3.h"
#include "triangle2d.h"
#include "triangle3d.h"
#include "voisin.h"
#include "arete.h"
#include "copl.h"
#include "facette.h"
#include "const.h"
#include "piece.h"
#include "page.h"
#include "../svg.hpp"

class Donnees { // DONNEES DEPLIAGE
private :
    std::vector<Vec3> pts;
    std::vector<std::vector<int>> faces;
    std::vector<std::string> groupes;
    std::vector<Triangle3d> t3d;
    std::vector<Triangle2d> t2d;
    std::vector<std::array<Voisin, 3>> voisins;
    std::vector<Arete> aretes;
    std::vector<Copl> copl;
    int nbFaces;
    std::vector<Facette> facettes;
    std::vector<Page> pages;
    std::string fichierOBJ;
    std::vector<std::string> vLignes;

    Facette* premFaceLibre (int g);
    void init_triangles ();
    void calc_voisinage ();
    void calc_copl ();
    void num_aretes ();
    void affiche_points ();
    void affiche_triangles ();
    void affiche_faces ();
    void affiche_groupes ();
    void affiche_aretes ();
    void affiche_voisinage ();
    void charge ();
    Piece* pieceGetById (std::vector<Piece> &pieces, int id);

public :
    Donnees (std::string f);

    void affiche_depl ();
    void affiche_facettes (std::ostream& os);
    static Vec3 lit_points (std::string ch);
    static std::vector<int> lit_faces (std::string ch, int g);
    void init_depliage ();
    int pieceProchainId ();
    void depliage ();
    int getNbFaces ();  
    void affiche ();
    void cree_SVG();
};


#endif // DONNEES_H
