#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

// مرتب‌سازی فایل‌ها به‌صورت عددی
bool natural_compare(const fs::path& a, const fs::path& b) {
    static const std::regex re(R"(\d+)");
    std::sregex_iterator it_a(a.filename().string().begin(), a.filename().string().end(), re);
    std::sregex_iterator it_b(b.filename().string().begin(), b.filename().string().end(), re);
    std::sregex_iterator end;

    while (it_a != end && it_b != end) {
        int na = std::stoi((*it_a).str());
        int nb = std::stoi((*it_b).str());
        if (na != nb)
            return na < nb;
        ++it_a;
        ++it_b;
    }

    return a.filename().string() < b.filename().string();
}

int main() {
    std::string input_dir = "input";
    std::string output_file = "output.pkg";

    if (!fs::exists(input_dir) || !fs::is_directory(input_dir)) {
        std::cerr << "Input folder not found.\n";
        return 1;
    }

    std::vector<fs::path> files;
    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (entry.is_regular_file())
            files.push_back(entry.path());
    }

    std::sort(files.begin(), files.end(), natural_compare);

    std::ofstream out(output_file, std::ios::binary);
    if (!out) {
        std::cerr << "Cannot create output file.\n";
        return 1;
    }

    for (const auto& file : files) {
        std::ifstream in(file, std::ios::binary);
        if (!in) {
            std::cerr << "Failed to open: " << file << '\n';
            continue;
        }

        out << in.rdbuf();
        std::cout << "Merged: " << file.filename() << '\n';
    }

    std::cout << "Merging completed.\n";
    return 0;
}
