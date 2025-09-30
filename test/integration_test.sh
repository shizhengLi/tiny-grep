#!/bin/bash

echo "Running integration tests for tiny-grep..."

# Build the project first
make all > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "❌ Build failed"
    exit 1
fi

# Create test directory structure
mkdir -p test_integration
cd test_integration

# Create test files
echo "hello world" > file1.txt
echo "This is a test" > file2.txt
echo "Another hello line" >> file2.txt
echo "No match here" > file3.txt
echo "HELLO in uppercase" > file4.txt

mkdir subdir
echo "hello from subdir" > subdir/file5.txt
echo "World domination" > subdir/file6.txt

echo "✅ Test environment setup complete"

# Test 1: Basic search
echo -e "\n🧪 Test 1: Basic search"
output=$(../tiny-grep "hello" file1.txt file2.txt file3.txt)
if echo "$output" | grep -q "hello world" && echo "$output" | grep -q "Another hello line" && ! echo "$output" | grep -q "No match"; then
    echo "✅ Test 1 passed"
else
    echo "❌ Test 1 failed"
    echo "Output: $output"
fi

# Test 2: Case insensitive search
echo -e "\n🧪 Test 2: Case insensitive search"
output=$(../tiny-grep -i "hello" file1.txt file4.txt)
if echo "$output" | grep -q "hello world" && echo "$output" | grep -q "HELLO in uppercase"; then
    echo "✅ Test 2 passed"
else
    echo "❌ Test 2 failed"
    echo "Output: $output"
fi

# Test 3: Line numbers
echo -e "\n🧪 Test 3: Line numbers"
output=$(../tiny-grep -n "hello" file2.txt)
if echo "$output" | grep -q "2:Another hello line"; then
    echo "✅ Test 3 passed"
else
    echo "❌ Test 3 failed"
    echo "Output: $output"
fi

# Test 4: Count only
echo -e "\n🧪 Test 4: Count only"
output=$(../tiny-grep -c "hello" file1.txt file2.txt file3.txt)
if echo "$output" | grep -q "file1.txt:1" && echo "$output" | grep -q "file2.txt:1" && echo "$output" | grep -q "file3.txt:0"; then
    echo "✅ Test 4 passed"
else
    echo "❌ Test 4 failed"
    echo "Output: $output"
fi

# Test 5: Invert match
echo -e "\n🧪 Test 5: Invert match"
output=$(../tiny-grep -v "hello" file2.txt)
if echo "$output" | grep -q "This is a test" && ! echo "$output" | grep -q "Another hello line"; then
    echo "✅ Test 5 passed"
else
    echo "❌ Test 5 failed"
    echo "Output: $output"
fi

# Test 6: Recursive search
echo -e "\n🧪 Test 6: Recursive search"
output=$(../tiny-grep -r "hello" .)
if echo "$output" | grep -q "hello world" && echo "$output" | grep -q "hello from subdir"; then
    echo "✅ Test 6 passed"
else
    echo "❌ Test 6 failed"
    echo "Output: $output"
fi

# Test 7: Regex search
echo -e "\n🧪 Test 7: Regex search"
output=$(../tiny-grep -E "h.llo" file1.txt file2.txt)
if echo "$output" | grep -q "hello world" && echo "$output" | grep -q "Another hello line"; then
    echo "✅ Test 7 passed"
else
    echo "❌ Test 7 failed"
    echo "Output: $output"
fi

# Test 8: Only matching
echo -e "\n🧪 Test 8: Only matching"
output=$(../tiny-grep -o "hello" file1.txt file2.txt)
if echo "$output" | grep -q "hello" && echo "$output" | grep -q "hello"; then
    echo "✅ Test 8 passed"
else
    echo "❌ Test 8 failed"
    echo "Output: $output"
fi

# Test 9: Stdin input
echo -e "\n🧪 Test 9: Stdin input"
output=$(echo -e "hello world\nNo match\nAnother hello" | ../tiny-grep "hello")
if echo "$output" | grep -q "hello world" && echo "$output" | grep -q "Another hello" && ! echo "$output" | grep -q "No match"; then
    echo "✅ Test 9 passed"
else
    echo "❌ Test 9 failed"
    echo "Output: $output"
fi

# Test 10: Help message
echo -e "\n🧪 Test 10: Help message"
output=$(../tiny-grep -h)
if echo "$output" | grep -q "Usage:" && echo "$output" | grep -q "OPTIONS"; then
    echo "✅ Test 10 passed"
else
    echo "❌ Test 10 failed"
    echo "Output: $output"
fi

# Cleanup
cd ..
rm -rf test_integration

echo -e "\n🎉 Integration tests complete!"