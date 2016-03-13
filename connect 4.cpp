#include "iostream"

using namespace std;

/*  1 i win
	-1 u win
	0 game is on
	2 tie

	1 mine
	2 yours
	0 blank*/

int status(int board[][7])
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int point_u=0,point_m=0;
			for (int k = j; k < j+4; k++)
			{
				if (board[i][k]==1)
				{
					point_m++;
				}
				else if (board[i][k]==2)
				{
					point_u++;
				}
			}
			if (point_u==4)
			{
				return -1;
			}
			else if (point_m==4)
			{
				return 1;
			}
		}
	}
	int zeroes=0;
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int point_u=0,point_m=0;
			for (int k = j; k < j+4; k++)
			{
				if (board[k][i]==1)
				{
					point_m++;
				}
				else if (board[k][i]==2)
				{
					point_u++;
				}
				else
					zeroes++;
			}
			if (point_u==4)
			{
				return -1;
			}
			else if (point_m==4)
			{
				return 1;
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j]==1&&board[i+1][j+1]==1&&board[i+2][j+2]==1&&board[i+3][j+3]==1)
			{
				return 1;
			}
			else if (board[i][j]==2&&board[i+1][j+1]==2&&board[i+2][j+2]==2&&board[i+3][j+3]==2)
			{
				return -1;
			}
		}
	}

	for (int i = 3; i <6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j]==1&&board[i-1][j+1]==1&&board[i-2][j+2]==1&&board[i-3][j+3]==1)
			{
				return 1;
			}
			else if (board[i][j]==2&&board[i-1][j+1]==2&&board[i-2][j+2]==2&&board[i-3][j+3]==2)
			{
				return -1;
			}
		}
	}

	if(zeroes)
		return 0;
	return 2;
}

int max(int board[][7],int state);

int min(int board[][7],int state)
{
	int game=status(board);
	if(game)
	{
		if (game==1)
		{
			return -999;
		}
		if (game==-1)
		{
			return 999;
		}
		else
			return 0;
	}

	if (state>=8)
	{
		return 0;
	}

	int min_value=999,po,ppo;
	for (int i = 0; i < 7; i++)
	{
		int j=0;
		if (board[5][i])
		{
			continue;
		}
		while(board[j][i])
			j++;
		board[j][i]=1;
		po=max(board,state+1);
		board[j][i]=0;
		if (po==-999)
		{
			min_value=po;
			ppo=i;
			break;
		}
		if (min_value>po)
		{
			min_value=po;
			ppo=i;
		}
	}
	if (state==1)
	{
		return ppo;
	}
	return min_value;
}

int max(int board[][7],int state)
{
	int game=status(board);
	if(game)
	{
		if (game==1)
		{
			return -999;
		}
		if (game==-1)
		{
			return 999;
		}
		else
			return 0;
	}

	if (state>=8)
	{
		return 0;
	}

	int max_value=-999,po;
	for (int i = 0; i < 7; i++)
	{
		int j=0;
		if (board[5][i])
		{
			continue;
		}
		while(board[j][i])
			j++;
		board[j][i]=2;
		po=min(board,state+1);
		board[j][i]=0;
		if (po==999)
		{
			max_value=po;
			//ppo=i;
			break;
		}
		if (max_value<po)
		{
			max_value=po;
			//ppo=i;
		}
	}
	return max_value;
}


int main()
{
	int board[6][7]={0};

	int game=status(board),move;
	//cout<<game;
	while (!game)
	{
		cin>>move;
		int j=0;
		while(board[j][move])
			j++;
		board[j][move]=2;
		int my_move=min(board,1);
		j=0;
		while(board[j][my_move])
			j++;
		board[j][my_move]=1;
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				cout<<board[i][j]<<" ";
			}
			cout<<'\n';
		}

		game=status(board);
		if (game==1)
		{
			cout<<"I WON!";
		}
		else if (game==-1)
		{
			cout<<"U WON!";
		}
		else if (game==2)
		{
			cout<<"TIE!";
		}
	}
	return 0;
}