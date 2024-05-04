#include "utils.h"

// substitute data from one source array to multiple destination arrays
void substitute_uint8_t(uint8_t* source, uint8_t** dest, unsigned int* sizes, unsigned int source_size, unsigned int dest_size)
{
    unsigned int dest_index = 0;
    unsigned int source_index = 0;
    unsigned int arr_index = 0;

    while (dest_index < dest_size)
    {
        if (source_index == source_size)
            return;

        dest[dest_index][arr_index] = source[source_index];

        source_index++;
        arr_index++;

        if (arr_index == sizes[dest_index])
        {
            dest_index++;
            arr_index = 0;
        }
    }
}

void substitute_float(float* source, float** dest, unsigned int* sizes, unsigned int source_size, unsigned int dest_size)
{
    unsigned int dest_index = 0;
    unsigned int source_index = 0;
    unsigned int arr_index = 0;

    while (dest_index < dest_size)
    {
        if (source_index == source_size)
            return;

        dest[dest_index][arr_index] = source[source_index];

        source_index++;
        arr_index++;

        if (arr_index == sizes[dest_index])
        {
            dest_index++;
            arr_index = 0;
        }
    }
}

void mat4_to_float(mat4 mat, float* arr)
{
    int arr_index = 0;

    for (int c = 0; c < 4; c++)
    {
        for (int r = 0; r < 4; r++)
        {
            arr[arr_index] = mat[c][r];
            arr_index++;
        }
    }
}

float to_radians(float angle)
{
    float angle_rad;

    double pi = 3.141592654;

    angle_rad = angle * pi / 180;

    return angle_rad;
}

float point_plane_distance(vec3 plane_origin, vec3 plane_normal, vec3 point)
{
    float a = plane_normal.x;
    float b = plane_normal.y;
    float c = plane_normal.z;

    float d = -(a * plane_origin.x + b * plane_origin.y + c * plane_origin.z);

    float dist = abs((long)(a * point.x + b * point.y + c * point.z + d)) / sqrt(a * a + b * b + c * c);

    return dist;
}

float dot_product(const vec3 u, const vec3 v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

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