#include <string.h>
#include <iostream>
#include <stdio.h>
#include "io.h"

using namespace std;

char ldisk[L][B];

int main(){
    // 先将所有字符初始化为0
    FILE *fp = fopen("ldisk.txt","w+");
    memset(ldisk,0,sizeof(ldisk));
    // 前64个盘块为文件描述符，已经被系统占用，位图置为1
    for(int i = 0 ; i < 8 ; i++){
        ldisk[0][i] = 0xff;
    }
    // 目录文件描述符已被占用，其余均初始化为-1
    for(int i = 1 ; i < L ; i++){
        for(int j = 0 ; j < B ; j++){
            if(i==1&&j<4)continue;
            ldisk[i][j] = 0xff;
        }
    }
    // 将初始化的磁盘信息写入文件中
    for(int i = 0 ; i < L ; i++){
        fwrite(ldisk[i],sizeof(char),B,fp);
    }
    fclose(fp);
    return 0;
}