#include "grep.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

namespace fs = std::filesystem;

Grep::Grep(const std::string& pattern, const Options& opts)
    : pattern(pattern), options(opts) {

    if (options.use_regex) {
        regex_compiled = compileRegex();
    }
}

bool Grep::compileRegex() {
    try {
        auto flags = std::regex_constants::ECMAScript;
        if (options.ignore_case) {
            flags |= std::regex_constants::icase;
        }
        regex_pattern = std::regex(pattern, flags);
        return true;
    } catch (const std::regex_error& e) {
        std::cerr << "Error in regular expression: " << e.what() << std::endl;
        return false;
    }
}

void Grep::addFile(const std::string& filename) {
    files.push_back(filename);
}

bool Grep::matches(const std::string& text) const {
    if (options.use_regex && regex_compiled) {
        return std::regex_search(text, regex_pattern);
    } else {
        if (options.ignore_case) {
            std::string text_lower = text;
            std::string pattern_lower = pattern;
            std::transform(text_lower.begin(), text_lower.end(), text_lower.begin(), ::tolower);
            std::transform(pattern_lower.begin(), pattern_lower.end(), pattern_lower.begin(), ::tolower);
            return text_lower.find(pattern_lower) != std::string::npos;
        } else {
            return text.find(pattern) != std::string::npos;
        }
    }
}

std::string Grep::highlightMatch(const std::string& line) const {
    if (!options.color_output || !options.use_regex || !regex_compiled) {
        return line;
    }

    std::string result;
    std::sregex_iterator it(line.begin(), line.end(), regex_pattern);
    std::sregex_iterator end;

    size_t last_pos = 0;
    for (; it != end; ++it) {
        result += line.substr(last_pos, it->position() - last_pos);
        result += "\033[1;31m" + it->str() + "\033[0m";
        last_pos = it->position() + it->length();
    }
    result += line.substr(last_pos);

    return result;
}

void Grep::printMatch(const std::string& filename, int line_num, const std::string& line) {
    if (files.size() > 1) {
        std::cout << filename << ":";
    }
    if (options.line_numbers) {
        std::cout << line_num << ":";
    }

    if (options.color_output && options.use_regex && regex_compiled) {
        std::cout << highlightMatch(line);
    } else {
        std::cout << line;
    }
    std::cout << std::endl;
}

void Grep::searchInFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }

    std::string line;
    int line_num = 0;
    int match_count = 0;

    while (std::getline(file, line)) {
        line_num++;
        bool found = matches(line);

        if (options.invert_match) {
            found = !found;
        }

        if (found) {
            match_count++;
            if (!options.count_only) {
                if (options.only_matching) {
                    if (options.use_regex && regex_compiled) {
                        std::smatch match;
                        std::string::const_iterator search_start = line.cbegin();
                        while (std::regex_search(search_start, line.cend(), match, regex_pattern)) {
                            std::string output = match.str();
                            if (options.color_output) {
                                output = "\033[1;31m" + output + "\033[0m";
                            }
                            if (files.size() > 1) {
                                std::cout << filename << ":";
                            }
                            if (options.line_numbers) {
                                std::cout << line_num << ":";
                            }
                            std::cout << output << std::endl;
                            search_start = match.suffix().first;
                        }
                    } else {
                        size_t pos = options.ignore_case ?
                            std::string(line).find(std::string(pattern)) :
                            line.find(pattern);
                        if (pos != std::string::npos) {
                            std::string output = line.substr(pos, pattern.length());
                            if (options.color_output) {
                                output = "\033[1;31m" + output + "\033[0m";
                            }
                            if (files.size() > 1) {
                                std::cout << filename << ":";
                            }
                            if (options.line_numbers) {
                                std::cout << line_num << ":";
                            }
                            std::cout << output << std::endl;
                        }
                    }
                } else {
                    printMatch(filename, line_num, line);
                }
            }
        }
    }

    if (options.count_only) {
        if (files.size() > 1) {
            std::cout << filename << ":";
        }
        std::cout << match_count << std::endl;
    }

    file.close();
}

void Grep::searchInDirectory(const std::string& dirname) {
    DIR* dir = opendir(dirname.c_str());
    if (!dir) {
        std::cerr << "Error: Cannot open directory " << dirname << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..") {
            continue;
        }

        std::string full_path = dirname + "/" + entry->d_name;
        struct stat statbuf;

        if (stat(full_path.c_str(), &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode) && options.recursive) {
                searchInDirectory(full_path);
            } else if (S_ISREG(statbuf.st_mode)) {
                searchInFile(full_path);
            }
        }
    }
    closedir(dir);
}

void Grep::searchStdin() {
    std::string line;
    int line_num = 0;
    int match_count = 0;

    while (std::getline(std::cin, line)) {
        line_num++;
        bool found = matches(line);

        if (options.invert_match) {
            found = !found;
        }

        if (found) {
            match_count++;
            if (!options.count_only) {
                if (options.only_matching) {
                    if (options.use_regex && regex_compiled) {
                        std::smatch match;
                        std::string::const_iterator search_start = line.cbegin();
                        while (std::regex_search(search_start, line.cend(), match, regex_pattern)) {
                            std::string output = match.str();
                            if (options.color_output) {
                                output = "\033[1;31m" + output + "\033[0m";
                            }
                            if (options.line_numbers) {
                                std::cout << line_num << ":";
                            }
                            std::cout << output << std::endl;
                            search_start = match.suffix().first;
                        }
                    } else {
                        size_t pos = options.ignore_case ?
                            std::string(line).find(std::string(pattern)) :
                            line.find(pattern);
                        if (pos != std::string::npos) {
                            std::string output = line.substr(pos, pattern.length());
                            if (options.color_output) {
                                output = "\033[1;31m" + output + "\033[0m";
                            }
                            if (options.line_numbers) {
                                std::cout << line_num << ":";
                            }
                            std::cout << output << std::endl;
                        }
                    }
                } else {
                    if (options.line_numbers) {
                        std::cout << line_num << ":";
                    }
                    if (options.color_output && options.use_regex && regex_compiled) {
                        std::cout << highlightMatch(line) << std::endl;
                    } else {
                        std::cout << line << std::endl;
                    }
                }
            }
        }
    }

    if (options.count_only) {
        std::cout << match_count << std::endl;
    }
}

void Grep::search() {
    if (options.use_regex && !regex_compiled) {
        return;
    }

    if (files.empty()) {
        searchStdin();
    } else {
        for (const auto& file : files) {
            struct stat statbuf;
            if (stat(file.c_str(), &statbuf) == 0) {
                if (S_ISDIR(statbuf.st_mode)) {
                    if (options.recursive) {
                        searchInDirectory(file);
                    } else {
                        std::cerr << "Error: " << file << " is a directory. Use -r for recursive search." << std::endl;
                    }
                } else {
                    searchInFile(file);
                }
            } else {
                std::cerr << "Error: Cannot access file " << file << std::endl;
            }
        }
    }
}