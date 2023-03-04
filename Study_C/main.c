//
//  main.c
//  Study_C
//
//  Created by jillian on 2023/2/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 通过指向地址改变地址中的值，但int* a, int* b只是两个指针
 */
//void change(int* a, int* b) {
////    a = b;   //解注释后结果为1,2。这里是原本指向a的指针指向了b，变量a的值由于没有指针指向就没有改变
//    int temp = *a;
//    *a = *b;   //*a=2
//    *b = temp;
//}

void add(int num1, int num2) {
    printf("num1 + num2 = %d\n", num1+num2);
}

void minus(int num1, int num2) {
    printf("num1 - num2 = %d\n", num1-num2);
}

//函数指针
void operate(void(*method)(int,int), int a, int b) {
    method(a, b);
}

void print_file(char* mesaage){
    printf("打印内容: %s", mesaage);
}

void method(void(*print_f)(char*)) {
    print_f("消息来了");
}

/* 功能：将str字符串中的oldstr字符串替换为newstr字符串
 * 参数：str：操作目标 oldstr：被替换者 newstr：替换者
 * 返回值：返回替换之后的字符串
 * */
char* str_replace(char* str, char* src, char* dest) {
    char str_new[strlen(str)+strlen(dest)];
    //初始化str_new数组全为0
    memset(str_new, 0, sizeof(str_new));

    for(int i=0; i<strlen(str); i++) {
        if(strncmp(str+i, src, strlen(src)) == 0) {  //在当前位置取和src等长字符，若相等
            strcat(str_new, dest);
            i += strlen(src);  //i后移
        }
        strncat(str_new, str+i, 1);   //不等则str_new追加str+i位置的第一个字符
    }
    return str_new;
}

/**
 计算文件大小
 */
int getFileSize(FILE *file) {
    if(!file) return 0;  //文件不存在
    //这三行是计算文件大小
    fseek(file, 0, SEEK_END);  //文件指针指向文件末尾
    int fileSize = (int)ftell(file);
    fseek(file, 0, SEEK_SET); //文件指针指回开头
    return fileSize;
}

int main(int argc, const char * argv[]) {
    char* fileName = "/Users/zouji/Downloads/IMG_20230116_162407.jpg";
    FILE *file = fopen(fileName, "rb");
    int fileSize = getFileSize(file);
//    // 定义切割数量
    int fileNum = 3;
    // 二维数组: image_0.jpg, image_1.jpg, image_2.jpg
    char** fileNames = (char**)malloc(sizeof(char*)*fileNum);
    // 拷贝
    for (int i  =0; i<fileNum; i++) {
        // 二维数组存放的每个一维数组(image文件)的空间开辟，每个100个字节
        fileNames[i] = (char*)malloc(sizeof(char)*100);
        // 每个文件的名字
        sprintf(fileNames[i], "/Users/zouji/Downloads/IMG_20230116_162407_%d.jpg", i);
        printf("%s\n", fileNames[i]);
    }

    int perFileSize = fileSize / fileNum;
    for (int i=0; i<fileNum; i++) {
        // 从源文件往切割文件写数据
        FILE* curFile = fopen(fileNames[i], "wb");
        // 计算写多少
        int start = i*perFileSize;
        int end = (i+1)*perFileSize;
        if(i == fileNum-1) {  //最后一个切割文件，直接切割到文件末尾
            end = fileSize;
        }
        for(int j=start; j<end; j++) {
            fputc(fgetc(file), curFile);   //从file中读，写入至curFile; fgetc在读file时指针会后移，第二个文件 就从后移指针处读
        }
        fclose(curFile);
//        free(fileNames[i]);
    }
    
    
    // 文件合并
    FILE* fpwrite = fopen("/Users/zouji/Downloads/IMG_merge.jpg", "wb");
    for(int i=0; i<fileNum; i++) {
        FILE* fpread = fopen(fileNames[i], "rb");
        if(fpread!=NULL && fpwrite!=NULL) {
            char* p;
            p = (char*)malloc(sizeof(char));  //读写缓冲区
            while (!feof(fpread)) {
                fread(p, sizeof(char), 1, fpread);
                fwrite(p, sizeof(char), 1, fpwrite);
            }
        }
        fclose(fpread);
    }
    
    fclose(file);
    fclose(fpwrite);
    free(fileNames);
}
