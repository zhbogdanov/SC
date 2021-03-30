#include <iostream>

using namespace std;

void ParallelBubbleSort(int* arr, int size)
{
        for (int i = 0; i < size; ++i)
                {
                        int tmp = 0;
                        if (i % 2 == 0)
                        {
                                #pragma omp parallel for private(tmp)
                                for (int j = 0; j < size; j += 2)
                                        if (j < size - 1)
                                                if(arr[j] > arr[j+1])
                                                {
                                                        tmp = arr[j];
                                                        arr[j] = arr[j+1];
                                                        arr[j+1] = tmp;
                                                }
                        }
                        else
                        {
                                #pragma omp parallel for private(tmp)
                                for (int j = 1; j < size; j += 2)
                                        if (j < size - 1)
                                                if(arr[j] > arr[j+1])
                                                {
                                                        tmp = arr[j];
                                                        arr[j] = arr[j+1];
                                                        arr[j+1] = tmp;
                                                }
                        }
                }
}

int main(void)
{
        int size, elem;
        cin >> size;
        int *arr;
        arr = new int[size];
        for (int i = 0; i < size; ++i)
        {
                cin >> elem;
                arr[i] = elem;
        }
	ParallelBubbleSort(arr, size);
        for (int i = 0; i < size; ++i)
        {
                cout << arr[i] << ' ';
        }
}
