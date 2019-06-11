#pragma once

using namespace std;

#define L 4096//最大磁盘块数
#define B 512//每个磁盘块大小

void read_block(int i, char *p);//读取磁盘块
void write_block(int i, char *p);//写入磁盘块
bool read_file();//从文件中恢复磁盘状态
bool write_file();//将磁盘状态保存至文件中
