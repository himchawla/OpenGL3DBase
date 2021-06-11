#pragma once

#include <functional>

#include <glew.h>
#include <glm.hpp>
#include <iostream>
#include <gtc/matrix_transform.hpp>

class Object;

class Camera
{
public:
	Object* cube;
	bool flag;
	bool quadFlag;
	void setPosition();
	Camera(const glm::vec3& position, const glm::vec3& viewPoint, const glm::vec3& upVector, float moveSpeed = 10.0f, float rotationSpeed = 135.0f);
	
	glm::mat4 Project(glm::mat4 modelMat) const;

	void setMoveSpeed(float moveSpeed);
	void setRotationSpeed(float degreesPerSecond);
	void setControls(int forwardKeyCode, int backwardKeyCode, int rotateLeftKeyCode, int rotateRightKeyCode, int rotateDown, int rotateUp);

	glm::mat4 getViewMatrix() const;
	void update(const std::function<bool(int)>& keyInputFunc,
	            const std::function<float(float)>& speedCorrectionFunc);
	glm::vec3 getPosition()
	{
		return _position;
	}
private:
	void moveBy(float distance)
	{
		glm::vec3 vOffset = getNormalizedViewVector();
		vOffset *= distance;
		_position += vOffset;
		_viewPoint += vOffset;
		std::cout << _position.x << '\t' << _position.z << '\n';
	}


	void rotateBy(float angleInDegrees, char axis = 'y')
	{
		glm::mat4 rotationMatrix;
		if (axis == 'y')
			rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
		else
			rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec4 rotatedViewVector = rotationMatrix * glm::vec4(getNormalizedViewVector(), 0.0f);
		_viewPoint = _position + glm::vec3(rotatedViewVector.x, rotatedViewVector.y, rotatedViewVector.z);
		std::cout << _position.x << '\t' << _position.z << '\n';
	}

	glm::vec3 getNormalizedViewVector() const
	{
		return glm::normalize(_viewPoint - _position);
	}

	glm::vec3 _position;
	glm::vec3 _viewPoint;
	glm::vec3 _upVector;

	float _moveSpeed;
	float _rotationSpeed;
	int _forwardKeyCode;
	int _backwardKeyCode;
	int _rotateLeftKeyCode;
	int _rotateRightKeyCode;
	int _rotateDownKeyCode;
	int _rotateUpKeyCode;
	int _resetKeyCode;
	glm::mat4 projectionMatrix;
};
