#ifndef DBALANCE_H
#define DBALANCE_H
#include "Filters.h"

#include "DProfiler.h"
#include <math.h>
#include <daran.h>

#define ACC_M 57.2958
struct angle {
    float x;
    float y;
    float z;
};
class DBalance {
public:
    DBalance();
    ~DBalance();
    void start() {
        gettimeofday(&t2, nullptr);
    }
    inline void proc() {
        gettimeofday(&t1, nullptr);


        f.filter(__data);



        float GX = float(__data[0]) / GYROSENS;
        float GY = float(__data[1]) / GYROSENS;
        float GZ = float(__data[2]) / GYROSENS;

        float AX = float(__data[3]) / ACCELSENS;
        float AY = float(__data[4]) / ACCELSENS;
        float AZ = float(__data[5]) / ACCELSENS;


        gyroSpeed.x = GX;
        gyroSpeed.y = GY;
        gyroSpeed.z = GZ;



        deltaT = PROFILER::fsec_dif(&t2, &t1);
        t2 = t1;


//        gyroBufferX.push_back(GX);
//        gyroBufferY.push_back(GY);
//        gyroBufferZ.push_back(GZ);

//        if(deltaT) {
//            float __t = deltaT / gyroBufferX.size();
//            gyroAngle.x = 0;
//            gyroAngle.y = 0;
//            gyroAngle.z = 0;
//            FOR_VALUE(gyroBufferX.size(), i) {
//                gyroBufferX[i] *= __t;
//                gyroBufferY[i] *= __t;
//                gyroBufferZ[i] *= __t;
//            }

//            FOR_VALUE(gyroBufferX.size(), i) {

//                gyroAngle.x += gyroBufferX[i] * __t;
//                gyroAngle.y += gyroBufferX[i] + gyroBufferZ[i];
//                gyroAngle.z += gyroBufferY[i] + gyroBufferX[i];
//            }
//            gyroBufferX.drop();
//            gyroBufferY.drop();
//            gyroBufferZ.drop();
//        }




        accelAngle.x = ACC_M * atan( AX * q_rsqrt(AY * AY + AZ * AZ));
        accelAngle.y = ACC_M * atan( AY * q_rsqrt(AX * AX + AZ * AZ));
        accelAngle.z = ACC_M * atan( AZ * q_rsqrt(AX * AX + AY * AY));

        totalAngle.x = ( (1.0f - ALPHA) * (totalAngle.x + 0 )) + (ALPHA) * (accelAngle.x);
        totalAngle.y = ( (1.0f - ALPHA) * (totalAngle.y + 0 )) + (ALPHA) * (accelAngle.y);
        totalAngle.z = ( (1.0f - ALPHA) * (totalAngle.z + 0 )) + (ALPHA) * (accelAngle.z);
    }

    void setGyrosen(float s);
    void setAccelsen(float s);
    void setAlpha(float a);
    int16_t* data();
    Filter16MultiCascade& filter();

    angle aAngle() const;
    angle gAngle() const;
    angle tAngle() const;
    angle gSpeed() const;

    float xAcceleration() const;
    float yAcceleration() const;
    float zAcceleration() const;

    float xRotateSpeed() const;
    float yRotateSpeed() const;
    float zRotateSpeed() const;

private:
    void clear();
private:
    int16_t *__data;
private:
    struct timeval t1, t2;
    Filter16MultiCascade f;
    float GYROSENS;
    float ACCELSENS;
    float ALPHA;
    float deltaT;

    DArray<float> gyroBufferX;
    DArray<float> gyroBufferY;
    DArray<float> gyroBufferZ;

    angle gyroSpeed;
    angle accelAngle;
    angle gyroAngle;
    angle totalAngle;
};


#endif // DBALANCE_H
