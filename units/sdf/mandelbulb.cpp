#include <taichi/math/sdf.h>
#include <taichi/common/loader.h>

TC_NAMESPACE_BEGIN

using namespace sdf;
using namespace std;

real mandelbulb(Vector3 p) {
    Vector3 z = p;
    float dr = 1.0;
    float r = 0.0;
    const real power = 8.0f;
    for (int i = 0; i < 10; i++) {
        r = length(z);
        if (r > 10.0f) break;

        float theta = acos(z.z/r);
        float phi = atan2(z.y,z.x);
        dr =  pow( r, power-1.0f)*power*dr + 1.0;

        float zr = pow( r,power);
        theta = theta*power;
        phi = phi*power;

        z = zr*Vector3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
        z+=p;
    }
    real de =  0.5f*log(r)*r/dr;
    return de;
}

real content(Vector3 p) {
    return mandelbulb(p);
}

real boundary(const Vector3 &p) {
    return combine_smooth(p.y + 2.5, p.z + 7, 0.5f);
}

class MandelbulbSDF final : public SDF {
    public:
        real eval(const Vector3 &p) const {
            //return std::min(content(p), boundary(p));
            return content(p);
        }
};

TC_IMPLEMENTATION_LOADER(SDF, MandelbulbSDF, "mandelbulb_sdf");

TC_NAMESPACE_END

extern "C" void on_load() {}

extern "C" void on_unload() {}

