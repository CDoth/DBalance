#include "DBalance.h"

DBalance::DBalance() {
    clear();
}
DBalance::~DBalance() {
    free_mem(__data);
}
void DBalance::setGyrosen(float s) {
    if(s > 0.0f) {
        GYROSENS = s;
    }
}
void DBalance::setAccelsen(float s) {
    if(s > 0.0f) {
        ACCELSENS = s;
    }
}
void DBalance::setAlpha(float a) { ALPHA = a; }
int16_t *DBalance::data() { return __data; }
Filter16MultiCascade &DBalance::filter() { return f; }

angle DBalance::aAngle() const { return accelAngle; }
angle DBalance::gAngle() const { return gyroAngle; }
angle DBalance::tAngle() const { return totalAngle;}

angle DBalance::gSpeed() const
{
    return gyroSpeed;
}
float DBalance::xAcceleration() const { return __data[3];}
float DBalance::yAcceleration() const { return __data[4];}
float DBalance::zAcceleration() const { return __data[5];}
float DBalance::xRotateSpeed() const { return __data[0]; }
float DBalance::yRotateSpeed() const { return __data[1]; }
float DBalance::zRotateSpeed() const { return __data[2]; }

void DBalance::clear() {
    __data = get_zmem<int16_t>(6);
    f.createCascades(6);
    GYROSENS = 1.0f;
    ACCELSENS = 1.0f;
    ALPHA = 0.995f;
    deltaT = 0.0f;

    zero_mem(&accelAngle, 1);
    zero_mem(&gyroAngle, 1);
    zero_mem(&totalAngle, 1);
}


