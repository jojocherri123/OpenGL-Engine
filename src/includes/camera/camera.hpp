#ifndef CAMERA_HPP
#define CAMERA_HPP

#include<glm/glm.hpp>

class Camera{

    public:
        Camera();
        
        glm::mat4 getViewMatrix() const;

        void MouseLook(int mouseX, int mouseY);
        void MoveForward(float speed);
        void MoveBackward(float speed);
        void MoveLeft(float speed);
        void MoveRight(float speed);
        glm::vec3 mEye;

    private:
        glm::vec3 mViewDirection;
        glm::vec3 mUpVector;

        glm::vec2 mOldMousePosition;
};

#endif