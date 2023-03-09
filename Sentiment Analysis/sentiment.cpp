#pragma once
#include "Sentiment.h"

bool isValid(string str)
{
    return str.find_first_not_of("01234567890.-") == string::npos;
}

void readFile(string filename, map<string, double>& allWords, map<string, double>& posWords,
    map<string, double>& negWords)
{
    ifstream inFile;
    inFile.open(filename);
    if (!inFile.is_open())
    {
        cout << "Unable to open file \n";
        return;
    }
    string inStr, tempWord;
    double tempValue;
    int commaPos;
    while (inFile >> inStr)
    {
        commaPos = inStr.find(',');
        tempWord = inStr.substr(0, commaPos);
        tempValue = stod(inStr.substr(commaPos + 1));
        allWords.insert(pair<string, double> (tempWord, tempValue));
        if (tempValue > 1.5)
        {
            posWords.insert(pair<string, double>(tempWord, tempValue));
        }
        else if (tempValue < -1.5)
        {
            negWords.insert(pair<string, double>(tempWord, tempValue));
        }
    }
    inFile.close();
}

void readFile(string filename, queue<wordData>& neutralWords,
    queue<wordData>& posWords, queue<wordData>& negWords, map<string, double>const& dictionary) {
    ifstream inFile;
    inFile.open(filename);
    if (!inFile.is_open())
    {
        cout << "Unable to open file \n";
        return;
    }
    string inStr;
    int wordNumber = 0;
    while (inFile >> inStr)
    {
        string tempWord;
        for (int i = 0; i < inStr.length(); i++)
        {
            tempWord[i] = tolower(inStr[i]);
        }
        if (!ispunct(inStr[inStr.length() - 1]))
        {
            pushWord(neutralWords, posWords, negWords, inStr, dictionary.find(tempWord)->second, "", wordNumber);
        }
        else
        {
            //need to take out last character of string then run that if statement again
        }
        wordNumber++;
    }
}

void pushWord(queue<wordData>& neutralWords,
    queue<wordData>& posWords, queue<wordData>& negWords,
    string wordStr, double wordVal, string punctuation, int order) {
    wordData tempWord;
    tempWord.word = wordStr;
    tempWord.value = wordVal;
    tempWord.wordOrder = order;
    tempWord.punctuationAfter = "";
    if (tempWord.value > 1.5)
    {
        posWords.push(tempWord);
    }
    else if (tempWord.value < -1.5)
    {
        negWords.push(tempWord);
    }
    else
    {
        neutralWords.push(tempWord);
    }
}

void printMap(string filename, map<string, double> wordMap)
{
    ofstream fout;
    fout.open(filename);

    for (pair<string, double> word : wordMap)
    {
        fout << word.first << ": " << word.second << endl;
    }
}