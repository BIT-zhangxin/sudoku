#include "stdafx.h"

int main(int argc, char *argv[])
{
	Sudoku s;
	if (argc != 3)
	{
		printf("Please enter the correct command!\n");
		return 0;
	}
	int start = clock();
	if (!strcmp(argv[1], "-c"))
	{
		int n = atoi(argv[2]);
		if (n <= 0 || n > 1000000)
		{
			printf("Please enter the correct data range!\n");
			return 0;
		}
		s.Create_File(n, 1120161967);
		printf("生成成功！\n");
	}
	else if (!strcmp(argv[1], "-s"))
	{
		if (s.Solve_File(argv[2]))
		{
			printf("Please enter the correct address!\n");
			return 0;
		}
		printf("求解成功！\n");
	}
	int end = clock();
	printf("消耗时间：%dms\n", end - start);
	return 0;
}
