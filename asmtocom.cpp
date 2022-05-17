#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

struct Register //Структура регистра
{
    string name; //Имя регистра
    string code; //Двоичный код регистра
    string w; //w = 0 - 8 битный регистр, w = 1 - 16 битный регистр
};
Register arrayOfReg[16] =
{
   {"ax", "000", "1"}, {"cx", "001", "1"}, {"dx", "010", "1"}, {"bx", "011", "1"}, {"sp", "100", "1"},{"bp", "101", "1"}, {"si", "110", "1"}, {"di", "111", "1"}, 
   {"al", "000", "0"}, {"cl", "001", "0"}, {"dl", "010", "0"}, {"bl", "011", "0"}, {"ah", "100", "0"}, {"ch", "101", "0"}, {"dh", "110", "0"},{"bh", "111", "0"},
};

struct CodeFormat //Структура команды
{
    string name; //Имя команды
    string KOP; //Двоичный код операции команды
};

CodeFormat arrayOfCommands[5]
{
    {"je", "01110100"}, //74
    {"jne", "01110101"}, //75
    {"cmp", "001110"},  //rr8 38 rr16 39 //rrm8 3A rrm16 3B
    {"sub", "001010"},  //rr8 28 rr16 29 //rrm8 2A rrm16 2B
};

struct InderectAdressing
{
    string name;
    string code;
};

InderectAdressing arrayOfIA[5]
{
    {"[bx+si]","000"}, {"[bx+di]","001"}, {"[si]","100"}, {"[di]","101"}, {"[bx]","111"},
};


string ConvertDigit(string str, int fromD, int toN) //Преобразует число из системы счисления fromD к системе счисления toN
{
    //Преобразуем число из fromD системы считсления к 10-ой системе счисления
    int digit = 0;
    string output = "";
    if (fromD == 10) digit = stoi(str);
    else
    {
        for (int i = str.length() - 1; i >= 0; i--)
        {
            if (isdigit(str[i])) digit += (str[i] - '0') * pow(fromD, (str.length() - 1 - i));
            else
            {
                string temp = "1";
                temp.push_back(str[i] - 17);
                digit += stoi(temp) * pow(fromD, (str.length() - 1 - i));
            }
        }
    }
    if (toN == 10) return to_string(digit);
    //Переводим число из 10-ной системы счисления к toN-ой системе счисления
    while (digit)
    {
        if (digit % toN >= 10) output.push_back((char)((digit % toN) + 55));
        else output.push_back((char)((digit % toN) + 48));
        digit /= toN;
    }
    reverse(output.begin(), output.end());
    return output;
}

CodeFormat FindCommandByName(string nameOfCommand)
{
    for (int i = 0; i < 5; i++)
        if (arrayOfCommands[i].name == nameOfCommand) return arrayOfCommands[i];
    return CodeFormat{ "","" };
}

Register FindRegisterByName(string nameOfRegister)
{
    for (int i = 0; i < 16; i++)
        if (arrayOfReg[i].name == nameOfRegister) return arrayOfReg[i];
    return Register{ "","", "" };
}

InderectAdressing FindIAByName(string nameOfIA)
{
    for (int i = 0; i < 5; i++)
        if (arrayOfIA[i].name == nameOfIA) return arrayOfIA[i];
    return InderectAdressing{ "","" };
}

int main()
{
    vector<vector<string>> lines; //Сюда будем записывать строки входного файла
    string line;

    ifstream in("hellonew.asm");
    if (in.is_open())
    {
        while (getline(in, line))
            if (line != "")
            {
                vector<string> buffer;
                string temp = "";
                for (int i = 0; i <= line.size(); i++)
                {
                    if (line[i] != ' ' && line[i] != '\0' && line[i] != '\t' && line[i] != ',') temp += line[i]; //считываем слово
                    else if (temp != "")
                    {
                        buffer.push_back(temp);
                        temp = "";
                    }
                }
                lines.push_back(buffer);
            }
    }
    in.close();

    int shift = 0;
    vector<int> displacement;
    for (int i = 0; i < lines.size(); i++)
    {
        displacement.push_back(shift);
        for (int j = 0; j < lines[i].size(); j++)
            if (lines[i][j] == "je" || lines[i][j] == "jne" || lines[i][j] == "sub" || lines[i][j] == "cmp") 
                shift += 2;
    }

    vector<int> machineCode;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            if (lines[i][j] == "je" || lines[i][j] == "jne")
            {
                //1)
                machineCode.push_back(stoi(ConvertDigit(FindCommandByName(lines[i][j]).KOP, 2, 10)));

                //2)
                int displacementOfLabel = 0;
                for (int k = 0; k < lines.size(); k++)
                    if (lines[k][0].substr(0, lines[k][0].size() - 1) == lines[i][j + 1]) 
                        displacementOfLabel = displacement[k];
                if (displacementOfLabel > displacement[i])
                    machineCode.push_back(displacementOfLabel - displacement[i] - 2);
                else 
                    machineCode.push_back(stoi(ConvertDigit("FE", 16, 10)) - (displacement[i] - displacementOfLabel));
            }
            else if (lines[i][j] == "sub" || lines[i][j] == "cmp")
            {
                if (lines[i][j + 2] == "[bx]" || lines[i][j + 2] == "[si]" || lines[i][j + 2] == "[di]" || lines[i][j + 2] == "[bx+si]" || lines[i][j + 2] == "[bx+di]")
                {
                    CodeFormat foundCode = FindCommandByName(lines[i][j]);  //r,m
                    Register foundRegister = FindRegisterByName(lines[i][j + 1]);
                    InderectAdressing foundIA = FindIAByName(lines[i][j + 2]);

                    machineCode.push_back(stoi(ConvertDigit(foundCode.KOP +"1"+ foundRegister.w, 2, 10)));
                    machineCode.push_back(stoi(ConvertDigit("00" + foundRegister.code + foundIA.code, 2, 10)));
                }
                else
                {
                    CodeFormat foundCode = FindCommandByName(lines[i][j]);  //r,r
                    Register foundRegisterD = FindRegisterByName(lines[i][j + 1]);
                    Register foundRegisterS = FindRegisterByName(lines[i][j + 2]);

                    machineCode.push_back(stoi(ConvertDigit(foundCode.KOP +"0"+ foundRegisterD.w, 2, 10)));
                    machineCode.push_back(stoi(ConvertDigit("11" + foundRegisterS.code + foundRegisterD.code, 2, 10)));
                }
            }
        }
    }

    ofstream fs("output1.bin", ios::out | ios::binary);
    if (!fs)
    {
        cout << "Cannot open file!" << endl;
        return 1;
    }

    for (int i = 0; i < machineCode.size(); i++)
        fs.write((char*)&machineCode[i], sizeof(char));
    return 1;
}