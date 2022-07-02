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
std::map<std::string, std::string> op_code{
    {"addi", "001000"},
    {"lhu", "100101"},
    {"sh", "101001"},
    {"lw", "100011"},
    {"sw", "101011"},
    {"lh", "100001"},
    {"lb", "100000"},
    {"lbu", "100100"},
    {"sb", "101000"},
    {"j", "000010"},
    {"jal", "000011"},
    {"addu", "100001"},
    {"addiu", "001001"},
    {"ll", "110000"},
    {"sc", "111000"},
    {"lui", "001111"},
    {"andi", "001100"},
    {"ori", "011101"},
    {"beq", "000100"},
    {"bneq", "000101"},
    {"sltu", "101001"},
    {"slti", "001010"},
    {"sltiu", "001001"},
    {"subu", "100011"},
    {"xori", "001110"}};
std::map<std::string, std::string> funcn_code{
    {"add", "100000"},
    {"sub", "100010"},
    {"jr", "001000"},
    {"and", "100100"},
    {"or", "100101"},
    {"nor", "100111"},
    {"sll", "000000"},
    {"srl", "000010"},
    {"sra", "000011"},
    {"srav", "000111"},
    {"sllv", "000100"},
    {"srlv", "000110"},
    {"slt", "101010"},
    {"xor", "100110"},

};
//to store all the addresses of registers
std::map<std::string, std::string> register_codes{
    {"$zero", "00000"},
    {"$v0", "00010"},
    {"$v1", "00011"},
    {"$a0", "00100"},
    {"$a1", "00101"},
    {"$a2", "00110"},
    {"$a3", "00111"},
    {"$t0", "01000"},
    {"$t1", "01001"},
    {"$t2", "01010"},
    {"$t3", "01011"},
    {"$t4", "01100"},
    {"$t5", "01101"},
    {"$t6", "01110"},
    {"$t7", "01111"},
    {"$s0", "10000"},
    {"$s1", "10001"},
    {"$s2", "10010"},
    {"$s3", "10011"},
    {"$s4", "10100"},
    {"$s5", "10101"},
    {"$s6", "10110"},
    {"$s7", "10111"},
    {"$t8", "11000"},
    {"$t9", "11001"},
    {"$gp", "11100"},
    {"$sp", "11101"},
    {"$fp", "11110"},
    {"$ra", "11111"}};

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

//A function to convert a binary string to a Hexadecimal string
string GetHexFromBin(string &_binary)
{
    string _hex("0x"), _temp, chr = "";
    int len = _binary.length() / 4;
    chr = chr.substr(0, len);
    _binary = chr + _binary;
    for (int i = 0; i < _binary.length(); i += 4)
    {
        _temp = _binary.substr(i, 4);
        if (!_temp.compare("0000"))
        {
            _hex = _hex + "0";
        }
        else if (!_temp.compare("0001"))
        {
            _hex = _hex + "1";
        }
        else if (!_temp.compare("0010"))
        {
            _hex = _hex + "2";
        }
        else if (!_temp.compare("0011"))
        {
            _hex = _hex + "3";
        }
        else if (!_temp.compare("0100"))
        {
            _hex = _hex + "4";
        }
        else if (!_temp.compare("0101"))
        {
            _hex = _hex + "5";
        }
        else if (!_temp.compare("0110"))
        {
            _hex = _hex + "6";
        }
        else if (!_temp.compare("0111"))
        {
            _hex = _hex + "7";
        }
        else if (!_temp.compare("1000"))
        {
            _hex = _hex + "8";
        }
        else if (!_temp.compare("1001"))
        {
            _hex = _hex + "9";
        }
        else if (!_temp.compare("1010"))
        {
            _hex = _hex + "A";
        }
        else if (!_temp.compare("1011"))
        {
            _hex = _hex + "B";
        }
        else if (!_temp.compare("1100"))
        {
            _hex = _hex + "C";
        }
        else if (!_temp.compare("1101"))
        {
            _hex = _hex + "D";
        }
        else if (!_temp.compare("1110"))
        {
            _hex = _hex + "E";
        }
        else if (!_temp.compare("1111"))
        {
            _hex = _hex + "F";
        }
        else
        {
            continue;
        }
    }
    return _hex;
}

string GetBinFromDeci(const string &_decimal, int _size)
{
    int n = atoi(_decimal.c_str());
    // array to store binary number
    int binaryNum[32];
    // counter for binary array
    int i = 0;
    while (n > 0)
    {

        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
    std::string _binary;
    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--)
    {
        string temp = to_string(binaryNum[j]);
        _binary.append(temp);
    }
    while (_binary.length() < _size)
    {
        _binary = "0" + _binary;
    }
    return _binary;
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
    cout << "MIPS file Name for converting: ";
    cin >> file_name;
    ifstream asm_file;
    asm_file.open(file_name, ios::in);
    file_name = file_name.erase(file_name.find('.')) + "bin.txt";
    ofstream bin_file;
    bin_file.open(file_name, ios::out);
    if (asm_file)
    {
        std::string _instructions, _instrct;
        std::string bin_output = "test";
        while (getline(asm_file, _instructions))
        {
            std::string hex_output;
            std::string _copy_instructions = _instructions;
            char *c = const_cast<char *>(_instructions.c_str());
            char *token;
            token = strtok(c, " ");
            int count = 0;
            std::vector<std::string> words;
            while (token != NULL)
            {
                if (token[0] == '#')
                {
                    break;
                }
                else
                {
                    _instrct = token;
                    if (_instrct.back() == ',')
                    {
                        _instrct.erase(_instrct.end() - 1);
                    }
                    words.push_back(_instrct);
                }
                token = strtok(NULL, " ");
                count++;
            }
            //case 1:if instruction is of r-type
            if (check(r_format_in, words[0]))
            {
                r_format r_frmt_in;
                r_frmt_in._funct = words[0];
                if (r_frmt_in._funct == "jr")
                {
                    r_frmt_in._rd = words[1];

                    bin_output = "000000" + register_codes[r_frmt_in._rs] + "000000000000000" + funcn_code[r_frmt_in._funct];
                }

                else if (r_frmt_in._funct == "sll" || r_frmt_in._funct == "srl")
                {
                    r_frmt_in._rd = words[1];
                    r_frmt_in._rt = words[2];
                    r_frmt_in._shamt = GetBinFromDeci(words[3], 5);

                    bin_output = "00000000000" + register_codes[r_frmt_in._rt] + register_codes[r_frmt_in._rd] + r_frmt_in._shamt + funcn_code[r_frmt_in._funct];
                }
                else
                {
                    r_frmt_in._rd = words[1];
                    r_frmt_in._rs = words[2];
                    r_frmt_in._rt = words[3];

                    bin_output = "000000" + register_codes[r_frmt_in._rs] + register_codes[r_frmt_in._rt] + register_codes[r_frmt_in._rd] + r_frmt_in._shamt + funcn_code[r_frmt_in._funct];
                }
            }
            //Case 2: If the instruction is of i-type
            else if (check(i_format_in, words[0]))
            {
                _i_format i_frmt_in;
                i_frmt_in._op = words[0];
                if (i_frmt_in._op == "lui")
                {
                    i_frmt_in._rt = words[1];
                    i_frmt_in._constant = GetBinFromDeci(words[2], 16);

                    bin_output = op_code[i_frmt_in._op] + "00000" + register_codes[i_frmt_in._rt] + i_frmt_in._constant;
                }
                else if (i_frmt_in._op == "addi" || i_frmt_in._op == "andi" || i_frmt_in._op == "ori" || i_frmt_in._op == "beq" || i_frmt_in._op == "bneq" || i_frmt_in._op == "slti" || i_frmt_in._op == "sltiu")
                {
                    i_frmt_in._rt = words[1];
                    i_frmt_in._rs = words[2];
                    i_frmt_in._constant = GetBinFromDeci(words[3], 16);

                    bin_output = op_code[i_frmt_in._op] + register_codes[i_frmt_in._rs] + register_codes[i_frmt_in._rt] + i_frmt_in._constant;
                }
                else if (i_frmt_in._op == "lw" || i_frmt_in._op == "sw" || i_frmt_in._op == "lh" || i_frmt_in._op == "lhu" || i_frmt_in._op == "sh" || i_frmt_in._op == "lb" || i_frmt_in._op == "lbu" || i_frmt_in._op == "sb" || i_frmt_in._op == "ll" || i_frmt_in._op == "sc")
                {
                    i_frmt_in._rt = words[1];
                    i_frmt_in._rs = words[2].substr(words[2].find('(') + 1, 3);
                    i_frmt_in._constant = GetBinFromDeci(words[2].erase(words[2].find('(')), 16);

                    bin_output = op_code[i_frmt_in._op] + register_codes[i_frmt_in._rs] + register_codes[i_frmt_in._rt] + i_frmt_in._constant;
                }
            }
            //Case 3: If the instruction is of j-type
            else if (check(j_format_in, words[0]))
            {
                j_format j_frmt_in;
                j_frmt_in._op = words[0];
                j_frmt_in._address = GetBinFromDeci(words[1], 26);

                bin_output = op_code[j_frmt_in._op] + j_frmt_in._address;
            }

            hex_output = GetHexFromBin(bin_output);
            bin_file << bin_output << " (" << hex_output << ';' << _copy_instructions << ')' << endl;
        }
        asm_file.close();
        bin_file.close();
    }
    return 0;
}
