/* 成绩管理系统 */

#include <stdio.h>
#include <stdlib.h>
#define M 4 /*课程数量*/
#define N 50 /*学号最大长度, 学生数量*/
typedef struct {
	char id[N];
	float score[M];
}Student;
Student stu[N];
int record(Student *stu);
void report(Student *stu, int n);
void sort(Student *stu);
void search(Student *stu);

void main() {
	int n; /*录入成绩的学生人数*/
	n = record(stu);
	report(stu, n);
	system("pause");
}

int record(Student *stu) {
	int i, j, n;
	FILE *fp;
	errno_t err;
	char filename[10] = "score.txt";

	printf("学生人数：");
	scanf_s("%d", &n);
	for (i = 0; i < n; i++) {
		scanf_s("%s ",stu[i].id, N);
		for (j = 0; j < M; j++) {
			scanf_s("%f", &stu[i].score[j]);
		}
	}
	printf("数据确认：\n");
	for (i = 0; i < n; i++) {
		printf("%s ", stu[i].id);
		for (j = 0; j < M; j++) {
			printf("%.1f ", stu[i].score[j]);
		}
		printf("\n");
	}

	/*生成文件score.txt*/
	if ((err = fopen_s(&fp, filename, "w")) != 0) {
		printf("无法打开此文件\n");
		exit(0);
	}
	for (i = 0; i < n; i++) {
		fprintf(fp, "%s ", stu[i].id);
		for (j = 0; j < M; j++) {
			fprintf(fp, "%.1f ", stu[i].score[j]);
		}
		fprintf(fp, "\n");
	}
	printf("已生成文件\"score.txt\"\n");
	fclose(fp);
	return n;
}

void report(Student *stu, int n) {
	int i, j;
	char course = 'A';
	float max[M], min[M], average[M], sum[N];
	FILE *fp;
	errno_t err;
	char filename[20] = "report.txt";
	/*各科最大最小平均值*/
	for (j = 0; j < M; j++, course++) {
		average[j] = stu[0].score[j];
		max[j] = stu[0].score[j];
		min[j] = stu[0].score[j];
		printf("统计数据：\n");
		for (i = 1; i < n; i++) {
			average[j] += stu[i].score[j];
			if (max[j] < stu[i].score[j]) max[j] = stu[i].score[j];
			if (min[j] > stu[i].score[j]) min[j] = stu[i].score[j];
		}
		average[j] /= n;
		printf("课程%c的最高分：%.2f, 最低分：%.2f, 平均分：%.2f\n", course, max[j],min[j],average[j]);
	}
	/*每人总成绩*/
	for (i = 0; i < n; i++) {
		sum[i] = stu[i].score[0];
		for (j = 1; j < M; j++) {
			sum[i] += stu[i].score[j];
		}
		printf("%s 的总成绩为: %.2f\n", stu[i].id, sum[i]);
	}

	/*生成文件report.txt*/
	if ((err = fopen_s(&fp, filename, "w")) != 0) {
		printf("无法打开此文件\n");
		exit(0);
	}
	fprintf(fp, "课程名称 \t最高分 最低分 平均分\n");
	for (course = 'A', j = 0; j < M; j++,course++) {
		fprintf(fp, "    %c\t%7.2f%7.2f%7.2f\n", course, max[j], min[j], average[j]);
	}
	fprintf(fp, "    学号\t\t总成绩\n");
	for (i = 0; i < n;i++) {
			fprintf(fp, "%s\t%8.2f\n",stu[i].id, sum[i]);
		}
	fclose(fp);
	printf("已生成文件\"report.txt\"\n");
	}