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
#include <iostream>
#include <stdio.h>
#include <unistd.h>

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
* @todo make cross platform
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

    uint64_t lineNumber = 1;

    while (getline(std::cin, input)) {
        parseLine(input);

        // @todo calculate spaces to align left
        if (displayLineNumber)
            std::cout << lc::string(std::to_string(lineNumber) + ".", fg::BLACK, bg::WHITE, fx::BOLD);

        std::cout << input << std::endl;
        lineNumber++;
    }

    exit(EXIT_SUCCESS);
}

inline void parseLine(std::string& line) noexcept
{
    std::string str;
    size_t pos = 0;

    /**
     * @todo make array stack-needle
    */
    for (auto it = defaultColorScheme.begin(); it < defaultColorScheme.end(); it++) {
        pos = line.find(it->find);

        if (pos != std::string::npos)
            line.replace(pos, it->find.length(), it->replace);
    }
}

void parse(char* filename)
{
    std::ifstream file(filename);

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
