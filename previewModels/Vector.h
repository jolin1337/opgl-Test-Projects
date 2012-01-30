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
};
