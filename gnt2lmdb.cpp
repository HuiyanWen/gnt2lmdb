#include <stdafx.h>
#include <algorithm>   //输出数组的内容、对数组进行升幂排序、反转数组内容、复制数组内容等操作
#include <fstream>  // NOLINT(readability/streams)
#include <string>
#include <utility>    //utility头文件定义了一个pair类型,pair类型用于存储一对数据
#include <vector>     //会自动扩展容量的数组
#include <iostream>
#include "boost/scoped_ptr.hpp"   //智能指针头文件
#include "gflags/gflags.h"       //Google的一个命令行参数库
#include "glog/logging.h"        //日志头文件
#include "caffe/proto/caffe.pb.h"  
#include "caffe/util/db.hpp"      //引入包装好的lmdb操作函数
#include "caffe/util/format.hpp"  
#include "caffe/util/io.hpp"      //引入opencv的图像操作函数
#include "caffe/util/rng.hpp"
#include <direct.h>

using namespace std;
using namespace caffe;  // NOLINT(build/namespaces)引入全部caffe命名空间
using std::pair;        //引入pair对命名空间
using boost::scoped_ptr;

DEFINE_bool(gray, false,
	"When this option is on, treat images as grayscale ones");
DEFINE_bool(shuffle, true,
	"Randomly shuffle the order of images and their labels");
DEFINE_int32(resize_width, 112, "Width images are resized to");
DEFINE_int32(resize_height, 112, "Height images are resized to");


int main(int argc, char** argv){
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	cout << "Loading..." << endl;
	if (argc < 4) {
		argv[1] = "./lexicon3755.txt";
		argv[2] = "./hwtst.gnt";
		argv[3] = "./3755_test_RGB3/";
	}
	//Delete the dir
	string mdb1 = "lock.mdb";
	string mdb2 = "data.mdb";
	char dirName[100];
	strcpy(dirName, argv[3]);
	if (_access(argv[3], 0) != -1)
	{
		remove((argv[3] + mdb1).c_str());
		remove((argv[3] + mdb2).c_str());
		if (_rmdir(dirName) == -1){
			cout << "Delete failed. Please delete the data dir manually." << endl;
			return 0;
		}
	}
	FILE *fpIn;
	char fname[100];
	strcpy(fname, argv[2]);
	fopen_s(&fpIn, fname, "rb");

	if (fpIn == 0)
	{
		printf("Cannot open the file %s\n", fname);
		exit(1);
	}
	int gntlen;
	unsigned char* buff;
	long long fpLen;
	_fseeki64(fpIn, 0, SEEK_END);
	fpLen = _ftelli64(fpIn);
	cout << fpLen << endl;
	_fseeki64(fpIn, 0, SEEK_SET);
	long count = 0;
	std::vector<std::pair<std::string, int> > lines;
	std::ifstream infile(argv[1]);
	std::string line;
	string *line_ch = new string[3755];
	size_t pos;
	int label;
	int line_count = 0;
	while (std::getline(infile, line)) {
		line_ch[line_count] = line;
		line_count++;
	}
	while (_ftelli64(fpIn) < fpLen){
		int temp = sizeof(unsigned long int);
		fread(&gntlen, temp, 1, fpIn);
		buff = new unsigned char[gntlen];
		_fseeki64(fpIn, -temp, SEEK_CUR);
		fread(buff, gntlen, 1, fpIn);
		int len_all, len_label;
		len_label = 2 * sizeof(char);
		len_all = sizeof(unsigned long int);
		short width, height;
		char tagcode[3];
		tagcode[2] = '\0';
		memcpy(tagcode, buff + len_all, len_label);
		int position;
		for (int l = 0; l < 3755; l++){
			position = line_ch[l].find(tagcode);
			if (position != line_ch[l].npos){
				position = l;
				break;
			}
		}
		delete buff;
		lines.push_back(std::make_pair(to_string(count) + ".png", position));
		count++;
	}
	// randomly shuffle data
	if (FLAGS_shuffle == 1){
		cout << "Shuffling data" << endl;
		shuffle(lines.begin(), lines.end());
		cout << "Shuffling finished" << endl;
	}
	cout << "A total of " << lines.size() << " images." << endl;
	//_int64 fpLen;
	_fseeki64(fpIn, 0, SEEK_END);
	fpLen = _ftelli64(fpIn);
	cout << "len:" << fpLen << endl;
	_fseeki64(fpIn, 0, SEEK_SET);
	long count_flag = 0;
	long long *start = new long long[lines.size() + 1];
	cout << "linesize:" << lines.size() << endl;
	long long sum_len = 0;
	for (int m = 0; m < (lines.size() + 1); m++) start[m] = -1;
	gntlen = 0;
	cout << "Loading start array..." << endl;
	int temp = sizeof(unsigned long int);
	while (_ftelli64(fpIn) < fpLen)
	{
		fread(&gntlen, temp, 1, fpIn);
		buff = new unsigned char[gntlen];
		_fseeki64(fpIn, -temp, SEEK_CUR);
		fread(buff, gntlen, 1, fpIn);
		start[count_flag] = sum_len;
		sum_len += gntlen;
		count_flag++;
		delete buff;
	}

	start[lines.size()] = fpLen;
	_fseeki64(fpIn, 0, SEEK_SET);
	/***************************************************shuffle***************************************************/
	cout << "Start finished" << endl;
	//cout << "Shuffling buffer..." << endl;
	cout << fpLen << endl;
	char *new_gnt = new char[fpLen];
	long long flag_len = 0;
	for (long m = 0; m < lines.size(); m++){
		string cut = lines[m].first;
		int fin = cut.find_first_of(".");
		int n = atoi(cut.substr(0, fin).c_str());
		_fseeki64(fpIn, start[n], SEEK_SET);//移动到该图片所在指针
		fread(new_gnt + flag_len, start[n + 1] - start[n], 1, fpIn);
		flag_len += start[n + 1] - start[n];
		if (flag_len < 0) {
			cout << "n:" << n << endl;
			cout << flag_len << endl;
			cout << "start[n]:" << start[n] << endl;
			cout << "start[n+1]:" << start[n + 1] << endl;
			return 0;
		}
		if (m % 10000 == 0) cout << m << endl;
	}
	/***************************************************shuffle***************************************************/
	//cout << "Shuffling buffer finished" << endl;
	count = 0;//记录第几幅图
	// Create new DB
	scoped_ptr<db::DB> db(db::GetDB("lmdb"));
	db->Open(argv[3], db::NEW);
	scoped_ptr<db::Transaction> txn(db->NewTransaction());
	long long pos_ptr = 0;//记录指针位置
	Datum datum;
	while (pos_ptr<fpLen)//遍历new_gnt
	{

		memcpy(&gntlen, new_gnt + pos_ptr, temp);
		buff = new unsigned char[gntlen];
		_fseeki64(fpIn, -temp, SEEK_CUR);
		memcpy(buff, new_gnt + pos_ptr, gntlen);
		pos_ptr += gntlen;
		/************************************grayNorm************************************/
		short height, width;
		int len_all, len_label, len_height, len_width;
		len_label = 2 * sizeof(char);
		len_all = sizeof(unsigned long int);
		len_height = sizeof(unsigned short int);
		len_width = sizeof(unsigned short int);
		char tagcode[3];
		tagcode[2] = '\0';
		memcpy(tagcode, buff + len_all, len_label);
		memcpy(&width, buff + len_label + len_all, len_height);
		memcpy(&height, buff + len_label + len_all + len_height, len_width);
		Mat img(height, width, CV_8UC1, buff + len_label + len_all + len_height + len_width);
		img = 255 - img;
		threshold(img, img, 0, 255, THRESH_TOZERO | THRESH_OTSU);
		img = 255 - img;
		float x = 0.0;
		if ((height > width) & (height < 224)){
			x = 224.0 / height;
			cv::resize(img, img, cv::Size(int(width * x), 224), 0, 0, cv::INTER_LINEAR);
			int padding_left = int((224 - int(width * x)) / 2);
			int padding_right = 224 - padding_left - int(width * x);
			copyMakeBorder(img, img, 0, 0, padding_left, padding_right, BORDER_CONSTANT, Scalar(255, 255, 255));
		}
		else{
			x = 224.0 / width;
			cv::resize(img, img, cv::Size(224, (224, int(height * x))), 0, 0, cv::INTER_LINEAR);
			int padding_top = int((224 - int(height * x)) / 2);
			int padding_bottom = 224 - padding_top - int(height * x);
			copyMakeBorder(img, img, padding_top, padding_bottom, 0, 0, BORDER_CONSTANT, Scalar(255, 255, 255));
		}

		cv::resize(img, img, cv::Size(112, 112), 0, 0, cv::INTER_LINEAR);
		bitwise_not(img, img);
		float meanValue = 0.0f;
		float stdValue = 0.0f;
		int cnt = 0;
		for (size_t i = 0; i < img.cols; i++)
		{
			for (size_t j = 0; j < img.rows; j++)
			if (img.at<uchar>(j, i) > 0)
			{
				cnt++;
				meanValue += img.at<uchar>(j, i);
			}

		}
		meanValue /= (cnt + 0.1f);
		if (meanValue < 110)
		{
			for (size_t i = 0; i < img.cols; i++)
			{
				for (size_t j = 0; j < img.rows; j++)
				if (img.at<uchar>(j, i) > 0)
				{
					stdValue += (img.at<uchar>(j, i) - meanValue)*(img.at<uchar>(j, i) - meanValue);
				}
			}
			GaussianBlur(img, img, Size(3, 3), 0, 0);
			stdValue /= (cnt + 0.1f);
			stdValue = sqrt(stdValue);
			float power = log(185.0f / (185.0f + 40.0f)) / log(meanValue / (meanValue + 2 * stdValue));// method from Prof Liu
			float alpha = 185.0f / pow(meanValue, power);// method from Prof Liu
			for (size_t i = 0; i < img.cols; i++)
			{
				for (size_t j = 0; j < img.rows; j++)
				{
					int grayValue = img.at<uchar>(j, i);
					if (grayValue > 0)
					{
						img.at<uchar>(j, i) = min(255.0, alpha*pow(grayValue, power));// method from Prof Liu
					}
				}
			}
		}
		bitwise_not(img, img);
		if (FLAGS_gray == 0)
			cvtColor(img, img, COLOR_GRAY2BGR);
		CVMatToDatum(img, &datum);
		datum.set_label(lines[count].second);
		img.release();
		// Put in db
		string out;
		CHECK(datum.SerializeToString(&out));
		string key_str = caffe::format_int(count, 8) + "_" + lines[count].first;
		txn->Put(key_str, out);
		/************************************grayNorm************************************/
		if (++count % 10000 == 0) {
			// Commit db
			txn->Commit();
			txn.reset(db->NewTransaction());
			cout << "Processed " << count << " files." << endl;
		}
		delete buff;
	}
	if (count % 10000 != 0) {
		txn->Commit();
		cout << "Processed " << count << " files.";
	}
	return 0;
}