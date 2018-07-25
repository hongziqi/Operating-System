#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <random>
#include <memory.h>

using namespace std;

//检查是否排好序
bool is_sort (int a[] , int n)
{
    for(int i=1;i<n;i++)
        if(a[i]<a[i-1]) return false;
    
    return true;
}
//合并数组函数
void merge_array(int a[], int first, int mid, int last, int temp[])
{
    int i = first, j = mid + 1;  
    int k = 0; 
    while (i <= mid && j <= last) {
        if (a[i] < a[j])
        {
            temp[k++] = a[i++];
        }else
            temp[k++] = a[j++];
    }
    //前半段多余
    while(i <= mid)
        temp[k++] = a[i++];
    //后半段多余
    while(j <= last)
        temp[k++] = a[j++];

     for (i = 0; i < k; i++)  
        a[first + i] = temp[i];

}

//递归分组，排序
void merge_sort(int array[], int first, int last, int temp[])
{
      if (first < last)  
    {  
        int mid = (first + last) / 2;  
        merge_sort(array, first, mid, temp);
        merge_sort(array, mid + 1, last, temp);  
        merge_array(array, first, mid, last, temp); 
    } 
}

void merge_two_thread(int array1[], int size1, int array2[], int size2, int total[])
{
    int i, j, k;  
  
    i = j = k = 0;  
    while (i < size1 && j < size2)  
    {  
        if (array1[i] < array2[j])  
            total[k++] = array1[i++];  
        else  
            total[k++] = array2[j++];   
    }  
  
    while (i < size1)  
        total[k++] = array1[i++];  
  
    while (j < size2)  
        total[k++] = array2[j++]; 
}

void merge_start(int array[],int size)
{
    //double
    int* p1 = new int[size / 2 + 1]; 
    int* p2 = new int[size / 2 + 1]; 
    int* sa1 = new int[size / 2 + 1];
    int* sa2 = new int[size / 2 + 1];
    //single
    int* sa = new int[size + 1];

    
    clock_t start_time = clock();

    for(int i = 0; i < size / 2; i++)
        sa1[i] = array[i];
    for(int i = size / 2; i < size; i++)
        sa2[i - (size / 2)] = array[i];

   
    thread a(merge_sort, sa1, 0, size / 2 - 1, p1);        
    thread b(merge_sort, sa2, 0, size - size / 2 - 1, p2);
    a.join();
    b.join(); 

    memset(sa, 0, size + 1);
    //排完之后将两个有序的数组进行合并
    merge_two_thread(sa1, size / 2, sa2, size - (size / 2), sa);

    clock_t double_end_time = clock();
    //****************double************//

    if(is_sort(sa, size)){
        cout << "double threads sort :" << endl;
        cout << "running time is " << (double_end_time - start_time) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
    } 
    else
        cout << "Fail" << endl;
        


    memset(sa, 0, size + 1);

    merge_sort(array, 0, size - 1, sa);

    clock_t single_end_time = clock();
    //****************single************//

    if(is_sort(sa, size)){
        cout << "Single thread sort : " << endl;
        cout << "running time is " << (single_end_time - double_end_time) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
    }
    else
        cout << "Fail" << endl;

    delete []p1;
    delete []p2;
    delete []sa1;
    delete []sa2;
    delete []sa;
}



int main()
{
    int n = 10000000;
    int* number = new int[n+1];
    for(int i=0;i<n;i++)
    {
        number[i] = rand() % 10000000;
    }

    merge_start(number, n);

    delete []number;
    return 0;
}
