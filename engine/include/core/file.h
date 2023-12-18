#pragma once

#include "core/bit_mask.h"
#include "core/arr.h"

namespace nk {
    class Allocator;

    using FileHandle = FILE*;
    using Path = std::filesystem::path;

    DefineEnumBitMask(FileMode, (
        Read,
        Write,
        Binary
    ));

    class File {
    private:
        struct FileData {
            Path filepath;
            BitMask<FileMode> mode;
            bool open;
            FileHandle file;
        };

    public:
        File(const FileData& data);
        inline ~File() { close(); }

        static File* open(Allocator* allocator, Path filepath, BitMask<FileMode> mode);

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
        bool is_open_as(BitMask<FileMode> mode);
        bool erase();
        void close();

    private:
        Path m_filepath;
        BitMask<FileMode> m_mode;
        bool m_open;
        FileHandle m_file;
    };
}
