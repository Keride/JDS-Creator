

#pragma once

// GL Includes
#include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat RotationX;
	int lastX;

	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	// Default camera values
	/*const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 3.0f;
	const GLfloat SENSITIVTY = 0.25f;
	const GLfloat ZOOM = 45.0f;*/

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 front = glm::vec3(glm::vec3(-1.0f, -1.0f, 0.0f)))
	{
		this->Position = position;
		this->Front= front;
		this->WorldUp = up;
		this->updateCameraVectors(); 
		this->MovementSpeed = 1.0f;
		this->Zoom = 2.0f;
		this->RotationX = 0.0f;
	}
	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
			this->Position -= glm::vec3(velocity ,0, 0);
		if (direction == BACKWARD)
			this->Position += glm::vec3(velocity, 0, 0);
		if (direction == LEFT)
			this->Position += glm::vec3(0, 0, velocity);
		if (direction == RIGHT)
			this->Position -= glm::vec3(0, 0, velocity);
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseRotation(int xoffset, int yoffset)
	{
		this->RotationX += (xoffset - this->lastX)/10;
	}
	void SetLastX(int xoffset)
	{
		this->lastX = xoffset;
	}
	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(int yoffset)
	{
		if (this->Zoom >= 0.0f && this->Zoom <= 3.0f)
			this->Zoom -= yoffset/10;
		if (this->Zoom <= 0.0f)
			this->Zoom = 0.0f;
		if (this->Zoom >= 3.0f)
			this->Zoom = 3.0f;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		//glm::vec3 front;
		/*front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);*/
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
};

