#pragma once

#include "file.h"

#include <cstdint>
#include <string>
#include <vector>

namespace bfp
{
    constexpr char ARCHIVE_ID[4] =
    {
        'B', 'F', 'A', '1'
    };

    constexpr uint32_t ARCHIVE_MAJOR = 1;
    constexpr uint32_t ARCHIVE_MINOR = 0;


    struct ArchiveFileHeader
    {
        std::string fileName;

        uint32_t size = 0;
        uint32_t offset = 0;
    };


    class Archive
    {
    public:

        Archive() = default;
        ~Archive();


        // --------------------------------------------------------
        // Create
        // --------------------------------------------------------

        bool Create(
            const char* archiveFile,
            const char* rootDirectory,
            const std::vector<std::string>& files
        );


        // --------------------------------------------------------
        // Open / Close
        // --------------------------------------------------------

        bool Open(
            const char* archiveFile
        );

        void Close();

        bool IsOpen() const;


        // --------------------------------------------------------
        // Information
        // --------------------------------------------------------

        int GetFileCount() const;


        int FindFile(
            const char* fileName
        ) const;


        bool GetFileHeader(
            int index,
            ArchiveFileHeader& header
        ) const;


        // --------------------------------------------------------
        // Read
        // --------------------------------------------------------

        bool ReadFile(
            int index,
            void* buffer,
            int bufferSize
        );


        bool ReadFile(
            const char* fileName,
            void* buffer,
            int bufferSize
        );


    private:

        bool ReadHeader();


        bool WriteHeader(
            const std::vector<ArchiveFileHeader>& headers
        );


    private:

        FileInputStream m_file;

        std::vector<ArchiveFileHeader> m_headers;
    };
}