#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cmath>
#include <string.h>
#include "io.h"
#include "file.h"
using namespace std;
//char s[L][B];

char str[MAXN];
int main()
{
	//init();
	read_file();
	char filename[28];
	bool flag = true;
	int op;
	int length;
	char mem_area[100];
	cout << "文件管理系统" << endl;
	cout << "****************************************************" << endl;
	cout << "**\t1.\t创建文件\t**" << endl;
	cout << "**\t2.\t删除文件\t**" << endl;
	cout << "**\t3.\t打开文件\t**" << endl;
	cout << "**\t4.\t关闭文件\t**" << endl;
	cout << "**\t5.\t阅读文件\t**" << endl;
	cout << "**\t6.\t编辑文件\t**" << endl;
	cout << "**\t7.\t移动文件指针\t**" << endl;
	cout << "**\t8.\t打印文件目录\t**" << endl;
	cout << "**\t9.\t打开文件目录\t**" << endl;
	cout << "**\t0.\t退出文件系统\t**" << endl;
	cout << "****************************************************" << endl;
	while (flag) {
		cin >> op;
		switch (op)
		{
		case 0:
			flag = false;
			break;
		case 1:
			cout << "请输入创建文件的文件名：";
			cin >> filename;
			if (create(filename) < 0)
				cout << "创建文件错误！" << endl;
			else
				cout << "创建文件成功！" << endl;
			break;
		case 2:
			cout << "清输入删除文件的文件名:";
			cin >> filename;
			if (destory(filename) < 0)
				cout << "删除文件错误！" << endl;
			else
				cout << "删除文件成功！" << endl;
			break;
		case 3:
			cout << "请输入打开文件的文件名：";
			cin >> filename;
			if (open(filename) < 0)
				cout << "打开文件错误！" << endl;
			else
				cout << "打开文件成功！" << endl;
			break;
		case 4:
			cout << "请输入关闭文件的文件名：";
			cin >> filename;
			if (close(filename) < 0)
				cout << "关闭文件错误！" << endl;
			else
				cout << "关闭文件成功！" << endl;
			break;
		case 5:
			cout << "请输入阅读文件的文件名：";
			cin >> filename;
			cout << "请输入阅读的字节数：";
			cin >> length;
			if (read(filename,mem_area,length) <= 0)
				cout << "阅读文件错误！" << endl;
			else {
				cout << "文件内容如下：" << endl;
				mem_area[length] = '\0';
				cout << mem_area << endl;
			}
			break;
		case 6:
			cout << "请输入编辑文件的文件名：";
			cin >> filename;
			cout << "请输入编辑的内容：";
			cin >> mem_area;
			length = strlen(mem_area);
			if (write(filename, mem_area, length) < 0)
				cout << "编辑文件错误！" << endl;
			else
				cout << "编辑文件成功！" << endl;
			break;
		case 7:
			cout << "请输入移动文件指针的文件名：";
			cin >> filename;
			cout << "请输入移动指针的位置：";
			int pos;
			cin >> pos;
			if (lseek(filename, pos) < 0)
				cout << "移动指针错误！" << endl;
			else
				cout << "移动指针成功！" << endl;
			break;
		case 8:
			directory();
			break;
		case 9:
			directory_open();
			break;
		default:
			break;
		}
	}
	write_file();
	return 0;
}

