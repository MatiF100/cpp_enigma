//
// Created by Mateusz on 28.12.2021.
//

#include "Machine.h"

std::string Machine::process_message(const std::string msg) {
    std::string cipher;
    if (std::holds_alternative<DrumAssembly>(*this->assembly)){
        for(auto i = msg.begin(); i < msg.end(); i++){
            cipher += std::get<DrumAssembly>(*this->assembly).process_letter(*i);
        }
    }
    return cipher;
}

void Machine::set_drum_offsets(uint8_t reflector, uint8_t right, uint8_t middle, uint8_t left, uint8_t leftmost) {
    if (!this->assembly)
        return;
    if (std::holds_alternative<DrumAssembly>(*this->assembly)){
        DrumAssembly& my_asm = std::get<DrumAssembly>(*this->assembly);
        my_asm.set_drums_offset(left, middle, right, reflector);
    }else if(std::holds_alternative<DrumAssemblyK>(*this->assembly)){
        DrumAssemblyK my_asm = std::get<DrumAssemblyK>(*this->assembly);
        my_asm.set_drums_offset(leftmost, left, middle, right, reflector);
    }
}
