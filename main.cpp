/*
 * logcolor is hosted at http://github.com/bariscelik/logcolor
 * and distributed under the MIT License (MIT).
 *
 * logcolor - colorized log viewer utility
 * Copyright (c) 2020 Barış Çelik <bariscelikweb@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include "logcolor.h"
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

#define CHAR_SPACE ' '

using namespace lc;

extern void initColorScheme();

extern void parse(char*);

extern void usage(int);

extern void readPipe();

extern inline void parseLine(std::string&) noexcept;

bool displayLineNumber = false;

typedef struct {
    std::string find;
    std::string replace;
} MATCH;

std::vector<MATCH> defaultColorScheme;

/**
* @todo number of rows to display (last 300 lines: <lc -n 300>)
* @todo define color template files (lc.ini)
* @todo store data incoming from std::cin in cache buffer on RAM, catch throw if occures an allocation error (std::string exception)
* @test pipe a 0.5M size of file with below commands (to test memory allocation is ok):
*       systemd-run --scope -p MemoryLimit=100K cat /var/log/syslog | lc -n
*       or ulimit
*       create .sh file for tests
*/
int main(int argc, char* argv[])
{

    int opt;

    initColorScheme();

    while ((opt = getopt(argc, argv, ":f:hl")) != -1) {
        switch (opt) {
        case 'h':
            usage(EXIT_SUCCESS);
            break;
        case 'f':
            parse(optarg);
            break;
        case 'l':
            displayLineNumber = true;
            break;
        case ':':
            std::cout << "option needs a value" << std::endl;
            break;
        case '?':
            std::cout << "unknown option: " << (char)optopt << std::endl;
            break;
        default:
            usage(EXIT_FAILURE);
            break;
        }
    }

    // if buffer isn't empty, read pipe (|) input
#ifdef _WIN32
    if (!_isatty(_fileno(stdin)))
        readPipe();
#else
    if (!isatty(fileno(stdin)))
        readPipe();
#endif

    if (argc <= 1)
        usage(EXIT_FAILURE);

    return 0;
}

/*constexpr {
    return totalDigit - numDigit
}*/

inline std::size_t lineCount(std::ifstream* file = nullptr)
{
    std::size_t count = 0;

    assert(file != nullptr && "File stream is null.");

    count = std::count_if(std::istreambuf_iterator<char>(*file),
        std::istreambuf_iterator<char>(),
        [](char c) {
            return c == '\n';
        });

    file->seekg(0);

    return count;
}

/**
 * @brief calculate line count for pipe
 * @param os
 * @return line count in string
 */
inline uint64_t lineCount(std::string* os = nullptr)
{
    uint64_t count = 0;

    assert(os != nullptr && "Stringstream is null");

    // pipe
    count = std::count_if(std::istreambuf_iterator<char>(std::cin),
        std::istreambuf_iterator<char>(),
        [&os](char c) {
            os->push_back(c);
            return c == '\n';
        });

    return count;
}

inline std::string appendAlignDigits(uint64_t& lineNumber, uint8_t maxDigit)
{
    std::string lineNumberStr = std::to_string(lineNumber);

    const size_t spaceCount = maxDigit - lineNumberStr.size();

    lineNumberStr.push_back('.');
    if (spaceCount > 0)
        lineNumberStr.append(spaceCount, CHAR_SPACE);

    lineNumberStr = lc::string(lineNumberStr, fg::BLACK, bg::WHITE, fx::BOLD);

    return lineNumberStr;
}

void initColorScheme()
{
    defaultColorScheme = {
        MATCH({ .find = "[info]", .replace = lc::string("[info]", fg::CYAN, bg::DEFAULT, fx::BOLD) }),
        MATCH({ .find = "[debug]", .replace = lc::string("[debug]", fg::DARK_GRAY, bg::WHITE, fx::BOLD) }),
        MATCH({ .find = "[warning]", .replace = lc::string("[warning]", fg::BLUE, bg::GREEN, fx::BOLD) }),
        MATCH({ .find = "[error]", .replace = lc::string("[error]", fg::WHITE, bg::RED, fx::BOLD) }),
    };
}

/**
 * \brief reads stdin pipe as input
 */
void readPipe()
{
    std::string input;

    // 1000 char cache buffer, preallocation for performance
    input.reserve(1000);

    uint64_t lineNumber = 1;

    if (!displayLineNumber) {
        while (getline(std::cin, input)) {
            parseLine(input);
            std::cout << input << std::endl;
        }
    } else {
        size_t lCount = 0;
        size_t maxDigit = 0;
        std::string s;

        try {

            // calculate line count
            lCount = lineCount(&s);
            maxDigit = std::to_string(lCount).size();

            std::istringstream str(s);

            while (getline(str, input)) {
                parseLine(input);
                std::cout << appendAlignDigits(lineNumber, maxDigit) << input << std::endl;
                lineNumber++;
            }

        } catch (const std::bad_alloc& ex) {

            std::cout << ex.what() << std::endl;
            std::cerr << "There is no enough memory!" << std::endl;
        }
    }

    exit(EXIT_SUCCESS);
}

inline void parseLine(std::string& line) noexcept
{
    std::string str;
    size_t pos = 0;

    for (auto it = defaultColorScheme.begin(); it < defaultColorScheme.end(); it++) {
        pos = line.find(it->find);

        if (pos != std::string::npos)
            line.replace(pos, it->find.length(), it->replace);
    }
}

void parse(char* filename)
{
    std::ifstream file(filename, std::ios::binary);

    lineCount(&file);

    uint64_t lineNumber = 1;

    if (!file.is_open()) {
        std::cout << "Error: File could not loaded." << std::endl;
        return;
    }

    std::string str;

    while (std::getline(file, str)) {

        parseLine(str);

        if (displayLineNumber)
            std::cout << lc::string(std::to_string(lineNumber) + ".", fg::BLACK, bg::WHITE, fx::BOLD);

        std::cout << str << std::endl;
        lineNumber++;
    }
}

void usage(int status)
{
    if (status != EXIT_SUCCESS)

        std::cout << "Usage: logcolor [OPTION]... [FILE]...\n"
                  << "Try `logcolor --help' for more information.\n"
                  << std::endl;
    else {
        std::cout << "Usage: logcolor [OPTION]... [FILE]...\n"
                  << "Colorize log file(s), or standard input, to standard output.\n"
                  << "-f, --file           input file\n"
                  << "-n, --line-count     number of last visible lines \n\n"
                  << "-h  --help     display this help and exit\n"
                  << std::endl;

        std::cout << "Examples:\n"
                  << "logcolor -f file.log  Output colorized contents of file.log\n"
                  << "logcolor              Convert standard input to colorized standard output." << std::endl;
    }
    exit(status);
}
