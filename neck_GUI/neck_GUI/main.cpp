#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>

#include <atlstr.h>
#include <conio.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <winsock2.h>
#include <windows.h> 
#include <process.h>
#include <winspool.h>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <fcntl.h>
#include <WinIOCtl.h>
#include <io.h>

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv/cvaux.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace cv;
using namespace std;

#define RESIZE_FACTOR	10
#define IMAGE_W			1750
#define IMAGE_H			256

int x_slider_max;
int x_slider;

Mat frame_disp;

static void on_trackbar(int, void*)
{
	Mat frame_disp1;
	Rect roi = Rect(x_slider, 0, IMAGE_W, IMAGE_H + 50);
	frame_disp1 = frame_disp(roi).clone();
	imshow("Display", frame_disp1);
}

#define RESULT_COUNT	86
void read_result(int *result_data)
{
	char fullfilename[200];
	FILE *fp;
	double c1[RESULT_COUNT+10];
	double c2[RESULT_COUNT+10];
	double c3[RESULT_COUNT+10];
	double c4[RESULT_COUNT+10];

	sprintf(fullfilename, "d:/temp/0723/result.txt");
	fp = fopen(fullfilename, "r");
	if (fp == NULL) return;

	int timeval;
	char s1[100];
	char s2[100];
	char s3[100];
	char s4[100];
	for (int i = 0; i < RESULT_COUNT; i++) {
		fscanf(fp, "%d %s %s %s %s\n", &timeval, s1, s2, s3, s4);
		c1[i] = atof(s1);
		c2[i] = atof(s2);
		c3[i] = atof(s3);
		c4[i] = atof(s4);
	}
	fclose(fp);

	double c11[RESULT_COUNT + 10];
	double c22[RESULT_COUNT + 10];
	double c33[RESULT_COUNT + 10];
	double c44[RESULT_COUNT + 10];
	memset((double *)c11, 0, RESULT_COUNT * sizeof(double));
	memset((double *)c22, 0, RESULT_COUNT * sizeof(double));
	memset((double *)c33, 0, RESULT_COUNT * sizeof(double));
	memset((double *)c44, 0, RESULT_COUNT * sizeof(double));
	for (int i = 0; i < RESULT_COUNT; i++) {
		int k1 = i - 2;
		int k2 = i + 2;
		if (k1 < 0) k1 = 0;
		if (k2 > RESULT_COUNT) k2 = RESULT_COUNT;
		int p = 0;
		for (int k = k1; k < k2; k++) {
			c11[i] += c1[k];
			c22[i] += c2[k];
			c33[i] += c3[k];
			c44[i] += c4[k];
			p++;
		}
		c11[i] = c11[i] / p;
		c22[i] = c22[i] / p;
		c33[i] = c33[i] / p;
		c44[i] = c44[i] / p;
	}

	for (int i = 0; i < RESULT_COUNT; i++) {
		result_data[i] = 3;
		if (c11[i] > c22[i] && c11[i] > c33[i] && c11[i] > c44[i]) result_data[i] = 0;
		if (c22[i] > c11[i] && c22[i] > c33[i] && c22[i] > c44[i]) result_data[i] = 1;
		if (c33[i] > c11[i] && c33[i] > c22[i] && c33[i] > c44[i]) result_data[i] = 2;
	}
}

int main(int argc, char *argv[])
{
	char drive[10];
	char filename[100];
	int  seconds;
	int  total_points;
	int  result_data[RESULT_COUNT];

	if (argc == 4) {
		strcpy(drive, argv[1]);
		strcpy(filename, argv[2]);
		seconds = atoi(argv[3]);
		total_points = seconds * 5000;
	}
	else {
		strcpy(drive, "d:/temp/0723");
		strcpy(filename, "capture_data.txt");
		seconds = 10;
		total_points = seconds * 5000;
	}

	// read data
	char fullfilename[300];
	int  *data;
	FILE *fp;
	sprintf(fullfilename, "%s/%s", drive, filename);
	printf("process input data: %s\n", fullfilename);
	fp = fopen(fullfilename, "r");
	if (fp == NULL) {
		printf("No input data: %s Failed\n", fullfilename);
		exit(-1);
	}
	data = (int *)malloc(total_points * sizeof(int));
	char tmpstr[100];
	char valstr[100];
	for (int i = 0; i < total_points; i++) {
		fscanf(fp, "%s %s\n", tmpstr, valstr);
		data[i] = atoi(valstr);
	}
	fclose(fp);

	// read result
	read_result(result_data);

	// generate waveform
	Mat frame = Mat::zeros(Size(total_points, IMAGE_H + 50) , CV_8UC3);
	for (int i = 0; i < total_points-1; i++) {
		int y1 = 255 - data[i] / 16;
		int y2 = 255 - data[i+1] / 16;
		line(frame, Point(i, y1), Point(i+1, y2), Scalar(255, 255, 255), 2, 8, 0);
	}
	// generate result
	for (int i = 0; i < RESULT_COUNT; i++) {
		int x1 = i * 3500;
		int y1 = 256 + 10;
		if (result_data[i] == 0) {
			rectangle(frame, Rect(x1,y1,3500,40), cvScalar(0x00, 0xff, 0x00), CV_FILLED, 8, 0);
		}
		else if (result_data[i] == 1) {
			rectangle(frame, Rect(x1, y1, 3500, 40), cvScalar(0xff, 0x00, 0x00), CV_FILLED, 8, 0);
		}
		else if (result_data[i] == 2) {
			rectangle(frame, Rect(x1, y1, 3500, 40), cvScalar(0x00, 0x00, 0xff), CV_FILLED, 8, 0);
		}
	}
	frame_disp = frame.clone();
	resize(frame_disp, frame_disp, Size(total_points / RESIZE_FACTOR, IMAGE_H + 50));

	// display image
	namedWindow("Display", CV_WINDOW_NORMAL);
	moveWindow("Display", 100, 100);
	resizeWindow("Display", IMAGE_W, IMAGE_H + 50);

	x_slider = 0;
	x_slider_max = total_points/ RESIZE_FACTOR - IMAGE_W;

	char TrackbarName[50];
	sprintf(TrackbarName, "x %d", x_slider_max);
	createTrackbar(TrackbarName, "Display", &x_slider, x_slider_max, on_trackbar);
	on_trackbar(x_slider, 0);

	waitKey(0);
	free(data);
}