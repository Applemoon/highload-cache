#include <iostream>
#include <x86intrin.h>

using namespace std;

#define TWICE(x) x x

struct Node
{
    int data;
    Node* next;
};

void getRandomList(Node* list, size_t node_count)
{
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
            Node* next = list + shift;
            if (!next->next)
            {
                current->next = next;
                current = next;
                next_found = true;
            }
        }
    }
}

void getOrderedList(Node* list, size_t node_count)
{
    Node* current = list;
    for (int i = 0; i < node_count; i++)
    {
        current->data = i+1;
        if (i != node_count-1)
        {
            current->next = list + i + 1;
        }
        else
        {
            current->next = list;
        }
        current = current->next;
    }
}

int walk(void* list, size_t steps)
{
    Node* current = (Node *) list;
    uint64_t start = __rdtsc();
    for (int i = 0; i < steps; ++i) 
    {
        TWICE(TWICE(TWICE(TWICE(TWICE(TWICE(TWICE(TWICE(current = current->next;))))))))
    }
    return (__rdtsc() - start) / (steps * 256);
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    Node* list = NULL;
    const size_t steps = 1 << 15;

    for (int node_count_power = 1; node_count_power < 21; node_count_power++)
    {
        const size_t node_count = (size_t) (1 << node_count_power);
        const size_t node_size = sizeof(Node);
        list = (Node*)calloc(node_count, node_size);
        cout << node_size * node_count / 1024;

        if (list != NULL)
        {
            getRandomList(list, node_count);
            cout << " " << walk(list, steps);

            getOrderedList(list, node_count);
            cout << " " << walk(list, steps) << endl;

            free(list);
        }
    }

    return 0;
}