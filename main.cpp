#include <iostream>
#include "Drum.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Drum d1("E,K,M,F,L,G,D,Q,V,Z,N,T,O,W,Y,H,X,U,S,P,A,I,B,R,C,J", "0", "2");
    Drum d2("Y,R,U,H,Q,S,L,D,P,X,N,G,O,K,M,I,E,B,F,Z,C,W,V,J,A,T", 0, 0);
    std::string wiad = "AAAAAaaaaAAAABBBBBBBBBBBSSss";
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
