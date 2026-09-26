#!/bin/bash

# Program: Single Quotes and Double Quotes

name="Shashank"
message="Hello World"

echo "===== SINGLE QUOTES ====="

# Single quotes preserve literal content
echo 'Hello $name'
echo 'Welcome to Shell Scripting'
echo 'Special characters: $ @ # ! *'

echo ""
echo "===== DOUBLE QUOTES ====="

# Double quotes allow variable expansion
echo "Hello $name"
echo "Message: $message"
echo "Welcome to Shell Scripting"

echo ""
echo "===== COMPARISON ====="

echo 'Single quotes: $name'
echo "Double quotes: $name"

echo ""
echo "===== SPACES ====="

text="Linux Shell Programming"
echo "$text"
echo 'Linux Shell Programming'

echo ""
echo "===== EDGE CASES ====="

# Variable inside single quotes
echo 'Value of name is $name'

# Variable inside double quotes
echo "Value of name is $name"

# Special character handling
echo 'The price is $100'
echo "The price is \$100"

echo ""
echo "===== COMMAND PARSING ====="

command="echo Hello"
echo "Stored command: $command"

echo "Program completed successfully."
