#pragma once
#define MAX_PWD 50
#define MAX_USER 50
char *login(int choice); /*登录系统，登录失败返回-1，成功返回学生序号（若为老师返回非负值）*/
void welcome(int *ptchoice); /* 输入选择，在函数中改变选择 */
void passwd(char *pwd); /*给密码加星号*/
void teacher(); /*教师操作界面*/
void student(char *id); /*学生操作界面，可以查询自己的成绩和修改密码*/
