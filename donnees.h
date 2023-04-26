#ifndef DONNEES_H
#define DONNEES_H
#include <vector>
#include <array>
#include <string>
#include "vec3.h"
#include "triangle3d.h"
#include "voisin.h"
#include "arete.h"
#include "copl.h"
#include "facette.h"
#include "page.h"
#include "const.h"

#include <iostream>
#include "../svg.hpp"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include "vec2.h"
#include "vec3.h"
#include "triangle2d.h"
#include "piece.h"

using std::vector;
using std::array;
using std::string;
using std::fstream;

class Donnees { // DONNEES DEPLIAGE
private :
    vector<Vec3> pts;
    vector<vector<int>> faces;
    vector<string> groupes;
    vector<Triangle3d> t3d;
    vector<Triangle2d> t2d;
    vector<array<Voisin, 3>> voisins;
    vector<Arete> aretes;
    vector<Copl> copl;
    int nbFaces;
    vector<Facette> facettes;
    vector<Page> pages;
    string fichierOBJ;
    vector<string> vLignes;

    Facette* premFaceLibre(int g);

    void init_triangles();
    void calc_voisinage();
    void calc_copl();
    void num_aretes();
    void affiche_points ();
    void affiche_triangles ();
    void affiche_faces ();
    void affiche_groupes ();
    void affiche_aretes ();
    void affiche_voisinage ();
    void charge();

public :
    void affiche_depl ();
    static Vec3 lit_points (string ch);
    static vector<int> lit_faces (string ch, int g);
    void init_depliage();
    int pieceProchainID();
    void depliage();
    int getNbFaces();
    Donnees(string f);
    void affiche();
};

#endif // DONNEES_H
