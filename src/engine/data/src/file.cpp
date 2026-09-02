#include "../include/file.h"

namespace bfp
{
    // ============================================================
    // FileInputStream
    // ============================================================

    FileInputStream::~FileInputStream()
    {
        Close();
    }


    bool FileInputStream::Open(const char* filename, FileType type)
    {
        if (filename == nullptr)
            return false;

        if (IsOpen())
            Close();

        std::ios::openmode mode = std::ios::in;

        if (type == FileType::Binary)
            mode |= std::ios::binary;

        m_file.open(filename, mode);

        if (!m_file.is_open())
        {
            m_fileSize = 0;
            return false;
        }

        m_file.seekg(0, std::ios::end);

        std::streampos endPosition = m_file.tellg();

        if (endPosition < 0)
        {
            Close();
            return false;
        }

        m_fileSize = static_cast<int>(endPosition);

        m_file.seekg(0, std::ios::beg);

        return true;
    }


    void FileInputStream::Close()
    {
        if (m_file.is_open())
            m_file.close();

        m_fileSize = 0;
    }


    void FileInputStream::SeekToStart()
    {
        if (!IsOpen())
            return;

        m_file.clear();
        m_file.seekg(0, std::ios::beg);
    }


    void FileInputStream::SeekToEnd()
    {
        if (!IsOpen())
            return;

        m_file.clear();
        m_file.seekg(0, std::ios::end);
    }


    void FileInputStream::Seek(int offset)
    {
        if (!IsOpen())
            return;

        m_file.clear();
        m_file.seekg(offset, std::ios::cur);
    }


    int FileInputStream::GetPosition() const
    {
        if (!IsOpen())
            return -1;

        std::streampos position =
            const_cast<std::ifstream&>(m_file).tellg();

        if (position < 0)
            return -1;

        return static_cast<int>(position);
    }


    int FileInputStream::GetSize() const
    {
        return m_fileSize;
    }


    bool FileInputStream::IsOpen() const
    {
        return m_file.is_open();
    }


    bool FileInputStream::Read(void* buffer, int bytesToRead)
    {
        if (!IsOpen() ||
            buffer == nullptr ||
            bytesToRead <= 0)
        {
            return false;
        }

        m_file.read(
            static_cast<char*>(buffer),
            bytesToRead
        );

        return m_file.good() ||
               m_file.gcount() == bytesToRead;
    }


    // ============================================================
    // FileOutputStream
    // ============================================================

    FileOutputStream::~FileOutputStream()
    {
        Close();
    }


    bool FileOutputStream::Open(
        const char* filename,
        FileType type)
    {
        if (filename == nullptr)
            return false;

        if (IsOpen())
            Close();

        std::ios::openmode mode =
            std::ios::out |
            std::ios::trunc;

        if (type == FileType::Binary)
            mode |= std::ios::binary;

        m_file.open(filename, mode);

        if (!m_file.is_open())
        {
            m_fileSize = 0;
            return false;
        }

        m_fileSize = 0;

        return true;
    }


    void FileOutputStream::Close()
    {
        if (m_file.is_open())
            m_file.close();

        m_fileSize = 0;
    }


    void FileOutputStream::SeekToStart()
    {
        if (!IsOpen())
            return;

        m_file.clear();
        m_file.seekp(0, std::ios::beg);
    }


    void FileOutputStream::SeekToEnd()
    {
        if (!IsOpen())
            return;

        m_file.clear();
        m_file.seekp(0, std::ios::end);
    }


    void FileOutputStream::Seek(int offset)
    {
        if (!IsOpen())
            return;

        m_file.clear();
        m_file.seekp(offset, std::ios::cur);
    }


    int FileOutputStream::GetPosition() const
    {
        if (!IsOpen())
            return -1;

        std::streampos position =
            const_cast<std::ofstream&>(m_file).tellp();

        if (position < 0)
            return -1;

        return static_cast<int>(position);
    }


    int FileOutputStream::GetSize() const
    {
        return m_fileSize;
    }


    bool FileOutputStream::IsOpen() const
    {
        return m_file.is_open();
    }


    bool FileOutputStream::Write(
        const void* buffer,
        int bytesToWrite)
    {
        if (!IsOpen() ||
            buffer == nullptr ||
            bytesToWrite <= 0)
        {
            return false;
        }

        m_file.write(
            static_cast<const char*>(buffer),
            bytesToWrite
        );

        if (!m_file.good())
            return false;

        m_fileSize += bytesToWrite;

        return true;
    }
}