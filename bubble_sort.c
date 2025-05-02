#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_SIZE 100

int original_list[MAX_SIZE];
int sorted_list[MAX_SIZE];
int n;

typedef struct
{
    int start;
    int end;
} thread_data;

void bubble_sort(int arr[], int start, int end)
{
    for (int i = start; i < end; i++)
    {
        for (int j = start; j < start + end - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void *sorting_thread(void *arg)
{
    thread_data *data = (thread_data *)arg;
    bubble_sort(original_list, data->start, data->end);
    pthread_exit(NULL);
}

void *merge_thread(void *arg)
{
    thread_data *data = (thread_data *)arg;
    int mid = data->start;
    int end = data->end;

    int i = 0;
    int j = mid;
    int k = 0;

    while (i < mid && j < end)
    {
        if (original_list[i] <= original_list[j])
        {
            sorted_list[k++] = original_list[i++];
        }
        else
        {
            sorted_list[k++] = original_list[j++];
        }
    }

    while (i < mid)
    {
        sorted_list[k++] = original_list[i++];
    }

    while (j < end)
    {
        sorted_list[k++] = original_list[j++];
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[3];
    thread_data data[2];

    printf("Enter the number of elements (max %d): ", MAX_SIZE);
    scanf("%d", &n);

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &original_list[i]);
    }

    printf("\nOriginal list:\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", original_list[i]);
    }
    printf("\n");

    int mid = n / 2;

    data[0].start = 0;
    data[0].end = mid;
    pthread_create(&threads[0], NULL, sorting_thread, (void *)&data[0]);

    data[1].start = mid;
    data[1].end = n;
    pthread_create(&threads[1], NULL, sorting_thread, (void *)&data[1]);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    printf("\nSorting thread 1 (first half):\n");
    for (int i = 0; i < mid; i++)
    {
        printf("%d ", original_list[i]);
    }

    printf("\nSorting thread 2 (second half):\n");
    for (int i = mid; i < n; i++)
    {
        printf("%d ", original_list[i]);
    }
    printf("\n");

    thread_data merge_data;
    merge_data.start = mid;
    merge_data.end = n;
    pthread_create(&threads[2], NULL, merge_thread, (void *)&merge_data);

    pthread_join(threads[2], NULL);

    printf("\nMerge thread result (sorted list):\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", sorted_list[i]);
    }
    printf("\n");

    return 0;
}
