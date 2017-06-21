#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <ctype.h>
#include <cstring>

#define DEBUG 0
using namespace std;



bool ValidKey(string pKey)
{
    unsigned short int keyLen = pKey.length();
    string key = pKey.substr(2,keyLen);
    if (key=="")
        return false;
    else
        return true;
}

string StripKey(string pKey)
{
    using std::tolower;
    vector<char> usedLetters;
    string strippedKey;
    unsigned short int keyLen = pKey.length();
    string key = pKey.substr(2,keyLen);

    for (unsigned int x = 0; x < key.length(); x++)
    {
        key[x] = toupper(key[x]);
    }

    bool flag = false;
    for (unsigned int x = 0; x < key.length(); x++)
    {
        for (auto const& vLetter: usedLetters )
        {
            if (key[x] == vLetter)
            {
                flag = true;
            }

        }
        if (!flag)
        {
            strippedKey = strippedKey + key[x];
            usedLetters.push_back(key[x]);
        }
        else
        {
            flag = false;
        }
    }
    return strippedKey;
}

map<char,char> GenerateCipherMap(string pKey)
{
    map<char,char> tempCipherMap;
    string key = pKey;
    string originalKey = pKey;
    string cipherEnding = "";
    bool flag = false;
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (unsigned int c = 0; c < alphabet.length(); c++)
    {
        for (unsigned int x = 0; x < key.length(); x++)
        {
            if (key[x] == alphabet[c])
            {
                flag = true;
            }

        }
        if (!flag)
        {
            cipherEnding = cipherEnding + alphabet[c];
        }
        else
            flag = false;


    }

    string reverseCipherEnding;

    for (unsigned int c = 1; c <= cipherEnding.length(); c++)
    {
        reverseCipherEnding = reverseCipherEnding + cipherEnding[cipherEnding.length() - c];
    }

    string cipheredSide = key + reverseCipherEnding;

    for (unsigned int c = 0; c < alphabet.length(); c++)
    {
        char plain = alphabet[c];
        char ciph = cipheredSide[c];
        tempCipherMap[plain]=ciph;
    }
    return tempCipherMap;
}

string GenerateCipherText(string pPlainText, map<char,char> pCipherMap)
{
    string tempCipherText;
    string plainText = pPlainText;

    for (unsigned int x = 0; x < plainText.length(); x++)
    {
        plainText[x] = toupper(plainText[x]);
    }

    for (unsigned int c = 0; c < plainText.length(); c++)
    {
        tempCipherText = tempCipherText + pCipherMap[plainText[c]];
    }
    return tempCipherText;
}

string DecodeCipherText(string pCipherText, map<char,char> pCipherMap)
{
    string tempPlainText;
    string cipherText = pCipherText;
    map<char,char> cipherMap = pCipherMap;
    map<char,char>::key_compare mycomp = cipherMap.key_comp();
    char highest = pCipherMap.rbegin()->first;
    for (unsigned int x = 0; x < cipherText.length(); x++)
    {
        std::map<char,char>::iterator it = cipherMap.begin();
        do {

            if (it->second == cipherText[x])
                tempPlainText = tempPlainText + it->first;
        } while ( mycomp((*it++).first, highest) );
    }
    return tempPlainText;
}

void SaveStringToFile(string pText, string pFile)
{
    ofstream outFile(pFile,ofstream::out);
    outFile << pText;
    outFile.close();
}

string LoadFileToString(string pFile)
{
    ifstream plainTextFile(pFile,ifstream::in);
    string plainTextBuffer;
    char charBuffer = plainTextFile.get();
    while (plainTextFile.good())
    {
        plainTextBuffer = plainTextBuffer + charBuffer;
        charBuffer = plainTextFile.get();
    }
    plainTextFile.close();
    return plainTextBuffer;
}

void Encrypt(string key, string inFile, string outFile)
{
    map<char,char> cipherMap;
    if (!ValidKey(key))
    {
        cout << "Invalid Key Supplied!" << endl;
        return;
    }
    else
    {
        key = StripKey(key);
        cipherMap = GenerateCipherMap(key);

        //ENCRYPT TIME
        string cipherText = GenerateCipherText(LoadFileToString(inFile), cipherMap);
        SaveStringToFile(cipherText,outFile);
    }

}

void Decrypt(string key, string inFile, string outFile)
{
    map<char,char> cipherMap;
    if (!ValidKey(key))
    {
        cout << "Invalid Key Supplied!" << endl;
        return;
    }
    else
    {
        key = StripKey(key);
        cipherMap = GenerateCipherMap(key);

        //DECRYPT TIME
        string plainText = DecodeCipherText(LoadFileToString(inFile),cipherMap);
        SaveStringToFile(plainText, outFile);
    }
}


vector<pair<char,int>> CountCharacters(string pCipherText)
{
    cout << "CC" << endl;
    vector<pair<char,int>> characterOccurences;
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (unsigned int x = 0; x < alphabet.length(); x++)
    {
        pair<char,int> tempPair;
        tempPair.first = alphabet[x];
        tempPair.second = 0;
        characterOccurences.push_back(tempPair);
    }
    for (unsigned int x = 0; x < pCipherText.length(); x++)
    {
        for(unsigned int y = 0; y < 27; y++)
        {
            if (pCipherText[x] == characterOccurences[y].first)
            {
                characterOccurences[y].second = characterOccurences[y].second + 1;
            }
        }
    }
    return characterOccurences;
}

vector<pair<char,int>> FreqAnalyze(string inFile, bool printResult = true)
{
    cout << "FA" << endl;
    cout << "infile: " << inFile << endl;
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string cipherText = LoadFileToString(inFile);
    vector<pair<char,int>> mapCharacterOccurence;
    mapCharacterOccurence = CountCharacters(cipherText);
    if (printResult)
    {
        for (unsigned int x = 0; x < alphabet.length(); x++)
        {
            for(auto & value: mapCharacterOccurence)
            {
                cout << "Try " << alphabet[x] << endl;
                if (alphabet[x] == value.first)
                {
                    cout << alphabet[x] << ":" << value.first << " | " << ((value.second/26)*100) << "%" << endl;
                }
            }
        }
    }
    return mapCharacterOccurence;
}

void Attack(string inFile, string outFile, bool printResult = true)
{
    string cipherText = LoadFileToString(inFile);
    vector<pair<char,int>> mapCharacterOccurence = FreqAnalyze(inFile,false);
    //Least to most occurrence alphabet.
    string alphabetStdOccurence = "ZQXJKVBPYGFWMUCLDRHSNIOATE";
    map<char,char> matchedCharacters;
    cout << "Likely Matches:" << endl;
    cout << "Plain | Cipher" << endl;
    for (unsigned int x = 0; x < alphabetStdOccurence.length(); x++)
    {
        for(auto const& value: mapCharacterOccurence)
        {
            if (alphabetStdOccurence[x] == value.first)
            {
                cout << alphabetStdOccurence[x] << " = " << value.first << endl;
                matchedCharacters[alphabetStdOccurence[x]]=value.first;
            }
        }

    }

    string plainText = DecodeCipherText(cipherText,matchedCharacters);
    cout << "Plain Text Below:" << endl;
    cout << plainText;
}


void PrintUsage(string pProgramName)
{
    cout <<"usage: "<< pProgramName << " <-e to encrypt | -d to decrypt> <-k$$$$$$ cipher key> <filename of text or ciphertext> <filename of output>\n";
    cout <<"<cont> "<< pProgramName << " <-f freq Analysis | -a freq Attack> <filename of text or ciphertext> <for -a filename of output>\n";
}

int main( int argc, char *argv[] )
{

    cout << argc << endl;

    //No Args, manual entry
    if (argc < 2)
    {
        cout << "1 - Decrypt 2 - Encrypt 3 - FreqAnalyze 4 - Attack 5 - Exit" << endl;
        int menu = 0;
        cin >> menu;
        if (menu == 1)
        {
            cout << endl;
            cout << "CipherKey:";
            string cipherKey;
            cin >> cipherKey;
            cipherKey = "-k" + cipherKey;
            cout << endl;
            cout << "InFile:";
            string inFile;
            cin >> inFile;
            cout << endl;
            cout << "OutFile:";
            string outFile;
            cin >> outFile;
            Decrypt(cipherKey,inFile, outFile);
            return 0;
        }
        if (menu == 2)
        {
            cout << endl;
            cout << "CipherKey:";
            string cipherKey;
            cin >> cipherKey;
            cipherKey = "-k" + cipherKey;
            cout << endl;
            cout << "InFile:";
            string inFile;
            cin >> inFile;
            cout << endl;
            cout << "OutFile:";
            string outFile;
            cin >> outFile;
            Encrypt(cipherKey,inFile, outFile);
            return 0;
        }
        if (menu == 3)
        {
            cout << endl;
            cout << "InFile:";
            string inFile;
            cin >> inFile;
            cout << endl;
            FreqAnalyze(inFile);
            return 0;
        }
        if (menu == 4)
        {
            cout << endl;
            cout << "InFile:";
            string inFile;
            cin >> inFile;
            cout << endl;
            cout << "OutFile:";
            string outFile;
            cin >> outFile;
            Attack(inFile, outFile);
            return 0;
        }

        return 0;
    }
    cout <<"strArgs INIT" << endl;
    string strArgs[5];
    if(argc > 0)
        strArgs[0] = argv[0];
    if(argc > 1)
        strArgs[1] = argv[1];
    if(argc > 2)
        strArgs[2] = argv[2];
    if(argc > 3)
        strArgs[3] = argv[3];
    if(argc > 4)
        strArgs[4] = argv[4];


    if (strArgs[1] == "-d")
    {
        if ( argc != 5)
        {
            //If we do not have the 4 needed args, tell them how to use it.
            PrintUsage(argv[0]);
            return 0;
        }
        //decrypt
        Decrypt(strArgs[2],strArgs[3],strArgs[4]);
    }


    if (strArgs[1] == "-e")
    {
        if ( argc != 5)
        {
            //If we do not have the 4 needed args, tell them how to use it.
            PrintUsage(argv[0]);
            return 0;
        }
        //encrypt
        Encrypt(strArgs[2],strArgs[3],strArgs[4]);

    }

    if (strArgs[1] == "-f")
    {
        cout << strArgs[1] << endl;
        if ( argc != 3)
        {
            //If we do not have the 2 needed args, tell them how to use it.
            PrintUsage(argv[0]);
            return 0;
        }
        //frequency analysis
        FreqAnalyze(strArgs[2]);

    }

    if (strArgs[1] == "-a")
    {
        if ( argc != 4)
        {
            //If we do not have the 3 needed args, tell them how to use it.
            PrintUsage(argv[0]);
            return 0;
        }
        //attack (with freq analysis
        Attack(strArgs[2],strArgs[3]);

    }

    if (strArgs[1] != "-d" && strArgs[1] != "-e" && strArgs[1] != "-f" && strArgs[1] != "-a")
    {

    return 0;
    }
}
