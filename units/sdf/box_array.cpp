#include <taichi/levelset/sdf.h>
#include <taichi/common/loader.h>

TC_NAMESPACE_BEGIN

using namespace sdf;

real tet(Vector3 z) {
    z *= 0.7f;
    real dist, d;
    const real s = 2.0f;
    real factor = 1.0f;
    const Vector3 offset(1, 1, 1);
    for (int i = 0; i < 5; i++) {
        factor *= (1.0f / s);
        if (z.x + z.y < 0) {
            z = Vector3(-z.y, -z.x, z.z);
        }
        if (z.x + z.z < 0) {
            z = Vector3(-z.z, z.y, -z.x);
        }
        if (z.y + z.z < 0) {
            z = Vector3(z.x, -z.z, -z.y);
        }
        z = lerp(s, offset, z);
    }
    return length(z) * factor - 0.03f;
}

real content(Vector3 p) {
    p -= Vector3(0, 0.5, 0);
    p.x = cmod(p.x, 0.2);
    p.z = cmod(p.z, 0.2);
    return length(p) - 0.1;
}

real boundary(const Vector3 &p) {
    return combine_smooth(p.y + 2.5, p.z + 7, 0.5f);
}

real box_array(Vector3 p) {
    p.x = cmod(p.x, 1.0f, -2, 2);
    p.y = cmod(p.y, 1.0f, -2, 1);
    p.z = cmod(p.z, 1.0f, -2, 5);
    real a = combine_smooth(box(p, Vector3(0.5f, 0.02f, 0.2f)), cylinder(p, 0.05f, 0.5f), 0.1f);
    real b = combine(
            sphere(p - Vector3(0.25, 0, 0), 0.15f),
            sphere(p + Vector3(0.25, 0, 0), 0.15f)
            );
    return cut(a, b);
}

class BoxArraySDF final : public SDF {
public:
    real eval(const Vector3 &p) const {
        return std::min(box_array(p), boundary(p));
    }
};

TC_IMPLEMENTATION_LOADER(SDF, BoxArraySDF, "box_array_sdf");

TC_NAMESPACE_END

extern "C" void on_load() {}

extern "C" void on_unload() {}

