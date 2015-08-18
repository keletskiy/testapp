#pragma once

#include <ode/ode.h>

class OdObject {};

class OdBodyPart: public OdObject
{
public:
    explicit OdBodyPart(const dWorldID & _worldId,const dSpaceID & _spaceId, const void * parameters):worldId(_worldId),spaceId(_spaceId){
        setParameters(parameters);
        createBodyPart();
    }

protected :
    const dWorldID & worldId;
    const dSpaceID & spaceId;

    dJointID jointId;
    dJointGroupID jointGroupID;

    // Create an object
    virtual void createBodyPart() = 0;
    virtual void setParameters(const void *) = 0;
};

class OdSimulationExecutableObject: public OdObject
{
public:
    virtual void execute() = 0;
};