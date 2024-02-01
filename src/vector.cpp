#include "vector.h"
#include <assert.h>
#include <math.h>

#define EPSILON 1e-6
//using namespace Vector;

Vector::Vector(float x, float y, float z) {
    X = x;
    Y = y;
    Z = z;
}

Vector::Vector() {
    // nichts?
}

//               const Vector& = cannot modify Vector v
//                                 const = cant change class member variables || unless variable marked mutable
float Vector::dot(const Vector& v) const //pass by refrence?
{
    return v.X * X + v.Y * Y + v.Z * Z;
}

Vector Vector::cross(const Vector& v) const {
    Vector result = Vector((Y * v.Z) - (Z * v.Y),
        (Z * v.X) - (X * v.Z),
        (X * v.Y) - (Y * v.X));
    return result;
}


Vector Vector::operator+(const Vector& v) const {

    return Vector(X + v.X,
        Y + v.Y,
        Z + v.Z);
}

Vector Vector::operator-(const Vector& v) const {

    return Vector(X - v.X,
        Y - v.Y,
        Z - v.Z);
}

Vector Vector::operator*(float c) const {
    return Vector(X * c,
        Y * c,
        Z * c);
}

Vector Vector::operator-() const {

    return Vector(-X,
        -Y,
        -Z);
}


Vector& Vector::operator+=(const Vector& v) {
    // TODO: Read up on this crap, the refrencing that is
    this->X += v.X;
    this->Y += v.Y;
    this->Z += v.Z;
    return *this;
}


Vector& Vector::normalize() {
    float length = this->length();
    this->X /= length;
    this->Y /= length;
    this->Z /= length;
    return *this;
}

float Vector::length() const {

    return sqrt(lengthSquared());
}

float Vector::lengthSquared() const {

    return X * X + Y * Y + Z * Z;
}

Vector Vector::reflection(const Vector& normal) const {
    //TODO read up on this mess
    float dotProduct = dot(normal);
    Vector v = Vector(2 * dotProduct * normal.X,
        2 * dotProduct * normal.Y,
        2 * dotProduct * normal.Z);
    return *this - v;
}

bool Vector::triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const {


    Vector ab = b - a;
    Vector ac = c - a;
    Vector normal = (ab.cross(ac)).normalize();

    //s = skalar von geraden um den schnittpunkt zu finden
    s = (a - *this).dot(normal) / d.dot(normal);
    if (s <= 0) {
        return false;
    }
    Vector intersectionPoint = *this + d * s;

    //Calculate Triangle Areas
    float area_abc = ab.cross(ac).length() / 2;
    float area_abp = ab.cross(intersectionPoint - a).length() / 2;
    float area_acp = ac.cross(intersectionPoint - a).length() / 2;
    float area_bcp = (b - c).cross(intersectionPoint - b).length() / 2;
    if (area_abc - (area_acp + area_abp + area_bcp) >= 0 - EPSILON) {
        return true;
    }
    return false;


}


