#include "Camera.h"
#include "Object.h"

void Camera::setPosition()
{
	_position = cube->transform.position;
	_position.z -= 5.0f;
	_position.y += 10.0f;
	_viewPoint = _position;
	_viewPoint.z += 1.0f;
	_viewPoint.y -= 2.0f;
}

Camera::Camera(const glm::vec3& position, const glm::vec3& viewPoint, const glm::vec3& upVector, float moveSpeed,
               float rotationSpeed)
	: _position(position)
	, _viewPoint(viewPoint)
	, _upVector(upVector)
	, _moveSpeed(moveSpeed)
	, _rotationSpeed(rotationSpeed)
{
	projectionMatrix = glm::perspective(45.0f, float(1000) / float(1000), 0.5f, 10000.0f);
	flag = true;
	setControls((int)'i', (int)'k', (int)'j', (int)'l', (int)',', (int)'.');
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

void Camera::setControls(int forwardKeyCode, int backwardKeyCode, int rotateLeftKeyCode, int rotateRightKeyCode, int rotateDown, int rotateUp)
{
	_forwardKeyCode = forwardKeyCode;
	_backwardKeyCode = backwardKeyCode;
	_rotateLeftKeyCode = rotateLeftKeyCode;
	_rotateRightKeyCode = rotateRightKeyCode;
	_rotateDownKeyCode = rotateDown;
	_rotateUpKeyCode = rotateUp;
	_resetKeyCode = 'r';
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(_position, _viewPoint, _upVector);
}

void Camera::update(const std::function<bool(int)>& keyInputFunc, const std::function<float(float)>& speedCorrectionFunc)
{
	if(!flag)
	{
		_position = cube->transform.position;
		_position.z -= 5.0f;
		_position.y += 5.0f;
		_viewPoint = _position;
		_viewPoint.z += 1.0f;
		_viewPoint.y -= 0.5f;
		if (keyInputFunc(_rotateLeftKeyCode)) {
			rotateBy(speedCorrectionFunc(_rotationSpeed));
		}

		if (keyInputFunc(_rotateRightKeyCode)) {
			rotateBy(-speedCorrectionFunc(_rotationSpeed));
		}
	}

	else if(quadFlag)
	{
		_position = glm::vec3(-2.90719509, 100.663536, -181.227371);
		_viewPoint = glm::vec3(-2.24208665f, 100.195961f, -180.645126f);
		quadFlag = false;
	}
	
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
	if (keyInputFunc(_rotateUpKeyCode)) {
		rotateBy(speedCorrectionFunc(_rotationSpeed), 'z');
		
	}

	if (keyInputFunc(_rotateDownKeyCode)) {
		rotateBy(-speedCorrectionFunc(_rotationSpeed), 'z');
		flag = true;
	}

	if(keyInputFunc(_resetKeyCode))
	{
		flag = false;
	}

	if(keyInputFunc('z'))
	{
		quadFlag = true;
	}
}
