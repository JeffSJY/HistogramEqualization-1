#include "pch.h"
#include "Bitmap.h"
#include <iostream>

int main()
{
	for (int i = 1; i <= 10; ++i) {
		char file[100];
		sprintf_s(file, "%d.bmp", i);
		Bitmap bmp;
		if (!bmp.ReadBmp(file)) {
			cout << "打开文件" << file << "失败！" << endl;
			continue;
		}
		cout << "打开文件" << file << "成功！" << endl;

		if (bmp.bi.biBitCount < 24) {
			bmp.GrayDrawHistogram();
			bmp.GrayHistogramEqualization();
			bmp.GrayDrawHistogram();
		}
		else {
			bmp.ColorDrawHistogram();
			bmp.ColorHistogramEqualization();
			bmp.ColorDrawHistogram();
		}

		sprintf_s(file, "%d-out.bmp", i);
		bmp.SaveBmp(file);
	}

	return 0;
}