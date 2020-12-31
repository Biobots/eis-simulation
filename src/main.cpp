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

	video.addMat3Uniform("trans0", Eis::calTransform(0.099f, 0.101f, 0.101f));
	video.addMat3Uniform("trans1", Eis::calTransform(0.100f, 0.102f, 0.102f));
	video.addMat3Uniform("trans2", Eis::calTransform(0.101f, 0.103f, 0.103f));
	video.addMat3Uniform("trans3", Eis::calTransform(0.102f, 0.104f, 0.104f));
	video.addMat3Uniform("trans4", Eis::calTransform(0.103f, 0.105f, 0.105f));
	video.addMat3Uniform("trans5", Eis::calTransform(0.104f, 0.106f, 0.106f));
	video.addMat3Uniform("trans6", Eis::calTransform(0.105f, 0.107f, 0.107f));
	video.addMat3Uniform("trans7", Eis::calTransform(0.106f, 0.108f, 0.108f));
	video.addMat3Uniform("trans8", Eis::calTransform(0.107f, 0.109f, 0.109f));
	video.addMat3Uniform("trans9", Eis::calTransform(0.108f, 0.110f, 0.110f));

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