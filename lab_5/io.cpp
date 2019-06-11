#include <stdio.h>
#include <iostream>
#include <string.h>
#include "io.h"
using namespace std;

char ldisk[L][B];//使用字符数组模拟磁盘块

//读取磁盘块
void read_block(int i, char *p) {
	memcpy(p, ldisk[i], B);
}
//写入磁盘块
void write_block(int i, char *p) {
	memcpy(ldisk[i], p, B);
}
//从文件中恢复磁盘状态
bool read_file() {
	FILE *fp;
	if ((fp = fopen( "ldisk.txt", "r")) == NULL) {
		cout << "打开文件时出错！" << endl;
		return false;
	}
	for (int i = 0; i < L; i++)
		fread(ldisk[i], sizeof(char), B, fp);
	fclose(fp);
	return true;
}
//将磁盘状态保存至文件中
bool write_file() {
	FILE *fp;
	if ((fp = fopen("ldsik.txt", "w+")) == NULL) {
		cout << "打开文件时出错！" << endl;
		return false;
	}
	for (int i = 0; i < L; i++)
		fwrite(ldisk[i], sizeof(char), B, fp);
	fclose(fp);
	return true;
}
