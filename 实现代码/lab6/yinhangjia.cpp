#include <iostream>
#include <cstring>

using namespace std;
#define MAX 21
int AVAILABLE[MAX];
int ALLOCATION[MAX][MAX];
int NEED[MAX][MAX];
int safety[MAX];
int n, m; //n线程数量; m资源种类

int bankman ()
{
	bool finish[MAX];	//表示线程是否被分配
	int count = 0;	//计算分配的线程数量
	bool flag = 0;	//找到是否有线程需求的资源数量大于所提供的资源数量
	memset (finish, 0, sizeof(finish));

	for (int k = 0; k < n; ++k)
		for (int i = 0; i < n; ++i)
		{
			flag = 0;
			if (!finish[i]){
				for (int j = 0; j < m; ++j)
					if (NEED[i][j] > AVAILABLE[j]){
						flag = 1;
						break;
					}

				if (flag == 0)
				{
					safety[count++]=i;
					finish[i] = 1;
					for (int j = 0; j < m; ++j)
						AVAILABLE[j] += ALLOCATION[i][j];
					break;
				}
			}
		}
	
	return count;
}

int main(int argc, char const *argv[])
{
	int t;
	cin >> t;
	while (t--){	
		cin >> n >> m;
		for (int i = 0; i < m; ++i)
			cin >> AVAILABLE[i];

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
				cin >> ALLOCATION[i][j];

		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
				cin >> NEED[i][j];

		if (n != bankman())
			cout << "No" << endl;
		else{
			cout << "Yes";
            for(int i = 0;i < n; ++i)
            {
                cout << " " << safety[i];
            }
            cout << endl;
		}
	}

	return 0;
}