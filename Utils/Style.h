//
// Created by Cata on 2/27/2024.
//

#ifndef LICENTA_STYLE_H
#define LICENTA_STYLE_H

#endif //LICENTA_STYLE_H

#include <string>

class ButtonStyle {
public:
    static std::string getButtonStyle() {
        return "QPushButton {\n"
               "\tbackground: rgb(0, 170, 255);\n"
               "\tborder-radius: 20px;\n"
               "}\n"
               "\n"
               "QPushButton:hover {\n"
               "\tbackground: rgb(0, 145, 217)\n"
               "}";
    }
};