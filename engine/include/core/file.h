#pragma once

#include "core/arr.h"

namespace nk {
    using FileModeFlag = u8;
    namespace FileMode {
        enum : FileModeFlag {
            Read,
            Write,
            Binary,
        };
    }

    class Allocator;

    using FileHandle = FILE*;
    using Path = std::filesystem::path;

    class File {
    private:
        struct FileData {
            Path filepath;
            FileModeFlag mode;
            bool open;
            FileHandle file;
        };

    public:
        File(const FileData& data);
        inline ~File() { close(); }

        static File* open(Allocator* allocator, Path filepath, FileModeFlag mode);

        Arr<char> read_binary(Allocator* allocator, bool auto_close = true);
        Arr<char> read_text(Allocator* allocator, bool auto_close = true);
        bool read_line(char* out_line, szt buffer_length);

        str read_binary_string(bool auto_close = true);
        str read_text_string(bool auto_close = true);
        bool read_line_string(str& out_line, szt buffer_length = 512);

        void write_binary(void* memory, szt size, bool auto_close = true);
        szt write(char* memory, szt length, bool append = false, bool auto_close = true);

        szt size();
        bool exists();
        bool is_open();
        bool is_open_as(FileModeFlag mode);
        bool erase();
        void close();

    private:
        Path m_filepath;
        FileModeFlag m_mode;
        bool m_open;
        FileHandle m_file;
    };
}
