#include "headers/Transform.hpp"

Transform::Transform(glm::vec3 vec, float rotationDeg, glm::vec3 rotationAxis, glm::vec3 scale) {
	// matrix = glm::scale(
    //     glm::rotate(
    //         glm::translate(glm::mat4(1), vec),
    //         glm::radians(rotationDeg), rotationAxis
    //     ),
    //     scale
    // );

    matrix = glm::mat4(1);
}

void Transform::translate(glm::vec3 vec) {
	matrix = glm::translate(matrix, vec);
}

void Transform::rotate(glm::vec3 axis, float rotationDeg) {
    matrix = glm::rotate(matrix, glm::radians(rotationDeg), axis);
}

void Transform::scale(glm::vec3 scalar) {
    matrix = glm::scale(matrix, scalar);
}

void Transform::setPosition(glm::vec3 position) {
    matrix[3] = glm::vec4(position, 1);
}

void Transform::setRotation(glm::vec3 axis, float rotationDeg) {
    glm::vec3 translation = glm::vec3(matrix[3]);
    glm::vec3 scale = glm::vec3(
        glm::length(glm::vec3(matrix[0])),  // X-axis scale
        glm::length(glm::vec3(matrix[1])),  // Y-axis scale
        glm::length(glm::vec3(matrix[2]))   // Z-axis scale
    );
    matrix = glm::mat4(1);
    matrix = glm::rotate(matrix, glm::radians(rotationDeg), axis);
    
    matrix[0] *= scale.x;
    matrix[1] *= scale.y;
    matrix[2] *= scale.z;
    
    matrix[3] = glm::vec4(translation, 1.f);
}

void Transform::setScale(glm::vec3 scalar) {
    glm::vec3 translation = glm::vec3(matrix[3]);
    glm::mat3 rotationMatrix = glm::mat3(matrix);
    rotationMatrix[0] = glm::normalize(rotationMatrix[0]);
    rotationMatrix[1] = glm::normalize(rotationMatrix[1]);
    rotationMatrix[2] = glm::normalize(rotationMatrix[2]);
    glm::mat4 newTransform = glm::mat4(1.0f);
    newTransform[0] = glm::vec4(rotationMatrix[0] * scalar.x, 0.0f);
    newTransform[1] = glm::vec4(rotationMatrix[1] * scalar.y, 0.0f);
    newTransform[2] = glm::vec4(rotationMatrix[2] * scalar.z, 0.0f);
    
    newTransform[3] = glm::vec4(translation, 1.0f);
    
    matrix = newTransform;
}