//
// Created by wangrl on 2021/3/24.
//

/**
 * 参考文档
 *
 * https://learnopengl.com/Getting-started/Camera
 *
 * 1. 摄像机的位置
 *
 * 摄像机的位置简单来说就是世界空间中一个指向摄像机位置的向量。
 *
 * glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
 *
 * 不要忘记正Z轴是从屏幕指向你的，如果希望相机向后移动，就沿着z轴正方向移动。
 *
 * 2. 摄像机的方向(Camera Direction)
 *
 * 让相机指向场景原点(0, 0, 0)
 *
 * glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
 * glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
 *
 * 3. 右轴(Right)
 *
 * 定义一个向上的向量，然后进行叉乘。
 *
 * glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
 * glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
 *
 * 4. 上轴(Up)
 *
 * glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
 *
 * 5. Look At函数
 *
 *          | Rx Ry Rz 0 |   | 1 0 0 -Px |
 *          | Ux Uy Uz 0 |   | 0 1 0 -Py |
 * LookAt = | Dx Dy Dz 0 | * | 0 0 1 -Pz |
 *          | 0  0  0  1 |   | 0 0 0 0   |
 *
 * 其中R是右向量，U是上向量，D是方向向量，P是相机位置向量。
 *
 * glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
 *          glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
 *
 * 通过位置、目标、上向量创建一个View矩阵，修改相机的x和y轴可以让相机围绕场景旋转。
 *
 * 6. 相机自由移动
 *
 * 设置相机的方向
 *
 * glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
 * view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
 *
 * 通过WASD按键的任意一个，相机的位置都会相应更新。如果希望 向前或向后移动，就把位置向量加上或减去方向向量。
 * 如果希望左右移动，使用叉乘来创建一个右向量，并沿着它的方向相应移动，这样就可以得到相机横移的效果。
 *
 * 7. 视角移动
 *
 * 根据鼠标的输入改变cameraFront的值
 *
 * 欧拉角分为三种：俯仰角(Pitch)、偏航角(Yaw)、滚转角(Roll)
 *
 * direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
 * direction.y = sin(glm::radians(pitch));
 * direction.z = cos(glm::radians(pitch)) *sin(glm::radians(yaw));
 *
 * 通过鼠标的上下、左右移动来改变偏航角和俯仰角的值。
 *
 * 8. 缩放
 *
 * 使用滚轮来改变视野(Field of View)来进行缩放。
 *
 * projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement.
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates
// the corresponding euler angles, vectors and matrices for use
// in OpenGL.
class Camera {
public:
    // camera attributes
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;
    // euler angles
    float mYaw;
    float mPitch;
    // camera options
    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW,
           float pitch = PITCH) :
            mFront(glm::vec3(0.0f, 0.0f, -1.0f)),
            mMovementSpeed(SPEED),
            mMouseSensitivity(SENSITIVITY),
            mZoom(ZOOM) {
        mPosition = position;
        mWorldUp = up;
        mYaw = yaw;
        mPitch = pitch;

        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(mPosition, mPosition + mFront, mUp);
    }

    void processKeyboard(CameraMovement direction, float deltaTime) {
        float velocity = mMovementSpeed * deltaTime;
        if (direction == FORWARD) {
            mPosition += mFront * velocity;
        } else if (direction == BACKWARD) {
            mPosition -= mFront * velocity;
        } else if (direction == LEFT) {
            mPosition -= mRight * velocity;
        } else if (direction == RIGHT) {
            mPosition += mRight * velocity;
        }
    }

    void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
        xOffset *= mMouseSensitivity;
        yOffset *= mMouseSensitivity;

        mYaw += xOffset;
        mPitch += yOffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped.
        if (constrainPitch) {
            if (mPitch > 89.0f)
                mPitch = 89.0f;
            if (mPitch < -89.0f)
                mPitch = -89.0f;
        }
        // Update front, right and up vectors using the updated euler angles
        updateCameraVectors();
    }

    void processMouseScroll(float yOffset) {
        mZoom -= (float) yOffset;
        if (mZoom < 1.0f)
            mZoom = 1.0f;
        if (mZoom > 45.0f)
            mZoom = 45.0f;
    }

private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
        front.y = sin(glm::radians(mPitch));
        front.z = cos(glm::radians(mPitch)) * sin(glm::radians(mYaw));

        // Also re-calculate the right and up vector.
        mRight = glm::normalize(glm::cross(mFront, mWorldUp));
        mUp = glm::normalize(glm::cross(mRight, mFront));
    }
};
