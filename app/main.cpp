#include "ODE_DOUBLE.h"

// sample7.cpp  by Kosei Demura 2006-2009

static dWorldID world;
static dSpaceID space;
static dGeomID  ground;
static dJointID joint;
static dJointGroupID contactgroup;
static int flag = 0;
dsFunctions fn;
const dReal   radius = 0.2;
const dReal   mass   = 1.0;

typedef struct {
  dBodyID body;   // a body
  dGeomID geom;   //  a geometry
  dReal   radius, length,  mass; // radius[m], length[m], mass[kg]
} myLink;

myLink ball, pole;

void start()
{
  static float xyz[3] = {0.0,-3.0,1.0};
  static float hpr[3] = {90.0,0.0,0.0};
  dsSetViewpoint (xyz,hpr);
}

static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
  const int N = 10;
  dContact contact[N];

  int isGround = ((ground == o1) || (ground == o2));

  int n =  dCollide(o1,o2,N,&contact[0].geom,sizeof(dContact));

  if (isGround)  {
    if (n >= 1) flag = 1;
    else        flag = 0;
    for (int i = 0; i < n; i++) {
      contact[i].surface.mode = dContactBounce;
      contact[i].surface.mu   = dInfinity;
      contact[i].surface.bounce     = 0.0; // (0.0~1.0) restitution parameter
      contact[i].surface.bounce_vel = 0.0; // minimum incoming velocity for bounce
      dJointID c = dJointCreateContact(world,contactgroup,&contact[i]);
      dJointAttach (c,dGeomGetBody(contact[i].geom.g1),dGeomGetBody(contact[i].geom.g2));
    }
  }
}

static void simLoop (int pause)
{
  const dReal *pos1,*R1,*pos2,*R2; 
  dSpaceCollide(space,0,&nearCallback); // Collision detection
  dWorldStep(world,0.0001); // Step a world for 0.01 [sec]
  dJointGroupEmpty(contactgroup); // Empty the joint group

  // Draw a ball
  dsSetColor(1.0,0.0,0.0);   // Set red color
  pos1 = dBodyGetPosition(ball.body); //Get a position
  R1   = dBodyGetRotation(ball.body); //Get an orientation
  dsDrawSphere(pos1,R1,ball.radius);  //Draw a sphere

 // Draw a capsule
  pos2 = dBodyGetPosition(pole.body); //Get a position
  R2   = dBodyGetRotation(pole.body); //Get an orientation
  dsDrawCapsule(pos2,R2,pole.length,pole.radius); //Draw a capsule
}

// Create an object
void createBallPole()
{
  dMass m1, m2;
  dReal x0 = 0.0, y0 = 0.0, z0 = 2.5;    // a ball
  ball.radius = 0.2;
  ball.mass   = 1.0;
  ball.body = dBodyCreate(world);
  dMassSetZero(&m1);
  dMassSetSphereTotal(&m1,ball.mass,ball.radius);
  dBodySetMass(ball.body,&m1);
  dBodySetPosition(ball.body, x0, y0, z0);
  ball.geom = dCreateSphere(space,ball.radius);
  dGeomSetBody(ball.geom,ball.body);

  // a capsule
  pole.radius = 0.025;
  pole.length = 1.0;
  pole.mass   = 1.0;
  pole.body =  dBodyCreate(world);
  dMassSetZero(&m2);
  dMassSetCapsuleTotal(&m2,pole.mass,3,pole.radius,pole.length);
  dBodySetMass(pole.body,&m2);
  dBodySetPosition(pole.body, x0, y0+0.05, z0 - 0.5 * pole.length);
  pole.geom =  dCreateCapsule(space,pole.radius,pole.length);
  dGeomSetBody(pole.geom,pole.body);

  // hinge joint
  joint = dJointCreateHinge(world, 0); // Create a hinge joint
  dJointAttach(joint, ball.body,pole.body); // Attach joint to bodies
  dJointSetHingeAnchor(joint, x0, y0, z0 - ball.radius); // Set a joint anchor
  dJointSetHingeAxis(joint, 1, 0, 0); //Set a hinge axis(1,0,0)
}

void  prepDrawStuff() {
  fn.version = DS_VERSION;
  fn.start   = &start;
  fn.step    = &simLoop;
  fn.command = NULL;
  fn.stop    = NULL;
  fn.path_to_textures = "drawstuff_texture";
}

int main (int argc, char **argv)
{
  prepDrawStuff();   // Set a draw function ;
  dInitODE();       // Initialize ODE
  world = dWorldCreate(); // Create a world
  space = dHashSpaceCreate(0); // Create a collision space
  contactgroup = dJointGroupCreate(0); // Create a joint group
  dWorldSetGravity(world,0,0,-9.8);  // Set gravity
  ground = dCreatePlane(space,0,0,1,0); // Create a ground
  createBallPole(); // Create ball and capsule

  dsSimulationLoop (argc,argv,800,600,&fn);  // Simulation loop

  dWorldDestroy (world); // Destroy the world 　
  dCloseODE();  // Close ODE
  return 0;
}