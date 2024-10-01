#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <cmath>

std::vector<char> scrambledAlphabet = {'D', 'Q', 'W', 'R', 'Y', 'B', 'T', 'C', 'N', 'V', 'E', 'P', 'K', ' ', 'U', 'A', 'J', 'S', 'O', 'M', 'I', 'L', 'G', 'H', 'X', 'Z', 'F'};
std::vector<char> alphabet = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' '};


std::string encode(std::string msg){
    std::vector<int> encoded = {};
    for (char& c : msg){
        std::vector<int> temp = {};
        int corr {};
        auto it = std::find(alphabet.begin(), alphabet.end(), c);
        if (it != alphabet.end()){
            corr = int(scrambledAlphabet[std::distance(alphabet.begin(), it)]); //This grabs the corresponding letter from scrambled alphabet and casts it into an int
            while (corr > 0){ //Translates corr into binary, then pushes each binary bit to the end of the encoded vector
                temp.push_back(corr % 2);
                corr /= 2;
            }
        } else {
            return "Error";
        }
        std::reverse(temp.begin(), temp.end()); //Apparently the above algorithm puts the binary in reverse order, so this is needed
        encoded.push_back(0); //Apparently the algorithm misses the first 0 in each ASCII, so this is needed
        if (c == 'N')
            encoded.push_back(0); //Fixes the whitespace issue
        encoded.insert(encoded.end(), temp.begin(), temp.end()); //This is needed to ensure only the new letter is reversed and not the whole msg
    }

    std::vector<char> final = {}; //Convert 0 and 1 to s and b respectively
    for (int& c : encoded){
        if (c == 1)
            final.push_back('b');
        else
            final.push_back('s');
    }

    std::stringstream ss;
    std::copy(final.begin(), final.end(), std::ostream_iterator<char>(ss, ""));
    std::string result = ss.str();
    return result;
}

std::string decode(std::string msg){
    std::string newmsg;
    for (char&c : msg){
        if (tolower(c) == 's')
            newmsg.append("0");
        else if (tolower(c) == 'b')
            newmsg.append("1");
        else
            return "Error";
    }
    std::vector<std::string> splitted;
    for (size_t i = 0; i < newmsg.size(); i += 8){ //Code always comes in length 8 per unit
        splitted.push_back(newmsg.substr(i, 8)); //This cuts the msg into sizes of 8 and stores each into one element of splitted
        
    }

    std::vector<int> code;
    for (std::string binaryStr : splitted){
        int decimalValue = 0;
        int length = binaryStr.length();
        // Iterate through the binary string
        for (int i = 0; i < length; ++i) {
            // Check if the character is '1', and compute its value in decimal
            if (binaryStr[i] == '1') {
                decimalValue += std::pow(2, length - i - 1);
            }
        }
        code.push_back(decimalValue);
    }

    std::string final;
    for (int dec : code){
        auto it = std::find(scrambledAlphabet.begin(), scrambledAlphabet.end(), char(dec));
        if (it != alphabet.end()){
            char corr = alphabet[std::distance(scrambledAlphabet.begin(), it)]; //This grabs the corresponding letter from alphabet
            final += corr;
        } else {
            return "Error";
        }
    }
    return final;
}


int main(){
    while (1){
        std::string omsg {};
        std::cout << "Please enter the message to be encoded or decoded: ";
        std::getline(std::cin, omsg);
        std::string msg{};
        for (char c : omsg){ //Translates to upper case
            msg += toupper(c);
        }
        char instruction {};
        std::cout << "Please indicate if you would like to [E]ncode/[D]ecode the message: ";
        std::cin >> instruction;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //To remove the getline bug and enable whitespace entering
        if (tolower(instruction) == 'e'){
            std::string encoded = encode(msg);
            if (encoded == "Error"){
                std::cout << "There was an error encoding the message." << std::endl;
            }
            std::cout << encoded << std::endl;
        } else if (tolower(instruction) == 'd'){
            std::string decoded = decode(msg);
            if (decoded == "Error"){
                std::cout << "There was an error encoding the message." << std::endl;
            }
            std::cout << decoded << std::endl;
        } else
            std::cout << "Please try again and enter [E] or [D]" << std::endl;
    }
    return 0;
}
