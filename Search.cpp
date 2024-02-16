#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function prototypes at the top of the file for function decomposition

/**
 * Print instructions for using the program.
 */
void printMenu();
/**
 * Returns the 0-based index in the English alphabet where `c` appears,
 * or -1 if `c` is not an uppercase letter in the alphabet.
 *
 * For example:
 *  - `findIndexInAlphabet('A')` returns 0
 *  - `findIndexInAlphabet('D')` returns 3
 *  - `findIndexInAlphabet('+')` returns -1
 *  - `findIndexInAlphabet('a')` returns -1
 */
int findIndexInAlphabet(char c);
/**
 * Returns `c` rotated by `amount` many characters forward. If we run out
 * of letters in the alphabet, wrap around back to 'A'. This method
 * assumes that `c` is an uppercase letter.
 * For example:
 *  - `rot('A', 0)` returns 'A'
 *  - `rot('A', 1)` returns 'B'
 *  - `rot('Z', 1)` returns 'A'
 *  - `rot('A', 10)` returns 'K'
 *  - `rot('J', 25)` returns 'I'
 */
char rot(char c, int amount);
/**
 * Returns a new string in which every character has been rotated by `amount`
 * letters. Lowercase letters are converted to uppercase and rotated.
 * Non-alphabetic characters are left alone.
 * For example:
 *  `rot("A", 0)` returns "A"
 *  `rot("AA", 3)` returns "DD"
 *  `rot("HELLO", 0)` returns "HELLO"
 *  `rot("HELLO", 1)` returns "IFMMP"
 *  `rot("oh HEL-LO!", 1)` returns "PI IFM-MP!"
 */
string rot(const string& line, int amount);

void ReadDictionary(vector<string>& b, string& word);

void PrintVec(const vector<string>& b);

bool isValidDecryptedWord(const string& dW, const vector<string>& d);

void extractWords(const string& str, vector<string>& words);

int main() {
    string command;

    cout << "Welcome to Ciphers!" << endl;
    cout << "-------------------" << endl;
    cout << endl;

    do {
        printMenu();
        cout << endl
            << "Enter a command (case does not matter): ";

        // Use getline for all user input to avoid needing to handle
        // input buffer issues relating to using both >> and getline
        getline(cin, command);
        cout << endl;

        if (command == "C" || command == "c") {
            cout << "Enter the text to encrypt:\n";
            string phrase;
            getline(cin, phrase);

            cout << "Enter the number of characters to rotate by:\n";
            string rotAmount;
            getline(cin, rotAmount);

            cout << '\n' << rot(phrase, stoi(rotAmount)) << '\n';
        }
        else if (command == "D" || command == "d") {
            vector<string> words;
            vector<string> dictionary;
            string text;
            int count = 0;


            ReadDictionary(dictionary, text);

            cout << "Enter the text to Caesar-cipher decrypt:\n";
            getline(cin, text);

            for (int k = 0; k < 26; ++k) {
                string newMessage = "";

                for (int i = 0; i < text.size(); ++i) {
                    if (isalpha(text.at(i))) {
                        newMessage += rot(text.at(i), k);
                    } else {
                        newMessage += text.at(i);
                    }
                }
                
                extractWords(newMessage, words);
                
                for (string w : words) {
                    if (isValidDecryptedWord(w, dictionary)) {
                        ++count;

                        if (count > words.size() / 2) {
                            cout << k << " th: " << text << '\n';
                            break;
                        }
                    }
                }

                count = 0;
                words.clear();
            }
        } else if (command == "V" || command == "v") {
            string text;
            cout << "Enter text to encrypt:\n";
            getline(cin, text);

            string key;
            cout << "Enter the Vigenère key:\n";
            getline(cin, key);

            string newKey;
            for (int i = 0; i < key.size(); ++i) {
                if (isalpha(key.at(i))) {
                    newKey += toupper(key.at(i));
                }
            }

            cout << "New key: " << newKey << '\n';
            key = "";
            int j = 0;
            for (int i = 0; i < text.size(); ++i) {
                if (isalpha(text.at(i))) {
                    key += rot(text.at(i), findIndexInAlphabet(newKey.at(j)));
                    j = (j + 1) % newKey.size();
                } else {
                    key += text.at(i);
                }
            }
            cout << key << '\n';
        }

        cout << endl;

    } while (!(command == "x" || command == "X"));

    return 0;
}

void printMenu() {
    cout << "Ciphers Menu" << endl;
    cout << "------------" << endl;
    // Caesar Ciphers
    cout << "C - Encrypt with Caesar Cipher" << endl;
    cout << "D - Decrypt Caesar Cipher" << endl;
    // Vigenere
    cout << "V - Encrypt with Vigenère" << endl;
    // Utility
    cout << "X - Exit Program" << endl;
}

int findIndexInAlphabet(char c) { // NOTE: 65 -> A, 97 -> a
    return ALPHABET.find(toupper(c));
}

char rot(char c, int amount) {
    amount = amount % 90;
    if (isalpha(c)) {
        if (c > 90) {
            c = toupper(c);
        }

        if (c + amount > 90) {
            amount = amount + c - 90;
            c = '@';
            c += amount;
        }
        else {
            c += amount;
        }
    }

    return c;
}

string rot(const string& line, int amount) {
    string newLine;

    for (int i = 0; i < line.size(); ++i) {
        newLine += line.at(i);
    }

    for (int i = 0; i < line.size(); ++i) {
        newLine.at(i) = rot(line.at(i), amount);
    }

    return newLine;
}

void ReadDictionary(vector<string>& b, string& word) {
    ifstream dictionary("dictionary.txt");

    if (!dictionary) {
        cout << "Error opening dictionary.txt file\n";
    }

    while (dictionary >> word) {
        b.push_back(word);
    }

    dictionary.close();
}

void PrintVec(const vector<string>& b) {
    if (b.size() == 0) {
        cout << "No good decryptions found\n";
        return;
    }

    for (int i = 0; i < b.size(); ++i) {
        cout << b.at(i) << '\n';
    }
}

bool isValidDecryptedWord(const string& dW, const vector<string>& d) {
    for (int i = 0; i < d.size(); ++i) {
        if (dW == d.at(i)) {
            return true;
        }
    }
    return false;
}

void extractWords(const string& str, vector<string>& words) {
    string currWord;

    for (char c : str) {
        if (isalpha(c)) {
            currWord += c;
        } else if (!currWord.empty()) {
            words.push_back(currWord);
            currWord.clear();
        }
    }
