#include<iostream>
using namespace std;
#include<ctime>
#include<algorithm>
#include<string>

const unsigned int Size = 20;
int arr[Size];
int arr_c[Size];

bool check(int arr[], int arr_c[], const int size) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] != arr_c[i]) return false;
    }
    return true;
}
void random(int arr[], int arr_c[], const int size, const int min, const int max) {
    for (int i = 0; i < size; ++i) {
        arr[i] = arr_c[i] = rand() % (max - min + 1) + min;
    }
}
void print(int arr[], const int size) {
    for (int i = 0; i < size; ++i) {
        if (i) cout << ' ';
        cout << arr[i];
    }
    cout << endl;
}
void swap(int arr[], const int i, const int j) {
    if (i == j) return;
    arr[i] ^= arr[j];
    arr[j] ^= arr[i];
    arr[i] ^= arr[j];
}

//冒泡排序  O(n^2)
void bubble_sort(int arr[], const int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) swap(arr, j, j + 1);
        }
    }
}
//选择排序  O(n^2)
void selection_sort(int arr[], const int size) {
    for (int i = 0; i < size; ++i) {
        int min = i;
        for (int j = i; j < size; ++j) {
            if (arr[j] < arr[min]) min = j;
        }
        swap(arr, min, i);
    }
}
//插入排序  O(n^2)
void insertion_sort(int arr[], const int size) {
    for (int i = 1; i < size; ++i) {
        for (int j = i; j > 0; --j) {
            if (arr[j] < arr[j - 1]) swap(arr, j, j - 1);
        }
    }
}
//归并排序  O(nlogn)
void merge(int arr[], const int left, const int mid, const int right) {
    int* help = new int[right - left + 1];  //开辟临时空间
    int p1 = left, p2 = mid + 1, p = 0;
    while (p1 <= mid && p2 <= right) 
        help[p++] = (arr[p1] <= arr[p2] ? arr[p1++] : arr[p2++]);
    while (p1 <= mid) help[p++] = arr[p1++];
    while (p2 <= right) help[p++] = arr[p2++];
    for (int i = 0; i < p; ++i) arr[left + i] = help[i];
    delete[] help;  //释放临时空间
}
void merge_sort(int arr[], const int left, const int right) {
    if (left >= right) return;
    int mid = left + ((right - left) >> 1);
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}
//快速排序  O(nlogn)
void quick_sort(int arr[], const int left, const int right) {
    if (left >= right) return;
    int rp = left + rand() % (right - left + 1);
    swap(arr, rp, right);
    int num = arr[right];  //基准数
    int l = left - 1, r = right;  //左右的范围
    for (int i = left; i < r;) {
        if (arr[i] < num) swap(arr, i++, ++l);
        else if (arr[i] > num) swap(arr, i, --r);
        else ++i;
    }
    swap(arr, right, r++);
    quick_sort(arr, left, l);
    quick_sort(arr, r, right);
}
//堆排序  O(nlogn)
void heapify(int arr[], int index, const int heapsize) {
    int left = (index << 1) + 1;
    while (left < heapsize) {
        int largest = (left + 1 < heapsize && arr[left] < arr[left + 1]) ? left + 1 : left;
        largest = arr[largest] > arr[index] ? largest : index;
        if (index == largest) break;
        swap(arr, index, largest);
        index = largest;
        left = (index << 1) + 1;
    }
}
void heapinsert(int arr[], int index) {
    while (arr[index] > arr[(index - 1) >> 1]) {
        swap(arr, index, (index - 1) >> 1);
        index = (index - 1) >> 1;
    }
}
void heap_sort(int arr[], int heapsize) {
    for (int i = (heapsize >> 1) - 1; i >= 0; --i) {
        heapify(arr, i, heapsize);
    }
    while (heapsize > 1) {
        swap(arr, 0, --heapsize);
        heapify(arr, 0, heapsize);
    }
}
//计数排序  O(n)
void counting_sort(int arr[], const int size, const int min, const int max) {
    const int helpsize = max - min + 1;
    int* help = new int[helpsize];
    for (int i = 0; i < helpsize; ++i) help[i] = 0;
    for (int i = 0; i < size; ++i) ++help[arr[i]];
    int p = 0;
    for (int i = 0; i < helpsize; ++i) {
        for (int j = 0; j < help[i]; ++j) {
            arr[p++] = i;
        }
    }
}
//基数排序  O(kn)
int digit(int x) {
    int t = 0;
    while (x) {
        x /= 10;
        ++t;
    }
    return t;
}
int getDigit(int x, const int d) {
    return x / int(pow(10, d - 1)) % 10;
}
void radix_sort(int arr[], const int left, const int right) {
    int* help = new int[right - left + 1];
    int max = arr[0];
    for (int i = left; i <= right; ++i) {
        if (max < arr[i]) max = arr[i];
    }
    int maxDigit = digit(max);
    for (int dig = 1; dig <= maxDigit; ++dig) {
        int num[10] = { 0 };
        for (int i = left; i <= right; ++i) {
            ++num[getDigit(arr[i], dig)];
        }
        for (int i = 1; i < 10; ++i) {
            num[i] += num[i - 1];
        }
        for (int i = right; i >= left; --i) {
            help[--num[getDigit(arr[i], dig)]] = arr[i];
        }
        for (int i = left, j = 0; i <= right; ++i, ++j) {
            arr[i] = help[j];
        }
    }
    delete[] help;
}

//获取数组大小
template<typename T, size_t Size>
size_t size(T arr[Size]) {
    return Size;
}

/*
int main()
{
    int t = 0;
    const int index = 100;
    srand((unsigned int)time(NULL));
    for (int i = 1; i <= index; ++i) {
        random(arr, arr_c, Size, 0, 9);  //随机生成下一组
        radix_sort(arr, 0, Size - 1);  //我的排序
        sort(arr_c, arr_c + Size);  //std的排序
        string mode;
        if (check(arr, arr_c, Size)) {
            mode = "正确";
            ++t;
        }
        else mode = "错误";
        cout << "第 " << (i < 10 ? "0" + to_string(i) : to_string(i)) << " 组: " << mode;  //检查结果是否相同
        if (i % 4 == 0) cout << endl;
    }

    cout << "总共测试 " << index << " 组数据，有 " << t << " 组数据正确" << endl;
    cout << "正确率: " << t << '/' << index << endl;

    return 0;
}
*/