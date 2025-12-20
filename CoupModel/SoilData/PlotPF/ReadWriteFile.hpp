// ReadWriteFile.hpp
#pragma once
#include <fstream>
#include <string>

class ReadWriteFile {
public:
    enum class OpenMode {
        Text,
        Binary
    };

    ReadWriteFile() = default;

    ReadWriteFile(const std::string& path,
        OpenMode mode = OpenMode::Text,
        bool truncate = false)
    {
        open(path, mode, truncate);
    }

    bool open(const std::string& path,
        OpenMode mode = OpenMode::Text,
        bool truncate = false)
    {
        close();

        std::ios::openmode m = std::ios::in | std::ios::out;
        if (mode == OpenMode::Binary)
            m |= std::ios::binary;
        if (truncate)
            m |= std::ios::trunc;

        fs_.open(path, m);
        last_op_ = Op::None;
        return fs_.is_open();
    }

    bool is_open() const noexcept { return fs_.is_open(); }

    void close() {
        if (fs_.is_open())
            fs_.close();
        last_op_ = Op::None;
    }

    // Exponera underliggande stream vid behov
    std::fstream& stream() noexcept { return fs_; }
    const std::fstream& stream() const noexcept { return fs_; }

    // Läs en rad (textläge)
    bool getline(std::string& line) {
        if (!fs_.is_open())
            return false;

        sync_for_read();
        if (!std::getline(fs_, line))
            return false;

        last_op_ = Op::Read;
        return true;
    }

    // Skriv utan radslut
    void write(const std::string& s) {
        if (!fs_.is_open())
            return;

        sync_for_write();
        fs_ << s;
        last_op_ = Op::Write;
    }

    // Skriv en rad (med '\n')
    void write_line(const std::string& s) {
        if (!fs_.is_open())
            return;

        sync_for_write();
        fs_ << s << '\n';
        last_op_ = Op::Write;
    }

    // Gå till filens början
    void rewind() {
        if (!fs_.is_open())
            return;
        fs_.clear();
        fs_.seekg(0, std::ios::beg);
        fs_.seekp(0, std::ios::beg);
        last_op_ = Op::None;
    }

    // Gå till godtycklig position
    void seek(std::streampos pos) {
        if (!fs_.is_open())
            return;
        fs_.clear();
        fs_.seekg(pos);
        fs_.seekp(pos);
        last_op_ = Op::None;
    }

    // Gå till slutet
    void seek_end() {
        if (!fs_.is_open())
            return;
        fs_.clear();
        fs_.seekg(0, std::ios::end);
        fs_.seekp(0, std::ios::end);
        last_op_ = Op::None;
    }

    std::streampos tellg() { return fs_.tellg(); }
    std::streampos tellp() { return fs_.tellp(); }

private:
    enum class Op { None, Read, Write };

    void sync_for_read() {
        if (last_op_ == Op::Write) {
            fs_.flush();
            fs_.clear();
            auto pos = fs_.tellp();
            fs_.seekg(pos);
        }
    }

    void sync_for_write() {
        if (last_op_ == Op::Read) {
            fs_.clear();
            auto pos = fs_.tellg();
            fs_.seekp(pos);
        }
    }

    std::fstream fs_;
    Op last_op_{ Op::None };
};
