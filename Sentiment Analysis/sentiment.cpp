#include "Sentiment.h"

/// <summary>
/// gets input from a user to choose a file, will loop until valid file is found
/// </summary>
/// <param name="inputType">used to output if we're wanting the file for the map or the review</param>
/// <returns>string of the name of the file the user choose</returns>
string getFile(string inputType)
{
    string filename;
    ifstream inFile;
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
    inFile.close();
}

/// <summary>
/// Edits the review input file to create the file name for the output file
/// </summary>
/// <param name="inFile">the review input file</param>
/// <returns>the string of the output file name</returns>
string findOutputFile(string inFile)
{
    int posPeriod = inFile.find(".");
    string str1 = inFile.substr(0, posPeriod);
    string str2 = inFile.substr(posPeriod);
    return str1 + "Output" + str2;
}

/// <summary>
/// reads in the dictionary file
/// </summary>
/// <param name="filename">the file name of the dictionary</param>
/// <param name="allWords">all of the words that are in the dictionary</param>
/// <param name="posWords">the positive words of the dictionary</param>
/// <param name="negWords">the negative words of the dictionary</param>
void readDictionary(string filename, map<string, double>& allWords, map<string, double>& posWords,
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
        if (tempValue > DIC_THRESHOLD)
        {
            posWords.insert(pair<string, double>(tempWord, tempValue));
        }
        else if (tempValue < -DIC_THRESHOLD)
        {
            negWords.insert(pair<string, double>(tempWord, tempValue));
        }
    }
    inFile.close();
}

/// <summary>
/// reads the input review and stores in into the appropriate queues
/// </summary>
/// <param name="filename">the filename of the input review</param>
/// <param name="neutralWords">the neutral words of the review</param>
/// <param name="posWords">the positive words of the review</param>
/// <param name="negWords">the negative words of the review</param>
/// <param name="dictionary">the dictionary we reference to get the sentiment value for each word</param>
/// <returns>the total sentiment value of the review</returns>
double readReview(string filename, queue<wordData>& neutralWords,
    queue<wordData>& posWords, queue<wordData>& negWords, map<string, double>const& dictionary)
{
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
    inFile.close();
    return totalSentimentVal;
}

/// <summary>
/// pushes a word on to the appropriate queue
/// </summary>
/// <param name="neutralWords">the queue of neutral words</param>
/// <param name="posWords">queue of positive words</param>
/// <param name="negWords">queue of negative words</param>
/// <param name="wordStr">the string of the word we push</param>
/// <param name="wordVal">the value of the word that is pushed</param>
/// <param name="punctuation">the punctuation, if any, that comes after the word</param>
/// <param name="order">the order that the word comes in in the review</param>
/// <param name="isCapitalized">if the first letter of the word is capitalized or not</param>
void pushWord(queue<wordData>& neutralWords,
    queue<wordData>& posWords, queue<wordData>& negWords,
    string wordStr, double wordVal, string punctuation, int order, bool isCapitalized) 
{
    wordData tempWord;
    tempWord.word = wordStr;
    tempWord.value = wordVal;
    tempWord.wordOrder = order;
    tempWord.punctuationAfter = punctuation;
    tempWord.isCapitalized = isCapitalized;
    if (tempWord.value > REV_THRESHOLD)
    {
        posWords.push(tempWord);
    }
    else if (tempWord.value < -REV_THRESHOLD)
    {
        negWords.push(tempWord);
    }
    else
    {
        neutralWords.push(tempWord);
    }
}

/// <summary>
/// finds the sentiment value of a word using a dictionary
/// </summary>
/// <param name="word">the string of the word to search for</param>
/// <param name="dictionary">the dictionary to search for the word</param>
/// <returns>the value of the word</returns>
double findValue(string word, map<string, double>const& dictionary)
{
    map<string, double>::const_iterator reference = dictionary.find(word); // returns dictionary.end() if the word is not found.
    // if not able to find the word in the dictionary, return 0.
    if (reference != dictionary.end())
    {
        return reference->second;
    }
    else
    {
        return 0;
    }
}

/// <summary>
/// changes a queue to random words in taken from the dictionary
/// </summary>
/// <param name="words">the queue to change, passed as reference, so we can change it</param>
/// <param name="dictionary">the dictionary we check for values and find words using</param>
/// <returns>how much the sentiment value has changed</returns>
double changeQueue(queue<wordData>& words, map<string, double>const& dictionary)
{
    srand(time(0));
    queue<wordData> tempWords;
    double oldSentiment = 0;
    double newSentiment = 0;
    while (!words.empty())
    {
        wordData word = words.front();
        map<string, double>::const_iterator randWord = dictionary.begin();
        advance(randWord, rand() % dictionary.size()); //https://stackoverflow.com/questions/15425442/retrieve-random-key-element-for-stdmap-in-c
        cout << setw(MAX_OUTPUT_WIDTH / 5) << word.word << ": ";
        cout << setw(5) << word.value;
        cout << setw(MAX_OUTPUT_WIDTH / 5) << randWord->first << ": "; 
        cout << randWord->second << endl;
        oldSentiment += word.value;
        word.word = randWord->first;
        word.value = randWord->second;
        newSentiment += word.value;
        tempWords.push(word);
        words.pop();
    }
    words = tempWords;
    return newSentiment - oldSentiment; //gets and returns how much the sentiment value has changed
}

/// <summary>
/// formats the new review into a single output string
/// </summary>
/// <param name="posWords">the queue of positive words, if it hasn't been changed</param>
/// <param name="negWords">the queue of negative words, if it hasn't been changed</param>
/// <param name="neutralWords">the queue of neutral words</param>
/// <returns>the review as an output string</returns>
string writeNewReview(queue<wordData> posWords, queue<wordData> negWords, queue<wordData> neutralWords)
{
    int wordIteration = 0; // the iteration is what is used to determine what word comes next
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

/// <summary>
/// prints the review with correct formatting
/// </summary>
/// <param name="text">the text that gets formatted and output</param>
void printReview(string text)
{
    while (text.length() > MAX_OUTPUT_WIDTH)
    {
        string textToSearch = text.substr(0, MAX_OUTPUT_WIDTH);
        int lastWhitespace = textToSearch.find_last_of(" "); // find last instance of whitespace before the max output width
        string outputLine = text.substr(0, lastWhitespace); // cut text off at that whitespace
        cout << outputLine << endl; // output that partial string
        text = text.substr(lastWhitespace + 1);
    }
    cout << text << endl;
}

/// <summary>
/// prints the review with correct formatting, and outputs it to a file
/// </summary>
/// <param name="filename">the file name to output to</param>
/// <param name="text">the text that gets formatted and output</param>
/// <param name="sentimentVal">the sentimental value to output, needed because it is used to output to the file as well.</param>
void printReview(string filename, string text, double sentimentVal)
{
    ofstream fout(filename); //https://stackoverflow.com/questions/478075/creating-files-in-c
    while (text.length() > MAX_OUTPUT_WIDTH)
    {
        string textToSearch = text.substr(0, MAX_OUTPUT_WIDTH);
        int lastWhitespace = textToSearch.find_last_of(" "); //https://cplusplus.com/reference/string/string/find_last_of/
        string outputLine = text.substr(0, lastWhitespace);
        cout << outputLine << endl;
        fout << outputLine << endl;
        text = text.substr(lastWhitespace + 1);
    }
    cout << text << endl;
    fout << text << endl;
    cout << "New Sentiment Value = " << sentimentVal << endl;
    fout << "New Sentiment Value = " << sentimentVal << endl;
    fout.close();
}