#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Eis
{
public:
	glm::mat3 transforms[10];
	glm::mat3 cameraModel;

	float pitch;
	float yaw;
	float roll;

	Eis()
	{
		//generate diagonal matrixs
		for(glm::mat3 t : transforms)
		{
			t = glm::diagonal3x3(glm::vec3(1.0f, 1.0f, 1.0f));
			//std::cout << ((const float*)glm::value_ptr(t))[4] << std::endl;
		}
	}
	~Eis()
	{

	}

	void generateCameraModel(float width, float height, float focal)
	{
		cameraModel = glm::mat3(
			glm::vec3(1.0, 0.0, -width),
			glm::vec3(0.0, 1.0, -height),
			glm::vec3(0.0, 0.0, focal)
		);
	}
	//array
	void updateGyro(float p, float y, float r)
	{
		pitch = p;
		yaw = y;
		roll = r;
	}
	void updateTransform()
	{
		glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));
		glm::quat q = glm::normalize(qPitch * qYaw * qRoll);
		transforms[0] = glm::mat3(
			glm::vec3(1-2*q.y*q.y-2*q.z*q.z, 2*q.x*q.y-2*q.z*q.w, 2*q.x*q.z+2*q.y*q.w),
			glm::vec3(2*q.x*q.y+2*q.z*q.w, 1-2*q.x*q.x-2*q.z*q.z, 2*q.y*q.z-2*q.x*q.w),
			glm::vec3(2*q.x*q.z-2*q.y*q.w, 2*q.y*q.z+2*q.x*q.w, 1-2*q.x*q.x-2*q.y*q.y)
		);
	}
	glm::mat3* getTransform(int& num)
	{
		num = sizeof(transforms)/sizeof(*transforms);
		return transforms;
	}
};