#pragma once

#include <fstream>

namespace bfp
{
    enum class FileType
    {
        Text,
        Binary
    };


    class FileStream
    {
    public:
        virtual ~FileStream() = default;

        virtual bool Open(const char* filename, FileType type) = 0;
        virtual void Close() = 0;

        virtual void SeekToStart() = 0;
        virtual void SeekToEnd() = 0;
        virtual void Seek(int offset) = 0;

        virtual int GetPosition() const = 0;
        virtual int GetSize() const = 0;

        virtual bool IsOpen() const = 0;

    protected:
        int m_fileSize = 0;
    };


    class FileInputStream : public FileStream
    {
    public:
        FileInputStream() = default;

        FileInputStream(const char* filename, FileType type)
        {
            Open(filename, type);
        }

        ~FileInputStream() override;

        bool Open(const char* filename, FileType type) override;
        void Close() override;

        void SeekToStart() override;
        void SeekToEnd() override;
        void Seek(int offset) override;

        int GetPosition() const override;
        int GetSize() const override;

        bool IsOpen() const override;

        bool Read(void* buffer, int bytesToRead);

    private:
        std::ifstream m_file;
    };


    class FileOutputStream : public FileStream
    {
    public:
        FileOutputStream() = default;

        FileOutputStream(const char* filename, FileType type)
        {
            Open(filename, type);
        }

        ~FileOutputStream() override;

        bool Open(const char* filename, FileType type) override;
        void Close() override;

        void SeekToStart() override;
        void SeekToEnd() override;
        void Seek(int offset) override;

        int GetPosition() const override;
        int GetSize() const override;

        bool IsOpen() const override;

        bool Write(const void* buffer, int bytesToWrite);

    private:
        std::ofstream m_file;
    };
}