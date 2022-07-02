#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <string>
#include <cstring>
#include <iomanip>
#include <math.h>
#include <vector>
using namespace std;

//A map to store the function codes & opcodes of different type of functions.
std::map<std::string, std::string> rop_code{
    {"001000", "addi"},
    {"100101", "lhu"},
    {"101001", "sh"},
    {"100011", "lw"},
    {"101011", "sw"},
    {"100001", "lh"},
    {"100000", "lb"},
    {"100100", "lbu"},
    {"101000", "sb"},
    {"000010", "j"},
    {"000011", "jal"},
    {"100001", "addu"},
    {"001001", "addiu"},
    {"110000", "ll"},
    {"111000", "sc"},
    {"001111", "lui"},
    {"001100", "andi"},
    {"011101", "ori"},
    {"000100", "beq"},
    {"000101", "bne"},
    {"101001", "sltu"},
    {"001010", "slti"},
    {"001001", "sltiu"},
    {"100011", "subu"},
    {"001110", "xori"}};
std::map<std::string, std::string> rfuncn_code{
    {"100000", "add"},
    {"100010", "sub"},
    {"001000", "jr"},
    {"100100", "and"},
    {"100101", "or"},
    {"100111", "nor"},
    {"000000", "sll"},
    {"000010", "srl"},
    {"000011", "sra"},
    {"000111", "srav"},
    {"000100", "sllv"},
    {"000110", "srlv"},
    {"101010", "slt"},
    {"100110", "xor"}

};
//to store all the addresses of registers
std::map<std::string, std::string> rregister_codes{
    {"00000", "$zero"},
    {"00010", "$v0"},
    {"00011", "$v1"},
    {"00100", "$a0"},
    {"00101", "$a1"},
    {"00110", "$a2"},
    {"00111", "$a3"},
    {"01000", "$t0"},
    {"01001", "$t1"},
    {"01010", "$t2"},
    {"01011", "$t3"},
    {"01100", "$t4"},
    {"01101", "$t5"},
    {"01110", "$t6"},
    {"01111", "$t7"},
    {"10000", "$s0"},
    {"10001", "$s1"},
    {"10010", "$s2"},
    {"10011", "$s3"},
    {"10100", "$s4"},
    {"10101", "$s5"},
    {"10110", "$s6"},
    {"10111", "$s7"},
    {"11000", "$t8"},
    {"11001", "$t9"},
    {"11100", "$gp"},
    {"11101", "$sp"},
    {"11110", "$fp"},
    {"11111", "$ra"}};

// r-format instruction with its different fields
typedef struct _r_format
{
    std::string _funct;
    std::string _rs = "$zero";
    std::string _rt = "$zero";
    std::string _rd = "$zero";
    std::string _shamt = "00000";
} r_format;

// i-format instruction with its different fields
typedef struct _i_format
{
    std::string _op;
    std::string _rs = "$zero";
    std::string _rt = "$zero";
    std::string _constant = "0";
} i_format;

// j-format instruction with its different fields
typedef struct _j_format
{
    std::string _op;
    std::string _address;
} j_format;

std::string GetDeciFromBin(std::string &_binary)
{
    long long n = 0;
    int len = _binary.length();
    std::string _decimal;
    if (_binary[0] == '1')
    {
        n = -1 * (1 << len - 1);
    }
    for (int i = 1; i < len; ++i)
    {
        n += (_binary[i] - '0') * (1 << len - 1 - i);
    }
    _decimal = to_string(n);
    return _decimal;
}

//to check if the given key exists in the given vector
bool check(std::vector<std::string> &_vector, std::string key)
{
    if (std::find(_vector.begin(), _vector.end(), key) != _vector.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    //we write vectors to store different functions/operations
    std::vector<std::string> r_format_in =
        {"add",
         "sub",
         "and",
         "or",
         "nor",
         "xor",
         "jr",
         "sll",
         "slt",
         "srl"};
    std::vector<std::string> i_format_in =
        {"lw",
         "sw",
         "beq",
         "bneq",
         "addi",
         "andi",
         "ori",
         "slti",
         "sltiu",
         "lh",
         "lhu",
         "sh",
         "lb",
         "lbu",
         "sb",
         "ll",
         "sc",
         "lui"};

    std::vector<std::string> j_format_in =
        {"j",
         "jal"};
    std::string file_name;
    cout << "Binary file Name for converting: ";
    cin >> file_name;
    ifstream bin_file;
    bin_file.open(file_name, ios::in);
    file_name = file_name.erase(file_name.find('.') - 3) + "asm.txt";
    ofstream asm_file;
    asm_file.open(file_name, ios::out);
    if (bin_file)
    {
        std::string _instructions, _instrct;
        std::string asm_output;
        while (getline(bin_file, _instructions))
        {
            //case 1:if instruction is of r-type
            if (_instructions.substr(0, 6) == "000000")
            {
                r_format r_frmt_in;
                r_frmt_in._funct = rfuncn_code[_instructions.substr(26, 6)];
                if (r_frmt_in._funct == "jr")
                {
                    r_frmt_in._rd = rregister_codes[_instructions.substr(16, 5)];

                    asm_output = r_frmt_in._funct + " " + r_frmt_in._rd;
                }
                else if (r_frmt_in._funct == "sll" || r_frmt_in._funct == "srl")
                {
                    r_frmt_in._rs = rregister_codes[_instructions.substr(6, 5)];
                    r_frmt_in._rt = rregister_codes[_instructions.substr(11, 5)];
                    r_frmt_in._rd = rregister_codes[_instructions.substr(16, 5)];
                    r_frmt_in._shamt = rregister_codes[_instructions.substr(21, 5)];

                    asm_output = r_frmt_in._funct + " " + r_frmt_in._rd + " " + r_frmt_in._rt + " " + GetDeciFromBin(r_frmt_in._shamt);
                }
                else
                {
                    r_frmt_in._rs = rregister_codes[_instructions.substr(6, 5)];
                    r_frmt_in._rt = rregister_codes[_instructions.substr(11, 5)];
                    r_frmt_in._rd = rregister_codes[_instructions.substr(16, 5)];
                    r_frmt_in._shamt = rregister_codes[_instructions.substr(21, 5)];

                    asm_output = r_frmt_in._funct + " " + r_frmt_in._rd + " " + r_frmt_in._rs + " " + r_frmt_in._rt;
                }
            }
            //Case 2: If the instruction is of i-type
            else if (check(i_format_in, rop_code[_instructions.substr(0, 6)]))
            {
                i_format i_frmt_in;
                i_frmt_in._op = rop_code[_instructions.substr(0, 6)];
                if (i_frmt_in._op == "lui")
                {
                    i_frmt_in._rt = rregister_codes[_instructions.substr(11,5)];
                    std::string temp=_instructions.substr(16,16);
                    i_frmt_in._constant = GetDeciFromBin(temp);

                    asm_output = i_frmt_in._op  + " " + i_frmt_in._rt + " " + i_frmt_in._constant;


                }

                else if (i_frmt_in._op == "addi" || i_frmt_in._op == "andi" || i_frmt_in._op == "ori" || i_frmt_in._op == "beq" || i_frmt_in._op == "bneq" || i_frmt_in._op == "slti" || i_frmt_in._op == "sltiu")
                {
                    i_frmt_in._rs= rregister_codes[_instructions.substr(6,5)];
                    i_frmt_in._rt = rregister_codes[_instructions.substr(11,5)];
                    std::string temp = _instructions.substr(16,16);
                    i_frmt_in._constant = GetDeciFromBin(temp);
                    asm_output = i_frmt_in._op + " " + i_frmt_in._rt + ", " + i_frmt_in._rs + ", " + i_frmt_in._constant;
                }

                else if (i_frmt_in._op == "lw" || i_frmt_in._op == "sw" || i_frmt_in._op == "lh" || i_frmt_in._op == "lhu" || i_frmt_in._op == "sh" || i_frmt_in._op == "lb" || i_frmt_in._op == "lbu" || i_frmt_in._op == "sb" || i_frmt_in._op == "ll" || i_frmt_in._op == "sc")
                {
                    i_frmt_in._rs = rregister_codes[_instructions.substr(6,5)];
                    i_frmt_in._rt = rregister_codes[_instructions.substr(11,5)];
                    std::string temp = _instructions.substr(16,16);
                    i_frmt_in._constant = GetDeciFromBin(temp);

                    asm_output = i_frmt_in._op + " " + i_frmt_in._rt + ", " + i_frmt_in._constant + "(" + i_frmt_in._rs + ")";
                }

            }
            //Case 3: If the instruction is of j-type
            else{
                j_format j_frmt_in;
                j_frmt_in._op = rop_code[_instructions.substr(0,6)];
                std::string temp = _instructions.substr(6,26);
                j_frmt_in._address = GetDeciFromBin(temp);

                asm_output = j_frmt_in._op + " " + j_frmt_in._address;
            }
             asm_file << asm_output << endl;
        }
    }
    return 0;
}