#pragma once

namespace bfp
{
    enum class EndianType
    {
        Unknown,
        Little,
        Big,
        Middle
    };


    class Endian
    {
    public:
        Endian();

        EndianType GetType() const;

        void Resolve(
            void* data,
            int size,
            EndianType inputEndian
        );

        void SwapBytes(
            void* data,
            int size
        );

        void SwapBytes(
            void* data,
            int size,
            int number
        );

    private:
        EndianType m_systemEndian;
    };
}