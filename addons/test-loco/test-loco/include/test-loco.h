//------------------------------------------------------------------------------
//
//
//
//
//
//------------------------------------------------------------------------------
#ifndef     TEST_LOCO_H
#define     TEST_LOCO_H

#include    "vehicle.h"
#include    "brake-crane.h"
#include    "brake-mech.h"
#include    "physics.h"
#include    "controls.h"
#include    "reservoir.h"
#include    "airdistributor.h"
#include    "pneumo-relay.h"


class TestLoco : public Vehicle
{
public:

    TestLoco();
    ~TestLoco();

private:

    double traction_level;

    bool inc_loc;
    bool dec_loc;

    bool inc_crane_loc;
    bool dec_crane_loc;

    int crane_pos;
    double crane_motion;
    double crane_duration;

    double pz;
    bool inc_brake;
    bool dec_brake;

    QString brake_crane_module;
    QString brake_crane_config;

    BrakeCrane  *brake_crane;
    BrakeMech   *brake_mech;

    QString     brake_mech_module;
    QString     brake_mech_config;

    double charge_press;

    IncTractionTrigger incTracTrig;
    DecTractionTrigger decTracTrig;

    IncBrakeCrane incBrakeCrane;
    DecBrakeCrane decBrakeCrane;

    IncChargePress incChargePress;
    DecChargePress decChargePress;

    AirDistributor *airdist;

    QString     airdist_module;
    QString     airdist_config;

    Reservoir   *supply_reservoir;

    PneumoReley *repiter;

    void step(double t, double dt);

    double traction_char(double V);

    void initialization();

    void loadConfig(QString cfg_path);

    void keyProcess();    

    void stepper(int &value, int duration, double dt);
};




#endif // TESTLOCO_H
