#include "Sentiment.h"
#include "WordData.h"

string getFile(ifstream& inFile, string inputType)
{
    string filename;
    do
    {
        cout << "Please enter the input file you would like to use for the " << inputType << ": ";
        cin >> filename;
        inFile.open(filename);
        if (!inFile.is_open())
        {
            cout << "Unable to open file, please enter a valid file name.\n";
        }
        else
        {
            return filename;
        }
    } while (!inFile.is_open());

}

string findOutputFile(string inFile)
{
    int posPeriod = inFile.find(".");
    string str1 = inFile.substr(0, posPeriod);
    string str2 = inFile.substr(posPeriod);
    return str1 + "Output" + str2;
}

void readFile(string filename, map<string, double>& allWords, map<string, double>& posWords,
    map<string, double>& negWords)
{
    string inStr, tempWord;
    double tempValue;
    int commaPos;
    ifstream inFile(filename);
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

double readReview(string filename, queue<wordData>& neutralWords,
    queue<wordData>& posWords, queue<wordData>& negWords, map<string, double>const& dictionary) {
    
    string inStr;
    
    int wordNumber = 0;
    double totalSentimentVal = 0;
    bool isCapitalized = false;
    ifstream inFile(filename);
    while (inFile >> inStr)
    {
        if (isupper(inStr[0]))
        {
            isCapitalized = true;
            inStr[0] = tolower(inStr[0]);
        }
        if (!ispunct(inStr.back()))
        {
            double wordVal = findValue(inStr, dictionary);
            totalSentimentVal += wordVal;
            pushWord(neutralWords, posWords, negWords, inStr, 
                wordVal, "", wordNumber, isCapitalized);
        }
        else
        {
            string punctuation(1, inStr[inStr.length() - 1]); //https://www.geeksforgeeks.org/how-to-convert-a-single-character-to-string-in-cpp/
            inStr.pop_back();
            double wordVal = findValue(inStr, dictionary);
            totalSentimentVal += wordVal;
            pushWord(neutralWords, posWords, negWords, inStr, 
                wordVal, punctuation, wordNumber, isCapitalized);
        }
        isCapitalized = false;
        wordNumber++;
    }
    return totalSentimentVal;
}

void pushWord(queue<wordData>& neutralWords,
    queue<wordData>& posWords, queue<wordData>& negWords,
    string wordStr, double wordVal, string punctuation, int order, bool isCapitalized) {
    wordData tempWord;
    tempWord.word = wordStr;
    tempWord.value = wordVal;
    tempWord.wordOrder = order;
    tempWord.punctuationAfter = punctuation;
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

double changeQueue(queue<wordData>& words, map<string, double>const& dictionary)
{
    srand(time(0));
    queue<wordData> tempWords;
    double sentimentChange = 0;
    while (!words.empty())
    {
        wordData word = words.front();
        map<string, double>::const_iterator randWord = dictionary.begin();
        advance(randWord, rand() % dictionary.size()); //https://stackoverflow.com/questions/15425442/retrieve-random-key-element-for-stdmap-in-c
        cout << word.word << ": " << word.value << " | " << randWord->first << ": " << randWord->second << endl;
        word.word = randWord->first;
        word.value = randWord->second;
        sentimentChange += word.value;
        tempWords.push(word);
        words.pop();
    }
    words = tempWords;
    return sentimentChange;
}

void printMap(string filename, map<string, double> wordMap)
{
    ofstream fout;
    fout.open(filename);

    for (pair<string, double> word : wordMap)
    {
        fout << word.first << ": " << word.second << endl;
    }
    fout.close();
}

void printQueue(queue<wordData> words)
{
    while (!words.empty())
    {
        wordData dataWord = words.front();
        cout << dataWord.word << ": " << dataWord.value << endl;
        words.pop();
        //dataWord.print();
    }
}
string writeNewReview(queue<wordData> posWords, queue<wordData> negWords, queue<wordData> neutralWords)
{
    int wordIteration = 0;
    wordData posWord, negWord, neutralWord;

    string outputText = "";
    if (!posWords.empty())
    {
        posWord = posWords.front();
    }
    if (!negWords.empty())
    {
        negWord = negWords.front();
    }
    if (!neutralWords.empty())
    {
        neutralWord = neutralWords.front();
    }
    while (!posWords.empty() || !negWords.empty() || !neutralWords.empty())
    {
        if (posWord.wordOrder == wordIteration)
        {
            if (posWord.isCapitalized) {
                posWord.word[0] = toupper(posWord.word[0]);
            }
            outputText += posWord.word + posWord.punctuationAfter + " ";
            posWords.pop();
            if (!posWords.empty())
            {
                posWord = posWords.front();
            }
        }
        else if (negWord.wordOrder == wordIteration)
        {
            if (negWord.isCapitalized) {
                negWord.word[0] = toupper(negWord.word[0]);
            }
            outputText += negWord.word + negWord.punctuationAfter + " ";
            negWords.pop();
            if (!negWords.empty())
            {
                negWord = negWords.front();
            }
        }
        else if (neutralWord.wordOrder == wordIteration)
        {
            if (neutralWord.isCapitalized) {
                neutralWord.word[0] = toupper(neutralWord.word[0]);
            }
            outputText += neutralWord.word + neutralWord.punctuationAfter + " ";
            neutralWords.pop();
            if (!neutralWords.empty())
            {
                neutralWord = neutralWords.front();
            }
        }
        wordIteration++;
    }
    
    return outputText;
}

void printReview(string text, int maxWidth)
{
    while (text.length() > maxWidth)
    {
        string textToSearch = text.substr(0, maxWidth);
        int lastWhitespace = textToSearch.find_last_of(" ");
        cout << text.substr(0, lastWhitespace) << endl;
        text = text.substr(lastWhitespace + 1);
    }
    cout << text << endl;
}