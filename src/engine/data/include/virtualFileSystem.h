#pragma once

#include "archive.h"

#include <memory>
#include <string>
#include <vector>


namespace bfp
{
    class VirtualFileSystem
    {
    public:

        VirtualFileSystem() = default;
        ~VirtualFileSystem();

        bool MountDirectory(
            const char* directory
        );

        bool MountArchive(
            const char* archiveFile
        );

        void UnmountAll();

        bool FileExists(
            const char* fileName
        ) const;

        bool ReadFile(
            const char* fileName,
            std::vector<char>& data
        );

        int GetMountCount() const;


    private:

        struct Mount
        {
            enum class Type
            {
                Directory,
                Archive
            };

            Type type = Type::Directory;

            std::string directory;

            std::unique_ptr<Archive> archive;
        };


        std::vector<std::unique_ptr<Mount>> m_mounts;
    };
}