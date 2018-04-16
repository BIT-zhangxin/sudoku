#pragma once
#ifndef _MY_SUDOKU
#define _MY_SUDOKU

#include <cstdio>
#include <cstring>
#include "random.h"
#include "dlx.h"

class Sudoku
{
private:

	RANDOM random;
	int sudoku[9][9];

	int Source[9] = { 1,2,3,4,5,6,7,8,9 };
	int Binary[10] = { 0,1,2,4,8,16,32,64,128,256 };
	int Search_Failure = -1;

public:

	void Create_File(int number, int id)
	{
		FILE *fp = fopen("sudoku.txt", "w");
		for (int t = 0; t < number; t++)
		{
			char tmp[170] = { 0 };
			int idx = 0;
			Create(id);
			if (t != 0) tmp[idx++] = '\n';
			for (int row = 0; row < 9; row++)
			{
				for (int col = 0; col < 9; col++)
				{
					if (col != 0) tmp[idx++] = ' ';
					tmp[idx++] = sudoku[row][col] + '0';
				}
				if (t != number - 1 || row != 8) tmp[idx++] = '\n';
			}
			fputs(tmp, fp);
		}
		fclose(fp);
	}

	int Solve_File(const char *absolute_path_of_puzzlefile)
	{
		FILE *fp1 = fopen(absolute_path_of_puzzlefile, "r");
		if (fp1 == NULL) return 1;
		FILE *fp2 = fopen("sudoku.txt", "w");
		while (1)
		{
			char tmp[170] = { 0 };
			int idx = 0;
			for (int row = 0; row < 9; row++)
			{
				idx = 0;
				fgets(tmp, 20, fp1);
				for (int col = 0; col < 9; col++)
				{
					sudoku[row][col] = tmp[idx++] - '0';
					idx++;
				}
			}
			Solve();
			idx = 0;
			for (int row = 0; row < 9; row++)
			{
				for (int col = 0; col < 9; col++)
				{
					if (col != 0) tmp[idx++] = ' ';
					tmp[idx++] = sudoku[row][col] + '0';
				}
				tmp[idx++] = '\n';
			}
			fputs(tmp, fp2);
			if (fgetc(fp1) == EOF) break;
			fputc('\n', fp2);
		}
		fclose(fp1);
		fclose(fp2);
		return 0;
	}

public:

	Sudoku()
	{
		InitSudoku();
	}

	void Create(int id)
	{
		InitSudoku();
		int digit1 = id % 10;
		int digit2 = (id % 100 - digit1) / 10;
		int n = (digit1 + digit2) % 9 + 1;
		Create_By_First_number(n);
		Search_Four_Grid(0);
	}

	void Create_1()
	{
		InitSudoku();
		Create_Five_Grid();
		Search_Four_Grid(0);
	}

	void Create_2()
	{
		InitSudoku();
		Search_Nine_Grid(0);
	}

	void Create_3()
	{
		InitSudoku();
		Create_Five_Grid();
		DLX dlx(sudoku);
	}

	void Create_4()
	{
		InitSudoku();
		DLX dlx(sudoku);
	}

	void Solve()
	{
		Solve_1();
	}

	void Solve_1()
	{
		Search_Empty_Grid(0);
	}

	void Solve_2()
	{
		DLX dlx(sudoku);
	}

	bool Check()
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (sudoku[i][j] == 0 || !Check_Grid(i, j)) return false;
			}
		}
		return true;
	}

	void Print()
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				printf((j > 0) ? " %d" : "%d", sudoku[i][j]);
			}
			putchar(10);
		}
	}

	void Clear()
	{
		InitSudoku();
	}

private:

	void InitSudoku()
	{
		memset(sudoku, 0, sizeof(sudoku));
	}

	void Disorganize(int *Destination, int *Source, int length)
	{
		memset(Destination, 0, sizeof(int) * 9);
		for (int i = 0; i < length;)
		{
			int loc = random.Random(length);
			if (Destination[loc] == 0)
			{
				Destination[loc] = Source[i++];
			}
		}
	}

	void Disorganize_1(int *Destination, int *Source, int length)
	{
		memset(Destination, 0, sizeof(int) * length);
		for (int i = 0; i < length;)
		{
			int loc = random.Random(length);
			if (Destination[loc] == 0)
			{
				Destination[loc] = Source[i++];
			}
		}
	}

	void Disorganize_2(int *Destination, int *Source, int length)
	{
		int Change_Number = 30;
		memset(Destination, 0, sizeof(int) * length);
		for (int i = 0; i < length; i++)
		{
			Destination[i] = Source[i];
		}
		for (int i = 0; i < Change_Number; i++)
		{
			int tmp1 = random.Random(length);
			int tmp2 = random.Random(length);
			int tmp3 = Destination[tmp1];
			Destination[tmp1] = Destination[tmp2];
			Destination[tmp2] = tmp3;
		}
	}

	void Disorganize_3(int *Destination, int *Source, int length)
	{
		memset(Destination, 0, sizeof(int) * length);
		for (int i = 0; i < length; i++)
		{
			int r = random.Random(length - i) + 1;
			int loc = 0;
			while (1)
			{
				if (Destination[loc] == 0)
				{
					r--;
					if (r == 0) break;
				}
				loc++;
			}
			Destination[loc] = Source[i];
		}
	}

	int HashList[10][7] =
	{
		1,0,0,0,0,0,0,
		1,0,0,0,0,0,0,
		1,1,0,0,0,0,0,
		2,1,2,0,0,0,0,
		2,1,3,0,0,0,0,
		4,1,2,3,4,0,0,
		2,1,5,0,0,0,0,
		6,1,2,3,4,5,6,
		4,1,3,5,7,0,0,
		6,1,2,4,5,7,8,
	};

	void Disorganize_4(int *Destination, int *Source, int length)
	{
		memset(Destination, 0, sizeof(int) * length);
		for (int i = 0; i < length; i++)
		{
			int loc = random.Random(length);
			int HashCode = HashList[length][1 + random.Random(HashList[length][0])];
			while (Destination[loc]) loc = (loc + HashCode) % length;
			Destination[loc] = Source[i];
		}
	}

	int Extract_Number(int row, int col)
	{
		int BitNumber = 0;
		int length = 0;
		int tmp[9] = { 0 };
		for (int i = 0; i < 9; i++)
		{
			BitNumber |= Binary[sudoku[row][i]];
			BitNumber |= Binary[sudoku[i][col]];
			BitNumber |= Binary[sudoku[row / 3 * 3 + i / 3][col / 3 * 3 + i % 3]];
		}
		for (int i = 1; i <= 9; i++)
		{
			if ((BitNumber & 1) == 0) tmp[length++] = i;
			BitNumber >>= 1;
		}
		if (length == 0) return Search_Failure;
		return tmp[random.Random(length)];
	}

	int Extract_Array(int *Destination, int row, int col)
	{
		int BitNumber = 0;
		int length = 0;
		int tmp[9] = { 0 };
		memset(Destination, 0, sizeof(int) * 9);
		for (int i = 0; i < 9; i++)
		{
			BitNumber |= Binary[sudoku[row][i]];
			BitNumber |= Binary[sudoku[i][col]];
			BitNumber |= Binary[sudoku[row / 3 * 3 + i / 3][col / 3 * 3 + i % 3]];
		}
		for (int i = 1; i <= 9; i++)
		{
			if ((BitNumber & 1) == 0) tmp[length++] = i;
			BitNumber >>= 1;
		}
		if (length == 0) return Search_Failure;
		Disorganize(Destination, tmp, length);
		return length;
	}

	bool Check_Grid(int row, int col)
	{
		if (sudoku[row][col] == 0) return false;
		for (int i = 0; i < 9; i++)
		{
			if (sudoku[row][i] == sudoku[row][col] && i != col) return false;
			if (sudoku[i][col] == sudoku[row][col] && i != row) return false;
			if (sudoku[row / 3 * 3 + i / 3][col / 3 * 3 + i % 3] == sudoku[row][col] &&
				row / 3 * 3 + i / 3 != row && col / 3 * 3 + i % 3 != col) return false;
		}
		return true;
	}

	void Create_Five_Grid()
	{
		int Destination[9] = { 0 };
		//前三行
		for (int row = 0; row < 2; row++)
		{
			if (row == 0)
			{
				Disorganize(Destination, Source, 9);
				for (int col = 0; col < 9; col++)
				{
					sudoku[0][col] = Destination[col];
				}
			}
			else if (row == 1)
			{
				Disorganize(Destination, Source, 6);
				for (int i = 0; i < 3; i++)
				{
					sudoku[1][Destination[i] - 1] = sudoku[0][6 + i];
				}
			}

		}
		for (int row = 1; row < 3; row++)
		{
			for (int col = 0; col < 9; col++)
			{
				if (sudoku[row][col] != 0) continue;
				else sudoku[row][col] = Extract_Number(row, col);
			}

		}
		//前三列
		for (int col = 0; col < 2; col++)
		{
			if (col == 0)
			{
				for (int row = 3; row < 9; row++)
				{
					sudoku[row][0] = Extract_Number(row, 0);
				}
			}
			else if (col == 1)
			{
				int tmp[3] = { 0 };
				int length = 0;
				for (int i = 0; i < 3; i++)
				{
					bool flag = true;
					for (int j = 0; j < 3; j++)
					{
						if (sudoku[6 + i][0] == sudoku[j][1])
						{
							flag = false;
							break;
						}
					}
					if (flag) tmp[length++] = sudoku[6 + i][0];
				}
				Disorganize(Destination, Source, 3);
				for (int i = 0; i < length; i++)
				{
					sudoku[2 + Destination[i]][1] = tmp[i];
				}
			}
		}
		for (int col = 1; col < 3; col++)
		{
			for (int row = 0; row < 9; row++)
			{
				if (sudoku[row][col] != 0) continue;
				else sudoku[row][col] = Extract_Number(row, col);
			}
		}
	}

	void Create_By_First_number(int n)
	{
		int Destination[9] = { 0 };
		//前三行
		for (int row = 0; row < 2; row++)
		{
			if (row == 0)
			{
				sudoku[0][0] = n;
				for (int col = 1; col < 9; col++)
				{
					sudoku[0][col] = Extract_Number(row, col);
				}
			}
			else if (row == 1)
			{
				Disorganize(Destination, Source, 6);
				for (int i = 0; i < 3; i++)
				{
					sudoku[1][Destination[i] - 1] = sudoku[0][6 + i];
				}
			}

		}
		for (int row = 1; row < 3; row++)
		{
			for (int col = 0; col < 9; col++)
			{
				if (sudoku[row][col] != 0) continue;
				else sudoku[row][col] = Extract_Number(row, col);
			}

		}
		//前三列
		for (int col = 0; col < 2; col++)
		{
			if (col == 0)
			{
				for (int row = 3; row < 9; row++)
				{
					sudoku[row][0] = Extract_Number(row, 0);
				}
			}
			else if (col == 1)
			{
				int tmp[3] = { 0 };
				int length = 0;
				for (int i = 0; i < 3; i++)
				{
					bool flag = true;
					for (int j = 0; j < 3; j++)
					{
						if (sudoku[6 + i][0] == sudoku[j][1])
						{
							flag = false;
							break;
						}
					}
					if (flag) tmp[length++] = sudoku[6 + i][0];
				}
				Disorganize(Destination, Source, 3);
				for (int i = 0; i < length; i++)
				{
					sudoku[2 + Destination[i]][1] = tmp[i];
				}
			}
		}
		for (int col = 1; col < 3; col++)
		{
			for (int row = 0; row < 9; row++)
			{
				if (sudoku[row][col] != 0) continue;
				else sudoku[row][col] = Extract_Number(row, col);
			}
		}
	}

	bool Search_One_Grid(int Number, int Grid_Number)
	{
		if (Number == 9) return true;
		int row = Grid_Number / 3 * 3 + Number / 3;
		int col = Grid_Number % 3 * 3 + Number % 3;
		int Destination[9] = { 0 };
		int length = Extract_Array(Destination, row, col);
		if (length == Search_Failure) return false;
		for (int i = 0; i < length; i++)
		{
			sudoku[row][col] = Destination[i];
			if (Search_One_Grid(Grid_Number, Number + 1)) return true;
			sudoku[row][col] = 0;
		}
		return false;
	}

	bool Search_Four_Grid(int Number)
	{
		if (Number == 36) return true;
		int row = 3 + Number / 6;
		int col = 3 + Number % 6;
		int Destination[9] = { 0 };
		int length = Extract_Array(Destination, row, col);
		if (length == Search_Failure) return false;
		for (int i = 0; i < length; i++)
		{
			sudoku[row][col] = Destination[i];
			if (Search_Four_Grid(Number + 1)) return true;
			sudoku[row][col] = 0;
		}
		return false;
	}

	bool Search_Nine_Grid(int Number)
	{
		if (Number == 81) return true;
		int row = Number / 9;
		int col = Number % 9;
		int Destination[9] = { 0 };
		int length = Extract_Array(Destination, row, col);
		if (length == Search_Failure) return false;
		for (int i = 0; i < length; i++)
		{
			sudoku[row][col] = Destination[i];
			if (Search_Nine_Grid(Number + 1)) return true;
			sudoku[row][col] = 0;
		}
		return false;
	}

	bool Search_Empty_Grid(int Number)
	{
		if (Number == 81) return true;
		int row = Number / 9;
		int col = Number % 9;
		if (sudoku[row][col] == 0)
		{
			int Destination[9] = { 0 };
			int length = Extract_Array(Destination, row, col);
			if (length == Search_Failure) return false;
			for (int i = 0; i < length; i++)
			{
				sudoku[row][col] = Destination[i];
				if (Search_Empty_Grid(Number + 1)) return true;
				sudoku[row][col] = 0;
			}
		}
		else
		{
			if (Search_Empty_Grid(Number + 1)) return true;
		}
		return false;
	}

};

#endif // _MY_RANDOM
