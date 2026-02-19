#ifndef FPCAMERA_H
#define FPCAMERA_H

// GLM Headers
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "MouseController.h"

class FPCamera
{
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    FPCamera();
    ~FPCamera();

    void Init(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0,1,0));
    void Reset();
    void Update(double dt);
    void Shake(float duration, float magnitude);
private:
    glm::vec3 shakeOffset = glm::vec3(0.f);
    float shakeDuration = 0.f;
    float shakeMagnitude = 0.f;

    float speed;
    bool isDirty;   // indicate if there is a need to recalculate the camera attributes

    void Refresh();
};

#endif

