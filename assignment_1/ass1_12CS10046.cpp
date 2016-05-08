#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat apply_filter(const Mat &src, const Mat &kernal)
{
	int ksize_h = kernal.size().height;
	ksize_h = (ksize_h - 1) / 2;
	int ksize_w = kernal.size().width;
	ksize_w = (ksize_w - 1) / 2;
	Mat dst = Mat::zeros(src.size(), DataType<double>::type);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			double c = 0.0;
			for (int k = -ksize_h; k <= ksize_h; k++)
			{
				for (int l = -ksize_w; l <= ksize_w; l++)
				{
					if (!(((i + k) <0) || (i + k) >= src.rows || ((j + l) <0) || (j + l) >= src.cols))
					{
						c = c + double(src.at<uchar>(i + k, j + l))*kernal.at<double>(k + ksize_h, l + ksize_w);
					}
				}
			}
			dst.at<double>(i, j) = c;
		}
	}
	dst.convertTo(dst, CV_8UC1);
	return dst;
}
void normalise(Mat& src)
{
	src.convertTo(src, DataType<double>::type);
	int r = src.rows;
	int c = src.cols;
	double max = 0;
	double limit = 255.0;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			if (src.at<double>(i, j) > max)
			{
				max = src.at<double>(i, j);
			}
		}
	}
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			src.at<double>(i, j) *= (limit / max);
		}
	}
}
Mat add2images(const Mat& src1, const Mat& src2)
{
	Mat dst = Mat::zeros(src1.size(), DataType<double>::type);
	if (src1.size() != src2.size())
	{
		cout << "size of both images not same for add 2 images " << endl;
	}
	else
	{
		for (int i = 0; i < dst.rows; i++)
		{
			for (int j = 0; j < dst.cols; j++)
			{
				dst.at<double>(i, j) = double(src1.at<uchar>(i, j)) + double(src2.at<uchar>(i, j));
			}
		}
	}
	dst.convertTo(dst, CV_8UC1);
	return dst;
}
Mat multiply2images(const Mat& src1, const Mat& src2)
{
	Mat dst = Mat::zeros(src1.size(), DataType<double>::type);
	if (src1.size() != src2.size())
	{
		cout << "size of both images not same for multiply 2 images " << endl;
	}
	else
	{
		for (int i = 0; i < dst.rows; i++)
		{
			for (int j = 0; j < dst.cols; j++)
			{
				dst.at<double>(i, j) = double(src1.at<uchar>(i, j)) * double(src2.at<uchar>(i, j));
			}
		}
	}
	// normalize(dst, dst, 0, 255, NORM_MINMAX, DataType<double>::type);
	normalise(dst);
	dst.convertTo(dst, CV_8UC1);
	return dst;
}
Mat scalar_multiplication(const Mat& src, double c)
{
	Mat dst = Mat::zeros(src.size(), DataType<double>::type);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			dst.at<double>(i, j) = double(src.at<uchar>(i, j)) * c;
		}
	}
	return dst;
}
int main()
{
	string filename = "test2_12CS10046.jpg";
	Mat source = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);

	// we get the image now we need to apply laplacian operator 

	double n = 1, d = 9;
	Mat laplace = (Mat_<double>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);
	Mat sobel_h = (Mat_<double>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
	Mat sobel_v = (Mat_<double>(3, 3) << -1, -0, 1, -2, 0, 2, -1, 0, 1);
	Mat average = (Mat_<double>(3, 3) << n / d, n / d, n / d, n / d, n / d, n / d, n / d, n / d, n / d);

	// we have assigned various operators 
	// 1) Laplacian to get F2
	Mat laplace_image = apply_filter(source, laplace);
	imwrite("laplace image.jpg", laplace_image);
	// 3) add F1 and F2
	Mat sharp = add2images(laplace_image, source);
	imwrite("sharp image.jpg", sharp);
	// 4) Sobel operator on sharp image 
	Mat sobel_image = add2images(apply_filter(sharp, sobel_h), apply_filter(sharp, sobel_v));
	imwrite("sobel.jpg", sobel_image);
	// 5) Average sobel image 
	Mat mean_image = apply_filter(sobel_image, average);
	imwrite("smooth image.jpg", mean_image);
	// 6) Mask operation sharpen image and average image
	Mat mask_image = multiply2images(sharp, mean_image);

	imwrite("mask image.jpg", mask_image);
	// 7) high boost filtering 
	Mat na;
	for (double i = 0.3; i <= 2; i += 0.1)
	{
		na = scalar_multiplication(mask_image, i);
		na.convertTo(na, CV_8UC1);
		imwrite("final image"+to_string(i)+".jpg", add2images(source, na));
	}
	return 0;
}