#include "main.h"
#include <list>

/*
static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
const int N = 10;
dContact contact[N];

int isGround = ((groundId == o1) || (groundId == o2));

int n =  dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));

if (isGround)  {
if (n >= 1) flag = 1;
else        flag = 0;
for (int i = 0; i < n; i++) {
contact[i].surface.mode = dContactBounce;
contact[i].surface.mu   = dInfinity;
contact[i].surface.bounce     = 0.0; // (0.0~1.0) restitution parameter
contact[i].surface.bounce_vel = 0.0; // minimum incoming velocity for bounce
dJointID c = dJointCreateContact(worldId,jointGroupID,&contact[i]);
dJointAttach (c,dGeomGetBody(contact[i].geom.g1),dGeomGetBody(contact[i].geom.g2));
}
}
}*/


static dWorldID worldId;
static dSpaceID spaceId;
static dGeomID  groundId;

static int flag = 0;
dsFunctions fn;

static  std::list<OdObject *> objects;

static void simulationMainLoop (int pause)
{
    std::list<OdObject *>::const_iterator it;

    for(it = objects.begin(); it != objects.end(); ++it )
    {
        //dSpaceCollide(spaceId,0,&nearCallback); // Collision detection

        //OdSimulationExecutableObject * execObj = static_cast<SimulationExecutableObject *>(*it);
        //if (execObj)
        //{
        //    execObj->execute();
        //}
    }

    dWorldStep(worldId,0.0001); // Step a world for 0.01 [sec]

    /*for(it = objects.begin(); it != objects.end(); ++it )
    {
        DrawableObject * drObj = static_cast<DrawableObject *>(*it);
        if (drObj)
        {
            drObj->draw();
        }
    }*/
}


void startFunction()
{
    // set camera
    static float xyz[3] = {0.0,-3.0,1.0};
    static float hpr[3] = {90.0,0.0,0.0};
    dsSetViewpoint (xyz,hpr);
}

void  prepareWorldHandles() {
    fn.version = DS_VERSION;
    fn.start   = &startFunction;
    fn.step    = &simulationMainLoop;
    fn.command = NULL;
    fn.stop    = NULL;
    fn.path_to_textures = "drawstuff_texture";
}


int main (int argc, char **argv)
{
    
    dInitODE();       // Initialize ODE
    worldId = dWorldCreate(); // Create a world
    spaceId = dHashSpaceCreate(0); // Create a collision space
    prepareWorldHandles();   // Set a draw function ;
    dWorldSetGravity(worldId,0,0,-9.8);  // Set gravity
    groundId = dCreatePlane(spaceId,0,0,1,0); // Create a ground

    //createLeg(); // Create ball and capsule

    dsSimulationLoop (argc,argv,800,600,&fn);  // Simulation loop

    dWorldDestroy (worldId); // Destroy the world 　
    dCloseODE();  // Close ODE
    return 0;
}
