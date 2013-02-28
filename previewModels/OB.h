#ifndef _OB_
#define _OB_
#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Vector.h"
using namespace std;
struct Color{
	float r,g,b;
};
class OB {
    public:
	vector<Vector > vertices;
	vector<Vector > Faces;
	Vector position;
	Color material;
	OB(){
		material.r=0.0f;
		material.g=0.0f;
		material.b=0.0f;
		
	}
	~OB(){
	}

	static OB loadFromFile(string file){
		ifstream o;
		o.open( file.c_str(), ifstream::in );
		OB ans;
		vector<Vector > V;
		while (o.good()){
			string s="";
			o>>s;
			if(s=="v"){
				float x,y,z;
				o>>x;
				o>>y;
				o>>z;
				V.push_back(Vector(x,y,z));
			}
			if(s=="f"){
				int i;
				o>>i;
				ans.Faces.push_back(V[i-1]);
				o>>i;
				ans.Faces.push_back(V[i-1]);
				o>>i;
				ans.Faces.push_back(V[i-1]);
				//o>>i;
				//cout <<endl<<i<<endl;
				//ans.Faces.push_back(V[i-1]);
			}
		}
		o.close();
		return ans;
	}
};
#endif
