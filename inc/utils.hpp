#include "opencv2/opencv.hpp"
#include <tide/external.hpp>

using namespace cv;

typedef unsigned char uchar;

GLuint cvMat2TexInput(Mat& img)
{
    transpose(img, img);
    flip(img, img, 1);
    flip(img, img, 0);
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.cols, img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    return tex;
}

uchar* cvMat2Uchar(Mat& img)
{
    //cvtColor(img, img, COLOR_BGR2RGB);
    transpose(img, img);
    flip(img, img, 1);
    flip(img, img, 0);
    return img.data;
}

Mat EulerToQuaternion(float roll, float pitch, float hdg)
{
	Mat rst(Size(1,4), CV_32F);

	float cosRoll    = cos(roll  * 0.5);
	float sinRoll    = sin(roll  * 0.5);
	float cosPitch   = cos(pitch * 0.5);
	float sinPitch   = sin(pitch * 0.5);
	float cosHeading = cos(hdg   * 0.5);
	float sinHeading = sin(hdg   * 0.5);

	rst.at<float>(0, 0) = cosRoll * cosPitch * cosHeading + sinRoll * sinPitch * sinHeading;
	rst.at<float>(1, 0) = sinRoll * cosPitch * cosHeading - cosRoll * sinPitch * sinHeading;
	rst.at<float>(2, 0) = cosRoll * sinPitch * cosHeading + sinRoll * cosPitch * sinHeading;
	rst.at<float>(3, 0) = cosRoll * cosPitch * sinHeading - sinRoll * sinPitch * cosHeading;

	return rst;
}