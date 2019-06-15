#include <iostream>
#include <string.h>
#include <stdio.h>
#include <Windows.h>

using namespace std;

const double PI = acos(-1);

#pragma once
class Bitmap {
public:
	/*********************************************************
	* ���캯��
	*********************************************************/
	Bitmap() {};
	Bitmap(Bitmap& bmp);

	/*********************************************************
	* �ļ�����
	*********************************************************/
	bool ReadBmp(const char *FileName);
	bool SaveBmp(const char *FileName);

	/*********************************************************
	* ֱ��ͼ����
	*********************************************************/
	void GrayDrawHistogram();
	void ColorDrawHistogram();
	void GrayHistogramEqualization();
	void ColorHistogramEqualization();

public:
	BITMAPFILEHEADER bf;			//�ļ�ͷ
	BITMAPINFOHEADER bi;			//��Ϣͷ
	RGBQUAD *rgbPalette;			//��ɫ��
	int sizeOfHang;					//һ����ռ�ֽ���
	void** img;						//���ͼƬ��Ϣ
};