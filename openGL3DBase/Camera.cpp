#include "Camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& viewPoint, const glm::vec3& upVector, float moveSpeed,
	float rotationSpeed)
	: _position(position)
	, _viewPoint(viewPoint)
	, _upVector(upVector)
	, _moveSpeed(moveSpeed)
	, _rotationSpeed(rotationSpeed)
{
	projectionMatrix = glm::perspective(45.0f, float(1000) / float(1000), 0.5f, 1000.0f);

	setControls((int)'w', (int)'s', (int)'j', (int)'l');
}

glm::mat4 Camera::Project(glm::mat4 modelMat) const
{
	return projectionMatrix * getViewMatrix() * modelMat;
}

void Camera::setMoveSpeed(float moveSpeed)
{
	_moveSpeed = moveSpeed;
}

void Camera::setRotationSpeed(float degreesPerSecond)
{
	_rotationSpeed = degreesPerSecond;
}

void Camera::setControls(int forwardKeyCode, int backwardKeyCode, int rotateLeftKeyCode, int rotateRightKeyCode)
{
	_forwardKeyCode = forwardKeyCode;
	_backwardKeyCode = backwardKeyCode;
	_rotateLeftKeyCode = rotateLeftKeyCode;
	_rotateRightKeyCode = rotateRightKeyCode;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(_position, _viewPoint, _upVector);
}

void Camera::update(const std::function<bool(int)>& keyInputFunc, const std::function<float(float)>& speedCorrectionFunc)
{
	if (keyInputFunc(_forwardKeyCode)) {
		moveBy(speedCorrectionFunc(_moveSpeed));
	}

	if (keyInputFunc(_backwardKeyCode)) {
		moveBy(-speedCorrectionFunc(_moveSpeed));
	}

	if (keyInputFunc(_rotateLeftKeyCode)) {
		rotateBy(speedCorrectionFunc(_rotationSpeed));
	}

	if (keyInputFunc(_rotateRightKeyCode)) {
		rotateBy(-speedCorrectionFunc(_rotationSpeed));
	}
}
