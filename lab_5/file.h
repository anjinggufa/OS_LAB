#pragma once

using namespace std;

//定义文件描述符结构
typedef struct File_Dsecriptor {
	int len = 0;//文件的长度
	int block_index[7] = { -1,-1,-1,-1,-1,-1,-1 };//文件分配的磁盘块号
	//文件描述符的初始化函数
	/*File_Dsecriptor() {
		len = 0;//初始化文件的长度为0
		memset(block_index, -1, sizeof(int));//初始化文件分配的磁盘块号为-1
	}*/
}fd;

//定义目录结构
typedef struct Directory {
	char name[28];//文件名
	int index;//文件描述符序号
}dir;

//定义打开文件表结构
typedef struct Opening_File_Table {
	char* buf;//读写缓冲区
	int p;//读写指针
	int index;//文件描述符编号
}ft;

int get_decimal(char *mem);// 将从内存中读取的二进制数据转化为十进制
void get_binary(int num, char *mem);// 将输入的十进制数据转为二进制数据
int get_block();// 获取当前空闲的磁盘块
void destory_block(int index);// 将指定磁盘块清空
void add_dir(char *filename, int index);// 将文件添加至目录中
int remove_dir(char* filename)// 删除目录中的指定项
int find_file(char *filename);// 通过文件名在目录中寻找文件

int create(char* filename);//根据指定的文件名创建新文件，返回文件描述符序号
int destory(char* filename); //删除指定文件
int open(char* filename);//打开文件。该函数返回的索引号可用于后续的read, write, lseek,或close操作
int close(char* filename);//关闭指定文件   
int read(char* filename, char* mem_area, int count);//从指定文件顺序读入count个字节mem_area指定的内存位置。读操作从文件的读写指针指示的位置开始
int write(char* filename, char* mem_area, int count);//把mem_area指定的内存位置开始的count个字节顺序写入指定文件。写操作从文件的读写指针指示的位置开始
int lseek(char* filename, int pos);//把文件的读写指针移动到pos指定的位置
void directory();//列表显示所有文件及其长度                 
void directory_open();//列表显示所有打开文件及其长度