# C-Project

成绩管理系统v1.0（2017-12-13）:
实现4个基本功能，主函数直接调用，无菜单；可输出到txt文件；可从txt文件读入
On master
by 李姜辛

成绩管理系统v1.1（2017-12-21）:
加入菜单
On master
by 闫熠光

成绩管理系统v1.1.5（2017-12-21）:
初级登录界面
On dev
by 李姜辛

成绩管理系统v1.5.0(2017-12-23):
成熟登录认证系统，学生修改密码功能，修复紧急bug
On dev
by 李姜辛

成绩管理系统v2.0.0(2017-12-26):
密码加星号，教师修改密码功能，修复一系列bug
On master
by 李姜辛、闫熠光

成绩管理系统v2.0.5(2017-12-26):
将函数定义、宏定义、结构体定义写入头文件；将登录界面分为两级，可从后一级回到前一级
On master
by 李姜辛

systemB.c: 管理系统主文件，调用函数

classStructure.h: 结构体定义（Student类型，Num类型)

coreFunction.h：核心功能函数(score, report, sort, search)

loginSystem.h: 登录(1,2级)界面、认证系统，老师学生操作界面函数