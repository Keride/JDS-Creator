#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front){
	this->Position = position;
	this->Front = front;
	this->WorldUp = up;
	this->Up = this->WorldUp;
	this->updateCameraVectors();
	this->MovementSpeed = 0.1f;
	this->Zoom = 2.0f;
	this->RotationX = 0.0f;
}

glm::mat4 Camera::GetViewMatrix(){
	return glm::lookAt(this->Position, this->Front, this->Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime){
	GLfloat velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD) {
		this->Position.x -= velocity;
		this->Front.x -= velocity;
	}else if (direction == BACKWARD) {
		this->Position.x += velocity;
		this->Front.x += velocity;
	}else if (direction == LEFT) {
		this->Position.z -= velocity;
		this->Front.z -= velocity;
	}else if (direction == RIGHT) {
		this->Position.z += velocity;
		this->Front.z += velocity;
	}
}


void Camera::ProcessMouseRotation(int xoffset, int yoffset){
	this->RotationX += (xoffset - this->lastX) / 10;
}


void Camera::SetLastX(int xoffset){
	this->lastX = xoffset;
}

void Camera::ProcessMouseScroll(int yoffset)
{
	this->Zoom += float(yoffset/10.f);
	if (this->Zoom <= 0.1f) {
		this->Zoom = 0.1f;
	}else if (this->Zoom >= 10.0f) {
		this->Zoom = 10.0f;
	}
}


void  Camera::updateCameraVectors(){
	return;
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