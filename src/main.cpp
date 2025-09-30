#include "grep.h"
#include <iostream>
#include <vector>
#include <getopt.h>

void printUsage(const std::string& program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS] PATTERN [FILE...]" << std::endl;
    std::cout << "Search for PATTERN in FILEs (or stdin)." << std::endl << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -i, --ignore-case     Ignore case distinctions" << std::endl;
    std::cout << "  -n, --line-number     Print line numbers with output lines" << std::endl;
    std::cout << "  -o, --only-matching   Show only the part of lines matching PATTERN" << std::endl;
    std::cout << "  -r, --recursive       Read all files under each directory, recursively" << std::endl;
    std::cout << "  -v, --invert-match    Select non-matching lines" << std::endl;
    std::cout << "  -c, --count           Print only a count of matching lines per file" << std::endl;
    std::cout << "  -E, --extended-regexp PATTERN is an extended regular expression" << std::endl;
    std::cout << "  --color               Highlight matching strings" << std::endl;
    std::cout << "  -h, --help            Display this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    static struct option long_options[] = {
        {"ignore-case", no_argument, 0, 'i'},
        {"line-number", no_argument, 0, 'n'},
        {"only-matching", no_argument, 0, 'o'},
        {"recursive", no_argument, 0, 'r'},
        {"invert-match", no_argument, 0, 'v'},
        {"count", no_argument, 0, 'c'},
        {"extended-regexp", no_argument, 0, 'E'},
        {"color", no_argument, 0, 'C'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    Grep::Options options;
    std::string pattern;
    std::vector<std::string> files;

    int option_index = 0;
    int c;
    while ((c = getopt_long(argc, argv, "inorvcEh", long_options, &option_index)) != -1) {
        switch (c) {
            case 'i': options.ignore_case = true; break;
            case 'n': options.line_numbers = true; break;
            case 'o': options.only_matching = true; break;
            case 'r': options.recursive = true; break;
            case 'v': options.invert_match = true; break;
            case 'c': options.count_only = true; break;
            case 'E': options.use_regex = true; break;
            case 'C': options.color_output = true; break;
            case 'h': printUsage(argv[0]); return 0;
            case '?': printUsage(argv[0]); return 1;
        }
    }

    if (optind >= argc) {
        std::cerr << "Error: Pattern required" << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    pattern = argv[optind++];

    while (optind < argc) {
        files.push_back(argv[optind++]);
    }

    Grep grep(pattern, options);

    for (const auto& file : files) {
        grep.addFile(file);
    }

    grep.search();

    return 0;
}