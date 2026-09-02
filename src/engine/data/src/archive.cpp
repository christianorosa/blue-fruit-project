#include "../include/archive.h"

#include <cstring>
#include <filesystem>
#include <limits>


namespace bfp
{
    namespace
    {
        // ========================================================
        // On-disk archive header
        // ========================================================

        struct ArchiveHeaderDisk
        {
            char id[4];

            uint32_t majorVersion;
            uint32_t minorVersion;
            uint32_t totalFiles;
        };


        // ========================================================
        // On-disk file header
        // ========================================================

        struct ArchiveFileHeaderDisk
        {
            char fileName[256];

            uint32_t size;
            uint32_t offset;
        };


        // ========================================================
        // Little-endian conversion
        // ========================================================

        uint32_t ToLittleEndian(uint32_t value)
        {
            const uint16_t test = 0x0001;

            const bool isLittleEndian =
                *reinterpret_cast<const uint8_t*>(&test) == 0x01;

            if (isLittleEndian)
                return value;


            return
                ((value & 0x000000FFu) << 24) |
                ((value & 0x0000FF00u) << 8) |
                ((value & 0x00FF0000u) >> 8) |
                ((value & 0xFF000000u) >> 24);
        }


        uint32_t FromLittleEndian(uint32_t value)
        {
            // Conversion is symmetrical.

            return ToLittleEndian(value);
        }
    }


    // ============================================================
    // Destructor
    // ============================================================

    Archive::~Archive()
    {
        Close();
    }


    // ============================================================
    // Create
    // ============================================================

    bool Archive::Create(
        const char* archiveFile,
        const char* rootDirectory,
        const std::vector<std::string>& files)
    {
        if (archiveFile == nullptr ||
            rootDirectory == nullptr ||
            files.empty())
        {
            return false;
        }


        FileOutputStream output;


        if (!output.Open(
            archiveFile,
            FileType::Binary))
        {
            return false;
        }


        std::vector<ArchiveFileHeader> headers;

        headers.resize(files.size());


        const uint32_t archiveHeaderSize =
            static_cast<uint32_t>(
                sizeof(ArchiveHeaderDisk)
                );


        const uint32_t fileHeaderSize =
            static_cast<uint32_t>(
                sizeof(ArchiveFileHeaderDisk)
                );


        uint64_t currentOffset =
            static_cast<uint64_t>(
                archiveHeaderSize
                );


        currentOffset +=
            static_cast<uint64_t>(
                fileHeaderSize
                ) *
            static_cast<uint64_t>(
                files.size()
                );


        // --------------------------------------------------------
        // Build headers
        // --------------------------------------------------------

        for (size_t i = 0;
            i < files.size();
            ++i)
        {
            const std::string& virtualFile =
                files[i];


            if (virtualFile.empty())
            {
                output.Close();
                return false;
            }


            if (virtualFile.length() >= 256)
            {
                output.Close();
                return false;
            }


            std::filesystem::path physicalPath =
                std::filesystem::path(rootDirectory) /
                std::filesystem::path(virtualFile);


            physicalPath =
                physicalPath.lexically_normal();


            FileInputStream input;


            if (!input.Open(
                physicalPath.string().c_str(),
                FileType::Binary))
            {
                output.Close();
                return false;
            }


            int fileSize =
                input.GetSize();


            if (fileSize < 0)
            {
                input.Close();
                output.Close();

                return false;
            }


            if (static_cast<uint64_t>(fileSize) >
                std::numeric_limits<uint32_t>::max())
            {
                input.Close();
                output.Close();

                return false;
            }


            if (currentOffset >
                std::numeric_limits<uint32_t>::max())
            {
                input.Close();
                output.Close();

                return false;
            }


            if (currentOffset +
                static_cast<uint64_t>(fileSize) >
                std::numeric_limits<uint32_t>::max())
            {
                input.Close();
                output.Close();

                return false;
            }


            ArchiveFileHeader& header =
                headers[i];


            // ----------------------------------------------------
            // Store VIRTUAL path.
            //
            // Example:
            //
            // Physical:
            // assets/levels/level01.json
            //
            // Virtual:
            // levels/level01.json
            // ----------------------------------------------------

            header.fileName =
                virtualFile;


            header.size =
                static_cast<uint32_t>(
                    fileSize
                    );


            header.offset =
                static_cast<uint32_t>(
                    currentOffset
                    );


            currentOffset +=
                static_cast<uint64_t>(
                    fileSize
                    );


            input.Close();
        }


        // --------------------------------------------------------
        // Archive header
        // --------------------------------------------------------

        ArchiveHeaderDisk archiveHeader{};


        std::memcpy(
            archiveHeader.id,
            ARCHIVE_ID,
            sizeof(ARCHIVE_ID)
        );


        archiveHeader.majorVersion =
            ToLittleEndian(
                ARCHIVE_MAJOR
            );


        archiveHeader.minorVersion =
            ToLittleEndian(
                ARCHIVE_MINOR
            );


        archiveHeader.totalFiles =
            ToLittleEndian(
                static_cast<uint32_t>(
                    headers.size()
                    )
            );


        if (!output.Write(
            &archiveHeader,
            sizeof(archiveHeader)))
        {
            output.Close();
            return false;
        }


        // --------------------------------------------------------
        // File headers
        // --------------------------------------------------------

        for (const ArchiveFileHeader& header :
            headers)
        {
            ArchiveFileHeaderDisk diskHeader{};


            std::memcpy(
                diskHeader.fileName,
                header.fileName.c_str(),
                header.fileName.length()
            );


            diskHeader.size =
                ToLittleEndian(
                    header.size
                );


            diskHeader.offset =
                ToLittleEndian(
                    header.offset
                );


            if (!output.Write(
                &diskHeader,
                sizeof(diskHeader)))
            {
                output.Close();
                return false;
            }
        }


        // --------------------------------------------------------
        // File data
        // --------------------------------------------------------

        char buffer[4096];


        for (const ArchiveFileHeader& header :
            headers)
        {
            std::filesystem::path physicalPath =
                std::filesystem::path(rootDirectory) /
                std::filesystem::path(header.fileName);


            physicalPath =
                physicalPath.lexically_normal();


            FileInputStream input;


            if (!input.Open(
                physicalPath.string().c_str(),
                FileType::Binary))
            {
                output.Close();
                return false;
            }


            uint32_t remaining =
                header.size;


            while (remaining > 0)
            {
                int chunk =
                    remaining >
                    sizeof(buffer)
                    ? static_cast<int>(
                        sizeof(buffer)
                        )
                    : static_cast<int>(
                        remaining
                        );


                if (!input.Read(
                    buffer,
                    chunk))
                {
                    input.Close();
                    output.Close();

                    return false;
                }


                if (!output.Write(
                    buffer,
                    chunk))
                {
                    input.Close();
                    output.Close();

                    return false;
                }


                remaining -=
                    static_cast<uint32_t>(
                        chunk
                        );
            }


            input.Close();
        }


        output.Close();

        return true;
    }


    // ============================================================
    // Open
    // ============================================================

    bool Archive::Open(
        const char* archiveFile)
    {
        if (archiveFile == nullptr)
            return false;


        Close();


        if (!m_file.Open(
            archiveFile,
            FileType::Binary))
        {
            return false;
        }


        if (!ReadHeader())
        {
            Close();
            return false;
        }


        return true;
    }


    // ============================================================
    // Close
    // ============================================================

    void Archive::Close()
    {
        m_headers.clear();

        m_file.Close();
    }


    // ============================================================
    // IsOpen
    // ============================================================

    bool Archive::IsOpen() const
    {
        return m_file.IsOpen();
    }


    // ============================================================
    // GetFileCount
    // ============================================================

    int Archive::GetFileCount() const
    {
        return static_cast<int>(
            m_headers.size()
            );
    }


    // ============================================================
    // FindFile
    // ============================================================

    int Archive::FindFile(
        const char* fileName) const
    {
        if (fileName == nullptr)
            return -1;


        for (size_t i = 0;
            i < m_headers.size();
            ++i)
        {
            if (m_headers[i].fileName ==
                fileName)
            {
                return static_cast<int>(i);
            }
        }


        return -1;
    }


    // ============================================================
    // ReadFile by index
    // ============================================================

    bool Archive::ReadFile(
        int index,
        void* buffer,
        int bufferSize)
    {
        if (!IsOpen() ||
            index < 0 ||
            index >= GetFileCount() ||
            buffer == nullptr ||
            bufferSize <= 0)
        {
            return false;
        }


        const ArchiveFileHeader& header =
            m_headers[
                static_cast<size_t>(index)
            ];


        if (bufferSize <
            static_cast<int>(header.size))
        {
            return false;
        }


        m_file.SeekToStart();


        m_file.Seek(
            static_cast<int>(
                header.offset
                )
        );


        return m_file.Read(
            buffer,
            static_cast<int>(
                header.size
                )
        );
    }


    // ============================================================
    // ReadFile by name
    // ============================================================

    bool Archive::ReadFile(
        const char* fileName,
        void* buffer,
        int bufferSize)
    {
        int index =
            FindFile(fileName);


        if (index < 0)
            return false;


        return ReadFile(
            index,
            buffer,
            bufferSize
        );
    }


    // ============================================================
    // GetFileHeader
    // ============================================================

    bool Archive::GetFileHeader(
        int index,
        ArchiveFileHeader& header) const
    {
        if (index < 0 ||
            index >= GetFileCount())
        {
            return false;
        }


        header =
            m_headers[
                static_cast<size_t>(index)
            ];


        return true;
    }


    // ============================================================
    // ReadHeader
    // ============================================================

    bool Archive::ReadHeader()
    {
        if (!IsOpen())
            return false;


        ArchiveHeaderDisk header{};


        if (!m_file.Read(
            &header,
            sizeof(header)))
        {
            return false;
        }


        // --------------------------------------------------------
        // Validate magic
        // --------------------------------------------------------

        if (std::memcmp(
            header.id,
            ARCHIVE_ID,
            sizeof(ARCHIVE_ID)) != 0)
        {
            return false;
        }


        // --------------------------------------------------------
        // Convert from little endian
        // --------------------------------------------------------

        uint32_t majorVersion =
            FromLittleEndian(
                header.majorVersion
            );


        uint32_t minorVersion =
            FromLittleEndian(
                header.minorVersion
            );


        uint32_t totalFiles =
            FromLittleEndian(
                header.totalFiles
            );


        // --------------------------------------------------------
        // Validate version
        // --------------------------------------------------------

        if (majorVersion !=
            ARCHIVE_MAJOR)
        {
            return false;
        }


        if (minorVersion !=
            ARCHIVE_MINOR)
        {
            return false;
        }


        // --------------------------------------------------------
        // Validate file count
        // --------------------------------------------------------

        if (totalFiles == 0)
            return false;


        if (totalFiles > 100000)
            return false;


        m_headers.clear();


        m_headers.resize(
            totalFiles
        );


        // --------------------------------------------------------
        // Read file headers
        // --------------------------------------------------------

        for (uint32_t i = 0;
            i < totalFiles;
            ++i)
        {
            ArchiveFileHeaderDisk diskHeader{};


            if (!m_file.Read(
                &diskHeader,
                sizeof(diskHeader)))
            {
                m_headers.clear();

                return false;
            }


            // Guarantee null termination.

            diskHeader.fileName[
                sizeof(diskHeader.fileName) - 1
            ] = '\0';


            m_headers[i].fileName =
                diskHeader.fileName;


            m_headers[i].size =
                FromLittleEndian(
                    diskHeader.size
                );


            m_headers[i].offset =
                FromLittleEndian(
                    diskHeader.offset
                );
        }


        return true;
    }
}