#ifndef camera
#define camera

#include  <glad/gl.h>
#include "../PlutoMath/plutomath.hpp"

enum CAMERA_MOVEMENT {
    FORWARD,
    BACK,
    LEFT,
    RIGHT
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float CAMERA_SPEED = 10.0f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f; // FOV seems to equivalent but might not be true in the future

class Camera {
public:
    plutom::vec3f Position;
    plutom::vec3f Front;
    plutom::vec3f Up;
    plutom::vec3f Right;
    plutom::vec3f World_Up;

    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    explicit Camera(const plutom::vec3f pos = plutom::vec3f(0.0f), const plutom::vec3f up = plutom::vec3f(0.0f,1.0f,0.0f),
           const float yaw = YAW, const float pitch = PITCH) : Front(plutom::vec3f(0.0f, 0.0f, -1.0f)), MovementSpeed(CAMERA_SPEED),
                                                               MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = pos;
        World_Up = up;
        Yaw = yaw;
        Pitch = pitch;
        update_camera_vectors();
    }

    explicit Camera(const float posX = 0.0f, const float posY = 0.0f, const float posZ = 0.0f, const float upX = 0.0f, const float upY = 1.0f, const float upZ = 0.0f,
           const float yaw = YAW, const float pitch = PITCH) : Front(plutom::vec3f(0.0f, 0.0f, -1.0f)), MovementSpeed(CAMERA_SPEED),
           MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = plutom::vec3f(posX,posY,posZ);
        World_Up = plutom::vec3f(upX,upY,upZ);
        Yaw = yaw;
        Pitch = pitch;
        update_camera_vectors();
    }

    [[nodiscard]] plutom::mat4f get_view_matrix() const {
        return plutom::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(const CAMERA_MOVEMENT direction, const float deltaTime){
        const float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position -= Front * velocity;
        if (direction == BACK)
            Position += Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    void ProcessMouseMovement(float x_offset, float y_offset, const GLboolean constrainPitch = true){
        x_offset *= MouseSensitivity;
        y_offset *= MouseSensitivity;

        Yaw   += x_offset;
        Pitch += y_offset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        update_camera_vectors();
    }

    void ProcessMouseScroll(const float y_offset){
        Zoom -= y_offset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    void update_camera_vectors() {
        plutom::vec3f front;
        front.x = cos(plutom::radians(Yaw)) * cos(plutom::radians(Pitch));
        front.y = sin(plutom::radians(Pitch));
        front.z = sin(plutom::radians(Yaw)) * cos(plutom::radians(Pitch));
        Front = front.normalize();
        // also re-calculate the Right and Up vector
        Right = Front.cross(World_Up).normalize();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = Right.cross(Front).normalize();
    }
};

#endif //camera
