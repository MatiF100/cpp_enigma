//
// Created by Mateusz on 29.12.2021.
//

#include "GuiLogic.h"
#include "Drum.h"
#include "DrumAssembly.h"
/*
 *
    Drum d1("E,K,M,F,L,G,D,Q,V,Z,N,T,O,W,Y,H,X,U,S,P,A,I,B,R,C,J", 0, 'Q' - 'A');
    Drum d2("A,J,D,K,S,I,R,U,X,B,L,H,W,T,M,C,Q,G,Z,N,P,Y,F,V,O,E", 0, 'E' - 'A');
    Drum d3("B,D,F,H,J,L,C,P,R,T,X,V,Z,N,Y,E,I,W,G,A,K,M,U,S,Q,O", 0, 'V' - 'A');
    Drum rev("Y,R,U,H,Q,S,L,D,P,X,N,G,O,K,M,I,E,B,F,Z,C,W,V,J,A,T", 0, 0);;

    DrumAssembly module = DrumAssembly(d1, d2, d3, rev);
    module.set_drums_offset(1,0,0, 0);
    this->enigma.set_wehrenigma(module);
 */
GuiLogic::GuiLogic(wxPanel* parent)
:wxPanel(parent, -1, wxPoint(-1,-1), wxSize(-1,-1), wxBORDER_SUNKEN)
{

    Drum d1("E,K,M,F,L,G,D,Q,V,Z,N,T,O,W,Y,H,X,U,S,P,A,I,B,R,C,J", 0, 'Q' - 'A');
    Drum d2("A,J,D,K,S,I,R,U,X,B,L,H,W,T,M,C,Q,G,Z,N,P,Y,F,V,O,E", 0, 'E' - 'A');
    Drum d3("B,D,F,H,J,L,C,P,R,T,X,V,Z,N,Y,E,I,W,G,A,K,M,U,S,Q,O", 0, 'V' - 'A');
    Drum rev("Y,R,U,H,Q,S,L,D,P,X,N,G,O,K,M,I,E,B,F,Z,C,W,V,J,A,T", 0, 0);;

    DrumAssembly module = DrumAssembly(d1, d2, d3, rev);
    module.set_drums_offset(1,0,0, 0);
    this->enigma.set_wehrenigma(module);

    this->output = new wxStaticText(this, -1, wxT(""), wxPoint(20, 20));
}


