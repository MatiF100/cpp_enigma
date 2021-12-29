#include <iostream>
#include "Drum.h"
#include "DrumAssembly.h"
#include "DrumAssemblyK.h"
#include "Machine.h"
#include "Gui.h"
wxIMPLEMENT_APP(Gui);
/*int main() {
    std::cout << "Hello, World!" << std::endl;
    Drum d1("E,K,M,F,L,G,D,Q,V,Z,N,T,O,W,Y,H,X,U,S,P,A,I,B,R,C,J", 0, 'Q' - 'A');
    Drum d2("A,J,D,K,S,I,R,U,X,B,L,H,W,T,M,C,Q,G,Z,N,P,Y,F,V,O,E", 0, 'E' - 'A');
    Drum d3("B,D,F,H,J,L,C,P,R,T,X,V,Z,N,Y,E,I,W,G,A,K,M,U,S,Q,O", 0, 'V' - 'A');
    Drum rev("Y,R,U,H,Q,S,L,D,P,X,N,G,O,K,M,I,E,B,F,Z,C,W,V,J,A,T", 0, 0);;

    std::string wiad = "aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa aaaaa";
    std::cout << "Przed szyfrowaniem: " << wiad << std::endl << "Po szyfrowaniu: ";
    std::string szyfrogram;
    DrumAssembly module(d1, d2, d3, rev);
    module.set_drums_offset(1,0,0, 0);
    Machine wenigma;
    wenigma.set_wehrenigma(module);
    wenigma.detatch_drums();
    wenigma.set_wehrenigma(module);


    szyfrogram = wenigma.process_message(wiad);
    std::cout << szyfrogram << std::endl << "Po deszyfracji: ";

    wenigma.set_drum_offsets(0, 0, 0, 1);
    szyfrogram = wenigma.process_message(szyfrogram);
    std::cout << szyfrogram << std::endl ;

    Gui gui;
    return 0;
}
*/