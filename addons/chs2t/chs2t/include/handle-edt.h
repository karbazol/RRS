//------------------------------------------------------------------------------
//
//      Рукоятка задатчика тормозного усилия для реостатного тормоза
//      электровозов серии ЧС ("карандаш")
//
//------------------------------------------------------------------------------

#ifndef     HANDLE_EDT_H
#define     HANDLE_EDT_H

#include    "device.h"

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
class HandleEDT : public Device
{
public:

    HandleEDT(QObject *parent = Q_NULLPTR);

    ~HandleEDT();

    void setPipiLinePressure(double press) { pFL = press; }

    void setBrefPressure(double press) { p_bref = press; }

    void setBrakeKey(int key) { brakeKey = key; }

    void setReleaseKey(int key) { releaseKey = key; }

    float getHandlePos() const { return static_cast<float>(pos); }

    double getQ_bref() const { return Q_bref; }

private:

    int brakeKey;

    int releaseKey;

    int pos;

    double Q_bref;

    double p_bref;

    double pFL;

    enum
    {
        POS_RELEASE = -1,
        POS_HOLD = 0,
        POS_BRAKE = 1
    };

    enum
    {
        NUM_COEFFS = 3
    };

    std::array<double, NUM_COEFFS> K;

    void preStep(state_vector_t &Y, double t);

    void ode_system(const state_vector_t &Y, state_vector_t &dYdt, double t);

    void load_config(CfgReader &cfg);

    void stepKeysControl(double t, double dt);
};

#endif // HANDLEEDT_H