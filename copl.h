#ifndef COPL_H
#define COPL_H


class Copl { // COPLANARITY
public :
    int nF;
    int nV;
    int cop;

    Copl();
    Copl(int, int, int);
};

Copl* getCopl(int F, int V);


#endif // COPL_H
