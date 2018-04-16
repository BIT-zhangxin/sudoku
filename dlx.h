#pragma once
#ifndef _MY_DLX
#define _MY_DLX

#include <cstring>

class DLX
{
public:

	DLX(int sudoku[9][9])
	{
		InitDLX(sudoku);
		Solve(sudoku);
	}

private:

	char Matrix[729][324];
	int Left[3421], Right[3421], Up[3421], Down[3421];
	int Row_Loc[3421], Col_Loc[3421], Col_Size[3421];
	int Head, Node_Number;

	int Create_Node(int left, int right, int up, int down)
	{
		Left[Node_Number] = left; Right[left] = Node_Number;
		Right[Node_Number] = right; Left[right] = Node_Number;
		Up[Node_Number] = up; Down[up] = Node_Number;
		Down[Node_Number] = down; Up[down] = Node_Number;
		return Node_Number++;
	}

	void InitDLX(int sudoku[9][9])
	{
		memset(Matrix, 0, sizeof(Matrix));
		memset(Left, 0, sizeof(Left));
		memset(Right, 0, sizeof(Right));
		memset(Up, 0, sizeof(Up));
		memset(Down, 0, sizeof(Down));
		memset(Row_Loc, 0, sizeof(Row_Loc));
		memset(Col_Loc, 0, sizeof(Col_Loc));
		memset(Col_Size, 0, sizeof(Col_Size));
		memset(Left, 0, sizeof(Left));
		Node_Number = 3420;
		for (int row = 0; row < 9; row++)
		{
			for (int col = 0; col < 9; col++)
			{
				int num = row * 9 + col;
				if (sudoku[row][col] == 0)
				{
					for (int i = 0; i < 9; i++)
					{
						Matrix[num * 9 + i][num] = 1;
						Matrix[num * 9 + i][81 + row * 9 + i] = 1;
						Matrix[num * 9 + i][162 + col * 9 + i] = 1;
						Matrix[num * 9 + i][243 + (row / 3 * 3 + col / 3) * 9 + i] = 1;
					}
				}
				else
				{
					int i = sudoku[row][col] - 1;
					Matrix[num * 9 + i][num] = 1;
					Matrix[num * 9 + i][81 + row * 9 + i] = 1;
					Matrix[num * 9 + i][162 + col * 9 + i] = 1;
					Matrix[num * 9 + i][243 + (row / 3 * 3 + col / 3) * 9 + i] = 1;
				}
			}
		}
		Head = Create_Node(Node_Number, Node_Number, Node_Number, Node_Number);//建立头结点
		Node_Number = 0;
		for (int i = 0; i < 324; i++)//建立辅助节点
		{
			Create_Node(Left[Head], Head, Node_Number, Node_Number);
			Col_Loc[i] = i; Col_Size[i] = 0;
		}
		for (int i = 0; i < 324; i++)//建立内部节点
		{
			int loc = -1;
			for (int j = 0; j < 729; j++)
			{
				if (!Matrix[i][j]) continue;
				if (loc == -1)
				{
					loc = Create_Node(Node_Number, Node_Number, Up[Col_Loc[j]], Col_Loc[j]);
					Row_Loc[loc] = i; Col_Loc[loc] = j; Col_Size[j]++;
				}
				else
				{
					loc = Create_Node(loc, Right[loc], Up[Col_Loc[j]], Col_Loc[j]);
					Row_Loc[loc] = i; Col_Loc[loc] = j; Col_Size[j]++;
				}
			}
		}
	}

	void Remove(int Node)
	{
		Left[Right[Node]] = Left[Node];
		Right[Left[Node]] = Right[Node];
		for (int i = Down[Node]; i != Node; i = Down[i])
		{
			for (int j = Right[i]; j != i; j = Right[j])
			{
				Up[Down[j]] = Up[j];
				Down[Up[j]] = Down[j];
				Col_Size[Col_Loc[j]]--;
			}
		}
	}

	void Resume(int Node)
	{
		Right[Left[Node]] = Node;
		Left[Right[Node]] = Node;
		for (int i = Up[Node]; i != Node; i = Up[i])
		{
			for (int j = Left[i]; j != i; j = Left[j])
			{
				Col_Size[Col_Loc[j]]++;
				Down[Up[j]] = j;
				Up[Down[j]] = j;
			}
		}
	}

	bool Solve(int sudoku[9][9])
	{
		if (Right[Head] == Head) return true;
		int Node;
		int Min = 2 << 29;
		for (int i = Right[Head]; i != Head; i = Right[i])
		{
			if (Col_Size[i] < Min)
			{
				Min = Col_Size[i];
				Node = i;
			}
		}
		Remove(Node);
		for (int i = Down[Node]; i != Node; i = Down[i])
		{
			int loc = Row_Loc[i];
			sudoku[loc / 81][loc / 9 % 9] = loc % 9 + 1;
			for (int j = Right[i]; j != i; j = Right[j]) Remove(Col_Loc[j]);
			if (Solve(sudoku)) return true;
			for (int j = Left[i]; j != i; j = Left[j]) Resume(Col_Loc[j]);
		}
		Resume(Node);
		return false;
	}
};

#endif // _MY_DLX
