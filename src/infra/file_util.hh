/*********************************************************************
 * @file    file_util.hh
 * @author 	Nick Weber
 * @date    Jun 24, 2018
 * @brief 	
 * @bugs 	TBD
 * @todos 	TBD
 * 
 * @section	DESCRIPTION
 * TBD
 * 
 * @section USE
 * TBD
 ********************************************************************/
#pragma once

#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

namespace FileUtil
{

    inline bool exists(const std::string& aPath) noexcept { return fs::exists(aPath); }

    inline bool isFile(const std::string& aPath) noexcept { return fs::is_regular_file(aPath); }

    inline bool isRawDevice(const std::string& aPath) noexcept { return fs::is_block_file(aPath); }

    inline size_t fileSize(const std::string& aPath) noexcept { return fs::file_size(aPath); }

    inline bool hasValidDir(const std::string& aPath) noexcept
    {
        const fs::path lPath = aPath;
        fs::path lTmpPath;
        const auto end = --lPath.end();
        for(auto it = lPath.begin(); it != end; ++it){ lTmpPath /= *it; } //iterate over path components and add them to tmp path
        return fs::is_directory(lTmpPath);
    }

    inline void resize(const std::string aPath, size_t aSize) noexcept
    {
        fs::resize_file(aPath, aSize); //will throw if fails
    }

    inline void create(const std::string& aPath) noexcept
    {
        std::ofstream ofs(aPath, std::ofstream::out);
        ofs.close();
    }

    inline bool remove(const std::string& aPath) noexcept
    {
        return fs::remove(aPath);
    }
} //end namespace FileUtil
