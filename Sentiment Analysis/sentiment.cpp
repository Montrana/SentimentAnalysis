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

void readReview(string filename, queue<wordData>& neutralWords,
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
    bool isCapitalized = false;
    while (inFile >> inStr)
    {
        if (isupper(inStr[0]));
        {
            isCapitalized = true;
            inStr[0] = tolower(inStr[0]);
        }
        if (!ispunct(inStr.back()))
        {
            pushWord(neutralWords, posWords, negWords, inStr, findValue(inStr, dictionary), "", wordNumber, isCapitalized);
        }
        else
        {
            string punctuation(1, inStr[inStr.length() - 1]); //https://www.geeksforgeeks.org/how-to-convert-a-single-character-to-string-in-cpp/
            inStr.pop_back();
            pushWord(neutralWords, posWords, negWords, inStr, findValue(inStr, dictionary), punctuation, wordNumber, isCapitalized);
        }
        wordNumber++;
    }
}

void pushWord(queue<wordData>& neutralWords,
    queue<wordData>& posWords, queue<wordData>& negWords,
    string wordStr, double wordVal, string punctuation, int order, bool isCapitalized) {
    wordData tempWord;
    tempWord.word = wordStr;
    tempWord.value = wordVal;
    tempWord.wordOrder = order;
    tempWord.punctuationAfter = "";
    tempWord.isCapitalized = isCapitalized;
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

double findValue(string word, map<string, double>const& dictionary)
{
    map<string, double>::const_iterator reference = dictionary.find(word);
    if (reference != dictionary.end())
    {
        return reference->second;
    }
    else
    {
        return 0;
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

void printQueue(string filename, queue<wordData> words)
{
    ofstream fout;
    fout.open(filename);

    while (!words.empty())
    {
        wordData dataWord = words.front();
        fout << dataWord.word << ": " << dataWord.value << endl;
        words.pop();
    }
}