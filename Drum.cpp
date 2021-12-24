//
// Created by Mateusz on 16.12.2021.
//

#include "Drum.h"
#include "sstream"

Drum::Drum() {
    this->notch_location = 0;
    this->ring_offset = 0;
    this->reflector = false;
}

Drum::Drum(const Drum &templ) {
    //Vector performs deep copy by default, so this is enough
    this->outputs = templ.outputs;

    this->ring_offset = templ.ring_offset;
    this->notch_location = templ.notch_location;
    this->reflector = templ.reflector;
}

//Calling other constructor with partially parsed values - example of initialization list
Drum::Drum(const std::string &alphabet, const std::string &ro, const std::string &nl)
: Drum(alphabet, std::stoi(ro), std::stoi(nl)){}

Drum::Drum(const std::string &alphabet, uint8_t ro, uint8_t nl) {

    //Setting reflector settings by default
    this->ring_offset = 0;
    this->notch_location = 0;
    this->reflector = true;

    //Stringstream allows for easier operations on strings, such as splitting by delimiter
    std::stringstream ss(alphabet);
    //Temporary value holding character before putting it into vector
    std::string tmpstr;

    //Loop reading over the characters and insterting them into vector
    while(std::getline(ss, tmpstr, ',')){
        //Check if an ASCII value with correct size was read
        if (tmpstr.length() != 1 )continue;
        char tmp = tmpstr[0];

        //Ugly, although easy way to capitalize a number
        if (tmp >= 'a' && tmp <= 'z') tmp -= 32;
        this->outputs.push_back(tmp);
    }

    //Check if the drum has reflector property (all cycles of length 2)
    for(auto iter = this->outputs.begin(); iter<this->outputs.end(); ++iter){
        if (process_character_forward(*iter, 0) != process_character_backward(*iter, 0)){

            //Setting standard drum settins
            this->ring_offset = ro;
            this->notch_location = nl;
            this->reflector = false;

            break;
        }
    }
}

Drum::Drum(std::vector<char> &alphabet, uint8_t ro, uint8_t nl) {

    //Setting reflector settings by default
    this->ring_offset = 0;
    this->notch_location = 0;
    this->reflector = true;

    //std::vector should posses an implementation of copy constructor. Otherwise this might not work
    this->outputs = alphabet;

    //Check if the drum has reflector property (all cycles of length 2)
    for(auto iter = this->outputs.begin(); iter<this->outputs.end(); ++iter){
        if (process_character_forward(*iter, 0) != process_character_backward(*iter, 0)){

            //Setting standard drum settins
            this->ring_offset = ro;
            this->notch_location = nl;
            this->reflector = false;

            break;
        }
    }
}
Drum::~Drum() {
    //Nothing necessary here.
    //Since std::vector is used, it's destructor will do all the deallocating work for us
}
//A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z
//E,K,M,F,L,G,D,Q,V,Z,N,T,O,W,Y,H,X,U,S,P,A,I,B,R,C,J
char Drum::process_character_forward(char c, uint8_t offset){

    //This is 0 for capital letters and 1 otherwise
    uint8_t capital = 0;

    //Check if the number is an ASCII character, and return it if it's not
    if (!(c >='A' && c <= 'Z') && !(c >= 'a' && c <= 'z')) return c;

    //Check if the passed leter is capital or not
    if ((c >= 'a' && c <= 'z')) {c-=32; capital = 1;}

    //Calculate
    uint8_t real_offset = (offset + this->ring_offset + c - 65) % DRUM_ALPHABET_LEN;

    return (char)(this->outputs[real_offset] + capital * 32);
}

char Drum::process_character_backward(char c, uint8_t offset){

    //This is 0 for capital letters and 1 otherwise
    uint8_t capital = 0;

    //Check if the number is an ASCII character, and return it if it's not
    if (!(c >='A' && c <= 'Z') && !(c >= 'a' && c <= 'z')) return c;

    //Check if the passed leter is capital or not
    if ((c >= 'a' && c <= 'z')) {c-=32; capital = 1;}

    //Calculate
    //First we need to find index of character in a scrambled alphabet of the drum
    uint8_t reversed_offset = 0;
    for (; reversed_offset<DRUM_ALPHABET_LEN; reversed_offset++){
        if (c==this->outputs[reversed_offset]) break;
    }

    //Then using known current offset values we can calculate corresponding value in a non scrambled alphabet
    int8_t real_c_idx = (reversed_offset - this->ring_offset - offset) % DRUM_ALPHABET_LEN;
    if (real_c_idx < 0){
        real_c_idx += DRUM_ALPHABET_LEN;
    }

    return (char)(real_c_idx + 65 + capital * 32);
}
