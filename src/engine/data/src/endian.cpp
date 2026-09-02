#include "../include/endian.h"

#include <cassert>
#include <cstdint>

namespace bfp
{
    Endian::Endian()
        : m_systemEndian(EndianType::Unknown)
    {
        uint32_t data = 0x12345678;

        const uint8_t* bytes =
            reinterpret_cast<const uint8_t*>(&data);

        if (bytes[0] == 0x78 &&
            bytes[1] == 0x56 &&
            bytes[2] == 0x34 &&
            bytes[3] == 0x12)
        {
            m_systemEndian = EndianType::Little;
        }
        else if (bytes[0] == 0x12 &&
                 bytes[1] == 0x34 &&
                 bytes[2] == 0x56 &&
                 bytes[3] == 0x78)
        {
            m_systemEndian = EndianType::Big;
        }
        else if (bytes[0] == 0x34 &&
                 bytes[1] == 0x12 &&
                 bytes[2] == 0x78 &&
                 bytes[3] == 0x56)
        {
            m_systemEndian = EndianType::Middle;
        }
    }


    EndianType Endian::GetType() const
    {
        return m_systemEndian;
    }


    void Endian::Resolve(
        void* data,
        int size,
        EndianType inputEndian)
    {
        if (data == nullptr ||
            size <= 1)
        {
            return;
        }

        if (m_systemEndian == EndianType::Unknown ||
            inputEndian == EndianType::Unknown ||
            m_systemEndian == inputEndian)
        {
            return;
        }

        char* bytes =
            static_cast<char*>(data);

        int half = size / 2;


        // --------------------------------------------------------
        // Middle ↔ Big
        // --------------------------------------------------------

        if ((m_systemEndian == EndianType::Middle &&
             inputEndian == EndianType::Big) ||
            (m_systemEndian == EndianType::Big &&
             inputEndian == EndianType::Middle))
        {
            SwapBytes(bytes, half);
            SwapBytes(bytes + half, half);

            return;
        }


        // --------------------------------------------------------
        // Middle ↔ Little
        // --------------------------------------------------------

        if ((m_systemEndian == EndianType::Middle &&
             inputEndian == EndianType::Little) ||
            (m_systemEndian == EndianType::Little &&
             inputEndian == EndianType::Middle))
        {
            SwapBytes(bytes, size);
            SwapBytes(bytes, half);
            SwapBytes(bytes + half, half);

            return;
        }


        // --------------------------------------------------------
        // Little ↔ Big
        // --------------------------------------------------------

        SwapBytes(bytes, size);
    }


    void Endian::SwapBytes(
        void* data,
        int size)
    {
        if (data == nullptr ||
            size <= 1)
        {
            return;
        }

        char* bytes =
            static_cast<char*>(data);

        for (int i = 0, j = size - 1;
             i < j;
             ++i, --j)
        {
            char temp = bytes[i];

            bytes[i] = bytes[j];
            bytes[j] = temp;
        }
    }


    void Endian::SwapBytes(
        void* data,
        int size,
        int number)
    {
        if (data == nullptr ||
            size <= 1 ||
            number <= 0)
        {
            return;
        }

        char* bytes =
            static_cast<char*>(data);

        for (int i = 0; i < number; ++i)
        {
            SwapBytes(bytes, size);

            bytes += size;
        }
    }
}