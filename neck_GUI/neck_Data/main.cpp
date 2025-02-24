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
	Rect roi = Rect(x_slider, 0, IMAGE_W, IMAGE_H);
	frame_disp1 = frame_disp(roi).clone();
	imshow("Display", frame_disp1);
}

#if 1
// multiple: read single file auto generate multiple sample (every 200 points)
int main(int argc, char *argv[])
{
	char path[10];
	char filename[50];
	int  seconds;
	int  total_points;

	seconds = 10;
	total_points = seconds * 5000;

	// read data
	char fullfilename[300];
	strcpy(path, "d:/temp/0723/data");
	strcpy(filename, "capture_data_10sec_water_sf_4");
	sprintf(fullfilename, "%s/%s.txt", path, filename);
	int  *data;
	FILE *fp;
	fp = fopen(fullfilename, "r");
	if (fp == NULL) {
		printf("No input data: %s Failed\n", fullfilename);
		exit(-1);
	}
	data = (int *)malloc(2*total_points * sizeof(int));
	char tmpstr[100];
	char valstr[100];
	for (int i = 0; i < total_points; i++) {
		fscanf(fp, "%s %s\n", tmpstr, valstr);
		data[i] = atoi(valstr);
	}
	fclose(fp);

	// generate sample by shifting
	for (int k = 1; k <= (total_points-17500)/200; k++) {
		sprintf(fullfilename, "%s/data3/class1/train_water_0001_%02d.txt", path, k);
		fp = fopen(fullfilename, "w");
		int start_points = k * 200;
		for (int i = start_points; i < start_points + 17500; i++) {
			fprintf(fp, "%d\n", data[i]);
		}
		printf("%s ok\n", fullfilename);
		fclose(fp);
	}

	// generate sample by shifting
	for (int k = 1; k <= (total_points - 17500) / 200; k++) {
		sprintf(fullfilename, "%s/data3/class1/train_water_0002_%02d.txt", path, k);
		fp = fopen(fullfilename, "w");
		int start_points = k * 200;
		int avgr_3500 = 0;
		int avgr_14000 = 0;
		for (int i = start_points; i < start_points + 3500; i++) {
			avgr_3500 += data[i];
		}
		avgr_3500 = avgr_3500 / 3500;
		for (int i = start_points+14000; i < start_points + 17500; i++) {
			avgr_14000 += data[i];
		}
		avgr_14000 = avgr_14000 / 3500;
		int p = 0;
		for (int i = start_points; i < start_points + 17500; i++) {
			     if (p < 3500)  fprintf(fp, "%d\n", avgr_3500);
			else if (p > 14000) fprintf(fp, "%d\n", avgr_14000);
			else                fprintf(fp, "%d\n", data[i]);
				 p++;
		}
		printf("%s ok\n", fullfilename);
		fclose(fp);
	}

	// generate sample by every two points and shifiting 100
	for (int k = 1; k <= (total_points - 35000) / 100; k++) {
		sprintf(fullfilename, "%s/data3/class1/train_water_0003_%02d.txt", path, k);
		fp = fopen(fullfilename, "w");
		int start_points = k * 100;
		for (int i = start_points; i < start_points + 35000; i=i+2) {
			fprintf(fp, "%d\n", data[i]);
		}
		printf("%s ok\n", fullfilename);
		fclose(fp);
	}

	free(data);
}
#endif

#if 0
// multiple: read single file and cut roi and generate multiple sample (every 200 points)
int main(int argc, char *argv[])
{
	char path[10];
	char filename[50];
	int  seconds;
	int  total_points;

	seconds = 10;
	total_points = seconds * 5000;

	// read data
	char fullfilename[300];
	strcpy(path, "d:/temp/0719");
	strcpy(filename, "capture_data_10sec_cough");
	sprintf(fullfilename, "%s/%s.txt", path, filename);
	int  *data;
	FILE *fp;
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

	for (int f = 1; f <= 50; f++) {
		// generate waveform
		Mat frame = Mat::zeros(Size(total_points, IMAGE_H), CV_8UC3);
		for (int i = 0; i < total_points - 1; i++) {
			int y1 = 255 - data[i] / 16;
			int y2 = 255 - data[i + 1] / 16;
			line(frame, Point(i, y1), Point(i + 1, y2), Scalar(255, 255, 255), 2, 8, 0);
		}
		frame_disp = frame.clone();
		resize(frame_disp, frame_disp, Size(total_points / RESIZE_FACTOR, 256));

		// display image
		namedWindow("Display", CV_WINDOW_NORMAL);
		moveWindow("Display", 100, 100);
		resizeWindow("Display", IMAGE_W, IMAGE_H);

		x_slider = 0;
		x_slider_max = total_points / RESIZE_FACTOR - IMAGE_W;

		char TrackbarName[50];
		sprintf(TrackbarName, "x %d", x_slider_max);
		createTrackbar(TrackbarName, "Display", &x_slider, x_slider_max, on_trackbar);
		on_trackbar(x_slider, 0);

		waitKey(0);
		for (int k = 1; k <= 25; k++) {
			sprintf(fullfilename, "%s/data3/class0/train_cough_%d_%02d.txt", path, f, k);
			fp = fopen(fullfilename, "w");
			int start_points = x_slider * 10 + k * 200;
			for (int i = start_points; i < start_points + 17500; i++) {
				fprintf(fp, "%d\n", data[i]);
			}
			fclose(fp);
		}
		destroyAllWindows();
	}
	free(data);
}
#endif

#if 0
// multiple: read file from folder and cut roi and generate multiple sample (every 200 points)
int main(int argc, char *argv[])
{
	char path[10];
	int  seconds;
	int  total_points;

	if (argc == 3) {
		strcpy(path, argv[1]);
		seconds = atoi(argv[2]);
		total_points = seconds * 5000;
	}
	else {
		strcpy(path, "hb");
		seconds = 7;
		total_points = seconds * 5000;
	}

	// read data
	char fullfilename[300];
	int  *data;
	FILE *fp;
	for (int f = 1; f <= 50; f++) {
		sprintf(fullfilename, "D:/Software/ZoneDevelope/ScienceFair/Neck/data_0715/train_data_0715/%s/%s_%d.txt", path, path, f);
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

		// generate waveform
		Mat frame = Mat::zeros(Size(total_points, IMAGE_H), CV_8UC3);
		for (int i = 0; i < total_points - 1; i++) {
			int y1 = 255 - data[i] / 16;
			int y2 = 255 - data[i + 1] / 16;
			line(frame, Point(i, y1), Point(i + 1, y2), Scalar(255, 255, 255), 2, 8, 0);
		}
		frame_disp = frame.clone();
		resize(frame_disp, frame_disp, Size(total_points / RESIZE_FACTOR, 256));

		// display image
		namedWindow("Display", CV_WINDOW_NORMAL);
		moveWindow("Display", 100, 100);
		resizeWindow("Display", IMAGE_W, IMAGE_H);

		x_slider = 0;
		x_slider_max = total_points / RESIZE_FACTOR - IMAGE_W;

		char TrackbarName[50];
		sprintf(TrackbarName, "x %d", x_slider_max);
		createTrackbar(TrackbarName, "Display", &x_slider, x_slider_max, on_trackbar);
		on_trackbar(x_slider, 0);

		waitKey(0);
		for (int k = 1; k <= 25; k++) {
			sprintf(fullfilename, "D:/Software/ZoneDevelope/ScienceFair/Neck/data_0715/train_data_0715/%s/train_%s_%d_%02d.txt", path, path, f, k);
			fp = fopen(fullfilename, "w");
			int start_points = x_slider * 10 + k * 200;
			for (int i = start_points; i < start_points + 17500; i++) {
				fprintf(fp, "%d\n", data[i]);
			}
			fclose(fp);
		}
		free(data);
	}
}
#endif

#if 0
// normal: read file from folder and cut roi and generate one sample
int main(int argc, char *argv[])
{
	char path[10];
	int  seconds;
	int  total_points;

	if (argc == 3) {
		strcpy(path, argv[1]);
		seconds = atoi(argv[2]);
		total_points = seconds * 5000;
	}
	else {
		strcpy(path, "water");
		seconds = 7;
		total_points = seconds * 5000;
	}

	// read data
	char fullfilename[300];
	int  *data;
	FILE *fp;
	for (int f = 1; f <= 50; f++) {
		sprintf(fullfilename, "D:/Software/ZoneDevelope/ScienceFair/Neck/data_0715/train_data_0715/%s/%s_%d.txt", path, path, f);
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

		// generate waveform
		Mat frame = Mat::zeros(Size(total_points, IMAGE_H), CV_8UC3);
		for (int i = 0; i < total_points - 1; i++) {
			int y1 = 255 - data[i] / 16;
			int y2 = 255 - data[i + 1] / 16;
			line(frame, Point(i, y1), Point(i + 1, y2), Scalar(255, 255, 255), 2, 8, 0);
		}
		frame_disp = frame.clone();
		resize(frame_disp, frame_disp, Size(total_points / RESIZE_FACTOR, 256));

		// display image
		namedWindow("Display", CV_WINDOW_NORMAL);
		moveWindow("Display", 100, 100);
		resizeWindow("Display", IMAGE_W, IMAGE_H);

		x_slider = 0;
		x_slider_max = total_points / RESIZE_FACTOR - IMAGE_W;

		char TrackbarName[50];
		sprintf(TrackbarName, "x %d", x_slider_max);
		createTrackbar(TrackbarName, "Display", &x_slider, x_slider_max, on_trackbar);
		on_trackbar(x_slider, 0);

		waitKey(0);
		sprintf(fullfilename, "D:/Software/ZoneDevelope/ScienceFair/Neck/data_0715/train_data_0715/%s/train_%s_%d.txt", path, path, f);
		fp = fopen(fullfilename, "w");
		for (int i = x_slider * 10; i < x_slider * 10 + 17500; i++) {
			fprintf(fp, "%d\n", data[i]);
		}
		fclose(fp);
		free(data);
	}
}
#endif
