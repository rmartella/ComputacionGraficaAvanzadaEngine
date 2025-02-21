#include "Headers/ThirdPersonCamera.hpp"

ThirdPersonCamera::ThirdPersonCamera(float distanceFromTarget){
    pitch = glm::radians(20.0f);
    yaw = 0.0f;
    angleAroundTarget = 0.0f;
    distanceFromTarget = 1.0f;
    sensitivity = SENSITIVTY;
    worldUp = glm::vec3(0.0, 1.0, 0.0);
    updateCamera();
}

void ThirdPersonCamera::mouseMoveCamera(float xoffset, float yoffset, float dt){
    // Camera controls
    float cameraSpeed = sensitivity * dt;

    // Calculate pitch
    pitch += yoffset * cameraSpeed;
    // Calculate Angle Arround
    angleAroundTarget -= xoffset * cameraSpeed;
    if(pitch > M_PI / 2)
        pitch = M_PI / 2 - 0.01;
    if(pitch < -M_PI / 2)
        pitch = -M_PI / 2 + 0.01;
    updateCamera();
}

void ThirdPersonCamera::scrollMoveCamera(float soffset, float dt){
    // Camera controls
    float cameraSpeed = sensitivity * dt;
    // Calculate zoom
    float zoomLevel = soffset * cameraSpeed;
    distanceFromTarget -= zoomLevel;
    updateCamera();
}

void ThirdPersonCamera::updateCamera(){
    glm::vec3 targetPosition = glm::vec3(0.0);
    glm::vec3 targetForward = glm::vec3(0.0);
    float angleTarget = 0.0;

    if(modelTarget != nullptr){
        targetPosition = glm::vec3(modelTarget->getModelMatrix()[3]);
        targetForward = glm::normalize(glm::vec3(modelTarget->getModelMatrix()[0]));
        
        float dotProduct = glm::dot(targetForward, glm::vec3(1, 0, 0));
        float magnitudeA = glm::length(targetForward);
        float magnitudeB = glm::length(glm::vec3(1, 0, 0));
        angleTarget = glm::acos(dotProduct / (magnitudeA * magnitudeB));
        
        glm::vec3 crossProduct = glm::cross(glm::vec3(1, 0, 0), targetForward);
        if (crossProduct.y < 0)
            angleTarget = -angleTarget;
    }

    //Calculate Horizontal distance
    float horizontalDistance = distanceFromTarget * cos(pitch);
    //Calculate Vertical distance
    float verticalDistance = distanceFromTarget * sin(pitch);

    //Calculate camera position
    float theta = angleTarget + angleAroundTarget;
    float offsetx = horizontalDistance * sin(theta);
    float offsetz = horizontalDistance * cos(theta);
    position.x = targetPosition.x - offsetx;
    position.y = targetPosition.y + verticalDistance;
    position.z = targetPosition.z - offsetz;

    yaw = angleTarget - (180 + angleAroundTarget);

    if (distanceFromTarget < 0)
    	front = glm::normalize(position - targetPosition);
    else
    	front = glm::normalize(targetPosition - position);

    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
