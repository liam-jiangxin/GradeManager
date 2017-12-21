/* 成绩管理系统 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 10 /*最大课程数量*/
#define N 50 /*学号最大长度, 学生数量*/
typedef struct {
	char id[N];
	float score[M];
	float sum;
}Student;
typedef struct {
	int course; /*课程数量*/
	int stu; /*学生数量*/
}Num;
Num score(Student *stu);
void report(Student *stu, Num num);
void sort(Student *stu, Num num);
void search(Student *stu, Num num);

void main() {
	
	Num num;
	Student stu_score[N];
	Student stu_report[N];
	Student stu_sort[N];
	Student stu_search[N];
	num = score(stu_score);
	report(stu_report, num);
	sort(stu_sort, num);
	search(stu_search, num);
	system("pause");
}

Num score(Student *stu) {
	int i, j;
	Num num = { 0,0 };
	FILE *fp;
	errno_t err;
	char filename[10] = "score.txt";

	printf("输入学生人数和课程数目：");
	scanf_s("%d%d", &num.stu, &num.course);
	for (i = 0; i < num.stu; i++) {
		scanf_s("%s",stu[i].id, N);
		for (j = 0; j < num.course; j++) {
			scanf_s("%f", &stu[i].score[j]);
		}
	}
	getchar();
	printf("数据确认：\n");
	for (i = 0; i < num.stu; i++) {
		printf("%s ", stu[i].id);
		for (j = 0; j < num.course; j++) {
			printf("%.1f ", stu[i].score[j]);
		}
		printf("\n");
	}

	/*生成文件score.txt*/
	if ((err = fopen_s(&fp, filename, "w")) != 0) {
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
	printf("已生成文件\"%s\"\n",filename);
	fclose(fp);
	return num;
}

void report(Student *stu, Num num) {
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
		printf("课程%c的最高分：%.2f, 最低分：%.2f, 平均分：%.2f\n", course, max[j],min[j],average[j]);
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
	for (course = 'A', j = 0; j < num.course; j++,course++) {
		fprintf(out, "    %c\t%7.2f%7.2f%7.2f\n", course, max[j], min[j], average[j]);
	}
	fprintf(out, "    学号\t\t总成绩\n");
	for (i = 0; i < num.stu;i++) {
			fprintf(out, "%s\t%8.2f\n",stu[i].id, stu[i].sum);
		}
	fclose(out);
	printf("已生成文件\"%s\"\n", filename_out);
	}

void sort(Student *stu, Num num) {
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

void search(Student *stu, Num num) {
	char id[N];
	char course = 'A';
	int i, j;
	FILE *in, *out;
	errno_t err;
	char filename_in[20] = "score.txt";
	char filename_out[20] = "search.txt";

	printf("输入您要查找的学号：");
	gets(id);
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
	for (i = 0; i < num.stu; i++) {
		if (strcmp(id, stu[i].id) == 0) {
			printf("%s的成绩单：\n", id);
			fprintf(out, "%s的成绩单：\n", id);
			for (j = 0; j < num.course; j++,course++) {
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
	int i, j, k;
	Student temp;
	/*选择法排序*/
	for (i = 0; i < num.stu-1; i++) {
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
