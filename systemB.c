/* 成绩管理系统 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 10 /*最大课程数量*/
#define N 50 /*学号、密码最大长度, 学生数量*/
#define USER "admin"
#define PWD "admin"
#define MAX_PWD 50
#define MAX_USER 50


typedef struct {
	char id[N];
	char pwd[N];
	float score[M];
	float sum;
}Student;
typedef struct {
	int course; /*课程数量*/
	int stu; /*学生数量*/
}Num;

char *login(int choice); /*登录系统，登录失败返回-1，成功返回学生序号（若为老师返回非负值）*/
Num score();
void report( Num num);
void sort( Num num);
void search( Num num);
void student_search(char *id);
void modify(char *id); /*修改密码*/
void teacher(); /*教师操作界面*/
void student(char *id); /*学生操作界面，可以查询自己的成绩和修改密码*/

Num num = { 0, 0 };

void main() {
	int choice; /*菜单选择变量*/
	char *which;
	/* 若已经创建过info.txt，则从info.txt读取学生、课程数量和学号 */
	FILE *fp;
	errno_t err;
	char filename[20] = "info.txt";
	if ((err = fopen_s(&fp, filename, "r")) == 0) {
		/*读取数量*/
		fscanf_s(fp, "%d %d", &num.stu, &num.course);
		fclose(fp);
	}
	
	printf("\t-----------------欢迎使用学生成绩管理系统！----------------------\n");
	printf("\t| 教师登录                                            \t- 请输入1 |\n");
	printf("\t| 学生登录                                            \t- 请输入2 |\n");
	printf("\t| 退出                                            \t- 请输入3 |\n");
	printf("\n输入您的选择：");
	scanf_s("%d", &choice);
	getchar();
	while (choice != 1 && choice != 2) {
		if (choice == 3) {
			printf("成功退出！\n");
			system("pause");
			return;
		}
		printf("输入有误，请重新输入:");
		scanf_s("%d", &choice);
		getchar();
	}
	/*通过login()返回值判断用户身份，若验证通过，给出操作界面*/
	if (choice == 1 && *(which=login(choice)) != '-1') {
		printf("\n教师%s, 您已成功登录！\n\n", USER);
		system("pause");
		system("cls");
		teacher();
	}
	if (choice == 2 && *(which=login(choice)) != '-1') {
		printf("\n学生%s, 您已成功登录！\n\n", which);
		system("pause");
		system("cls");
		/* 将返回的学号保存到数组中，并传递给student函数*/
		char id[MAX_USER];
		strcpy_s(id, MAX_USER, which);
		student(id);
	}
	
}

Num score() {
	Student stu[N];
	int i, j;
	FILE *fp;
	errno_t err;
	char filename1[10] = "score.txt";
	char filename2[10] = "info.txt"; /* 存放当前已记录学生人数，课程数；学生的账号（学号）和密码（初始值为学号）*/

	printf("请输入学生人数和课程数目：");
	scanf_s("%d%d", &num.stu, &num.course);
	printf("请输入学生的学号和成绩：\n");
	for (i = 0; i < num.stu; i++) {
		scanf_s("%s", stu[i].id, N);
		for (j = 0; j < num.course; j++) {
			scanf_s("%f", &stu[i].score[j]);
		}
	}
	getchar();
	printf("---------------------------------------\n");
	getchar();
	printf("数据确认：\n");
	for (i = 0; i < num.stu; i++) {
		printf("%s ", stu[i].id);
		for (j = 0; j < num.course; j++) {
			printf("%.1f ", stu[i].score[j]);
		}
		printf("\n");
	}
	printf("\n");

	/*生成文件score.txt*/
	if ((err = fopen_s(&fp, filename1, "w")) != 0) {
		printf("无法打开此文件\n");
		exit(0);
	}
	for (i = 0; i < num.stu; i++) {
		fprintf(fp, "%s ", stu[i].id);
		for (j = 0; j < num.course; j++) {
			fprintf(fp, "%.1f ", stu[i].score[j]);
		}
		fprintf(fp, "\n");
	}
	printf("已生成文件\"%s\"\n", filename1);
	fclose(fp);

	/*生成文件info.txt*/
	if ((err = fopen_s(&fp, filename2, "w")) != 0) {
		printf("无法打开此文件\n");
		exit(0);
	}
	fprintf(fp, "%d %d\n", num.stu, num.course);
	for (i = 0; i < num.stu; i++) {
		strcpy_s(stu[i].pwd, N, stu[i].id); /*初始密码为学号*/
		fprintf(fp, "%s %s", stu[i].id, stu[i].pwd);
		fprintf(fp, "\n");
	}
	printf("已生成文件\"%s\"\n", filename2);
	fclose(fp);

	return num;
}

void report( Num num) {
	Student stu[N];
	int i, j;
	char course = 'A';
	float max[M], min[M], average[M];
	FILE *in, *out;
	errno_t err;
	char filename_in[20] = "score.txt";
	char filename_out[20] = "report.txt";

	/*从score.txt读取数据*/
	if ((err = fopen_s(&in, filename_in, "r")) != 0) {
		printf("无法打开\"%s\"\n", filename_in);
		exit(0);
	}
	/*读取学号,分数*/
	for (i = 0; i < num.stu; i++) {
		fscanf_s(in, "%s", stu[i].id, N);
		for (j = 0; j < num.course; j++) {
			fscanf_s(in, "%f", &stu[i].score[j]);
		}
	}

	/*各科最大最小平均值*/
	for (j = 0; j < num.course; j++, course++) {
		average[j] = stu[0].score[j];
		max[j] = stu[0].score[j];
		min[j] = stu[0].score[j];
		printf("统计数据：\n");
		for (i = 1; i < num.stu; i++) {
			average[j] += stu[i].score[j];
			if (max[j] < stu[i].score[j]) max[j] = stu[i].score[j];
			if (min[j] > stu[i].score[j]) min[j] = stu[i].score[j];
		}
		average[j] /= num.stu;
		printf("课程%c的最高分：%.2f, 最低分：%.2f, 平均分：%.2f\n", course, max[j], min[j], average[j]);
	}
	/*每人总成绩*/
	for (i = 0; i < num.stu; i++) {
		stu[i].sum = stu[i].score[0];
		for (j = 1; j < num.course; j++) {
			stu[i].sum += stu[i].score[j];
		}
		printf("%s 的总成绩为: %.2f\n", stu[i].id, stu[i].sum);
	}

	/*生成文件report.txt*/
	if ((err = fopen_s(&out, filename_out, "w")) != 0) {
		printf("无法打开\"%s\"\n", filename_out);
		exit(0);
	}
	fprintf(out, "课程名称 \t最高分 最低分 平均分\n");
	for (course = 'A', j = 0; j < num.course; j++, course++) {
		fprintf(out, "    %c\t%7.2f%7.2f%7.2f\n", course, max[j], min[j], average[j]);
	}
	fprintf(out, "    学号\t\t总成绩\n");
	for (i = 0; i < num.stu; i++) {
		fprintf(out, "%s\t%8.2f\n", stu[i].id, stu[i].sum);
	}
	fclose(out);
	printf("已生成文件\"%s\"\n", filename_out);
}

void sort( Num num) {
	Student stu[N];
	void sort_by_final(Student *final_rank, Num num);
	void sort_by_course(Student *course_rank, Num num, int cour);
	int i, j;
	char course = 'A';
	Student course_rank[N];
	Student final_rank[N];
	FILE *in, *out;
	errno_t err;
	char filename_in[20] = "score.txt";
	char filename_out[20] = "sort.txt";
	/*从score.txt读取数据*/
	if ((err = fopen_s(&in, filename_in, "r")) != 0) {
		printf("无法打开\"%s\"\n", filename_in);
		exit(0);
	}
	/*读取学号,分数*/
	for (i = 0; i < num.stu; i++) {
		fscanf_s(in, "%s", stu[i].id, N);
		for (j = 0; j < num.course; j++) {
			fscanf_s(in, "%f", &stu[i].score[j]);
		}
	}
	fclose(in);

	/*生成文件sort.txt*/
	if ((err = fopen_s(&out, filename_out, "w")) != 0) {
		printf("无法打开\"%s\"\n", filename_out);
		exit(0);
	}
	/*将读到的数据放入另外两个结构course_rank, final_rank中*/
	for (i = 0; i < num.stu; i++) {
		course_rank[i] = stu[i];
		final_rank[i] = stu[i];
	}

	/*将结果输出到屏幕和文件中*/
	for (j = 0; j < num.course; j++, course++) {
		sort_by_course(course_rank, num, j);
		printf("按课程%c排名：\n", course);
		fprintf(out, "按课程%c排名：\n", course);
		for (i = 0; i < num.stu; i++) {
			printf("%s %.2f\n", course_rank[i].id, course_rank[i].score[j]);
			fprintf(out, "%s %.2f\n", course_rank[i].id, course_rank[i].score[j]);
		}
	}

	sort_by_final(final_rank, num);
	printf("按总成绩排名：\n");
	fprintf(out, "按总成绩排名：\n");
	for (i = 0; i < num.stu; i++) {
		printf("%s %.2f\n", final_rank[i].id, final_rank[i].sum);
		fprintf(out, "%s %.2f\n", final_rank[i].id, final_rank[i].sum);
	}
	printf("已生成文件\"%s\"\n", filename_out);
	fclose(out);
}

void search( Num num) {
	Student stu[N];
	char id[N];
	char course = 'A';
	int i, j;
	FILE *in, *out;
	errno_t err;
	char filename_in[20] = "score.txt";
	char filename_out[20] = "search.txt";

	printf("输入您要查找的学号：");
	scanf_s("%s", id, N);
	/*从score.txt读取数据*/
	if ((err = fopen_s(&in, filename_in, "r")) != 0) {
		printf("无法打开\"%s\"\n", filename_in);
		exit(0);
	}
	/*读取学号,分数*/
	for (i = 0; i < num.stu; i++) {
		fscanf_s(in, "%s", stu[i].id, N);
		for (j = 0; j < num.course; j++) {
			fscanf_s(in, "%f", &stu[i].score[j]);
		}
	}
	fclose(in);

	/*生成文件search.txt*/
	if ((err = fopen_s(&out, filename_out, "w")) != 0) {
		printf("无法打开\"%s\"\n", filename_out);
		exit(0);
	}
	/*算出每个人的总成绩*/
	for (i = 0; i < num.stu; i++) {
		stu[i].sum = 0;
		for (j = 0; j < num.course; j++) {
			stu[i].sum += stu[i].score[j];
		}
	}
	/* 将成绩打印到屏幕+文件中 */
	for (i = 0; i < num.stu; i++) {
		if (strcmp(id, stu[i].id) == 0) {
			printf("学生%s的成绩单：\n", id);
			fprintf(out, "学生%s的成绩单：\n", id);
			for (j = 0; j < num.course; j++, course++) {
				fprintf(out, "课程%c ", course);
				printf("课程%c ", course);
			}
			fprintf(out, "总成绩\n");
			printf("总成绩\n");
			for (j = 0; j < num.course; j++) {
				printf("%6.2f", stu[i].score[j]);
				fprintf(out, "%6.2f", stu[i].score[j]);
			}
			printf("%6.2f\n", stu[i].sum);
			fprintf(out, "%6.2f\n", stu[i].sum);
			break;
		}
	}
	if (i == num.stu) {
		printf("没有找到该学生！\n");
	}
	printf("已生成文件\"%s\"\n", filename_out);
	fclose(out);
}

void sort_by_course(Student *course_rank, Num num, int cour) {
	Student stu[N];
	int i, j, k;
	Student temp;
	/*选择法排序*/
	for (i = 0; i < num.stu - 1; i++) {
		k = i;
		for (j = i + 1; j < num.stu; j++) {
			if (course_rank[i].score[cour] < course_rank[j].score[cour]) {
				k = j;
			}
		}
		if (k != i) {
			temp = course_rank[i];
			course_rank[i] = course_rank[k];
			course_rank[k] = temp;
		}
	}

}

void sort_by_final(Student *final_rank, Num num) {
	Student stu[N];
	int i, j, k;
	Student temp;
	/*算出每个人的总成绩*/
	for (i = 0; i < num.stu; i++) {
		final_rank[i].sum = 0;
		for (j = 0; j < num.course; j++) {
			final_rank[i].sum += final_rank[i].score[j];
		}
	}
	/*选择法排序*/
	for (i = 0; i < num.stu - 1; i++) {
		k = i;
		for (j = i + 1; j < num.stu; j++) {
			if (final_rank[i].sum < final_rank[j].sum) {
				k = j;
			}
		}
		if (k != i) {
			temp = final_rank[i];
			final_rank[i] = final_rank[k];
			final_rank[k] = temp;
		}
	}

}

void teacher() {
	Student stu[N];
	int choice;
	while (1) { 
		printf("\t-----------------欢迎使用学生成绩管理系统！----------------------\n");
		printf("\t| 成绩初始化录入                                   \t- 请输入1 |\n");
		printf("\t| 计算各科成绩的最高分最低分平均分及每人的总成绩 \t- 请输入2 |\n");
		printf("\t| 按照单科成绩和个人总成绩进行降序排序                  - 请输入3 |\n");
		printf("\t| 按学号查询学生的各科成绩                         \t- 请输入4 |\n");
		printf("\t| 其他功能敬请期待...                             \t          |\n");
		printf("\t|                                                \t          |\n");
		printf("\t|                                                \t          |\n");
		printf("\t| 退出                                            \t- 请输入5 |\n");
		printf("\t-----------------------------------------------------------------\n");
		printf("你的选择是：  ");
		scanf_s("%d", &choice);

		switch (choice)
		{
		case 1: printf("-----------------正在录入---------------\n");
			num = score(score);
			printf("-----------------录入完成---------------\n");
			break;
		case 2: printf("-----------------成绩计算--------------\n");
			report(num);
			printf("-----------------计算完成--------------\n");
			break;
		case 3: printf("-----------------成绩排序--------------\n");
			sort(num);
			printf("-----------------排序完成--------------\n");
			break;
		case 4: printf("-----------------成绩查询--------------\n");
			search(num);
			printf("-----------------查询完成--------------\n");
			break;
		case 5: printf("-----------------退出系统--------------\n");
			return;
		}
		system("pause");
		system("cls");
		printf("\n\n");
	}
}

void student(char *id){
	Student stu[N];
	int choice;
	while (1) {
		printf("\t-----------------欢迎使用学生成绩管理系统！----------------------\n");
		printf("\t| 成绩查询                                            \t- 请输入1 |\n");
		printf("\t| 修改密码                                            \t- 请输入2 |\n");
		printf("\t| 退出                                            \t- 请输入3 |\n");
		printf("\t-----------------------------------------------------------------\n");
		printf("你的选择是：  ");
		scanf_s("%d", &choice);
		

		switch (choice)
		{
		case 1: printf("-----------------成绩查询--------------\n");
			student_search(id);
			printf("-----------------查询完成--------------\n");
			break;
		case 2: printf("-----------------修改密码--------------\n");
			modify(id);
			printf("-----------------修改成功--------------\n");
			break;
		case 3: printf("-----------------退出系统--------------\n");
			return;
		}
		system("pause");
		system("cls");
		printf("\n\n");
	}
}

char* login(int choice) {
	char user[MAX_USER], teacher_user[MAX_USER] = USER;
	char pwd[MAX_PWD], teacher_pwd[MAX_PWD] = PWD;
	char *which = "0"; /*登录成功返回值：学生学号*/
	char *fail = "-1";/*登录失败返回值*/
	int count = 0;
	int i=0;
	Student stu[N];

	/*教师的用户名为admin*/
	if (choice == 1) {
		do
		{
			if (count == 3) {
				printf("\t您已三次输错密码，系统将关闭！\n");
				return fail;
			}
			else if (count > 0)
				printf("\t密码错误！您还有%d次机会！\n", (3 - count));

			printf("\n用户名：");
			gets(user);
			printf("\n密码：");
			gets(pwd);
			count++;

		} while ((strcmp(user, teacher_user)) || (strcmp(pwd, teacher_pwd))); /*循环条件：密码或账号不匹配*/
	}
	
	if (choice == 2) {
		int i;
		FILE *in;
		errno_t err;
		char filename_in[20] = "info.txt";
		int success = 0; /*成功登录的标记*/

		/*从info.txt读取数据*/
		if ((err = fopen_s(&in, filename_in, "r")) != 0) {
			printf("无法打开\"%s\"\n", filename_in);
			exit(0);
		}
		/*读取学生的学号,密码*/
		int temp1=0, temp2=0;
		fscanf_s(in, "%d %d", &temp1, &temp2);
		for (i = 0; i < num.stu; i++) {
			fscanf_s(in, "%s %s", stu[i].id, N, stu[i].pwd, N);
		}
		fclose(in);
		
		/*请用户输入账号密码*/
		do
		{
			if (count == 3) {
				printf("\t您已三次输错密码，系统将关闭！\n");
				return fail;
			}
			else if (count > 0)
				printf("\n密码错误！您还有%d次机会!", (3 - count));

			printf("\n用户名：");
			gets(user);
			printf("\n密码：");
			gets(pwd);
			count++;

			for (i = 0; i < num.stu; i++) {
				if (strcmp(user, stu[i].id) == 0 && strcmp(pwd, stu[i].pwd) == 0) {
					which = stu[i].id;
					success = 1;
					break;
					}
				}
			if (success) break;
		} while (1);
	}
	return which;
}

void modify(char *id) {
	int i;
	FILE *fp;
	errno_t err;
	char filename[20] = "info.txt";
	int success = 0; /*成功登录的标记*/
	Student stu[N];

	 /*从info.txt读取数据*/
	if ((err = fopen_s(&fp, filename, "r")) != 0) {
		printf("无法打开\"%s\"\n", filename);
		exit(0);
	}
	/*读取学生的学号,密码*/
	int temp1 = 0, temp2 = 0;
	fscanf_s(fp, "%d %d", &temp1, &temp2);
	for (i = 0; i < num.stu; i++) {
		fscanf_s(fp, "%s %s", stu[i].id, N, stu[i].pwd, N);
	}
	fclose(fp);

	for (i = 0; i < num.stu; i++) {
		if (strcmp(id, stu[i].id) == 0) {
			char confirm_pwd[MAX_PWD];
			char new_pwd[MAX_PWD];
			printf("请输入新密码：");
			scanf_s("%s", new_pwd, MAX_PWD);
			printf("请确认新密码：");
			scanf_s("%s", confirm_pwd, MAX_PWD);
			if (strcmp(new_pwd, confirm_pwd) == 0) {
				strcpy_s(stu[i].pwd, N, new_pwd);
				printf("修改成功！\n");
			}
		}
		/* 修改info.txt 文件 */
		if ((err = fopen_s(&fp, filename, "w")) != 0) {
			printf("无法打开\"%s\"\n", filename);
			exit(0);
		}
		fprintf(fp, "%d %d", num.stu, num.course);
		fprintf(fp, "\n");
		for (i = 0; i < num.stu; i++) {
			fprintf(fp, "%s %s\n", stu[i].id, stu[i].pwd);
		}
		fclose(fp);


	}

}

void student_search(char *id) {
	int i, j;
	FILE *in;
	errno_t err;
	char filename_in[20] = "info.txt";
	int success = 0; /*成功登录的标记*/
	Student stu[N];
	char course = 'A';

	/*从score.txt读取数据*/
	if ((err = fopen_s(&in, filename_in, "r")) != 0) {
		printf("无法打开\"%s\"\n", filename_in);
		exit(0);
	}
	/*读取学号,分数*/
	for (i = 0; i < num.stu; i++) {
		fscanf_s(in, "%s", stu[i].id, N);
		for (j = 0; j < num.course; j++) {
			fscanf_s(in, "%f", &stu[i].score[j]);
		}
	}
	fclose(in);
	/*算出每个人的总成绩*/
	for (i = 0; i < num.stu; i++) {
		stu[i].sum = 0;
		for (j = 0; j < num.course; j++) {
			stu[i].sum += stu[i].score[j];
		}
	}
	/* 将成绩打印到屏幕上 */
	for (i = 0; i < num.stu; i++) {
		if (strcmp(id, stu[i].id) == 0) {
			printf("学生%s的成绩单：\n", id);
			for (j = 0; j < num.course; j++, course++) {
				printf("课程%c ", course);
			}
			printf("总成绩\n");
			for (j = 0; j < num.course; j++) {
				printf("%6.2f", stu[i].score[j]);
			}
			printf("%6.2f\n", stu[i].sum);
			break;
		}
	}
}

