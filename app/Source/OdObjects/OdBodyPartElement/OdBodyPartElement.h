#pragma once
#include <ode/ode.h>

class  OdBodyPartElement {
public:
    dBodyID body;   // a body
    dGeomID geom;   //  a geometry
    dReal   radius, length,  mass; // radius[m], length[m], mass[kg]
};