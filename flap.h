#ifndef FLAP_H
#define FLAP_H


class Flap
{
public:
    int fId;
    int nId;
    int state;
    //   0 : all off
    //   1 : F on  N off
    // - 1 : F off N On
    //   2 : all on

    Flap();
    Flap(int, int, int);
};

#endif // FLAP_H
