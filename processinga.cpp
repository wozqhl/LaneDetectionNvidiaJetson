//#####################################################################################//
//#####################################################################################//
//#####################################################################################//
//# Please include the Github Repositories web URL if you are using this material.    #//
//#####################################################################################//
//#####################################################################################//
//#####################################################################################//



#include "opencv2/opencv.hpp"
#include <iostream>
#include "opencv2/cudaarithm.hpp"
#include "opencv2/cudafilters.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudawarping.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "all_header.h"
#include "resize.h"
#include "hsv.h"
#include "gray.h"
#include "binary.h"
#include "imagewrap.h"
#include "sobel.h"

using namespace std;
using namespace cv;


void processinga_frame(cuda::GpuMat& src, cuda::GpuMat& resize, cuda::GpuMat& dst, cuda::GpuMat& gpu_mapa, cuda::GpuMat& gpu_mapb)
{

	cv::Point2f src_points[4];
	cv::Point2f dst_points[4];

	src_points[0] = cv::Point2f(290, 230);
	src_points[1] = cv::Point2f(350, 230);
	src_points[2] = cv::Point2f(520, 340);
	src_points[3] = cv::Point2f(130, 340);

	dst_points[0] = cv::Point2f(130, 0);
	dst_points[1] = cv::Point2f(520, 0);
	dst_points[2] = cv::Point2f(520, 360);
	dst_points[3] = cv::Point2f(130, 360);

	resizeframe resize_out;
	hsvframe hsv_out;
	grayframe grayout;
	binaryframe binaryout;
	wrapframe wrapout;
	sobelframe sobelout;


	Mat frame, cudaout_frame, MergeFrameOut, cudaout_framet;
	cuda::GpuMat  resize_framea, gray_framea, binary_framea, birdview_framea, hsv_framea, threshold_frame, sobel_frameout, gpu_undisort;

	resize_out.resize_frame(src, resize_framea);

	cuda::remap(resize_framea, gpu_undisort, gpu_mapa, gpu_mapb, cv::INTER_LINEAR);
	resize = gpu_undisort;

	wrapout.wrap_frame(resize_framea, birdview_framea, src_points, dst_points);
	sobelout.sobel_frame(birdview_framea, sobel_frameout);
	hsv_out.hsv_frame(birdview_framea, hsv_framea);
	grayout.gray_frame(hsv_framea, gray_framea);
	binaryout.binary_frame(gray_framea, binary_framea);

	cuda::addWeighted(binary_framea, 0.9, sobel_frameout, 0.1, -1, dst);
}