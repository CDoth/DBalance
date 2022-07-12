#ifndef FILTERS_H
#define FILTERS_H
#include <DArray.h>

class Filter16 {
public:
    explicit Filter16(int s = 0) {
        __buffer = nullptr;
        __b = nullptr;
        __e = nullptr;
        v = 0;
        pos = 0;
        size = 0;
        if(s > 0) setPower(s);
    }
    ~Filter16() {
        free_mem(__buffer);
    }
    void setPower(int power) {
        __buffer = reget_zmem(__buffer, power);
        size = power;
        __e = __buffer + size;
        pos = 0;
    }
    inline void put(int16_t v) {
        __buffer[pos] = v;
        if(++pos == size) pos = 0;
    }
    inline int16_t pull() {
        v = 0;
        __b = __buffer;
        while(__b != __e) {
            v += *__b++;
        }
        v /= size;
        return v;
    }
    inline int16_t value() const {return v;}
private:
    int16_t *__buffer;
    int16_t *__b;
    int16_t *__e;

    int v;
    int pos;
    int size;
};
class Filter16Cascade {
public:
    Filter16Cascade() {}
    void addFilter(int filterPower) {
        cascade.push_back(Filter16());
        cascade.back().setPower(filterPower);
        e = cascade.end();
    }
    inline int16_t filter(int16_t v) {
        b = cascade.begin();
        int16_t r = v;
        while(b!=e) {
            b->put(r);
            r = b->pull();
            ++b;
        }
        return r;
    }
    inline int16_t value() const {return cascade.constBack().value();}

private:

    DArray<Filter16, Direct>::iterator b;
    DArray<Filter16, Direct>::iterator e;
    DArray<Filter16, Direct> cascade;
};
class Filter16MultiCascade {
public:
    Filter16MultiCascade() {

    }
    explicit Filter16MultiCascade(int n) {
        mc.appendNs(n);
        e = mc.end();
    }
    void createCascades(int n) {
        mc.reform(n);
        e = mc.end();
    }
    void addFilterToEachCascade(int filterPower) {
        FOR_VALUE(mc.size(), i) {
            mc[i].addFilter(filterPower);
        }
    }
    inline void filter(int16_t *v) {
        b = mc.begin();
        while(b!=e) {
            *v = b->filter(*v);
            ++v;
            ++b;
        }
    }

private:
    DArray<Filter16Cascade, Direct>::iterator b;
    DArray<Filter16Cascade, Direct>::iterator e;
    DArray<Filter16Cascade, Direct> mc;
};


#endif // FILTERS_H
