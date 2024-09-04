#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader {

public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath); // constructor
	void use(); // activates shader program

	// set a uniform location
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, float x, float y) const; // opengl vec2
	void setVec2(const std::string &name, const glm::vec2 &value) const; // glm vec2
	void setVec3(const std::string &name, float x, float y, float z) const; // opengl vec3
	void setVec3(const std::string &name, const glm::vec3& value) const; // glm vec3
	void setVec4(const std::string &name, float x, float y, float z, float a) const; // opengl vec4 
	void setVec4(const std::string &name, const glm::vec4& value) const; // glm vec4
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3& mat) const;
	void setMat4(const std::string &name, const glm::mat4& mat) const;
};

#endif
