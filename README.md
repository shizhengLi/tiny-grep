# Tiny Grep

A lightweight, fast grep implementation in C++ with comprehensive features.

## Features

- 🔍 **Pattern Matching**: Support for both simple string and regex patterns
- 📁 **File Operations**: Search in single files, multiple files, or recursively in directories
- 🔤 **Case Insensitive Search**: Optional case-insensitive matching
- 📊 **Count Mode**: Show only the count of matching lines
- 🔄 **Invert Match**: Select non-matching lines
- 📝 **Line Numbers**: Display line numbers with matches
- 🎨 **Color Output**: Highlight matching strings (when using regex)
- 🎯 **Only Matching**: Show only the matching parts of lines
- 📂 **Recursive Search**: Search through directories recursively

## Installation

### Build from Source

```bash
git clone git@github.com:shizhengLi/tiny-grep.git
cd tiny-grep
make all
```

### Install System-wide (optional)

```bash
make install
```

## Usage

### Basic Usage

```bash
# Search for a pattern in a file
./tiny-grep "pattern" file.txt

# Search in multiple files
./tiny-grep "pattern" file1.txt file2.txt file3.txt

# Search recursively in directories
./tiny-grep -r "pattern" /path/to/directory

# Read from stdin
cat file.txt | ./tiny-grep "pattern"
```

### Command Line Options

| Option | Long Option | Description |
|--------|-------------|-------------|
| `-i` | `--ignore-case` | Ignore case distinctions in patterns and input data |
| `-n` | `--line-number` | Prefix each line of output with the line number |
| `-o` | `--only-matching` | Show only the part of lines that match the pattern |
| `-r` | `--recursive` | Read all files under each directory, recursively |
| `-v` | `--invert-match` | Select non-matching lines |
| `-c` | `--count` | Print only a count of matching lines per file |
| `-E` | `--extended-regexp` | Interpret pattern as an extended regular expression |
| `--color` | | Highlight matching strings in red |
| `-h` | `--help` | Display help message |

### Examples

#### Basic String Search

```bash
# Find all lines containing "error" in log.txt
./tiny-grep "error" log.txt

# Search for "hello" in multiple files
./tiny-grep "hello" *.txt
```

#### Case Insensitive Search

```bash
# Find "error", "Error", "ERROR", etc.
./tiny-grep -i "error" log.txt
```

#### Regular Expression Search

```bash
# Find all email addresses
./tiny-grep -E '\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b' file.txt

# Find lines starting with "error" or "warning"
./tiny-grep -E '^(error|warning)' log.txt
```

#### Count Matches

```bash
# Count how many times "error" appears in each file
./tiny-grep -c "error" *.log
```

#### Line Numbers

```bash
# Show line numbers with matches
./tiny-grep -n "error" log.txt
```

#### Invert Match

```bash
# Show all lines that don't contain "debug"
./tiny-grep -v "debug" log.txt
```

#### Recursive Search

```bash
# Search for "function" in all files under current directory
./tiny-grep -r "function" .

# Search in a specific directory
./tiny-grep -r "TODO" /path/to/project
```

#### Only Matching Parts

```bash
# Show only the matching parts (useful with regex)
./tiny-grep -o -E '\d{4}-\d{2}-\d{2}' file.txt  # Show only dates
```

#### Color Output

```bash
# Highlight matches in red (requires regex mode)
./tiny-grep -E --color "error" log.txt
```

## Testing

### Unit Tests

```bash
make test
```

### Integration Tests

```bash
./test/integration_test.sh
```

### Run All Tests

```bash
make test && ./test/integration_test.sh
```

## Project Structure

```
tiny-grep/
├── src/
│   ├── grep.h          # Header file with Grep class definition
│   ├── grep.cpp        # Implementation of Grep class
│   └── main.cpp        # Main program entry point
├── test/
│   ├── test_grep.cpp   # Unit tests
│   └── integration_test.sh  # Integration tests
├── Makefile            # Build configuration
└── README.md           # This documentation
```

## Architecture

### Core Components

1. **Grep Class**: Main class that handles all search operations
   - `matches()`: Pattern matching logic
   - `search()`: Main search orchestrator
   - `searchInFile()`: File-specific search
   - `searchInDirectory()`: Directory traversal
   - `searchStdin()`: Standard input processing

2. **Options Structure**: Configuration for search behavior
   - `ignore_case`: Case insensitive matching
   - `line_numbers`: Show line numbers
   - `only_matching`: Show only matches
   - `recursive`: Recursive directory search
   - `invert_match`: Invert match results
   - `count_only`: Show only counts
   - `color_output`: Highlight matches
   - `use_regex`: Use regex patterns

3. **Command Line Parsing**: Uses `getopt_long` for POSIX-style argument parsing

### Features Implementation

- **String Matching**: Simple substring search with optional case folding
- **Regex Support**: C++11 `<regex>` library with ECMAScript syntax
- **File Handling**: Standard C++ file I/O with proper error handling
- **Directory Traversal**: POSIX `dirent.h` for recursive search
- **Output Formatting**: Configurable output with prefixes and highlighting

## Building

### Prerequisites

- C++17 compatible compiler (g++ 7+, clang 5+)
- Make build system
- Standard C++ libraries

### Build Commands

```bash
# Build everything
make all

# Build main executable
make tiny-grep

# Build test runner
make test_runner

# Run tests
make test

# Clean build artifacts
make clean
```

## Performance

Tiny Grep is optimized for:
- **Memory efficiency**: Processes files line by line
- **Fast startup**: Minimal initialization overhead
- **Efficient searching**: Optimized pattern matching algorithms
- **Low resource usage**: No external dependencies

## Limitations

- Unicode support is limited (basic ASCII focus)
- Some advanced regex features may not be supported
- Large file performance could be further optimized
- No parallel processing for multiple files

## Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass
5. Submit a pull request

## License

This project is open source and available under the MIT License.

## Changelog

### v1.0.0
- Initial release
- Basic grep functionality
- Regex support
- Recursive directory search
- All major command line options
- Comprehensive test suite