#include "io.h"
#include "file.h"
#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>

using namespace std;

list<ft> table;

// 将从内存中读取的二进制数据转化为十进制
int get_decimal(char *mem) {
	int ans = 0;
	ans += ((unsigned char)mem[0]) << 24;
	ans += ((unsigned char)mem[1]) << 16;
	ans += ((unsigned char)mem[2]) << 8;
	ans += ((unsigned char)mem[3]);
	return ans;
}

// 将输入的十进制数据转为二进制数据
void get_binary(int num, char *mem) {
	mem[0] = (num & 0xff000000) >> 24;
	mem[1] = (num & 0x00ff0000) >> 16;
	mem[2] = (num & 0x0000ff00) >> 8;
	mem[3] = (num & 0x000000ff);
}

// 获取当前空闲的磁盘块
int get_block() {
	char mem[B];
	// 读取位示图的信息
	read_block(0, mem);
	for (int i = 8; i < B; i++) {
		for (int j = 0; j < 8; j++) {
			if ((((int)mem[i])&(1 << j)) == 0) {
				mem[i] = ((unsigned char)mem[i]) | (1 << j);
				write_block(0, mem);
				return 8 * i + j;
			}
		}
	}
	return -1;
}

// 清空指定磁盘块
void destory_block(int index) {
	char mem[B];
	int m = index / 8;
	int n = index % 8;
	// 更新位示图的信息
	read_block(0, mem);
	mem[m] = ((unsigned char)mem[m] & ~(1 << n));
	write_block(0, mem);
	// 将磁盘块的信息清除
	read_block(index, mem);
	memset(mem, 0, sizeof(mem));
	write_block(index, mem);
}

// 将文件添加至目录中
void add_dir(char *filename, int index) {
	char mem[B];
	fd file;
	bool flag = true;
	int j = 0;
	// 读取文件目录保留符
	read_block(1, mem);
	// 获取目录文件长度和文件磁盘块
	file.len = get_decimal(mem);
	for (int i = 0; i < 7; i++)
		file.block_index[i] = get_decimal(mem + (i + 1) * 4);
	// 寻找未使用的目录序号
	while (flag && file.block_index[j] != -1) {
		read_block(file.block_index[j], mem);
		for (int i = 0; i < B; i = i + 32) {
			int len = get_decimal(mem + i);
			if (len == -1) {
				strcpy_s(mem + i, filename);
				get_binary(index, mem + i + 28);
				write_block(file.block_index[j], mem);
				flag = false;
			}
		}
		j++;
	}
	// 已有目录磁盘块无剩余空间则需申请新的磁盘空间
	if (flag) {
		file.block_index[j] = get_block();
		read_block(file.block_index[file.len], mem);
		strcpy_s(mem, strlen(filename) + 1, filename);
		get_binary(index, mem + 28);
		write_block(file.block_index[file.len], mem);
	}
	// 将目录文件的修改写回磁盘
	file.len++;
	read_block(1, mem);
	get_binary(file.len, mem);
	for (int i = 0; i < 7; i++)
		get_binary(file.block_index[i], mem + (i + 1) * 4);
	write_block(1, mem);
}

// 删除目录中的指定项
int remove_dir(char* filename) {
	char mem[B];
	bool flag = false;
	int index = -1;
	// 获取文件目录文件描述符
	read_block(1, mem);
	fd file;
	file.len = get_decimal(mem);
	for (int i = 0; i < 7; i++)
		file.block_index[i] = get_decimal(mem + (i + 1) * 4);
	// 根据文件名寻找目录项
	for (int i = 0; i < 7 && file.block_index[i] != -1; i++) {
		read_block(file.block_index[i], mem);
		for (int j = 0; j < B; j = j + 32) {
			if (strcmp(filename, mem + j)!=0) {
				// 获取文件的文件描述符编号
				index = get_decimal(mem + j + 28);
				// 清空该目录项
				memset(mem + j, -1, 32);
				write_block(file.block_index[i], mem);
				flag = true;
				break;
			}
		}
		if (flag)
			break;
	}
	return index;
}

// 根据文件名在目录中查找文件序号
int find_file(char* filename) {
	char mem[B];
	int index = -1;
	// 获取文件目录文件描述符
	read_block(1, mem);
	fd file;
	file.len = get_decimal(mem);
	for (int i = 0; i < 7; i++)
		file.block_index[i] = get_decimal(mem + (i + 1) * 4);
	// 根据文件名寻找目录项
	for (int i = 0; i < 7 && file.block_index[i] != -1; i++) {
		read_block(file.block_index[i], mem);
		for (int j = 0; j < B; j = j + 32) {
			if (strcmp(filename, mem + j) != 0)
				// 获取文件的文件描述符编号
				index = get_decimal(mem + j + 28);
		}
	}
	return index;
}

// 创建文件
int create(char* filename) {
	char mem[B];
	int index = -1;
	// 寻找空闲文件描述符
	for (int i = 1; i < 64; i++) {
		// 读取对应磁盘块的信息
		read_block(i, mem);
		for (int j = 0; j < B; j = j + 32) {
			int file_len = get_decimal(mem + j);
			if (file_len == -1) {
				index = (i - 1) * 16 + j / 32;
				add_dir(filename, index);
				fd file;
				// 初始化文件信息
				file.len = 0;
				file.block_index[0] = get_block();
				get_binary(file.len, mem + j);
				for (int k = 0; k < 7; k++)
					get_binary(file.block_index[k], mem + j + 4 * (k + 1));
				write_block(i, mem);
				break;
			}
		}
		// 文件创建之后直接退出
		if (index != -1)
			break;
	}
	return index;
}



// 删除指定文件
int destory(char* filename) {
	char mem[B];
	int index = remove_dir(filename);
	int m = index / 16 + 1;//获取文件标识符对应块号
	int n = (index % 16) * 32;//获取文件标识符对应块中偏移地址
	if (index != -1) {
		read_block(m, mem);
		// 将该文件对应的每一块磁盘清空
		for (int i = 0; get_decimal(mem + n + (i + 1) * 4) != -1; i++)
			destory_block(get_decimal(mem + n + (i + 1) * 4));
		memset(mem + n, -1, 32);
		write_block(m, mem);
	}
	return index;
}

// 打开文件
int open(char* filename) {
	char mem[B];
	int index = -1;
	ft item;
	// 找到该文件描述符序号
	index = find_file(filename);
	int m = index / 16 + 1;
	int n = (index % 16) * 32;
	read_block(m, mem);
	// 将其信息填入打开文件表中
	item.index = index;
	item.p = 0;
	read_block(get_decimal(mem + n + 4), item.buf);
	table.push_back(item);
	return index;
}

// 关闭文件
int close(char* filename) {
	char mem[B];
	int index = -1;
	fd file;
	index = find_file(filename);
	int m = index / 16 + 1;
	int n = (index % 16) * 32;
	read_block(m, mem);
	file.len = get_decimal(mem + n);
	for (int i = 0; i < 7; i++)
		file.block_index[i] = get_decimal(mem + n + (i + 1) * 4);
	for (list<ft>::iterator it = table.begin(); it != table.end(); it++) {
		if (it->index == index) {
			// 将缓冲区数据写回
			write_block(file.block_index[it->p / B], it->buf);
			// 将该文件从打开文件表中删除
			table.erase(it);
		}
	}
	return index;
}

// 读文件
int read(char* filename, char *mem_area, int count) {
	char mem[B];
	int index = -1;
	index = find_file(filename);
	int m = index / 16 + 1;
	int n = (index % 16) * 32;
	int offset = 0;
	fd file;
	read_block(m, mem);
	// 获取对应文件描述符
	file.len = get_decimal(mem + n);
	for (int i = 0; i < 7; i++)
		file.block_index[i] = get_decimal(mem + n + (i + 1) * 4);
	// 在打开文件表中查找该文件
	for (list<ft>::iterator it = table.begin(); it != table.end(); it++) {
		if (it->index == index) {
			int p = it->p / B;
			int q = it->p%B;
			while (count) {
				int num = min(B - q, count);
				// 在一个磁盘块内读取
				for (int i = 0; i < num; i++) {
					*(mem_area + offset) = *(it->buf + q + i);
					offset++;
				}
				// 改变偏移指针的大小
				it->p = it->p + q;
				// 读取新磁盘块至缓冲区
				if (it->p == B) {            
					q = 0;
					p++;
					if (p > 7)
						return -2;
					if (file.block_index[p] == -1)
						return -2;
					read_block(file.block_index[p], it->buf);
				}
				count = count - num;
			}
		}
	}
	return index;
}

// 写文件
int write(char* filename, char *mem_area, int count) {
	char mem[B];
	int index = -1;
	index = find_file(filename);
	int m = index / 16 + 1;
	int n = (index % 16) * 32;
	int offset = 0;
	fd file;
	read_block(m, mem);
	// 获取对应文件描述符
	file.len = get_decimal(mem + n) + count;
	for (int i = 0; i < 7; i++)
		file.block_index[i] = get_decimal(mem + n + (i + 1) * 4);
	for (list<ft>::iterator it = table.begin(); it != table.end(); it++) {
		if (it->index == index) {
			int p = it->p / B;
			int q = it->p%B;
			while (count) {
				int num = min(B - q, count);
				// 在一个磁盘块内读取
				for (int i = 0; i < num; i++) {
					*(it->buf + q + i) = *(mem_area + offset);
					offset++;
				}
				// 改变偏移指针的大小
				it->p = it->p + q;
				// 读取新磁盘块至缓冲区
				if (it->p == B) {
					write_block(file.block_index[p], it->buf);
					q = 0;
					p++;
					if (p > 7)
						return -2;
					if (file.block_index[p] == -1)
						file.block_index[p] = get_block();
					read_block(file.block_index[p], it->buf);
				}
				count = count - num;
			}
		}
	}
	// 更新目录文件标识符
	get_binary(file.len, mem + n);
	for (int i = 0; i < 7; i++)
		get_binary(file.block_index[i], mem + n + (i + 1) * 4);
	write_block(m, mem);
	return index;
}

// 移动文件读写指针
int lseek(char* filename, int pos) {
	int index = find_file(filename);
	for (list<ft>::iterator it = table.begin(); it != table.end(); it++) {
		if (it->index == index) {
			it->p = pos;
			return pos;
		}
	}
	return -1;
}

// 打印目录中的所有文件名及长度
void directory() {
	char mem[B], file_mem[B];
	int index = -1;
	// 获取文件目录文件描述符
	read_block(1, mem);
	fd file;
	file.len = get_decimal(mem);
	for (int i = 0; i < 7; i++)
		file.block_index[i] = get_decimal(mem + (i + 1) * 4);
	printf("%28s\t\t%s\n", "文件名", "文件长度");
	// 根据文件名寻找目录项
	for (int i = 0; i < 7 && file.block_index[i] != -1; i++) {
		read_block(file.block_index[i], mem);
		for (int j = 0; j < B; j = j + 32) {
			int index = get_decimal(mem + j + 28);
			if (index != -1) {
				char *filename;
				strcpy(filename, mem + j);
				int p = index / 16 + 1;
				int q = (index % 16) * 32;
				read_block(p, file_mem);
				int len = get_decimal(file_mem + q);
				printf("%28s\t\t%d\n", filename, len);
			}
		}
	}
}

// 打印正在运行的文件信息
void directory_open() {
	printf("%10s\t\t%10s\n", "标识符序号", "文件指针");
	for (list<ft>::iterator it = table.begin(); it != table.end(); it++) {
		printf("%10d\t\t%10d\n", it->index, it->p);
	}
}