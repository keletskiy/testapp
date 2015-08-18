#pragma once

#include "../../OdGraphics/_all.h"
#include "../_all.h"

class OdLeg : public OdBodyPart , public OdSimulationExecutableObject, public OdDrawableObject
{
public:

    void execute(){
        dJointGroupEmpty(jointGroupID); // Empty the joint group
    }

    void draw(){
        // Draw a ball
        dsSetColor(1.0,0.0,0.0);   // Set red color
        const dReal * pos1 = dBodyGetPosition(calfBone.body); //Get a position
        const dReal * R1   = dBodyGetRotation(calfBone.body); //Get an orientation
        dsDrawCapsule(pos1,R1,calfBone.length,calfBone.radius); //Draw a capsule

        // Draw a capsule
        const dReal * pos2 = dBodyGetPosition(footBone.body); //Get a position
        const dReal * R2   = dBodyGetRotation(footBone.body); //Get an orientation
        dsDrawCapsule(pos2,R2,footBone.length,footBone.radius); //Draw a capsule
    }

protected :

    dReal   mass , radius;
    OdBodyPartElement calfBone, footBone;

    // Create an object
    virtual void createBodyPart()
    {
        jointGroupID = dJointGroupCreate(0); // Create a joint group

        dMass m1, m2;
        dReal x0 = 0.0, y0 = 0.0, z0 = 2.5;    // a ball
        // a capsule
        calfBone.radius = 0.025;
        calfBone.length = 1.0;
        calfBone.mass   = 1.0;
        calfBone.body =  dBodyCreate(worldId);
        dMassSetZero(&m1);
        dMassSetSphereTotal(&m1,calfBone.mass,calfBone.radius);
        dBodySetMass(calfBone.body,&m1);
        dBodySetPosition(calfBone.body, x0, y0, z0);
        calfBone.geom =dCreateCapsule(spaceId,calfBone.radius,calfBone.length);
        dGeomSetBody(calfBone.geom,calfBone.body);

        // a capsule
        footBone.radius = 0.025;
        footBone.length = 1.0;
        footBone.mass   = 1.0;
        footBone.body =  dBodyCreate(worldId);
        dMassSetZero(&m2);
        dMassSetCapsuleTotal(&m2,footBone.mass,3,footBone.radius,footBone.length);
        dBodySetMass(footBone.body,&m2);
        dBodySetPosition(footBone.body, x0, y0+0.05, z0 - 0.5 * footBone.length);
        footBone.geom =  dCreateCapsule(spaceId,footBone.radius,footBone.length);
        dGeomSetBody(footBone.geom,footBone.body);

        // hinge joint
        jointId = dJointCreateHinge(worldId, 0); // Create a hinge joint
        dJointAttach(jointId, calfBone.body,footBone.body); // Attach joint to bodies
        dJointSetHingeAnchor(jointId, x0, y0, z0); // Set a joint anchor
        dJointSetHingeAxis(jointId, 1, 0, 0); //Set a hinge axis(1,0,0)
    }

    virtual void setParameters(const void *) {
        radius = 0.2;
        mass   = 1.0;
    }

};