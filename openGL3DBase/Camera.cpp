#include "Camera.h"

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
