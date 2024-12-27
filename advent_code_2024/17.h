//
// Created by ByteDance on 2024/12/18.
//

#ifndef INC_17_H
#define INC_17_H

#include <vector>
#include "util.h"

int ComboOperand(const int operand, const int reg_a, const int reg_b, const int reg_c) {
    switch (operand) {
        case 0:
        case 1:
        case 2:
        case 3:
            return operand;
        case 4:
            return reg_a;
        case 5:
            return reg_b;
        case 6:
            return reg_c;
        default:
            return -1;
    }
}

void DoOp(const int opcode, const int operand, const int combo, int &reg_a, int &reg_b, int &reg_c, int &idx, std::vector<int>& ret) {
    switch (opcode) {
        case 0: {
            reg_a = static_cast<int>(reg_a / (std::pow(2, combo)));
            return;
        }
        case 1: {
            reg_b = operand ^ reg_b;
            return;
        }
        case 2: {
            reg_b = combo % 8;
            return;
        }
        case 3: {
            return;
        }
        case 4: {
            reg_b = reg_b ^ reg_c;
            return;
        }
        case 5: {
            ret.emplace_back(combo % 8);
            return;
        }
        case 6: {
            reg_b = static_cast<int>(reg_a / (std::pow(2, combo)));
            return;
        }
        case 7: {
            reg_c = static_cast<int>(reg_a / (std::pow(2, combo)));
            return;
        }
        default:
            return;
    }
}

std::vector<int> RunTheThing(int reg_a, int reg_b, int reg_c, std::vector<int> program) {
    std::vector<int> result;
    int idx = 0;
    int idx_max = program.size();
    int opcode, operand;
    while (idx < idx_max) {
        opcode = program[idx];
        if ((idx + 1) == idx_max) {
            break;
        }
        operand = program[idx + 1];
        auto combo = ComboOperand(operand, reg_a, reg_b, reg_c);

        if (opcode == 3 && reg_a != 0) {
            idx = operand;
        } else {
            DoOp(opcode, operand, combo, reg_a, reg_b, reg_c, idx, result);
            idx+=2;
        }
    }
    return result;
}

int GetOutputs() {
    auto result = RunTheThing(117440, 0, 9, {0,3,5,4,3,0});
    PrintVec(result);
}

#endif //INC_17_H
