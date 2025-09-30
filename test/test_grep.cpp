#include "../src/grep.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

void test_basic_string_matching() {
    std::cout << "Testing basic string matching..." << std::endl;

    Grep::Options opts;
    Grep grep("hello", opts);

    assert(grep.matches("hello world") == true);
    assert(grep.matches("Hello world") == false);
    assert(grep.matches("world") == false);
    assert(grep.matches("helloworld") == true);

    std::cout << "✓ Basic string matching tests passed" << std::endl;
}

void test_case_insensitive() {
    std::cout << "Testing case insensitive matching..." << std::endl;

    Grep::Options opts;
    opts.ignore_case = true;
    Grep grep("hello", opts);

    assert(grep.matches("hello world") == true);
    assert(grep.matches("Hello world") == true);
    assert(grep.matches("HELLO world") == true);
    assert(grep.matches("world") == false);

    std::cout << "✓ Case insensitive tests passed" << std::endl;
}

void test_regex_matching() {
    std::cout << "Testing regex matching..." << std::endl;

    Grep::Options opts;
    opts.use_regex = true;
    Grep grep("h.llo", opts);

    assert(grep.matches("hello world") == true);
    assert(grep.matches("hallo world") == true);
    assert(grep.matches("hxllo world") == true);
    assert(grep.matches("hllo world") == false);

    std::cout << "✓ Regex matching tests passed" << std::endl;
}

void test_invert_match() {
    std::cout << "Testing invert match..." << std::endl;

    Grep::Options opts;
    opts.invert_match = true;
    Grep grep("hello", opts);

    // matches() function should not apply invert_match logic
    assert(grep.matches("hello world") == true);
    assert(grep.matches("world") == false);
    assert(grep.matches("hello") == true);

    std::cout << "✓ Invert match tests passed" << std::endl;
}

void test_file_search() {
    std::cout << "Testing file search..." << std::endl;

    // Create a temporary test file
    std::ofstream test_file("test_file.txt");
    test_file << "This is a test file" << std::endl;
    test_file << "hello world" << std::endl;
    test_file << "Another line" << std::endl;
    test_file << "Say hello again" << std::endl;
    test_file.close();

    // Test searching in the file
    Grep::Options opts;
    Grep grep("hello", opts);
    grep.addFile("test_file.txt");

    // Redirect stdout to capture output
    std::streambuf* orig = std::cout.rdbuf();
    std::ostringstream capture;
    std::cout.rdbuf(capture.rdbuf());

    grep.search();

    std::cout.rdbuf(orig);
    std::string output = capture.str();

    assert(output.find("hello world") != std::string::npos);
    assert(output.find("Say hello again") != std::string::npos);
    assert(output.find("This is a test file") == std::string::npos);

    // Clean up
    std::remove("test_file.txt");

    std::cout << "✓ File search tests passed" << std::endl;
}

void test_count_only() {
    std::cout << "Testing count only..." << std::endl;

    // Create a temporary test file
    std::ofstream test_file("test_count.txt");
    test_file << "hello world" << std::endl;
    test_file << "another line" << std::endl;
    test_file << "hello again" << std::endl;
    test_file.close();

    Grep::Options opts;
    opts.count_only = true;
    Grep grep("hello", opts);
    grep.addFile("test_count.txt");

    // Redirect stdout to capture output
    std::streambuf* orig = std::cout.rdbuf();
    std::ostringstream capture;
    std::cout.rdbuf(capture.rdbuf());

    grep.search();

    std::cout.rdbuf(orig);
    std::string output = capture.str();

    assert(output.find("2") != std::string::npos);

    // Clean up
    std::remove("test_count.txt");

    std::cout << "✓ Count only tests passed" << std::endl;
}

void test_line_numbers() {
    std::cout << "Testing line numbers..." << std::endl;

    // Create a temporary test file
    std::ofstream test_file("test_lines.txt");
    test_file << "first line" << std::endl;
    test_file << "hello second" << std::endl;
    test_file << "third line" << std::endl;
    test_file << "hello fourth" << std::endl;
    test_file.close();

    Grep::Options opts;
    opts.line_numbers = true;
    Grep grep("hello", opts);
    grep.addFile("test_lines.txt");

    // Redirect stdout to capture output
    std::streambuf* orig = std::cout.rdbuf();
    std::ostringstream capture;
    std::cout.rdbuf(capture.rdbuf());

    grep.search();

    std::cout.rdbuf(orig);
    std::string output = capture.str();

    assert(output.find("2:hello second") != std::string::npos);
    assert(output.find("4:hello fourth") != std::string::npos);

    // Clean up
    std::remove("test_lines.txt");

    std::cout << "✓ Line numbers tests passed" << std::endl;
}

int main() {
    std::cout << "Running unit tests for tiny-grep..." << std::endl;

    try {
        test_basic_string_matching();
        test_case_insensitive();
        test_regex_matching();
        test_invert_match();
        test_file_search();
        test_count_only();
        test_line_numbers();

        std::cout << "\n✅ All unit tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed: " << e.what() << std::endl;
        return 1;
    }
}