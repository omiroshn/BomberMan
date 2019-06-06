//
// Created by Vadim on 2019-06-06.
//

#ifndef BOMBERMAN_ZIPHOLDER_HPP
#define BOMBERMAN_ZIPHOLDER_HPP

#include <memory>
#include <string>
#include "zip.h"

class FileContents
{
public:
    FileContents(char    *data, size_t  size);
    ~FileContents();
    FileContents(FileContents const &) = delete;
    FileContents &operator=(FileContents const &) = delete;
    std::string             mData;
    size_t                  mSize;
};

class ZipHolder
{
public:
    explicit ZipHolder(std::string const &zipPath);
    ~ZipHolder();
    ZipHolder(ZipHolder const &) = delete;
    ZipHolder &operator=(ZipHolder const &) = delete;
    std::unique_ptr<FileContents> loadDataFromArchive(std::string const &path);
private:
    zip_t				*mArchive;
};

#endif //BOMBERMAN_ZIPHOLDER_HPP
