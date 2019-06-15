#include "pch.h"
#include "Bitmap.h"

/*********************************************************
* ���캯��
*********************************************************/

Bitmap::Bitmap(Bitmap & bmp) {
	bf = bmp.bf;
	bi = bmp.bi;
	sizeOfHang = bmp.sizeOfHang;

	rgbPalette = new RGBQUAD[1 << bi.biBitCount];
	memcpy(rgbPalette, bmp.rgbPalette, sizeof(RGBQUAD)*(1 << bi.biBitCount));

	img = new void*[bi.biHeight];							//ͼ����������
	for (int i = 0; i < bi.biHeight; i++) {
		img[i] = (void *) new unsigned char[sizeOfHang];	//Ϊÿ�з���ռ�
		memcpy(img[i], bmp.img[i], sizeOfHang);
	}
}

/*********************************************************
* �ļ�����
*********************************************************/
bool Bitmap::ReadBmp(const char *FileName) {
	//���ļ�
	FILE *fpIn;
	if (fopen_s(&fpIn, FileName, "rb")) //fopen_s���ļ��ɹ�����0��ʧ�ܷ��ش������
		return false; //���ļ�ʧ��

	//�����ļ�ͷ����Ϣͷ
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, fpIn);
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, fpIn);

	//������ܴ��ڵĵ�ɫ��
	if (bi.biBitCount < 16) {
		int sizeOfPalette = 1 << bi.biBitCount;						//��ɫ���С
		rgbPalette = new RGBQUAD[sizeOfPalette];					//��������ռ�
		fread(rgbPalette, sizeof(RGBQUAD), sizeOfPalette, fpIn);	//�����ɫ��
	}

	//����һ����ռ���ֽ���
	sizeOfHang = (bi.biBitCount * bi.biWidth + 31) / 32 * 4;

	cout << bi.biBitCount<<" "<<bi.biWidth << " " << bi.biHeight << endl;
	bi.biWidth = abs(bi.biWidth);
	bi.biHeight = abs(bi.biHeight);

	//����ÿһ�е�����ֵ
	img = new void*[bi.biHeight];									//ͼ����������
	for (int i = 0; i < bi.biHeight; i++) {
		img[i] = (void *) new BYTE[sizeOfHang];						//Ϊÿ�з���ռ�
		fread(img[i], sizeOfHang, 1, fpIn);
	}

	//�ر��ļ�
	if (fpIn) fclose(fpIn);

	//�����ļ��ɹ�
	return true;
}

bool Bitmap::SaveBmp(const char *FileName) {
	//���ļ�
	FILE *fpOut;
	if (fopen_s(&fpOut, FileName, "wb")) return false;

	//д���ļ�ͷ����Ϣͷ
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fpOut);
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fpOut);

	//д����ܴ��ڵĵ�ɫ��
	if (bi.biBitCount < 16) {
		int sizeOfPalette = 1 << bi.biBitCount;						//��ɫ���С
		fwrite(rgbPalette, sizeof(RGBQUAD), sizeOfPalette, fpOut);
	}

	//д������ֵ��Ϣ
	for (int i = 0; i < bi.biHeight; i++)
		fwrite(img[i], sizeOfHang, 1, fpOut);

	//�ر��ļ�
	if (fpOut) fclose(fpOut);

	//д���ļ��ɹ�
	return true;
}

/*********************************************************
* ֱ��ͼ����
*********************************************************/

void Bitmap::GrayDrawHistogram() {
	const int numberOfCol = 55;
	const int numberOfRow = 40;

	int cnt1[256]; //����ÿ������ֵ�ĳ��ִ���
	for (int i = 0; i < 256; ++i) cnt1[i] = 0;
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			int pix = (int)(((BYTE*)img[i])[j]);
			cnt1[pix]++;
		}

	int cnt2[numberOfCol]; //����ÿ�ж�Ӧ������ֵ�ĳ��ִ���
	int size = (256 + numberOfCol - 1) / numberOfCol;
	for (int i = 0; i < numberOfCol; ++i) cnt2[i] = 0;
	for (int i = 0; i < 256; ++i) cnt2[(i + 1) / size] += cnt1[i];

	int maxV = 0;
	for (int i = 0; i < numberOfCol; ++i) maxV = max(maxV, cnt2[i]);

	for (int i = numberOfRow; i >= 0; --i) {
		for (int j = 0; j < numberOfCol; ++j) {
			//��maxV���ֵ��������������а������ֲ�
			if (cnt2[j] * numberOfRow >= maxV * i) cout << "��";
			else cout << "  ";
		}
		cout << endl;
	}
}

void Bitmap::ColorDrawHistogram() {
	const int numberOfCol = 110;
	const int numberOfRow = 40;

	int cnt1[256]; //����ÿ������ֵ�ĳ��ִ���
	for (int i = 0; i < 256; ++i) cnt1[i] = 0;
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			int b = (int)(((BYTE*)img[i])[j * 3]);
			int g = (int)(((BYTE*)img[i])[j * 3 + 1]);
			int r = (int)(((BYTE*)img[i])[j * 3 + 2]);
			int I = (r + g + b) / 3;
			cnt1[I]++;
		}

	int cnt2[numberOfCol]; //����ÿ�ж�Ӧ������ֵ�ĳ��ִ���
	int size = (256 + numberOfCol - 1) / numberOfCol;
	for (int i = 0; i < numberOfCol; ++i) cnt2[i] = 0;
	for (int i = 0; i < 256; ++i) cnt2[(i + 1) / size] += cnt1[i];

	int maxV = 0;
	for (int i = 0; i < numberOfCol; ++i) maxV = max(maxV, cnt2[i]);

	for (int i = numberOfRow; i >= 0; --i) {
		for (int j = 0; j < numberOfCol; ++j) {
			//��maxV���ֵ��������������а������ֲ�
			if (cnt2[j] * numberOfRow >= maxV * i) cout << "��";
			else cout << "  ";
		}
		cout << endl;
	}
}

//�Ҷ�ͼֱ��ͼ����
void Bitmap::GrayHistogramEqualization() {
	const int L = 256; //����ֵ��ȡֵ��ΧΪ[0, L)
	int R[L]; //ԭͼ�и�����ֵ���ֵĴ�����ǰ׺��
	int S[L]; //ֱ��ͼ��������صĶ�Ӧֵ��ԭ����ֵΪi��ֵ��Ӧ��S[i]

	//ͳ��ÿ������ֵ���ִ���
	for (int i = 0; i < L; ++i) R[i] = 0;
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			int pix = (int)(((BYTE*)img[i])[j]);
			R[pix]++;
		}
	//ά��R����ǰ׺��
	for (int i = 1; i < L; ++i) R[i] += R[i - 1];

	//����S����
	for (int i = 0; i < L; ++i)
		S[i] = (int)(1.0 * (L - 1) * R[i] / R[L - 1]);

	//��ԭͼ����ֱ��ͼ����
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			int pix = (int)(((BYTE*)img[i])[j]);
			pix = S[pix];
			((BYTE*)img[i])[j] = (BYTE)pix;
		}
}

//��ɫͼֱ��ͼ����
void Bitmap::ColorHistogramEqualization() {
	const int L = 256; //����ֵ��ȡֵ��ΧΪ[0, L)
	int R[L]; //ԭͼ�и�����ֵ���ֵĴ�����ǰ׺��
	int S[L]; //ֱ��ͼ��������صĶ�Ӧֵ��ԭ����ֵΪi��ֵ��Ӧ��S[i]

	//ͳ��ÿ������ֵ���ִ���
	for (int i = 0; i < L; ++i) R[i] = 0;
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			//RGBͨ��תΪHSIͨ����I���������ȣ�ֵ
			int b = (int)(((BYTE*)img[i])[j * 3]);
			int g = (int)(((BYTE*)img[i])[j * 3 + 1]);
			int r = (int)(((BYTE*)img[i])[j * 3 + 2]);
			int I = (r + g + b) / 3;
			R[I]++;
		}
	//ά��R����ǰ׺��
	for (int i = 1; i < L; ++i) R[i] += R[i - 1];

	//����S����
	for (int i = 0; i < L; ++i)
		S[i] = (int)(1.0 * (L - 1) * R[i] / R[L - 1]);

	//��ԭͼ����ֱ��ͼ����
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			int b = (int)(((BYTE*)img[i])[j * 3]);
			int g = (int)(((BYTE*)img[i])[j * 3 + 1]);
			int r = (int)(((BYTE*)img[i])[j * 3 + 2]);

			double theta = (r == g && g == b) ? 0 : acos(0.5 * (r - g + r - b) / sqrt((r - g)*(r - g) + (r - b)*(g - b)));
			double I = S[(r + g + b) / 3];//���Ǿ���ֱ��ͼ�������I
			double H = b > g ? PI + PI - theta : theta;
			double S = 1 - 3.0 * min(r, min(g, b)) / (r + g + b);

			//���µ�Iֵ���صõ�ԭRGBֵ
			if (H < PI * 2 / 3) {
				b = I * (1 - S);
				r = I * (1 + S * cos(H) / cos(PI / 3 - H));
				g = 3 * I - r - b;
			}
			else if (H < PI * 4 / 3) {
				H -= PI * 2 / 3;
				r = I * (1 - S);
				g = I * (1 + S * cos(H) / cos(PI / 3 - H));
				b = 3 * I - r - g;
			}
			else {
				H -= PI * 4 / 3;
				g = I * (1 - S);
				b = I * (1 + S * cos(H) / cos(PI / 3 - H));
				r = 3 * I - g - b;
			}

			r = min(255, r); g = min(255, g); b = min(255, b);

			((BYTE*)img[i])[j * 3] = (BYTE)b;
			((BYTE*)img[i])[j * 3 + 1] = (BYTE)g;
			((BYTE*)img[i])[j * 3 + 2] = (BYTE)r;
		}
}