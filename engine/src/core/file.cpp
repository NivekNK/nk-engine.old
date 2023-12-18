#include "nkpch.h"

#include "core/file.h"

#include "memory/allocator.h"

namespace nk {
    File::File(const FileData& data)
        : m_filepath{data.filepath},
          m_mode{data.mode},
          m_open{data.open},
          m_file{data.file} {
    }

    File* File::open(Allocator* allocator, Path filepath, BitMask<FileMode> mode) {
        FileHandle file;
        if (mode & (FileMode::Read | FileMode::Write | FileMode::Binary)) {
            file = fopen(filepath.string().c_str(), "rb+");
            if (file == nullptr)
                file = fopen(filepath.string().c_str(), "wb+");
        } else if (mode & (FileMode::Read | FileMode::Write)) {
            file = fopen(filepath.string().c_str(), "r+");
            if (file == nullptr)
                file = fopen(filepath.string().c_str(), "w+");
        } else if (mode & (FileMode::Write | FileMode::Binary)) {
            file = fopen(filepath.string().c_str(), "wb");
        } else if (mode & FileMode::Write) {
            file = fopen(filepath.string().c_str(), "w");
        } else if (mode & (FileMode::Read | FileMode::Binary)) {
            file = fopen(filepath.string().c_str(), "rb");
        } else if (mode & FileMode::Read) {
            file = fopen(filepath.string().c_str(), "r");
        }

        if (file == nullptr)
            return nullptr;

        FileData data {
            .filepath = filepath,
            .mode = mode,
            .open = true,
            .file = file,
        };
        return allocator->construct<File>(data);
    }

    Arr<char> File::read_binary(Allocator* allocator, bool auto_close) {
        if (!is_open_as(FileMode::Read | FileMode::Binary))
            return Arr<char>(nullptr, 0);

        szt file_size = size();
        char* buffer = static_cast<char*>(allocator->allocate(file_size + 1, 1));
        fread(buffer, file_size, 1, m_file);
        buffer[file_size] = 0;

        if (auto_close)
            close();

        return Arr<char>(buffer, file_size + 1);
    }

    Arr<char> File::read_text(Allocator* allocator, bool auto_close) {
        if (!is_open_as(FileMode::Read))
            return Arr<char>(nullptr, 0);

        szt file_size = size();
        char* buffer = static_cast<char*>(allocator->allocate(file_size + 1, 1));
        szt bytes_read = fread(buffer, 1, file_size, m_file);
        buffer[bytes_read] = 0;

        if (auto_close)
            close();

        return Arr<char>(buffer, bytes_read + 1);
    }

    bool File::read_line(char* out_line, szt buffer_length) {
        if (!is_open_as(FileMode::Read))
            return false;

        if (fgets(out_line, buffer_length, m_file) != nullptr) {
            if (out_line[strlen(out_line) - 1] == '\n')
                out_line[strlen(out_line) - 1] = '\0';

            return true;
        }

        return false;
    }

    str File::read_binary_string(bool auto_close) {
        if (!is_open_as(FileMode::Read | FileMode::Binary))
            return nullptr;

        szt file_size = size();
        str buffer;
        buffer.resize(file_size);
        fread(&buffer[0], 1, file_size, m_file);

        if (auto_close)
            close();

        return buffer;
    }

    str File::read_text_string(bool auto_close) {
        if (!is_open_as(FileMode::Read))
            return nullptr;

        szt file_size = size();
        str buffer;
        buffer.resize(file_size);
        fread(&buffer[0], 1, file_size, m_file);

        if (auto_close)
            close();

        return buffer;
    }

    bool File::read_line_string(str& out_line, szt buffer_length) {
        if (!is_open_as(FileMode::Read))
            return false;

        out_line.resize(buffer_length);
        if (fgets(&out_line[0], buffer_length, m_file) != nullptr) {
            if (out_line[strlen(&out_line[0]) - 1] == '\n')
                out_line[strlen(&out_line[0]) - 1] = '\0';

            return true;
        }

        return false;
    }

    void File::write_binary(void* memory, szt size, bool auto_close) {
        if (!is_open_as(FileMode::Write | FileMode::Binary))
            return;

        fwrite(memory, size, 1, m_file);

        if (auto_close)
            close();
    }

    szt File::write(char* memory, szt length, bool append, bool auto_close) {
        if (!is_open_as(FileMode::Write))
            return 0;

        if (append)
            fseek(m_file, 0, SEEK_END);
        else
            fseek(m_file, 0, SEEK_SET);

        szt file_size = fwrite(memory, sizeof(char), length, m_file);

        if (auto_close)
            close();

        return file_size;
    }

    szt File::size() {
        Assert(m_open == true);
        szt current_pos = ftell(m_file);
        fseek(m_file, 0, SEEK_END);
        szt file_size = ftell(m_file);
        fseek(m_file, current_pos, SEEK_SET);
        return file_size;
    }

    bool File::exists() {
#if defined(NK_PLATFORM_WINDOWS)
        WIN32_FILE_ATTRIBUTE_DATA unused;
        return GetFileAttributesExA(m_filepath.string().c_str(), GetFileExInfoStandard, &unused);
#else
        i32 result = access(m_filepath.string().c_str(), F_OK);
        return result == 0;
#endif
    }

    bool File::is_open() {
        return m_open;
    }

    bool File::is_open_as(BitMask<FileMode> mode) {
        return m_open && (m_mode & mode);
    }

    bool File::erase() {
        i32 result = remove(m_filepath.string().c_str());
#if defined(NK_PLATFORM_WINDOWS)
        return result != 0;
#else
        return result == 0;
#endif
    }

    void File::close() {
        if (m_open && m_file != nullptr) {
            fclose(m_file);
            m_open = false;
        }
    }
}
