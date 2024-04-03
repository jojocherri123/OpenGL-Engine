#include<iostream>

#include "camera.hpp"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/rotate_vector.hpp>


Camera::Camera(){
    mEye = glm::vec3(0.0f,0.0f,0.0f);
    mViewDirection = glm::vec3(0.0f,0.0f,-1.0f);
    // mViewUpDirection = glm::vec3(0.0f,-1.0f,0.0f);

    mUpVector = glm::vec3(0.0f,1.0f,0.0f);
    mUpVector2 = glm::vec3(1.0f,0.0f,0.0f);
    glm::vec2 mOldMousePosition;
}

glm::mat4 Camera::getViewMatrix() const{
        return glm::lookAt(mEye, mEye + mViewDirection, mUpVector);
    }


void Camera::MouseLook(int mouseX, int mouseY){
    glm::vec2 currentMouse = glm::vec2(mouseX,mouseY);
    

    static bool firstLook = true;
    if(firstLook){
        mOldMousePosition=currentMouse;
        firstLook = false;
    }

    glm::vec2 mouseDelta = mOldMousePosition - currentMouse;

    mViewDirection = glm::rotate(mViewDirection,glm::radians(mouseDelta.x),mUpVector);

    mViewDirection = glm::rotate(mViewDirection,glm::radians(mouseDelta.y),mUpVector2);



    mOldMousePosition = currentMouse;
}

void Camera::MoveForward(float speed){
    mEye += (mViewDirection*speed);
}
void Camera::MoveBackward(float speed){
    mEye -= (mViewDirection*speed);
}
void Camera::MoveLeft(float speed){
    glm::vec3 rightVector = glm::cross(mViewDirection,mUpVector);
    mEye += (rightVector*speed);
}
void Camera::MoveRight(float speed){
    glm::vec3 rightVector = glm::cross(mViewDirection,mUpVector);
    mEye -= (rightVector*speed);
}