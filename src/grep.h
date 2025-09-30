#ifndef TINY_GREP_H
#define TINY_GREP_H

#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <iostream>

class Grep {
public:
    struct Options {
        bool ignore_case;
        bool line_numbers;
        bool only_matching;
        bool recursive;
        bool invert_match;
        bool count_only;
        bool color_output;
        bool use_regex;

        Options() : ignore_case(false), line_numbers(false), only_matching(false),
                   recursive(false), invert_match(false), count_only(false),
                   color_output(false), use_regex(false) {}
    };

    Grep(const std::string& pattern, const Options& opts = Options());

    void addFile(const std::string& filename);
    void search();

    bool matches(const std::string& text) const;

private:
    std::string pattern;
    Options options;
    std::vector<std::string> files;
    std::regex regex_pattern;
    bool regex_compiled = false;

    bool compileRegex();
    void searchInFile(const std::string& filename);
    void searchStdin();
    void searchInDirectory(const std::string& dirname);
    std::string highlightMatch(const std::string& line) const;
    void printMatch(const std::string& filename, int line_num, const std::string& line);
};

#endif