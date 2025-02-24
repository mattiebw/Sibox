#include "papipch.h"

glm::mat4 MathUtil::CreateTransformationMatrix(const glm::vec3 &translation, const glm::vec3 &rotation,
                                               const glm::vec3 &scale)
{
	glm::mat4 mat(1.0f); // Identity matrix
	mat = translate(mat, translation);
	mat = rotate(mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	mat = rotate(mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	mat = rotate(mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
	mat = glm::scale(mat, scale);
	return mat;
}
