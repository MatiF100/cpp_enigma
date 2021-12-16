#include <iostream>
#include "Drum.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Drum d1("E,K,M,F,L,G,D,Q,V,Z,N,T,O,W,Y,H,X,U,S,P,A,I,B,R,C,J", "5", "2");
    std::string wiad = "Projekt Fesza na CPP 1 xD '_'";
    std::cout << "Przed szyfrowaniem: " << wiad << std::endl << "Po szyfrowaniu: ";
    std::string szyfrogram;
    for (auto iter = wiad.cbegin(); iter != wiad.end(); ++iter){
        szyfrogram += d1.process_character_forward(*iter, 0);
    }
    std::cout << szyfrogram << std::endl << "Po deszyfracji: ";
    for (auto iter = szyfrogram.cbegin(); iter != szyfrogram.end(); ++iter){
        std::cout << d1.process_character_backward(*iter, 0);
    }
    return 0;
}
