// 页面置换算法.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <list>
#include <time.h>

using namespace std;

// 虚拟内存的尺寸，页面号不大于其
int N=1000;
// 页面调用序列的长度
int len_sequence=100;
// 装载页面块的大小
int len_memory = 8;
// 模拟的物理内存大小
int RealMemorySize;
// 工作集中包含的页数
int e=15;
// 工作集移动率
int m=100;
// 工作集的起始位置
int p=10;
// p赋新值的阈值，随机数大于其则赋新值
double t=0.5;

// 当前访问页面项结构体
typedef struct Page_table_item {
	// 该项对应的页号
	int id;
	// 该项对应的存在位
	int exist;
	// 该项对应的修改位
	int modify;
}Item;

// 定义PBA算法中的空闲队列
typedef list<Item> Free_list;

// 随机页面访问序列生成函数
int *RandomSequenceGenerator() {
	// 按照顺序输入各初始化参数
	cout << "Please input the necessary parameter!" << endl;
	cout << "Virtual Memory Size = ";
	cin >> N;
	cout << "Pages Working Cluster Contained = ";
	cin >> e;
	cout << "Move Ratio = ";
	cin >> m;
	cout << "Random Coefficient = [0,1]";
	cin >> t;
	cout << "Initial Point = ";
	cin >> p;
	cout << "Sequence Length:";
	cin >> len_sequence;
	// 为初始化序列动态分配内存空间
	int *sequence;
	sequence = (int*)malloc(len_sequence * sizeof(int));
	// 按照给出算法赋予序列值
	for (int i = 0; i < len_sequence / m; i++) {
		for (int j = 0; j < m; j++) {
			// 在p至p+e中生成随机序列值
			sequence[i*m + j] = ((rand() % e) + p) % N;
		}
		// 每生成m个序列更新p的值
		double r = (rand() % 10) / 10.;
		if (r < t) p = rand() % N;
		else p = (p + 1) % N;
	}
	// 返回生成的页面序列
	return sequence;
}

// 最佳置换算法函数
double OPT(int sequence[],int len_sequence,int len_memory) {
	// 当前访问页面块中存在的项数
	int full_num = 0;
	// 页面置换数
	int replace_num = 0;
	// 已有页面项中是否含需要访问页面的标识
	bool flag = true;
	// 根据内存块的大小为页表项动态申请空间
	Item *item = (Item*)malloc(len_memory * sizeof(Item));
	for (int i = 0; i < len_sequence; i++) {
		// 先在调用页面集进行搜索，如果对应页号存在则不进行替换
		flag = false;
		for (int j = 0; j < len_memory; j++) {
			if (sequence[i] == item[j].id)
				flag = true;
		}
		if(!flag){
			// 如若不存在，先判断当前调用页面是否还存在剩余空间，若存在插入其后
			if (full_num < len_memory) {
				item[full_num++].id = sequence[i];
			}
			else {
				// 如若已满，则找出最晚将被调用的页号，将其替换为需要调用的页号
				int max_replace_value = -1;
				int max_replace_index = 0;
				for (int j = 0; j < len_memory; j++) {
					item[j].exist = len_sequence;
					for (int k = i+1; k < len_sequence; k++) {
						if (item[j].id == sequence[k]) {
							item[j].exist = k;
							break;
						}
					}
					if (item[j].exist > max_replace_value) {
						max_replace_value = item[j].exist;
						max_replace_index = j;
					}
				}
				// 更新对应块的页号并记录缺页数
				item[max_replace_index].id = sequence[i];
				replace_num++;
			}
		}
		/*cout << "当前需要调用的页面为：";
		cout << sequence[i] << endl;
		cout << "当前工作集中的页面号为（置换后）：";
		for (int j = 0; j < full_num; j++)
			cout << item[j].id << ' ';
		cout << endl << endl;*/
	}
	cout << replace_num << ' ' << len_sequence << endl;
	// 返回缺页率
	return replace_num * 1. / len_sequence;
}

// 先进先出置换算法函数
double FIFO(int sequence[], int len_sequence, int len_memory) {
	// 当前访问页面块中存在的项数
	int full_num = 0;
	// 页面置换数
	int replace_num = 0;
	// 已有页面项中是否含需要访问页面的标识
	bool flag = true;
	// 根据内存块的大小为页表项动态申请空间
	Item *item = (Item*)malloc(len_memory * sizeof(Item));
	// 先在调用页面集进行搜索，如果对应页号存在则不进行替换
	for (int i = 0; i < len_sequence; i++) {
		flag = false;
		// 如若不存在，先判断当前调用页面是否还存在剩余空间，若存在插入其后
		for (int j = 0; j < len_memory; j++) {
			if (sequence[i] == item[j].id)
				flag = true;
		}
		// 如若已满，则找出最早被调用的页号，将其替换为需要调用的页号
		if (!flag) {
			if (full_num < len_memory) {
				item[full_num].id = sequence[i];
				item[full_num++].exist = i;
			}
			else {
				int min_replace_value = 99999999;
				int min_replace_index = 99999999;
				for (int j = 0; j < len_memory; j++) {
					if (item[j].exist < min_replace_value) {
						min_replace_value = item[j].exist;
						min_replace_index = j;
					}
				}
				// 更新对应块的页号并记录缺页数和被调用时刻
				item[min_replace_index].id = sequence[i];
				item[min_replace_index].exist = i;
				replace_num++;
			}
		}
		/*cout << "当前需要调用的页面为：";
		cout << sequence[i] << endl;
		cout << "当前工作集中的页面号为（置换后）：";
		for (int j = 0; j < full_num; j++)
			cout << item[j].id << ' ';
		cout << endl << endl;*/
	}
	// 返回缺页率
	return replace_num * 1. / len_sequence;
}

// 最近最久未使用置换算法函数
double LRU(int sequence[], int len_sequence, int len_memory) {
	// 当前访问页面块中存在的项数
	int full_num = 0;
	// 页面置换数
	int replace_num = 0;
	// 已有页面项中是否含需要访问页面的标识
	bool flag = true;
	// 根据内存块的大小为页表项动态申请空间
	Item *item = (Item*)malloc(len_memory * sizeof(Item));
	// 先在调用页面集进行搜索，如果对应页号存在则不进行替换
	for (int i = 0; i < len_sequence; i++) {
		flag = false;
		for (int j = 0; j < len_memory; j++) {
			if (sequence[i] == item[j].id) {
				flag = true;
				item[j].exist = i;
			}
		}
		// 如若已满，则找出最近最久被调用的页号，将其替换为需要调用的页号
		if (!flag) {
			if (full_num < len_memory) {
				item[full_num].id = sequence[i];
				item[full_num++].exist = i;
			}
			else {
				int min_replace_value = 99999999;
				int min_replace_index = 99999999;
				for (int j = 0; j < len_memory; j++) {
					if (item[j].exist < min_replace_value) {
						min_replace_value = item[j].exist;
						min_replace_index = j;
					}
				}
				item[min_replace_index].id = sequence[i];
				item[min_replace_index].exist = i;
				replace_num++;
			}
		}
		/*cout << "当前需要调用的页面为：";
		cout << sequence[i] << endl;
		cout << "当前工作集中的页面号为（置换后）：";
		for (int j = 0; j < full_num; j++)
			cout << item[j].id << ' ';
		cout << endl << endl;*/
	}
	// 返回缺页率
	return replace_num * 1. / len_sequence;
}

// 改良后的Clock算法函数
double Clock(int sequence[], int len_sequence, int len_memory) {
	// 当前访问页面块中存在的项数
	int full_num = 0;
	// 页面置换数
	int replace_num = 0;
	// 扫描指针当前位置
	int point = 0;
	// 已有页面项中是否含需要访问页面的标识
	bool flag = true;
	// 根据内存块的大小为页表项动态申请空间
	Item *item = (Item*)malloc(len_memory * sizeof(Item));
	// 先在调用页面集进行搜索，如果对应页号存在则不进行替换
	for (int i = 0; i < len_sequence; i++) {
		flag = false;
		for (int j = 0; j < len_memory; j++) {
			if (sequence[i] == item[j].id) {
				flag = true;
				item[j].exist = 1;
				// 按照一定概率将该项修改位置为1
				double p_modify = (rand() % 10) / 10.;
				if (p_modify > 0.5)
					item[j].modify = 1;
				else
					item[j].modify = 0;
				break;
			}
		}
		// 如若调用页面集中不存在，则开始进行扫描
		if (!flag) {
			if (full_num < len_memory) {
				item[full_num].id = sequence[i];
				item[full_num].exist = 1;
				double p_modify = (rand() % 10) / 10.;
				if (p_modify > 0.5)
					item[full_num++].modify = 1;
				else
					item[full_num++].modify = 0;
			}
			else {
				while (!flag) {
					// 第一轮扫描，没有符合条件相关的项则进入第二轮扫描
					for (int j = 0; j < len_memory; j++) {
						if (item[point].exist == 0 && item[point].modify == 0) {
							item[point].id = sequence[i];
							item[point].exist = 1;
							double p_modify = (rand() % 10) / 10.;
							if (p_modify > 0.5)
								item[point].modify = 1;
							else
								item[point].modify = 0;
							flag = true;
							replace_num++;
							break;
						}
						point = (point + 1) % len_memory;
					}
					// 第二轮扫描，同时对每一项的访问项进行置0
					if (!flag) {
						for (int j = 0; j < len_memory; j++) {
							if (item[point].exist == 0 && item[point].modify == 1) {
								item[point].id = sequence[i];
								item[point].exist = 1;
								double p_modify = (rand() % 10) / 10.;
								if (p_modify > 0.5)
									item[point].modify = 1;
								else
									item[point].modify = 0;
								flag = true;
								replace_num++;
								break;
							}
							item[point].exist = 0;
							point = (point + 1) % len_memory;
						}
					}
				}
			}
		}
		/*cout << "当前需要调用的页面为：";
		cout << sequence[i] << endl;
		cout << "当前工作集中的页面号为（置换后,括号内数值分别为访问位和修改位数值）：";
		for (int j = 0; j < full_num; j++)
			printf("%d(%d,%d)  ", item[j].id, item[j].exist, item[j].modify);
		cout << endl << endl;*/
	}
	// 返回缺页率
	return replace_num * 1. / len_sequence;
}

// 页面缓冲算法函数
double PBA(int sequence[], int len_sequence, int len_memory) {
	// 当前访问页面块中存在的项数
	int full_num = 0;
	// 页面置换数
	int replace_num = 0;
	// 扫描指针当前位置
	int point = 0;
	// 已有页面项中是否含需要访问页面的标识
	bool flag = true;
	// 根据内存块的大小为页表项动态申请空间
	Item *item = (Item*)malloc(len_memory * sizeof(Item));
	// 初始化空闲链表的长度
	int len_list = 2;
	// 初始化空闲链表
	Item temp;
	temp.id = -1;
	temp.exist = -1;
	temp.modify = -1;
	Free_list free_list;
	for (int i = 0; i < len_list; i++) {
		free_list.push_back(temp);
	}
	// 先在调用页面集进行搜索，如果对应页号存在则不进行替换
	for (int i = 0; i < len_sequence; i++) {
		flag = false;
		for (int j = 0; j < len_memory; j++) {
			if (sequence[i] == item[j].id)
				flag = true;
		}
		// 采用FIFO算法对页面进行置换，同时启用页面缓冲算法
		if (!flag) {
			if (full_num < len_memory) {
				item[full_num].id = sequence[i];
				item[full_num++].exist = i;
			}
			else {
				int min_replace_value = 99999999;
				int min_replace_index = 99999999;
				for (int j = 0; j < len_memory; j++) {
					if (item[j].exist < min_replace_value) {
						min_replace_value = item[j].exist;
						min_replace_index = j;
					}
				}
				// 遍历空闲队列，如果存在调用页号则将其从该队列中删除，插入被替换的页号置队尾，否则删除队头页号
				for (Free_list::iterator iter = free_list.begin(); iter != free_list.end(); iter++) {
					if (iter->id == sequence[i]) {
						iter = free_list.erase(iter);
						flag = true;
						break;
					}
				}
				if (!flag)
					free_list.pop_front();
				temp.id = item[min_replace_index].id;
				free_list.push_back(temp);
				item[min_replace_index].id = sequence[i];
				item[min_replace_index].exist = i;
				replace_num++;
			}
		}
		cout << "当前需要调用的页面为：";
		cout << sequence[i] << endl;
		cout << "当前工作集中的页面号为（置换后,括号内数值为最早进入时间）：";
		for (int j = 0; j < full_num; j++)
			printf("%d(%d) ", item[j].id, item[j].exist);
		// cout << item[j].id << ' ';
		cout << endl;
		cout << "空闲队列情况如下：" << endl;
		for (Free_list::iterator iter = free_list.begin(); iter != free_list.end(); iter++) {
			cout << iter->id << " ";
		}
		cout << endl;
	}
	cout << replace_num << ' ' << len_sequence << endl;
	// 返回缺页率
	return replace_num * 1. / len_sequence;
}

// 整体测试函数
void Evaluate() {
	// 得到随机化页号调用序列
	int *sequence = RandomSequenceGenerator();
	//定义算法运行的开始时间和结束时间
	clock_t t_start, t_end;

	// 依次打印各算法对同一序列的缺页率和运行时间，评价其性能
	t_start = clock();
	cout << "最佳置换算法的缺页率为：" << OPT(sequence, len_sequence, len_memory) << endl;
	t_end = clock();
	cout << "其运行时钟数为：" << (t_end - t_start) << endl;
	t_start = clock();
	cout << "先进先出置换算法的缺页率为：" << FIFO(sequence, len_sequence, len_memory) << endl;
	t_end = clock();
	cout << "其运行时钟数为：" << (t_end - t_start) << endl;
	t_start = clock();
	cout << "最近最久未使用置换算法的缺页率为：" << LRU(sequence, len_sequence, len_memory) << endl;
	t_end = clock();
	cout << "其运行时钟数为：" << (t_end - t_start) << endl;
	t_start = clock();
	cout << "改良Clock算法的缺页率为：" << Clock(sequence, len_sequence, len_memory) << endl;
	t_end = clock();
	cout << "其运行时钟数为：" << (t_end - t_start) << endl;
}

int main()
{
	/*int *a = RandomSequenceGenerator();
	for (int i = 0; i < len_sequence; i++)
		cout << a[i] << endl;*/
	//int a[20] = { 7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1 };
	Evaluate();
	//OPT(a, 20, 3);
	//FIFO(a, 20, 3);
	//LRU(a, 20, 3);
	//Clock(a, 20, 3);
	//PBA(a, 20, 3);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
