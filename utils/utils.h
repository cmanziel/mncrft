#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "glm_types.h"

// substitute data from one source array to multiple destination arrays
void substitute_uint8_t(uint8_t* source, uint8_t** dest, unsigned int* sizes, unsigned int source_size, unsigned int dest_size);

void substitute_float(float* source, float** dest, unsigned int* sizes, unsigned int source_size, unsigned int dest_size);

void mat4_to_float(mat4 mat, float* arr);

float to_radians(float angle);

//void substitute_VertexBuffer(std::vector<uint8_t> source, std::vector<VertexBuffer*> dest)
//{
//    unsigned int dest_index = 0;
//    unsigned int source_index = 0;
//    unsigned int arr_index = 0;
//
//    // set the m_CurrentDataSize field to zero because the previous number still refers to the previous vertices
//    dest[dest_index]->m_CurrentDataSize = 0;
//
//    while (dest_index < dest.size())
//    {
//        // current meshVB data
//        std::vector<uint8_t> data = dest[dest_index]->GetData();
//
//        if (source_index == source.size())
//            return;
//
//        data[arr_index] = source[source_index];
//
//        source_index++;
//        arr_index++;
//
//        dest[dest_index]->m_CurrentDataSize++;
//
//        if (arr_index == data.size())
//        {
//            dest_index++;
//            if(dest_index < dest.size())
//                dest[dest_index]->m_CurrentDataSize = 0;
//            arr_index = 0;
//        }
//    }
//}
//
//void substitute_TextureBuffer(std::vector<float> source, std::vector<TextureBuffer*> dest)
//{
//    unsigned int dest_index = 0;
//    unsigned int source_index = 0;
//    unsigned int arr_index = 0;
//
//    dest[dest_index]->m_CurrentDataSize = 0;
//
//    while (dest_index < dest.size())
//    {
//        std::vector<float> data = dest[dest_index]->GetData();
//
//        if (source_index == source.size())
//            return;
//
//        data[arr_index] = source[source_index];
//
//        source_index++;
//        arr_index++;
//
//        dest[dest_index]->m_CurrentDataSize++;
//
//        if (arr_index == data.size())
//        {
//            dest_index++;
//            if(dest_index < dest.size())
//                dest[dest_index]->m_CurrentDataSize = 0;
//            arr_index = 0;
//        }
//    }
//}