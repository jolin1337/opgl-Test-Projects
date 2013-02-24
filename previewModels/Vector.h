#include <math.h>

class Vector{
    public:
	float x,y,z;
	Vector():x(0),y(0),z(0) {}
	Vector(float x1, float y1, float z1){
		x=x1;
		y=y1;
		z=z1;
	}
	Vector(Vector *v){
		x=v->x;
		y=v->y;
		z=v->z;
	}
	float norm(){
		return sqrt(x*x + y*y + z*z);
	}
	void sub(Vector &v){
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	Vector cross(Vector &v){
		return Vector(
			this->y*v.z - v.y*this->z,
			this->z*v.x - v.z*this->x,
			this->x*v.y - v.x*this->y);
	}
	void normalize(){
		float m = norm();
		if(m > 0){
			x /= m;
			y /= m;
			z /= m;
		}
	}
};
