#include "opencv2/opencv.hpp"
#include <iostream>
#include <utils.hpp>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <tide/common.hpp>

#include <eis.hpp>

using namespace std;
using namespace cv;
using namespace tide;

#define WIDTH 1080
#define HEIGHT 720

int main()
{
	VideoCapture cap("1.mp4"); 
	if(!cap.isOpened())
	{
		cout << "Error opening video stream or file" << endl;
		return -1;
	}

	Context context("Test", WIDTH, HEIGHT);
	glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);
	Camera2D cam(context.getWindow(), WIDTH, HEIGHT, 25);

	Shader tex("tex.vert", "tex.frag");
	Element video(const_cast<GLfloat*>(&ImageVertices[0]), const_cast<GLint*>(&ImageIndices[0]), sizeof(ImageVertices), sizeof(ImageIndices), TWOD_TEXTURE_ATTR);
	video.attachShader(&tex);
	video.addTexture("tex", 0);
	video.addFloatUniform("width", WIDTH);
	video.addFloatUniform("height", HEIGHT);
	glm::quat qPitch1 = glm::angleAxis(0.1f, glm::vec3(1, 0, 0));
	glm::quat qYaw1 = glm::angleAxis(0.1f, glm::vec3(0, 1, 0));
	glm::quat qRoll1 = glm::angleAxis(0.1f, glm::vec3(0, 0, 1));
	glm::quat q = glm::normalize(qPitch1 * qYaw1 * qRoll1);
	glm::mat3 trans1 = glm::mat3(
			glm::vec3(1-2*q.y*q.y-2*q.z*q.z, 2*q.x*q.y-2*q.z*q.w, 2*q.x*q.z+2*q.y*q.w),
			glm::vec3(2*q.x*q.y+2*q.z*q.w, 1-2*q.x*q.x-2*q.z*q.z, 2*q.y*q.z-2*q.x*q.w),
			glm::vec3(2*q.x*q.z-2*q.y*q.w, 2*q.y*q.z+2*q.x*q.w, 1-2*q.x*q.x-2*q.y*q.y)
		);
	glm::quat qPitch2 = glm::angleAxis(1.0f, glm::vec3(1, 0, 0));
	glm::quat qYaw2 = glm::angleAxis(1.0f, glm::vec3(0, 1, 0));
	glm::quat qRoll2 = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));
	q = glm::normalize(qPitch2 * qYaw2 * qRoll2);
	glm::mat3 trans2 = glm::mat3(
			glm::vec3(1-2*q.y*q.y-2*q.z*q.z, 2*q.x*q.y-2*q.z*q.w, 2*q.x*q.z+2*q.y*q.w),
			glm::vec3(2*q.x*q.y+2*q.z*q.w, 1-2*q.x*q.x-2*q.z*q.z, 2*q.y*q.z-2*q.x*q.w),
			glm::vec3(2*q.x*q.z-2*q.y*q.w, 2*q.y*q.z+2*q.x*q.w, 1-2*q.x*q.x-2*q.y*q.y)
		);
	video.addMat3Uniform("trans1", trans1);
	video.addMat3Uniform("trans2", trans2);

	Eis eis;

	do
	{
		cam.loop();
		Mat frame;
		cap >> frame;
		if (frame.empty())
		{
			cap.set(cv::CAP_PROP_POS_FRAMES, 1);
			continue;
		}

		//process
		//warpAffine(frame, dst, W, Size(frame.cols, frame.rows));

		GLuint pretex = video.texdic["tex"];
		video.texdic["tex"] = cvMat2TexInput(frame);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		video.render();
		context.loop();
		glDeleteTextures(1, &pretex);
	}
	while(context.shouldClose(GLFW_KEY_ESCAPE));

	cap.release();
	destroyAllWindows();

	return 0;
}