#include "../ODE_DOUBLE.h"
#include "../COMMANDS/COMMANDS.h"

class LEG
{
public:
    LEG();
    virtual ~LEG();

    virtual void setUp() = 0;

    virtual void addForce() = 0;

    virtual void execute() = 0;
};