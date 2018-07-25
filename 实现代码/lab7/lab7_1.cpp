#include <bits/stdc++.h>
#include <iostream>
#include <cstdio>
using namespace std;

vector<char> frame;	// 页表
int page_fault;	// 页表错误
bool B_inside;	// 1:in; 0: not in

void _printf(string oper)
{
	if (oper == "get")
	{
		char B;
		cin >> B;
		if (find(frame.begin(), frame.end(), B) == frame.end())	
			B_inside = 0;
		cout << B_inside << endl;
	}
	else if (oper == "pf")
		cout << page_fault << endl;
	else	//oper == 'seq'
	{
		// cout << '*';
		for (int i = 0; i < frame.size(); ++i)
			cout << frame[i];
		cout << endl;
	}
}

int main()
{
	int n; //frame的长度
	cin >> n;
	string request_page;	//	请求页序列
	cin >> request_page;
	int k;	//k次查询操作
	cin >> k;
	while(k--)
	{
		int i = 0;			//遍历的请求序列的位置
		int choice;	// 算法选择
		string oper;	// 操作：seq，pf，get
		int A;
		page_fault = 0;
		B_inside = 1;
		frame.clear();	
		cin >> choice >> oper >> A;

		int count[10] = {0}; //0~9
		
		while(frame.size() < n && i < A)//对于一开始的页表来说，如果三个算法的操作都是一样的
		{

			if (find(frame.begin(), frame.end(), request_page[i]) != frame.end())
				i++;	//can find
			
			else		//can not
			{
				page_fault++;
				frame.push_back(request_page[i++]);

			}
			count[request_page[i]-'0'] = i;
		}
		if (choice == 1){
			while (i < A)
			{
				//cout << "--------------" << endl;
				if (find (frame.begin(), frame.end(), request_page[i]) != frame.end())
					i++;	//can find
				else		//can not
				{
					page_fault++;
					frame.erase(frame.begin());
					frame.push_back(request_page[i++]);
				}
			}
			_printf(oper);
		}

		else if (choice == 2)
		{
			while (i < A)
			{

				if (find (frame.begin(), frame.end(), request_page[i]) != frame.end());
			
				else		//can not
				{
					int temp;	//保存最近最少请求的页，表现为记录时间小
					int min = 1000;   //初始化min
					for (int j = 0; j < n; ++j)
						if (min > count[frame[j] - '0'])
						{
							min = count[frame[j] - '0'];
							temp = frame[j];	//记录时间小的那个页项
						}
					for (int j = 0; j < n; ++j)
						if (frame[j] == temp){	//找到时间最小的页的位置，将其替换成请求页
							frame[j] = request_page[i];
							break;
						}
					
					page_fault++;
				}
				count[request_page[i]-'0'] = i;
				i++;
			}
			_printf(oper);
		}

		else //(choice == 3)
		{
			while (i < A)
			{
				
				if (find (frame.begin(), frame.end(), request_page[i]) != frame.end());
		
				else		//can not
				{
					int max = -1; 
					int loc;
					int min = 100;
					vector <char> temp;	//用于存储之后不出现的请求的页
					temp.clear();	//每次请求都需要清空
				
					for (int j = 0; j < n; ++j)	//遍历整个页表
					{
						for (int p = i; p < request_page.size(); ++p)	//遍历后面请求的序列
						{
							if (frame[j] != request_page[p] && p == request_page.size() -1) //找不到的页插入temp中。
								temp.push_back(frame[j]);
							else if (frame[j] == request_page[p])
							{
								if (max < p)
								{
									max = p;
									loc = j;	//记录页表项需要被替换的位置。
								}
								break;
							}
							
						}
					}
					if (!temp.empty())	//先替换以后都不出现的请求的页（根据最先进入frame的顺序去替换）
					{
						char tihuan;
						for (int j = 0; j < temp.size(); ++j)
							if (min > count[temp[j] - '0']){
								min = count[temp[j] - '0'];
								tihuan = temp[j];
							}
						for (int j = 0; j < n; ++j)
							if (frame[j] == tihuan)
							{
								frame[j] = request_page[i];
								break;
							}
					}
					else	frame[loc] = request_page[i];	//再替换之后出现最晚的请求的页
					page_fault++;
					
				}
				count[request_page[i]-'0'] = i;
				i++;	//can find
			}
			_printf(oper);	
		}
	}
	
	return 0;
}