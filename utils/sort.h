#pragma once

#define X_COORD 0
#define Y_COORD 1
#define Z_COORD 2

void insertion_sort(int* arr, int start, int end)
{
    // ordine crescente

    int i = start;

    while (i <= end)
    {
        if (i > start)
        {
            int current = arr[i];
            int j = i - 1;

            while (j >= start && arr[j] > current)
            {
                arr[j + 1] = arr[j];

                j--;
            }

            arr[j + 1] = current;
        }

        i++;
    }
}

// sort array of multiple vec3
void insertion_sort_vector(vec3* vec, int start, int end, int coord)
{
    int i = start;

    while (i <= end)
    {
        if (i > start)
        {
            vec3 current = vec[i];
            int j = i - 1;

            while (j >= start && vec[j][coord] > current[coord])
            {
                vec[j + 1] = vec[j];

                j--;
            }

            vec[j + 1] = current;
        }

        i++;
    }
}

// sort vector of chunks

// TODO:
// try using copy constructor on chunk

void insertion_sort_chunks(std::vector<Chunk*> chunks, int start, int end, int coord)
{
    int i = start;

    while (i <= end)
    {
        if (i > start)
        {
            //Chunk* currentChunk = (Chunk*)malloc(sizeof(Chunk*));

            Chunk* currentChunk = new Chunk(*chunks[i], chunks[i]->GetPlayer());

            int j = i - 1;

            while (j >= start && chunks[j]->GetPosition()[coord] > currentChunk->GetPosition()[coord])
            {
                *chunks[j + 1] = *chunks[j];

                j--;
            }

            // construct an operator= function otherwise the fields of the chunks that are pointers will still be assigned as pointers
            *chunks[j + 1] = *currentChunk;
            
            delete currentChunk;
        }

        i++;
    }
}

// this implem raises a problem because the destructor of currentChunk instance gets called when it goes out of scope