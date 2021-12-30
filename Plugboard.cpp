//
// Created by Mateusz on 24.12.2021.
//

#include "Plugboard.h"

bool Plugboard::insert_plug(char in, char out) {
    //Checking if one of the letters is already connected or if the characters are the same
    if(in==out || this->substitutions.contains(in) || this->substitutions.contains(out))
        return false;

    //Creating two-way substitution context
    this->substitutions.insert({in, out});
    this->substitutions.insert({out, in});

    return true;
}

bool Plugboard::remove_plug(char target) {
    auto i = this->substitutions.find(target);
    if (i != this->substitutions.end()){
        this->substitutions.erase(i->second);
        this->substitutions.erase(i);

        return true;
    }
    return false;
}

char Plugboard::swap(char in) {
    bool capital = true;
    if (in > 'Z') {
        in -= 32;
        capital = false;
    }
    auto i = this->substitutions.find(in);
    if(i != this->substitutions.end()){
        return capital ? i->second : i->second + 32;
    }
    return capital ? in : in + 32;
}