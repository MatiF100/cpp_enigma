//
// Created by Mateusz on 16.12.2021.
//

#include "Drum.h"
#include "sstream"

Drum::Drum() {
    this->notch_location = 0;
    this->ring_offset = 0;
    this->reflector = false;
    this->narrow = false;
}

Drum::Drum(const Drum &templ) {
    //Vector performs deep copy by default, so this is enough
    this->outputs = templ.outputs;

    this->ring_offset = templ.ring_offset;
    this->notch_location = templ.notch_location;
    this->reflector = templ.reflector;
    this->narrow = templ.narrow;
    this->name = templ.name;
}

//Calling other constructor with partially parsed values - example of initialization list
Drum::Drum(const std::string &alphabet, const std::string &ro, const std::string &nl, bool narrow)
: Drum(alphabet, std::stoi(ro), std::stoi(nl), narrow){}

Drum::Drum(const std::string &alphabet, uint8_t ro, uint8_t nl, bool narrow) {

    //Setting reflector settings by default
    this->ring_offset = 0;
    this->notch_location = 0;
    this->reflector = true;
    this->narrow = narrow;

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

Drum::Drum(std::vector<char> &alphabet, uint8_t ro, uint8_t nl, bool narrow) {

    //Setting reflector settings by default
    this->ring_offset = ro;
    this->notch_location = 0;
    this->reflector = true;
    this->narrow = narrow;

    //std::vector should posses an implementation of copy constructor. Otherwise this might not work
    this->outputs = alphabet;

    //Check if the drum has reflector property (all cycles of length 2)
    for(auto iter = this->outputs.begin(); iter<this->outputs.end(); ++iter){
        if (process_character_forward(*iter, 0) != process_character_backward(*iter, 0)){

            //Setting standard drum settins
            this->notch_location = nl;
            this->reflector = false;

            break;
        }
    }
}
Drum::~Drum() = default;
    //Nothing necessary here.
    //Since std::vector is used, it's destructor will do all the deallocating work for us
    //So we can just use "default" keyword

char Drum::process_character_forward(char c, uint8_t offset){

    //This is 0 for capital letters and 1 otherwise
    uint8_t capital = 0;

    //Check if the number is an ASCII character, and return it if it's not
    if (!(c >='A' && c <= 'Z') && !(c >= 'a' && c <= 'z')) return c;

    //Check if the passed leter is capital or not
    if ((c >= 'a' && c <= 'z')) {c-=32; capital = 1;}

    //Calculate
    //real_offset refers to letter that the signal goes into, considering it's rotation and alphabetic ring's position
    uint8_t real_offset = (DRUM_ALPHABET_LEN + offset - this->ring_offset + c - 65) % DRUM_ALPHABET_LEN;

    //Return value is letter corresponding to one calculated above, but also with compensation for current drum's state
    return (char)((this->outputs[real_offset] - 'A' - offset + this->ring_offset + DRUM_ALPHABET_LEN) % DRUM_ALPHABET_LEN + 'A' + capital * 32);
}

char Drum::process_character_backward(char c, uint8_t offset){

    //This is 0 for capital letters and 1 otherwise
    uint8_t capital = 0;

    //Check if the number is an ASCII character, and return it if it's not
    if (!(c >='A' && c <= 'Z') && !(c >= 'a' && c <= 'z')) return c;

    //Check if the passed leter is capital or not
    if ((c >= 'a' && c <= 'z')) {c-=32; capital = 1;}

    //Calculate
    //real_rev_ce is offset of the drum output the signal goes into, considering the drum's rotation and its alphabetic ring's position
    uint8_t rev_real_c = ((c - 'A') + offset - this->ring_offset + DRUM_ALPHABET_LEN) % DRUM_ALPHABET_LEN;


    //Finding index of character in a scrambled alphabet of the drum
    uint8_t reversed_offset = 0;
    for (; reversed_offset<DRUM_ALPHABET_LEN; reversed_offset++){
        if (rev_real_c + 'A' == this->outputs[reversed_offset]) break;
    }

    //Then using known current offset values we can calculate corresponding value in a non scrambled alphabet
    //By taking drum's state into consideration
    int8_t real_c_idx = (reversed_offset - offset + this->ring_offset + DRUM_ALPHABET_LEN) % DRUM_ALPHABET_LEN;

    //As all calculations are already done, returning offset converted to number
    return (char)(real_c_idx + 65 + capital * 32);
}

Drum::Drum(std::fstream* handle, uint8_t ro) {
    std::string tmp;
    char nl;
    uint8_t narrow;
    *handle >> this->name;
    *handle >> tmp;
    *handle >> nl;
    *handle >> narrow;
    std::vector<char> alphabet(tmp.begin(), tmp.end());

    //Setting reflector settings by default
    this->ring_offset = ro;
    this->notch_location = nl;
    this->reflector = true;
    this->narrow = narrow ? true : false;

    //std::vector should posses an implementation of copy constructor. Otherwise this might not work
    this->outputs = alphabet;

    //Check if the drum has reflector property (all cycles of length 2)
    for(auto iter = this->outputs.begin(); iter<this->outputs.end(); ++iter){
        if (process_character_forward(*iter, 0) != process_character_backward(*iter, 0)){

            //Setting standard drum settins
            this->reflector = false;

            break;
        }
    }
}

std::ostream &operator<<(std::ostream &os, const Drum &drum) {
    os << drum.name << std::endl;
    for(auto i = drum.outputs.begin(); i<drum.outputs.end(); i++ ){
        os << *i;
    }
    os << std::endl << (char)(drum.notch_location + 'A') << std::endl << (unsigned int)drum.narrow;
    return os;
}

std::istream &operator>>(std::istream &is, Drum &drum) {
    std::string tmp;
    drum.reflector = true;
    char nl_tmp;
    int nr_tmp;
    is >> tmp >> nl_tmp >> nr_tmp;
    drum.outputs.resize(26);
    for(int i = 0; i < tmp.length(); i++){
        drum.outputs[i] = tmp[i];
    }
    drum.notch_location = nl_tmp - 'A';
    drum.narrow = nr_tmp == 1 ? true : false;

    for(auto iter = drum.outputs.begin(); iter<drum.outputs.end(); ++iter){
        if (drum.process_character_forward(*iter, 0) != drum.process_character_backward(*iter, 0)){
            //Setting standard drum settins
            drum.reflector = false;
            break;
        }
    }

    return is;
}

void Drum::set_ring_offset(uint8_t offset) {
    this->ring_offset = offset % 26;
}

