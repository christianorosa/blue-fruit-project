#include "../include/virtualFileSystem.h"

#include <filesystem>
#include <memory>


namespace bfp
{
    VirtualFileSystem::~VirtualFileSystem()
    {
        UnmountAll();
    }


    // ============================================================
    // MountDirectory
    // ============================================================

    bool VirtualFileSystem::MountDirectory(
        const char* directory)
    {
        if (directory == nullptr)
            return false;


        std::filesystem::path path(directory);


        if (!std::filesystem::exists(path))
            return false;


        if (!std::filesystem::is_directory(path))
            return false;


        auto mount =
            std::make_unique<Mount>();


        mount->type =
            Mount::Type::Directory;


        mount->directory =
            path.lexically_normal().string();


        m_mounts.push_back(
            std::move(mount)
        );


        return true;
    }


    // ============================================================
    // MountArchive
    // ============================================================

    bool VirtualFileSystem::MountArchive(
        const char* archiveFile)
    {
        if (archiveFile == nullptr)
            return false;


        auto mount =
            std::make_unique<Mount>();


        mount->type =
            Mount::Type::Archive;


        mount->archive =
            std::make_unique<Archive>();


        if (!mount->archive->Open(
            archiveFile))
        {
            return false;
        }


        m_mounts.push_back(
            std::move(mount)
        );


        return true;
    }


    // ============================================================
    // UnmountAll
    // ============================================================

    void VirtualFileSystem::UnmountAll()
    {
        m_mounts.clear();
    }


    // ============================================================
    // GetMountCount
    // ============================================================

    int VirtualFileSystem::GetMountCount() const
    {
        return static_cast<int>(
            m_mounts.size()
            );
    }


    // ============================================================
    // FileExists
    // ============================================================

    bool VirtualFileSystem::FileExists(
        const char* fileName) const
    {
        if (fileName == nullptr ||
            fileName[0] == '\0')
        {
            return false;
        }


        std::filesystem::path virtualPath(
            fileName
        );


        virtualPath =
            virtualPath.lexically_normal();


        const std::string virtualFile =
            virtualPath.string();


        for (auto it = m_mounts.rbegin();
            it != m_mounts.rend();
            ++it)
        {
            const Mount& mount =
                **it;


            // ----------------------------------------------------
            // Directory
            // ----------------------------------------------------

            if (mount.type ==
                Mount::Type::Directory)
            {
                std::filesystem::path physicalPath =
                    std::filesystem::path(
                        mount.directory
                    ) /
                    virtualPath;


                if (std::filesystem::exists(
                    physicalPath) &&
                    std::filesystem::is_regular_file(
                        physicalPath))
                {
                    return true;
                }
            }


            // ----------------------------------------------------
            // Archive
            // ----------------------------------------------------

            else
            {
                if (mount.archive == nullptr)
                    continue;


                if (mount.archive->FindFile(
                    virtualFile.c_str()) >= 0)
                {
                    return true;
                }
            }
        }


        return false;
    }


    // ============================================================
    // ReadFile
    // ============================================================

    bool VirtualFileSystem::ReadFile(
        const char* fileName,
        std::vector<char>& data)
    {
        data.clear();


        if (fileName == nullptr ||
            fileName[0] == '\0')
        {
            return false;
        }


        std::filesystem::path virtualPath(
            fileName
        );


        virtualPath =
            virtualPath.lexically_normal();


        const std::string virtualFile =
            virtualPath.string();


        // --------------------------------------------------------
        // Search mounts.
        //
        // Last mounted resource has priority.
        // --------------------------------------------------------

        for (auto it = m_mounts.rbegin();
            it != m_mounts.rend();
            ++it)
        {
            Mount& mount =
                **it;


            // ====================================================
            // DIRECTORY
            // ====================================================

            if (mount.type ==
                Mount::Type::Directory)
            {
                std::filesystem::path physicalPath =
                    std::filesystem::path(
                        mount.directory
                    ) /
                    virtualPath;


                if (!std::filesystem::exists(
                    physicalPath))
                {
                    continue;
                }


                if (!std::filesystem::is_regular_file(
                    physicalPath))
                {
                    continue;
                }


                FileInputStream file;


                if (!file.Open(
                    physicalPath.string().c_str(),
                    FileType::Binary))
                {
                    return false;
                }


                const int size =
                    file.GetSize();


                if (size < 0)
                {
                    file.Close();

                    return false;
                }


                if (size == 0)
                {
                    file.Close();

                    return true;
                }


                data.resize(
                    static_cast<size_t>(size)
                );


                if (!file.Read(
                    data.data(),
                    size))
                {
                    data.clear();

                    file.Close();

                    return false;
                }


                file.Close();

                return true;
            }


            // ====================================================
            // ARCHIVE
            // ====================================================

            if (mount.archive == nullptr)
                continue;


            const int index =
                mount.archive->FindFile(
                    virtualFile.c_str()
                );


            if (index < 0)
                continue;


            ArchiveFileHeader header;


            if (!mount.archive->GetFileHeader(
                index,
                header))
            {
                return false;
            }


            if (header.size == 0)
            {
                return true;
            }


            data.resize(
                static_cast<size_t>(
                    header.size
                    )
            );


            if (!mount.archive->ReadFile(
                index,
                data.data(),
                static_cast<int>(
                    data.size()
                    )))
            {
                data.clear();

                return false;
            }


            return true;
        }


        return false;
    }
}