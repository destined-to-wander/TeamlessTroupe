#include "FPCamera.h"
#include "KeyboardController.h"

//Include GLFW
#include <GLFW/glfw3.h>

FPCamera::FPCamera() : isDirty(false)
{
	this->position = glm::vec3(0, 0, 0);
	this->target = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 1, 0);
}

FPCamera::~FPCamera()
{
}

void FPCamera::Init(glm::vec3 position, glm::vec3 target, glm::vec3 up)
{
	this->position = position;
	this->target = target;
	this->up = up;
	this->speed = 10.f;
	this->isDirty = true;

	Refresh();
}

void FPCamera::Reset()
{
}

void FPCamera::Update(double dt)
{
	glm::vec3 worldUp(0.f, 1.f, 0.f);

	static const float ROTATE_SPEED = 60.0f;
	static const float ZOOM_SPEED = 10.0f;
	glm::vec3 view = glm::normalize(target - position); //calculate the new view vector

	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT)) {
		float angle = ROTATE_SPEED * static_cast<float>(dt);
		view = glm::rotate(glm::mat4(1.f), glm::radians(angle), worldUp)
			* glm::vec4(view, 0.f);
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_RIGHT)) {
		float angle = ROTATE_SPEED * static_cast<float>(dt);
		view = glm::rotate(glm::mat4(1.f), glm::radians(-angle), worldUp)
			* glm::vec4(view, 0.f);
	}

	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_UP)) {
		float angle = ROTATE_SPEED * static_cast<float>(dt);
		glm::vec3 right = glm::normalize(glm::cross(view, worldUp));
		view = glm::rotate(glm::mat4(1.f), glm::radians(angle), right)
			* glm::vec4(view, 0.f);
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_DOWN)) {
		float angle = ROTATE_SPEED * static_cast<float>(dt);
		glm::vec3 right = glm::normalize(glm::cross(view, worldUp));
		view = glm::rotate(glm::mat4(1.f), glm::radians(-angle), right)
			* glm::vec4(view, 0.f);
	}

	glm::vec3 right = glm::normalize(glm::cross(view, worldUp));
	up = glm::normalize(glm::cross(right, view));

	if (shakeDuration > 0.f)
	{
		shakeDuration -= static_cast<float>(dt);

		// Random offset in X, Y, Z
		float offsetX = ((rand() % 100) / 100.f - 0.5f) * 2.f * shakeMagnitude;
		float offsetY = ((rand() % 100) / 100.f - 0.5f) * 2.f * shakeMagnitude;
		float offsetZ = ((rand() % 100) / 100.f - 0.5f) * 2.f * shakeMagnitude;

		shakeOffset = glm::vec3(offsetX, offsetY, offsetZ);
	}
	else
	{
		shakeOffset = glm::vec3(0.f);
	}

	// Apply shake to camera position
	position += shakeOffset;

	target = position + view;
	isDirty = true;


	this->Refresh();
}

void FPCamera::Shake(float duration, float magnitude)
{
	shakeDuration = duration;
	shakeMagnitude = magnitude;
}


void FPCamera::Refresh()
{
	if (!this->isDirty) return;
	glm::vec3 view = glm::normalize(target - position);
	glm::vec3 right = glm::normalize(glm::cross(up, view));
	// Recalculate the up vector
	this->up = glm::normalize(glm::cross(view, right));
	this->isDirty = false;
}