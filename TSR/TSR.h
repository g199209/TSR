﻿#ifndef TSR_H
#define TSR_H

#include <QThread>
#include <QMutex>
#include <opencv2/opencv.hpp>

#define COLOR_RED cv::Scalar(0, 0, 255)
#define COLOR_GRAY cv::Scalar(155, 155, 155)

typedef struct {
	enum {Idle, ReadImg, Step1, Step2, Step3} ProcessStep;

	// 类型
	int SignType;

	// 检测区域设置
	bool DetectAreaEnabled;
	double DetectArea[3];
	int DetectDiv;

	// 图像增强设置
	bool EnhanceEnabled;
	int Saturation;
	bool Histogram;

	// 二值化设置
	int BinaryMethod;
	int BinaryHmin, BinaryHmax, BinarySmin, BinaryImin;
	int BinaryRed, BinaryYellow;
	int BinaryD;
	bool BinaryPost;
	int BinaryDilate, BinaryErode;

	// 形状检测
	int ShapeMethod;
	int HoughP1, HoughP2;
	int ShapeVariance;
	int ShapeDmin, ShapeDmax, ShapeCorner;
}TSRParam_t;

typedef struct {
	double ElapseTime;

	std::vector<cv::Vec3f> circles;

	std::vector<cv::Point> progressPoints;
	std::vector<cv::Point> points;
}TSRResult_t;

extern cv::Mat ImgRead;
extern cv::Mat ImgOut;
extern TSRParam_t TSRParam;
extern TSRResult_t TSRResult;
extern QMutex ImgReadLock, ImgOutLock, TSRParamLock, TSRResultLock;

class TSR : public QThread{
	Q_OBJECT

signals:
	void ImageReady();

public:
	TSR();

protected:
	void run();

private:
	TSRParam_t currentState;
	int64 startTime;
	int64 endTime;
	cv::Mat img;
	cv::Mat channels[3];

	void GetROIImage();
	void OutputROIImage();

	void SaturationEnhance();
	void HistogramEqualize();

	void Binary();

	void Shape();

	void BGR2HSI(const cv::Mat& src, cv::Mat& channelH, cv::Mat& channelS, cv::Mat& channelI);
	void BGR2HSI_2(const cv::Mat& src, cv::Mat& channelH, cv::Mat& channelS, cv::Mat& channelI);
};

#endif
