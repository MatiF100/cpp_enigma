//
// Created by Mateusz on 28.12.2021.
//

#include "Machine.h"

std::string Machine::process_message(const std::string& msg) {
    std::string cipher;
    if (std::holds_alternative<DrumAssembly>(*this->assembly)){
        for(auto i = msg.begin(); i < msg.end(); i++){
            char sub = this->plugboard ? this->plugboard->swap(*i) : *i;
            sub = std::get<DrumAssembly>(*this->assembly).process_letter(sub);
            cipher += this->plugboard ? this->plugboard->swap(sub) : sub;
        }
    }
    return cipher;
}

void Machine::set_drum_offsets(uint8_t reflector, uint8_t right, uint8_t middle, uint8_t left, uint8_t leftmost) {
    if (!this->assembly)
        return;
    if (std::holds_alternative<DrumAssembly>(*this->assembly)){
        auto& my_asm = std::get<DrumAssembly>(*this->assembly);
        my_asm.set_drums_offset(left, middle, right, reflector);
    }else if(std::holds_alternative<DrumAssemblyK>(*this->assembly)){
        DrumAssemblyK my_asm = std::get<DrumAssemblyK>(*this->assembly);
        my_asm.set_drums_offset(leftmost, left, middle, right, reflector);
    }
}

void Machine::attatch_plugboard(const Plugboard &pboard) {
    this->detatch_plugboard();
    this->plugboard = std::make_unique<Plugboard>(pboard);
}

void Machine::detatch_plugboard() {
    this->plugboard.reset();

}

void Machine::set_kriegsenigma(DrumAssemblyK& assembly){
    this->detatch_drums();
    this->assembly = std::make_unique<std::variant<DrumAssemblyK, DrumAssembly>>(assembly);
}

void Machine::set_wehrenigma(DrumAssembly& assembly){
    this->detatch_drums();
    this->assembly = std::make_unique<std::variant<DrumAssemblyK, DrumAssembly>>(assembly);
}

void Machine::detatch_drums(){
    this->assembly.reset();
}

Machine::Machine() {
    this->assembly.reset();
    this->plugboard.reset();
}

Machine::Machine(DrumAssembly& assem, Plugboard* pboard) {
    if(pboard== nullptr)
        this->plugboard = std::make_unique<Plugboard>();
    else
        this->plugboard = std::make_unique<Plugboard>(*pboard);
    this->assembly = std::make_unique<std::variant<DrumAssemblyK, DrumAssembly>>(assem);
}

std::tuple<uint8_t, uint8_t, uint8_t, uint8_t, uint8_t> Machine::get_offsets() {
    if (std::holds_alternative<DrumAssembly>(*this->assembly)){
        auto [r,m,l,ref] = std::get<DrumAssembly>(*this->assembly).get_offsets();
        return std::make_tuple(r,m,l,511,ref);


    }else if(std::holds_alternative<DrumAssemblyK>(*this->assembly)){
        auto [r,m,l, lm,ref] = std::get<DrumAssemblyK>(*this->assembly).get_offsets();
        return std::make_tuple(r,m,l,lm,ref);

    }
    return {511,511,511,511,511};
}

std::ostream& operator<<(std::ostream& os, const Machine& machine){
    os << machine.buffer;
    return os;
}

std::istream &operator>>(std::istream &is,Machine &machine) {
    std::string tmp;
    is >> tmp;
    machine.buffer+=machine.process_message(tmp);
    return is;
}

uint8_t Machine::get_variant() const {
    if (this->assembly == nullptr)
        return 0;
    if (std::holds_alternative<DrumAssembly>(*this->assembly))
        return 1;
    if (std::holds_alternative<DrumAssemblyK>(*this->assembly))
        return 2;
    return 0;
}
