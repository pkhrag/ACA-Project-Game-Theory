#include<iostream>

using namespace std;
int gcd(int a,int b)
{
	return b?gcd(b,a%b):a;
}
int main()
{
	int t;
	cin>>t;
	for(int i=0;i<t;i++){
		int n;
		cin>>n;
		int j,arr[n];
		int flag=0;
		cin>>arr[0];
		for(j=1;j<n;j++)
		{
			cin>>arr[j];
			if(arr[j]==1||gcd(arr[j-1],arr[j])==1)
			{
				flag=1;
				break;
			}
		}
		if(flag)
			cout<<n<<endl;
		else
			cout<<-1<<endl;
	}
}