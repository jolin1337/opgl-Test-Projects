#ifndef _GL_DATA_OBJECT_H_
#define _GL_DATA_OBJECT_H_

#include <GL/glew.h>
#include <GL/glut.h>

#include <fstream>
#include <vector>
#include "./utils/Vector3.h"
#include "./utils/Matrix4.h"

#include "config.h"


struct ShaderInfo {
	std::string name;
	GLint id;
	bool isDefined(){
		return name != "";
	}
	operator GLint(){
		return id;
	}
};
struct Uniform : public ShaderInfo {
	Uniform(){}
	Uniform(std::string name){this->name = name;}
	int setUp(GLuint prog){
		if(name == "")return 0;
		id = glGetUniformLocation(prog, name.c_str());
		if (id == -1) {
			fprintf(stderr, "Could not bind uniform %s\n", name.c_str());
			return 0;
		}
		printf("Loaded uniform %s, with id: %i\n", name.c_str(), id);	
		return 1;
	}

};
struct Attribute : public ShaderInfo {
	Attribute(){}
	Attribute(std::string name){this->name = name;}
	int setUp(GLuint prog){
		if(name == "")return 0;
		id = glGetAttribLocation(prog, name.c_str());
		if (id== -1) {
			fprintf(stderr, "Could not bind attribute %s\n", name.c_str());
			return 0;
		}
		printf("Loaded attribute %s, with id: %i\n", name.c_str(), id);
		return 1;
	}
};


struct Color {
	GLfloat color[4];
	std::string name;
};


struct Vertex {
	Vector3 position, localPosition, worldPosition,
			normal;
	GLfloat color[4];
};
struct Face {
	GLushort indicies[3];
	Vertex *vertices[3], normal;
};

Vector3 calcNormal(Vector3 f1, Vector3 f2, Vector3 f3){
	Vector3 v1 = f1 - f2,
			v2 = f1 - f3;
	return v1.cross(v2);
}

typedef std::vector<Vertex> Vertices;
typedef std::vector<GLushort> Faces;

typedef std::vector<Uniform> Uniforms;
typedef std::vector<Attribute> Attributes;

typedef GLuint BufferObject;

class GLobject {
public:
	friend void setupBuffers(GLobject &obj);
	friend void idle();
	friend int init_resources();
	GLobject():_allokatedMem(false), linkedShaders(false), prog(0), scale(1.0f,1.0f,1.0f){}
	~GLobject(){
		glDeleteProgram(prog);
		glDeleteBuffers(1, &vbo_vertices);
		glDeleteBuffers(1, &ibo_indicies);

		glDeleteBuffers(1, &vbo_textureCoordinates);
	}
	GLobject & setPosition(Vector3 v){
		position = v;
		return *this;
	}
	Vector3 getPosition() const{
		return position;
	}
	GLobject & setRotation(Vector3 r){
		rotation = r;
		return *this;
	}
	Vector3 getRotation() const{
		return rotation;
	}
	GLobject & setScale(Vector3 s){
		scale = s;
		return *this;
	}
	Vector3 getScale() const{
		return scale;
	}
	Matrix4 getTransformMatrix(){
		return Matrix4().Translate(getPosition()).Rotate(1.0f, getRotation()).Scale(getScale());
	}
	std::vector<Color > loadMaterials(std::string fileName){
		std::ifstream ifs(fileName.c_str());

		std::vector<Color> res;
		if(!ifs.is_open())
			return res;

		Color newColor;
		int material = 0;

		while(!ifs.eof()){
			std::string input;
			ifs >> input;
			if(input == "newmtl"){
				if(newColor.name != ""){
					res.push_back(newColor);
				}
				ifs.get();
				ifs >> newColor.name;
			}
			else if(input == "Kd"){
				ifs >> newColor.color[0];
				ifs >> newColor.color[1];
				ifs >> newColor.color[2];
				newColor.color[3] = 1.0;
			}
			getline(ifs, input, '\n');
		}
		if(newColor.name != "")
			res.push_back(newColor);
		return res;
	}
	void loadObj(std::string fileName){
		if(_allokatedMem){
			_allokatedMem = false;
		}
		std::ifstream ifs(fileName.c_str());
		if(!ifs.is_open())
			throw 401;
		std::string line;
		std::vector<Vector3> normals;
		std::vector<Color> colors;
		Color defaultColor;
		defaultColor.color[0] = 1.0f;
		defaultColor.color[3] = 1.0f;
		int mtl = -1;
		max = min = Vector3(0,0,0);

		while(!ifs.eof()){
			char c = ifs.get();
			Vertex v;
			GLfloat v2;
			Face f;
			GLushort f1,f2,f3;
			Vector3 normal(0,0,0);
			switch(c){
				case 'v':
				case 'V':
					c = ifs.get();
					if(c == ' '){// vertex
						ifs >> v.position.x >> v.position.y >> v.position.z;
						if(vertices.size() == 0){
							max = v.position;
							min = v.position;
						}
						if(v.position.x > max.x)
							max.x = v.position.x;
						if(v.position.y > max.y)
							max.y = v.position.y;
						if(v.position.z > max.z)
							max.z = v.position.z;

						if(v.position.x < min.x)
							min.x = v.position.x;
						if(v.position.y < min.y)
							min.y = v.position.y;
						if(v.position.z < min.z)
							min.z = v.position.z;
						vertices.push_back(v);
					}
					else if(c== 'n'){//vertex-normal
						ifs >> v.normal.x >> v.normal.y >> v.normal.z;
						normals.push_back(v.normal);
					}
					break;
				case 'f':
				case 'F':
					ifs >> f1;
					f1--;
					if(mtl > -1){
						vertices[f1].color[0] = colors[mtl].color[0];
						vertices[f1].color[1] = colors[mtl].color[1];
						vertices[f1].color[2] = colors[mtl].color[2];
						vertices[f1].color[3] = colors[mtl].color[3];
					}
					else{
						vertices[f1].color[0] = defaultColor.color[0];
						vertices[f1].color[1] = defaultColor.color[1];
						vertices[f1].color[2] = defaultColor.color[2];
						vertices[f1].color[3] = defaultColor.color[3];
					}
					faces.push_back(f1);
					c = ifs.get();
					if( c == '/'){
						// uvs
						ifs.get();
						// vertex-normal linking:
						ifs >> normal.x;
						vertices[faces[faces.size()-1]].normal = normals[normal.x-1];
					}

					ifs >> f2;
					f2--;
					if(mtl > -1){
						vertices[f2].color[0] = colors[mtl].color[0];
						vertices[f2].color[1] = colors[mtl].color[1];
						vertices[f2].color[2] = colors[mtl].color[2];
						vertices[f2].color[3] = colors[mtl].color[3];
					}
					else{
						vertices[f2].color[0] = defaultColor.color[0];
						vertices[f2].color[1] = defaultColor.color[1];
						vertices[f2].color[2] = defaultColor.color[2];
						vertices[f2].color[3] = defaultColor.color[3];
					}
					faces.push_back(f2);
					c = ifs.get();
					if( c == '/'){
						// uvs
						ifs.get();
						// vertex-normal linking:
						ifs >> normal.y;
						vertices[faces[faces.size()-1]].normal = normals[normal.y-1];
					}

					ifs >> f3;
					f3--;
					if(mtl > -1){
						vertices[f3].color[0] = colors[mtl].color[0];
						vertices[f3].color[1] = colors[mtl].color[1];
						vertices[f3].color[2] = colors[mtl].color[2];
						vertices[f3].color[3] = colors[mtl].color[3];
					}
					else{
						vertices[f3].color[0] = defaultColor.color[0];
						vertices[f3].color[1] = defaultColor.color[1];
						vertices[f3].color[2] = defaultColor.color[2];
						vertices[f3].color[3] = defaultColor.color[3];
					}
					faces.push_back(f3);
					c = ifs.get();
					if( c == '/'){
						// uvs
						ifs.get();
						// vertex-normal linking:
						ifs >> normal.z;
						vertices[faces[faces.size()-1]].normal = normals[normal.z-1];
					}
					else if(normal.norm() == 0){
						normal = calcNormal(vertices[f1].position, vertices[f2].position, vertices[f3].position).normalize();
						if(vertices[f1].normal.norm() != 0)
							vertices[f1].normal = vertices[f1].normal.add(&normal).normalize();
						else
							vertices[f1].normal = normal;

						if(vertices[f2].normal.norm() != 0)
							vertices[f2].normal = vertices[f2].normal.add(&normal).normalize();
						else
							vertices[f2].normal = normal;

						if(vertices[f3].normal.norm() != 0)
							vertices[f3].normal = vertices[f3].normal.add(&normal).normalize();
						else
							vertices[f3].normal = normal;
					}
					break;
				case 'u':
					ifs >> line;
					if( line == "semtl"){
						ifs.get();
						ifs >> line;
						mtl = -1;
						for (int i=colors.size(); i>0; i--) {
							if(colors[i-1].name == line){
								mtl = i-1;
								break;
							}
						}
					}
				case 'm':
					ifs >> line;
					if( line == "tllib"){
						ifs.get();
						getline(ifs, line, '\n');
						colors = loadMaterials(line);
					}
					break;
			}
			if(c != '\n')
				getline(ifs, line, '\n');
		}
		ifs.close();
		unsigned int vsize = vertices.size(), fsize = faces.size();
		if(vsize == 0 || fsize == 0)
			throw 501;
		_allokatedMem = true;
	}
	int setupUniforms(){
		if(!linkedShaders)return 0;
		int res = 1;
		for(Uniforms::iterator it = uniforms.begin(); it != uniforms.end(); it++)
			if((*it).setUp(prog) == 0)
				res = 0;
		return res;
	}
	int setupAttributes(){
		if(!linkedShaders)return 0;
		int res = 1;
		for(Attributes::iterator it = attributes.begin(); it != attributes.end(); it++)
			if((*it).setUp(prog) == 0)
				res = 0;
		return res;
	}
	int loadShaders(std::string fileName_v, std::string fileName_f){
		GLint link_ok = GL_FALSE;
		GLuint vs, fs;
		if ((vs = create_shader(fileName_v.c_str(), GL_VERTEX_SHADER))   == 0) return 0;
		if ((fs = create_shader(fileName_f.c_str(), GL_FRAGMENT_SHADER)) == 0) return 0;

		prog = glCreateProgram();

		glAttachShader(prog, vs);
		glAttachShader(prog, fs);

		glLinkProgram(prog);
		glGetProgramiv(prog, GL_LINK_STATUS, &link_ok);
		if (!link_ok) {
			linkedShaders = false;
			fprintf(stderr, "glLinkProgram:\n");
			return 0;
		}
		linkedShaders = true;

		if(this->setupUniforms() + this->setupAttributes() == 0)
			return 0;

		return 1;
	}
	void render(){
		Attribute coord = getAttribute("position"),
				  color = getAttribute("color"),
				  normal = getAttribute("normal");
		bool useAttr = coord.isDefined(), 
			useColor = color.isDefined() && color.name != coord.name,
			useNorm = normal.isDefined() && normal.name != color.name && normal.name != coord.name;

		if(useAttr)
			glEnableVertexAttribArray(coord);
		if(useColor)
			glEnableVertexAttribArray(color);
		if(useNorm)
			glEnableVertexAttribArray(normal);
		
		glBindBuffer(GL_ARRAY_BUFFER, getVboVerticesArray());
			int size;
			glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			if(useAttr)
				glVertexAttribPointer(
					coord, 
					3, 
					GL_FLOAT, 
					GL_FALSE, 
					sizeof(Vertex),
					(void*)(offsetof(Vertex,position))
				);
			if(useColor)
				glVertexAttribPointer(
					color, 
					4, 
					GL_FLOAT, 
					GL_FALSE, 
					sizeof(Vertex),
					(void*)(offsetof(Vertex,color))
				);
			if(useNorm)
				glVertexAttribPointer(
					normal, 
					3, 
					GL_FLOAT, 
					GL_FALSE, 
					sizeof(Vertex),
					(void*)(offsetof(Vertex,normal))
				);

		glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)(offsetof(Vertex,normal)));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getIboIndiciesArray());
			glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			//glDrawElementsInstanced(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0, 3);
			glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	
		if(useAttr)
			glDisableVertexAttribArray(coord);
		if(useColor)
			glDisableVertexAttribArray(color);
		if(useNorm)
			glDisableVertexAttribArray(normal);

	}
	void updateElement(){
		updateElement(0, size());
	}
	void updateElement(GLuint from, GLuint size){
		if(size+from > vertices.size()) 
			return;

		glBindBuffer(GL_ARRAY_BUFFER, getVboVerticesArray());
		glBufferSubData(GL_ARRAY_BUFFER, from, size*sizeof(Vertex), getVertices());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void updateElement(GLuint element){
		updateElement(element, 1);
	}
	GLuint getVboVerticesArray() const {
		return vbo_vertices;
	}
	GLuint getVboTextureArray() const {
		return vbo_vertices;
	}
	GLuint getIboIndiciesArray() const {
		return ibo_indicies;
	}
	bool isModel(){
		return _allokatedMem;
	}
	void addAttribute(Attribute a){
		attributes.push_back(a);
	}
	Attribute getAttribute(GLuint a) const{
		if(a < attributes.size())
			return attributes[a];
		return Attribute();
	}
	Attribute getAttribute(std::string name) const{
		for (int i = attributes.size(); i > 0; i--) {
			if(attributes[i-1].name == name)
				return attributes[i-1];
		}
		return Attribute();
	}
	Uniform getUniform(GLuint u) const{
		if(u < uniforms.size())
			return uniforms[u];
		return Uniform();
	}
	Uniform getUniform(std::string name) const{
		for (int i = uniforms.size(); i > 0; i--) {
			if(uniforms[i-1].name == name)
				return uniforms[i-1];
		}
		return Uniform();
	}
	void addUniform(Uniform u){
		uniforms.push_back(u);
	}
	GLuint getProgram() const{
		return prog;
	}

	GLfloat dimension(){
		return max.x - min.x;
	}
	Vertex *getVertices(){
		return &vertices[0];
	}
	GLuint size(){
		return vertices.size();
	}
	GLushort *getFaces(){
		return &faces[0];
	}
	GLuint f_size(){
		return faces.size();
	}

	Vector3 max, min;
protected:
	Vector3 position, rotation, scale;
	Vertices vertices;
	Faces faces;
	BufferObject vbo_vertices, ibo_indicies, vbo_textureCoordinates;
	
	Uniforms uniforms;
	Attributes attributes;
	GLuint prog;

	bool _allokatedMem,
		 linkedShaders;
};

class Rigdbody {
public:
	Vector3 force, velocity;
	void uppdateVelocity(){
		velocity.add(&force);
	}
	Rigdbody(){}
	~Rigdbody(){}
};
class WorldObject : public GLobject {
public:
	Rigdbody dynamics;
	WorldObject(){}
	~WorldObject(){}

	void clearForce(){
		dynamics.force.x = dynamics.force.y = dynamics.force.z = 0.0f;
	}
	void clearVelocity(){
		dynamics.velocity.x = dynamics.velocity.y = dynamics.velocity.z = 0.0f;
	}
	void addForce(Vector3 v){
		dynamics.force.add(&v);
	}
	void updateForce(){
		dynamics.uppdateVelocity();
		position.add(dynamics.velocity * 0.001);
	}
	void addVelocity(Vector3 v){
		dynamics.velocity.add(&v);
	}
};
class InstanceObject {
	GLenum mode, 
		   type;

	GLsizei count,
			instancecount;
	void *indicies;
	GLuint id;
public:
	Vector3 position, rotation, scale;
	WorldObject *object;
	InstanceObject():object(0){}
	//void instance(Wo...){}

	void update(){

	}
	void render(){
		if(object != 0){
			glBindVertexArray(id);
			glDrawElementsInstanced(mode, count, type, indicies, instancecount);
			glBindVertexArray(0);
		}
	}
};

struct VertexGroup {
	Vertex *vertex;
	GLfloat intencity;
	VertexGroup():intencity(1.0f), vertex(0){}
	bool isActive(){return vertex != 0;}
};
class Bone{
public:
	std::vector<VertexGroup> vertices;
	Matrix4 rotation, translation, scale;
	Bone() {}

	void emptyPosition(){
		for (std::vector<VertexGroup>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
			it->vertex->position = Vector3();
		}
	}
	void apply(){
		for (std::vector<VertexGroup>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
			it->vertex->localPosition = it->vertex->position;
		}
	}
	void refresh(bool reset = true){
		for (std::vector<VertexGroup>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
			Matrix4 boneMatrix = rotation * translation * scale;
			if(reset)
				it->vertex->position = Matrix4::mult(boneMatrix, it->vertex->localPosition);
			else
				it->vertex->position = boneMatrix * (it->vertex->position);

		}
	}
};


#endif
