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
	Mat K = Mat::eye(cv::Size(3,3), 6);
	K.at<double>(2, 0) = -640;
	K.at<double>(2, 1) = -360;
	Mat Ki;
	cv::invert(K, Ki);
	Mat Transa = getRotationMatrix2D(Point(0, 0), 30, 3);
	Mat Transb = getRotationMatrix2D(Point(0, 0), 10, 0.5);
	Mat Transbt;
	cv::transpose(Transb, Transbt);
	Mat tmp = Ki * (Transbt * (Transa * K));
	Mat W(cv::Size(3, 2), 6);
	W.at<double>(0, 0) = tmp.at<double>(0, 0);
	W.at<double>(0, 1) = tmp.at<double>(0, 1);
	W.at<double>(0, 2) = tmp.at<double>(0, 2);
	W.at<double>(1, 0) = tmp.at<double>(1, 0);
	W.at<double>(1, 1) = tmp.at<double>(1, 1);
	W.at<double>(1, 2) = tmp.at<double>(1, 2);

	Context context("Test", WIDTH, HEIGHT);
	glfwSetInputMode(context.getWindow(), GLFW_STICKY_KEYS, GL_TRUE);
	Camera2D cam(context.getWindow(), WIDTH, HEIGHT, 25);

	Shader tex("tex.vert", "tex.frag");
	Element video(const_cast<GLfloat*>(&ImageCVVertices[0]), const_cast<GLint*>(&ImageIndices[0]), sizeof(ImageCVVertices), sizeof(ImageIndices), TWOD_TEXTURE_ATTR);
	video.attachShader(&tex);
	video.addTexture("tex", 0);
	video.addFloatUniform("width", WIDTH);
	video.addFloatUniform("height", HEIGHT);

	do
	{
		cam.loop();
		Mat frame;
		Mat dst;
		cap >> frame;
		if (frame.empty())
		{
			cap.set(cv::CAP_PROP_POS_FRAMES, 1);
			continue;
		}

		//process

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