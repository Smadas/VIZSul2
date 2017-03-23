#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define LAP_KERNEL_OUT 1
#define LAP_KERNEL_IN -4

Mat applyLaplace(Mat src) {
	Mat  src_gray, dst;
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	/// Remove noise by blurring with a Gaussian filter
	GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

	/// Convert the image to grayscale
	cvtColor(src, src_gray, CV_BGR2GRAY);

	/// Apply Laplace function
	Mat abs_dst;

	Laplacian(src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(dst, abs_dst);

	return abs_dst;

}

unsigned char scalePixelVal(int pixelValue) {
	if (pixelValue < 0) {
		return 0;
	}
	if (pixelValue > 255) {
		return 255;
	}
	
	return (unsigned char)pixelValue;
}

unsigned char computeOnePixel(cv::Mat src, cv::Mat changed, int i, int j) {
	int pixelLaplaceVal = 0;
	pixelLaplaceVal = src.at<uchar>(j, i) * LAP_KERNEL_IN;
	if (j - 1 > -1) {
		pixelLaplaceVal += src.at<uchar>(j - 1, i)*LAP_KERNEL_OUT;
	}
	if (i - 1 > -1) {
		pixelLaplaceVal += src.at<uchar>(j, i - 1)*LAP_KERNEL_OUT;
	}
	if (j + 1 < src.size().height) {
		pixelLaplaceVal += src.at<uchar>(j + 1, i)*LAP_KERNEL_OUT;
	}
	if (i + 1 < src.size().width) {
		pixelLaplaceVal += src.at<uchar>(j, i + 1)*LAP_KERNEL_OUT;
	}
	return scalePixelVal(pixelLaplaceVal);
}

cv::Mat applyLaplaceProg(cv::Mat src) {
	cv::Mat changed;
	cvtColor(src, src, CV_BGR2GRAY);

	changed = src.clone();
	
	for (int i = 0; i < src.size().width; i++)
	{
		for (int j = 0; j < src.size().height; j++)
		{
			changed.at<uchar>(j, i) = computeOnePixel(src, changed, i, j);
		}
	}
	return changed;
}

cv::Mat substractImgs(cv::Mat img1, cv::Mat img2) {
	cv::Mat result;
	return result;
}

bool imgMatch(cv::Mat img1, cv::Mat img2) {
	return false;
}

int main(){
	//test branch
	Mat imageOrig, imgLaplaceCV, imagLaplaceProg;

	imageOrig = imread("Autobus.bmp", IMREAD_COLOR);

	if (!imageOrig.data){ // Check for invalid input
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	namedWindow("Original picture", WINDOW_AUTOSIZE);
	namedWindow("OpenCV laplace", WINDOW_AUTOSIZE);
	namedWindow("Programed laplace", WINDOW_AUTOSIZE);

	imgLaplaceCV = applyLaplace(imageOrig);
	imagLaplaceProg = applyLaplaceProg(imageOrig);

	imshow("Original picture", imageOrig); // Show our image inside it.
	imshow("OpenCV laplace", imgLaplaceCV); // Show our image inside it.
	imshow("Programed laplace", imagLaplaceProg); // Show our image inside it.

	//zapisanie obrazka so suboru
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	try {
		imwrite("vystup.png", imagLaplaceProg, compression_params);
	}
	catch (runtime_error& ex) {
		fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
		return 1;
	}

	waitKey(0); // Wait for a keystroke in the window
	return 0;
}