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
#ifndef LOGCOLOR_H
#define LOGCOLOR_H

#include <bits/stdc++.h>
#include <string>

#define ESC "\033"

namespace lc {

namespace fg {

    /**
     * @brief Foreground colors
     */
    enum COLOR {
        DEFAULT = 39,
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        LIGHT_GRAY = 37,
        DARK_GRAY = 90,
        LIGHT_RED = 91,
        LIGHT_GREEN = 92,
        LIGHT_YELLOW = 93,
        LIGHT_BLUE = 94,
        LIGHT_MAGENTA = 95,
        LIGHT_CYAN = 96,
        WHITE = 97
    };
}

namespace bg {

    /**
     * @brief Background colors
     */
    enum COLOR {
        DEFAULT = 49,
        BLACK = 40,
        RED = 41,
        GREEN = 42,
        YELLOW = 43,
        BLUE = 44,
        MAGENTA = 45,
        CYAN = 46,
        LIGHT_GRAY = 47,
        DARK_GRAY = 100,
        LIGHT_RED = 101,
        LIGHT_GREEN = 102,
        LIGHT_YELLOW = 103,
        LIGHT_BLUE = 104,
        LIGHT_MAGENTA = 105,
        LIGHT_CYAN = 106,
        WHITE = 107
    };
}

namespace fx {

    /**
     * @brief Text effects
     */
    enum STYLE {
        RESET_ALL = 0,
        BOLD = 1,
        DIM = 2,
        ITALIC = 3,
        UNDERLINE = 4,
        BLINK = 5,
        REVERSE = 7,
        HIDDEN = 8
    };
}

static inline const std::string string(std::string const& source,
    fg::COLOR fgColor = fg::DEFAULT,
    bg::COLOR bgColor = bg::DEFAULT,
    fx::STYLE effect = fx::RESET_ALL)
{
    std::stringstream str;

    str << ESC
        << "["
        << effect
        << ";"
        << bgColor
        << ";"
        << fgColor
        << "m"
        << source
        << ESC
        << "["
        << fx::RESET_ALL
        << "m";

    return str.str();
}

}

#endif // LOGCOLOR_H
