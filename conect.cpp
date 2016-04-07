#include <iostream>
#include <vector>
#include <algorithm>

#define State 12
using namespace std;
// int count=0;
/*  1 i win
	-1 u win
	0 game is on
	2 tie

	1 mine
	2 yours
	0 blank*/

/*1210
  1110 = 5

  1110 = 25

  0110 = 2*/

void merges(int a[][2],int,int,int);

void mergesort(int a[][2], int low, int high)
{
    int mid;
    if (low < high)
    {
        mid=(low+high)/2;
        mergesort(a,low,mid);
        mergesort(a,mid+1,high);
        merges(a,low,high,mid);
    }
    return;
}
void merges(int a[][2], int low, int high, int mid)
{
    int i, j, k, c[50][2];
    i = low;
    k = low;
    j = mid + 1;
    while (i <= mid && j <= high)
    {
        if (a[i][0] < a[j][0])
        {
            c[k][1] = a[i][1];
            c[k][0] = a[i][0];
            k++;
            i++;
        }
        else
        {
            c[k][1] = a[j][1];
            c[k][0] = a[j][0];
            k++;
            j++;
        }
    }
    while (i <= mid)
    {
        c[k][1] = a[i][1];
        c[k][0] = a[i][0];
        k++;
        i++;
    }
    while (j <= high)
    {
        c[k][1] = a[j][1];
        c[k][0] = a[j][0];
        k++;
        j++;
    }
    for (i = low; i < k; i++)
    {
        a[i][1] = c[i][1];
        a[i][0] = c[i][0];
    }
}

int evaluate_me(int board[][7])
{
	int points=0;
	for (int j = 0; j < 4; j++)		//horizontal
	{
		for (int i = 0; i < 6; i++)
		{
			int point_u=0,point_m=0,zeroes=0;
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
				else
					zeroes++;
			}
			if (zeroes>2)
			{
				break;
			}
			if(i==0||(board[i-1][j]&&board[i-1][j+1]&&board[i-1][j+2]&&board[i-1][j+3]))
			{
				if(zeroes==1)
				{
					if (point_u==3)
					{
						return 500;
					}
					else if (point_m==3)
					{
						return -500;
					}
				}
				if(zeroes==2)
				{
					if (point_u==2)
					{
						points+=3;
					}
					else if (point_m==2)
					{
						points-=3;
					}
				}
			}
			else
			{
				if (zeroes==1)
				{
					if (point_u==3)
					{
						points+=20;
					}
					else if (point_m==3)
					{
						points-=20;
					}
				}
			}
			if (zeroes==2)
			{
				break;
			}
		}
	}

	for (int i = 0; i < 7; i++)		//vertical
	{
		if (!board[1][i])
		{
			continue;
		}
		int zeroes=0;
		for (int j = 0; j < 3; j++)
		{
			if (!board[j+1][i])
			{
				break;
			}
			int point_u=0,point_m=0,k,pl;
			for (k = j; k < j+4; k++)
			{
				if (board[k][i]==1)
				{
					point_m++;
				}
				else if (board[k][i]==2)
				{
					point_u++;
				}
				else{
					zeroes++;
					pl=k;
					break;
				}
			}
			if (zeroes)
			{
				if (point_u==3)
				{
					return 500;
				}
				else if (point_m==3)
				{
					return -500;
				}
				if (pl<5)
				{
					if (point_u==2)
					{
						points+=3;
					}
					else if (point_m==2)
					{
						points-=3;
					}
				}
			}
		}
	}

	for (int j = 0; j < 4; j++)		//diagonal 1
	{
		for (int i = 0; i < 3; i++)
		{
			int point_u=0,point_m=0,zeroes=0,pl[2],t=0;
			for (int k = 0; k < 4; k++)
			{
				if (board[i+k][j+k]==1)
				{
					point_m++;
				}
				else if (board[i+k][j+k]==2)
				{
					point_u++;
				}
				else
				{
					zeroes++;
					if(zeroes<3){
						pl[t]=k;
						t++;}
				}	
			}
			if (zeroes>2)
			{
				break;
			}
			if(zeroes==1)
			{
				if (point_u==3&&((i==0&&pl[0]==0)||board[i+pl[0]-1][j+pl[0]]))
				{
					return 500;
				}
				else if (point_m==3&&((i==0&&pl[0]==0)||board[i+pl[0]-1][j+pl[0]]))
				{
					return -500;
				}
				if (point_u==3)
				{
					points+=20;
				}
				else if (point_m==3)
				{
					points-=20;
				}
			}
			if (zeroes==2)
			{
				if (point_m==2&&((i==0&&pl[0]==0)||board[i+pl[0]-1][j+pl[0]])&&board[i+pl[1]-1][j+pl[1]])
				{
					points-=3;
				}
				else if (point_u==2&&((i==0&&pl[0]==0)||board[i+pl[0]-1][j+pl[0]])&&board[i+pl[1]-1][j+pl[1]])
				{
					points+=3;
				}
				break;
			}
		}
	}

	for (int j = 0; j < 4; j++)		//diagonal 2
	{
		for (int i = 3; i < 6; i++)
		{
			int point_u=0,point_m=0,zeroes=0,pl[2],t=0;
			for (int k = 0; k < 4; k++)
			{
				if (board[i-k][j+k]==1)
				{
					point_m++;
				}
				else if (board[i-k][j+k]==2)
				{
					point_u++;
				}
				else{
					zeroes++;
					if (zeroes<3)
					{
						pl[t]=k;
						t++;
					}
				}
			}
			if (zeroes>2)
			{
				break;
			}
			if(zeroes==1)
			{
				if (point_u==3&&((i==0&&pl[0]==0)||board[i-pl[0]-1][j+pl[0]]))
				{
					return 500;
				}
				else if (point_m==3&&((i==0&&pl[0]==0)||board[i-pl[0]-1][j+pl[0]]))
				{
					return -500;
				}
				if (point_u==3)
				{
					points+=20;
				}
				else if (point_m==3)
				{
					points-=20;
				}
			}
			if (zeroes==2)
			{
				if (point_m==2&&((i==0&&pl[0]==0)||board[i-pl[0]-1][j+pl[0]])&&board[i-pl[1]-1][j+pl[1]])
				{
					points-=3;
				}
				else if (point_u==2&&((i==0&&pl[0]==0)||board[i-pl[0]-1][j+pl[0]])&&board[i-pl[1]-1][j+pl[1]])
				{
					points+=3;
				}
				break;
			}
		}
	}
	return points;
}

int status(int board[][7])
{
	int zeroes=0;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 6; i++)
		{
			if (!board[i][j])
			{
				zeroes++;
				break;
			}
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
				else{
					zeroes++;
					j=k;
					break;
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

	for (int i = 0; i < 7; i++)
	{
		if (!board[3][i])
		{
			continue;
		}
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
				else{
					j=3;
					break;
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

	for (int j = 0; j < 4; j++)
	{
		if (!board[0][3])
		{
			break;
		}
		for (int i = 0; i < 3; i++)
		{
			int point_u=0,point_m=0,zz=0;
			for (int k = 0; k < 4; k++)
			{
				if (board[i+k][j+k]==1)
				{
					point_m++;
				}
				else if (board[k+i][j+k]==2)
				{
					point_u++;
				}
				else{
					// j+=k-1;
					i=3;
					break;
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

	for (int j = 0; j < 4; j++)
	{
		if (!board[0][3])
		{
			break;
		}
		for (int i = 3; i <6; i++)
		{
			int point_u=0,point_m=0,zz=0;
			for (int k = 0; k < 4; k++)
			{
				if (board[i-k][j+k]==1)
				{
					point_m++;
				}
				else if (board[i-k][j+k]==2)
				{
					point_u++;
				}
				else{
					// j+=k-1;
					i=7;
					break;
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

	if(zeroes)
		return 0;
	return 2;
}

int max(int board[][7],int state,int baap_min,int baap_max);

int min(int board[][7],int state,int baap_max,int baap_min)
{
	// cout << 5;
	int game=status(board);
	// cout << 4;
	if(game)
	{
		if (game==1)
		{
			return -999+state;
		}
		if (game==-1)
		{
			return 999-state;
		}
		else
			return 0;
	}

	if (state>=State)
	{
		return evaluate_me(board);
	}
	int array[7][2];
	for (int i = 0; i < 7; i++)
	{
		array[i][1]=i;
		int j=0;
		if (board[5][i])
		{
			array[i][0]=600;
			continue;
		}
		while(board[j][i])
			j++;
		board[j][i]=1;
		array[i][0]=evaluate_me(board);
		board[j][i]=0;
	}
	// sort(array.begin(),array.end(),comp);

	mergesort(array,0,6);

	int po,ppo;
	
	for (int i = 0; i < 7; i++)
	{
		int j=0;
		if (board[5][array[i][1]])
		{
			continue;
		}
		while(board[j][array[i][1]])
			j++;
		board[j][array[i][1]]=1;
		po=max(board,state+1,baap_min,baap_max);
		board[j][array[i][1]]=0;
		if (po==-999)
		{
			baap_min=po;
			ppo=array[i][1];
			break;
		}
		if (state>1&&po<=baap_max)
		{
			return po;
		}
		if (baap_min>po)
		{
			baap_min=po;
			ppo=array[i][1];
		}
	}
	if (state==1)
	{
		return ppo;
	}
	return baap_min;
}

int max(int board[][7],int state,int baap_min,int baap_max)
{
	int game=status(board);
	if(game)
	{
		if (game==1)
		{
			return -999+state;
		}
		if (game==-1)
		{
			return 999-state;
		}
		else
			return 0;
	}

	if (state>=State)
	{
		return evaluate_me(board);
	}
	int array[7][2];
	for (int i = 0; i < 7; i++)
	{
		array[i][1]=i;
		int j=0;
		if (board[5][i])
		{
			array[i][0]=-600;
			continue;
		}
		while(board[j][i])
			j++;
		board[j][i]=2;
		array[i][0]=evaluate_me(board);
		board[j][i]=0;
	}
	// sort(array.begin(),array.end(),comp);

	mergesort(array,0,6);

	int po;
	for (int i = 6; i >= 0; i--)
	{
		int j=0;
		if (board[5][array[i][1]])
		{
			continue;
		}
		while(board[j][array[i][1]])
			j++;
		board[j][array[i][1]]=2;
		po=min(board,state+1,baap_max,baap_min);
		board[j][array[i][1]]=0;
		if (po==999)
		{
			baap_max=po;
			//ppo=i;
			break;
		}
		if (po>=baap_min)
		{
			return po;
		}
		if (baap_max<po)
		{
			baap_max=po;
			//ppo=i;
		}
	}
	return baap_max;
}


int main()
{
	int board[6][7]={0};

	int game=status(board),move;
	//cout<<game;
	do
	{
		// count=0;
		cin>>move;
		int j=0;
		// cout<<1<<endl;
		while(board[j][move])
			j++;
		board[j][move]=2;
		// cout<<2<<endl;
		int my_move=min(board,1,0,1000);
		j=0;
		// cout<<count<<endl;
		while(board[j][my_move])
			j++;
		board[j][my_move]=1;
		//cout<<evaluate_me(board)<<endl;
		for (int i = 5; i >= 0; i--)
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
	} while(!game);
	return 0;
}
