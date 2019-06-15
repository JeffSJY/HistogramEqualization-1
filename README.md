# GrayHistogramEqualization
8位bmp灰度图和24位bmp彩色图的直方图均衡



环境：VS2017 Win控制台程序

文件类型：只支持bmp，位深=8的灰度图像，以及位深=24的彩色图像

彩色直方图均衡：RGB转HSI，对I分量单独直方图均衡，再HSI转RGB

支持在控制台下绘制直方图



主要函数：

+ bool ReadBmp(const char *FileName);
+ bool SaveBmp(const char *FileName);
+ void GrayDrawHistogram();
+ void ColorDrawHistogram();
+ void GrayHistogramEqualization();
+ void ColorHistogramEqualization();