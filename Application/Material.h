#ifndef MATERIAL_H
#define MATERIAL_H
// GLM Headers
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

struct Material
{
	glm::vec3 kAmbient;
	glm::vec3 kDiffuse;
	glm::vec3 kSpecular;
	float kShininess;
	unsigned size;
	// Step 5 - Initialize size to 0 in the initializer list
	Material() : kAmbient(0.0f, 0.0f, 0.0f),
		kDiffuse(0.0f, 0.0f, 0.0f), kSpecular(0.0f, 0.0f,
			0.0f), kShininess(1.f), size(0)
	{
	}
	// Step 6 - Add the operator= function
	Material& operator=(const Material& rhs)
	{
		kAmbient = rhs.kAmbient;
		kDiffuse = rhs.kDiffuse;
		kSpecular = rhs.kSpecular;
		kShininess = rhs.kShininess;
		size = rhs.size;
		return *this;
	}
};

#endif

