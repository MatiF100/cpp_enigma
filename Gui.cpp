//
// Created by Mateusz on 29.12.2021.
//

#include "Gui.h"
#include "GuiFrame.h"

bool Gui::OnInit() {
    GuiFrame *frame = new GuiFrame();
    frame->Show(true);
    return true;

}
