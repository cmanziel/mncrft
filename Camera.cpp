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
    : m_fovy(M_PI / 4), m_FocalLength(0.1f), m_NearToFarDistance(99.9f)
{
    // place the camera at the centre of the chunk and at CHUNK_HEIGHT
    m_CameraPos = vec3(8.0, 20.0, 8.0);
    m_CameraDir = glm::normalize(vec3(0.0, 0.0, -1.0));
    //m_CameraDir = glm::normalize(-m_CameraPos);

    m_CameraFront = m_CameraPos + m_CameraDir * m_FocalLength;

    vec3 camera_left = glm::cross(worldUp, m_CameraDir);
    m_CameraUp = glm::normalize(glm::cross(m_CameraDir, camera_left));

    float near_plane_top = m_FocalLength * tan(m_fovy / 2);
    float aspect_ratio = 1280.0 / 960; // TODO: get the actual aspect ratio from the window class' field
    m_NearPlaneLeft = near_plane_top * aspect_ratio;

    //m_yaw = glm::degrees(glm::angle(vec3(1.0, 0.0, 0.0), m_CameraDir));
    m_yaw = -90.0; // correct for camera dir = vec3(0.0, 0.0, -1.0)
    m_pitch = 0.0;
    m_deltaTime = 0.0;
}

void Camera::Move(int dir)
{
    // modify m_CameraPos but also m_CameraFront
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

    m_CameraFront = m_CameraPos + m_CameraDir * m_FocalLength;
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
    m_CameraFront = m_CameraPos + m_CameraDir * m_FocalLength;
}

void Camera::UpdateTime(float dt) {
    m_deltaTime = dt;
}

mat4 Camera::GetModelMat4(vec3 blockTrans, int side)
{
    mat4 model = mat4(1.0);

    model = glm::translate(model, blockTrans);

    return model;
}

mat4 Camera::GetProjectionMat4()
{
    // see https://glm.g-truc.net/0.9.7/api/a00174.html

    //mat4 projection = glm::perspective(m_fovy, (m_FocalLength + m_NearToFarDistance) / m_FocalLength, m_FocalLength, m_FocalLength + m_NearToFarDistance);
    float near_plane_width = 2 * m_NearPlaneLeft;
    float near_plane_height = 2 * m_FocalLength * tan(m_fovy / 2);

    float aspect = near_plane_width / near_plane_height;

    mat4 projection = glm::perspective(m_fovy, aspect, m_FocalLength, m_FocalLength + m_NearToFarDistance);

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

// position; chunk's world position coordinates
bool Camera::IsPosInFrontOfCamera(vec3 playerChunkGridPos, vec3 position)
{
    // don't use camera position, use player's chunk grid position
    vec3 pos_from_player_chunk = position - playerChunkGridPos;

    // project pos_from_player_chunk on m_CameraDir
    vec3 projection = project_on_vector(m_CameraDir, pos_from_player_chunk);

    float t = 0;
    if (m_CameraDir.x != 0)
        t = projection.x / m_CameraDir.x;
    else if (m_CameraDir.y != 0)
        t = projection.y / m_CameraDir.y;
    else
        t = projection.z / m_CameraDir.z;

    if (t < 0.0)
        return false;

    return true;
}

bool Camera::IsInsideFrustum(vec3 position)
{
    vec3 camera_left = glm::normalize(glm::cross(m_CameraUp, m_CameraDir));

    // position coordinates relative to camera position
    vec3 pos_from_camera = position - m_CameraPos;

    // project pos_from_camera on m_CameraDir
    vec3 projection = project_on_vector(m_CameraDir, pos_from_camera);

    // find t for the projection point on the m_CameraDir line
    // if t < m_FocalLength then the point is behind the camera's near plane, the point on the m_CameraDir line one the near plane is: m_CameraPos + m_CameraDir * m_FocalLength

    float t = 0;
    if (m_CameraDir.x != 0)
        t = projection.x / m_CameraDir.x;
    else if (m_CameraDir.y != 0)
        t = projection.y / m_CameraDir.y;
    else
        t = projection.z / m_CameraDir.z;

    if (t < m_FocalLength)
        return false;

    // check if position's distance from near plane is inside m_NearToFaeDistance
    float distance_from_near = point_plane_distance(m_CameraFront, m_CameraDir, position);

    if (distance_from_near > m_NearToFarDistance)
        return false;

    float near_top = m_FocalLength * tan(m_fovy / 2);

    // frustum is simmetrical so right and bottom have the same values
    float plane_horizontal_boundary = distance_from_near / m_FocalLength * m_NearPlaneLeft;
    float plane_vertical_boundary = distance_from_near / m_FocalLength * near_top;

    // distances from camera's axis' planes, in fact the coordinates relative to the positive semiaxis of the camera's coordinate systems
    // if distance of position (function argument) from camera's plane whose normal is camera_left is greater than the distance of plane_left from the same plane, then position is outside frustum

    if (point_plane_distance(m_CameraFront, camera_left, position) > plane_horizontal_boundary)
        return false;

    // change m_CameraUp vector, now it's fixe to worldUp which is (0.0, 1.0, 0.0)
    vec3 camera_up = glm::cross(m_CameraDir, camera_left);
    if (point_plane_distance(m_CameraFront, camera_up, position) > plane_vertical_boundary)
        return false;

    return true;
}