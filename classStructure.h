#pragma once
#define M 10 /*最大课程数量*/
#define N 50 /*学号、密码最大长度, 学生数量*/
typedef struct {
	char id[N]; /* 学生学号 、登录账号 */
	char pwd[N]; /* 学生密码 */
	float score[M]; /* 各科分数 */
	float sum; /* 总分 */
}Student;

typedef struct {
	int course; /*课程数量*/
	int stu; /*学生数量*/
}Num;