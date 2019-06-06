//
// Created by Vadim on 2019-06-06.
//

#include "ResourceManagement/ZipHolder.hpp"
#include "CustomException.hpp"
#include <iostream>
FileContents::FileContents(char *data, size_t  size) : mData { data }, mSize{ size }
{
}

FileContents::~FileContents()
{
}

ZipHolder::ZipHolder(std::string const &zipPath)
{
    int err = 0;
    if (!(mArchive = zip_open(zipPath.c_str(), ZIP_CREATE, &err))) // Segfault is here, the reason in unknown
        std::cout << zip_strerror(mArchive) << std::endl;
}

ZipHolder::~ZipHolder()
{
    zip_close(mArchive);
}

std::unique_ptr<FileContents> ZipHolder::loadDataFromArchive(std::string const &path)
{
    struct zip_stat		            st;
    zip_file_t			            *f;
    char                            *file_contents;
    std::unique_ptr<FileContents>   res{nullptr};

    zip_stat_init(&st);
    zip_stat(mArchive, path.c_str(), 0, &st);
    if (st.size < 1)
        throw CustomException("ZIP_HOLDER: No such file");
    f = zip_fopen(mArchive, path.c_str(), 0);

    file_contents = new char[st.size];
    if (!f)
        throw CustomException(zip_strerror(mArchive));
    if (zip_fread(f, file_contents, st.size) < 1 || zip_fclose(f))
        throw CustomException(zip_strerror(mArchive));
    res = std::make_unique<FileContents>(file_contents, st.size);
    zip_fclose(f);
    delete[] file_contents;
    return (std::move(res));
}