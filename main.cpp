#include <iostream>
#include <x86intrin.h>

using namespace std;

struct Node
{
    int data;
    Node* next;
};

Node* getOrderedList(size_t node_count)
{
    Node* list = (Node*)calloc(node_count, sizeof(Node));
    Node* current = list;
    for (int i = 0; i < node_count; i++)
    {
        current->data = i+1;
        if (i != node_count-1)
        {
            current->next = &list[i + 1];
        }
        else
        {
            current->next = list;
        }
        current = current->next;
    }

    return list;
}

Node* getRandomList(size_t node_count)
{
    Node* list = (Node*)calloc(node_count, sizeof(Node));
    Node* current = list;
    for (int i = 0; i < node_count; i++)
    {
        current->data = i + 1;

        if (i == node_count - 1)
        {
            current->next = list;
            continue;
        }

        bool next_found = false;
        while (!next_found)
        {
            size_t shift = rand() % node_count;
            Node* next = &list[shift];
            if (next->data == 0)
            {
                current->next = next;
                current = next;
                next_found = true;
            }
        }
    }

    return list;
}

double walk(Node* list, size_t steps)
{
    Node* current = list;

//    clock_t start = clock();
    uint64_t start = __rdtsc();

    for (int i = 0; i < steps; ++i)
    {
        current->data++;
        current = current->next;
    }
//        return (double)(clock() - start)*1000000/(CLOCKS_PER_SEC*steps);
    return (__rdtsc() - start)/steps;
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    Node* list = NULL;
    for (int i = 1; i < 24; i++)
    {
        const int node_count_power = i;
        const size_t node_count = (size_t)(1 << node_count_power);
        const size_t steps = node_count;

        list = getRandomList(node_count);
        cout << node_count << ": " << walk(list, steps) << " ";

        list = getOrderedList(node_count);
        cout << walk(list, steps) << endl;
    }

    if (list != NULL)
    {
        free(list);
    }
    return 0;
}