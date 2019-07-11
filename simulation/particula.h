/*
    This file contains a struct that represents a
    particle caracteristics during simulation.
    
    The particle is describe by three vector in
    R3 space: postion, magnetic momentum and
    magnetization.
*/

struct particula {
    double r[3]; // position
    double m[3]; // magnetic momentum
    double magz[3]; // magnetization
};
