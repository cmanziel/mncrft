/*
    * own implementation of lookAt view matrix (http://www.songho.ca/)
*/

#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <GLFW/glfw3.h>

vec3 worldUp(0.0, 1.0, 0.0);

enum sides {
    front, back, left, right, top, bottom
};


Camera::Camera()
{
    // place the camera at the centre of the chunk and at CHUNK_HEIGHT
    m_CameraPos = vec3(8.0, 20.0, 8.0);
    m_CameraDir = glm::normalize(vec3(0.0, 0.0, -1.0));
    //m_CameraDir = glm::normalize(-m_CameraPos);

    m_TargetDist = 3.0;

    m_CameraFront = m_CameraPos + m_CameraDir * m_TargetDist;

    m_CameraUp = vec3(0.0, 1.0, 0.0);

    //m_yaw = glm::degrees(glm::angle(vec3(1.0, 0.0, 0.0), m_CameraDir));
    m_yaw = -90.0; // correct for camera dir = vec3(0.0, 0.0, -1.0)
    m_pitch = 0.0;
    m_deltaTime = 0.0;
}

void Camera::Move(int dir)
{
    float cameraStep = CAMERA_MOTION_SPEED * m_deltaTime;

    switch (dir)
    {
        case GLFW_KEY_W: {
            m_CameraPos += m_CameraDir * cameraStep;
        } break;

        case GLFW_KEY_S: {
            m_CameraPos -= m_CameraDir * cameraStep;
        } break;

        case GLFW_KEY_A: {
            glm::vec3 dir = -glm::normalize(glm::cross(m_CameraDir, worldUp));

            m_CameraPos += cameraStep * dir;
        }  break;

        case GLFW_KEY_D: {
            glm::vec3 dir = glm::normalize(glm::cross(m_CameraDir, worldUp));

            m_CameraPos += cameraStep * dir;
        } break;

        default:
            break;
    }
}

void Camera::Rotate(int dir)
{
    float rotStep = CAMERA_ROTATION_SPEED * m_deltaTime;
    //float rotStep = 1.5;

    if (dir == GLFW_KEY_RIGHT) {
        m_yaw += rotStep;
    }
    else if (dir == GLFW_KEY_LEFT) {
        m_yaw -= rotStep;
    }
    else if (dir == GLFW_KEY_UP) {
        m_pitch += rotStep;
    }
    else {
        m_pitch -= rotStep;
    }

    if (m_pitch > 89.0)
        m_pitch = 89.0;
    else if (m_pitch < -89.0)
        m_pitch = -89.0;

    m_CameraDir.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_CameraDir.y = sin(glm::radians(m_pitch));
    m_CameraDir.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    //m_CameraUp.y = sin(glm::radians(m_pitch));
    //m_CameraUp.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    //m_CameraUp.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));

    m_CameraDir = glm::normalize(m_CameraDir);

    // m_CameraFront is the point distant m_TragetDist from m_CameraPos in the direction of m_CameraDir, line: m_CameraPos + t * m_CameraDir
    m_CameraFront = m_CameraPos + m_CameraDir * m_TargetDist;
}

void Camera::UpdateTime(float dt) {
    m_deltaTime = dt;
}

mat4 Camera::GetModelMat4(vec3 blockTrans, int side)
{
    vec3 translation = vec3(0.0, 0.0, 0.0);
    float angle = 0.0;

    angle = to_radians(angle);

    mat4 rot_mat = mat4(1.0);

    switch (side)
    {
    case top: {
        translation = vec3(0.0, 1.0, 0.0);
        angle = to_radians(-90.0);

        rot_mat = mat4(
            1, 0, 0, 0,
            0, cos(angle), sin(angle), 0,
            0, -sin(angle), cos(angle), 0,
            0, 0, 0, 1
        );
    } break;

    case right: {
        translation = vec3(1.0, 0.0, 0.0);
        angle = to_radians(90.0);

        rot_mat = mat4(
            cos(angle), 0, -sin(angle), 0,
            0, 1, 0, 0,
            sin(angle), 0, cos(angle), 0,
            0, 0, 0, 1
        );
    } break;

    case left: {
        angle = to_radians(90.0);

        rot_mat = mat4(
            cos(angle), 0, -sin(angle), 0,
            0, 1, 0, 0,
            sin(angle), 0, cos(angle), 0,
            0, 0, 0, 1
        );
    } break;

    case bottom: {
        angle = to_radians(-90.0);

        rot_mat = mat4(
            1, 0, 0, 0,
            0, cos(angle), sin(angle), 0,
            0, -sin(angle), cos(angle), 0,
            0, 0, 0, 1
        );
    } break;

    case back: {
        translation = vec3(0.0, 0.0, -1.0);
    } break;
    }

    mat4 model = mat4(1.0);

    model = glm::translate(model, blockTrans);
    //model = glm::translate(model, translation);

    //model = model * rot_mat;

    return model;
}

mat4 Camera::GetProjectionMat4()
{
    mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 960.0f, 0.1f, 100.0f);

    return projection;
}

mat4 Camera::GetViewMat4()
{
    mat4 view = mat4(1.0);

    vec3 fwd = glm::normalize(-m_CameraDir);  // cameraPos + cameraFront = camera's target
    vec3 left = glm::normalize(glm::cross(m_CameraUp, fwd));
    vec3 up = glm::cross(fwd, left);

    float rotMatrix[] = {
        left.x, up.x, fwd.x, 0.0f,
        left.y, up.y, fwd.y, 0.0f,
        left.z, up.z, fwd.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    float posMatrix[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        -m_CameraPos.x, -m_CameraPos.y, -m_CameraPos.z, 1.0f
    };

    mat4 rotMat = glm::make_mat4(rotMatrix);
    mat4 transMat = glm::make_mat4(posMatrix);

    view = rotMat * transMat;

    return view;
}

mat4 Camera::GetMVP(vec3 blockTrans)
{
    mat4 model = mat4(1.0);
    model = glm::translate(model, blockTrans);

    mat4 projection = mat4(1.0);

    mat4 view = mat4(1.0f);

    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 960.0f, 0.1f, 100.0f);

    vec3 fwd = glm::normalize(-m_CameraDir);  // cameraPos + cameraFront = camera's target
    vec3 left = glm::normalize(glm::cross(m_CameraUp, fwd));
    vec3 up = glm::cross(fwd, left);

    float rotMatrix[] = {
        left.x, up.x, fwd.x, 0.0f,
        left.y, up.y, fwd.y, 0.0f,
        left.z, up.z, fwd.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    float posMatrix[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        -m_CameraPos.x, -m_CameraPos.y, -m_CameraPos.z, 1.0f
    };

    mat4 rotMat = glm::make_mat4(rotMatrix);
    mat4 transMat = glm::make_mat4(posMatrix);

    view = rotMat * transMat;

    mat4 mvp = projection * view * model;

	return mvp;
}

vec3 Camera::GetPosition()
{
    return m_CameraPos;
}