//
// Created by Mateusz on 28.12.2021.
//

#include "Machine.h"
#include "fstream"
#include "sstream"

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

Machine::Machine(DrumAssemblyK& assem, Plugboard* pboard) {
    if(pboard== nullptr)
        this->plugboard = std::make_unique<Plugboard>();
    else
        this->plugboard = std::make_unique<Plugboard>(*pboard);
    this->assembly = std::make_unique<std::variant<DrumAssemblyK, DrumAssembly>>(assem);
}

std::tuple<uint8_t, uint8_t, uint8_t, uint8_t, uint8_t> Machine::get_offsets() {
    if (std::holds_alternative<DrumAssembly>(*this->assembly)){
        auto [r,m,l,_lm,ref] = std::get<DrumAssembly>(*this->assembly).get_offsets();
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

bool Machine::save_config(std::string& filename) {

    std::fstream target;
    target.open(filename, std::ios_base::out);
    if (!target.good())
        return false;
    if (this->assembly == nullptr)
        return false;
    if(std::holds_alternative<DrumAssembly>(*this->assembly)){
        target << std::get<DrumAssembly>(*this->assembly).get_configuration();
    }else if(std::holds_alternative<DrumAssemblyK>(*this->assembly)){
        target << std::get<DrumAssemblyK>(*this->assembly).get_configuration();
    }
    for(int i=0; i<26; i++){
        if(this->plugboard==nullptr)
            target << (char)('A' + i);
        else{
            target << this->plugboard->swap('A' + i);
        }
    }
    return true;
}

bool Machine::load_config(std::string& filename) {
    std::fstream source;
    int type;
    std::string pboard;
    source.open(filename, std::ios_base::in);
    if (source.bad())
        return false;

    source >> type;
    source.seekg(0);
    if(type==1){
        DrumAssembly* tmp = new DrumAssembly();
        std::string line;
        std::stringstream config;

        //Type
        std::getline(source, line);
        config << line << std::endl;

        //Drum 1
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;

        //Drum 2
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;

        //Drum 3
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;

        //Reflector
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;

        //Offsets
        std::getline(source, line);
        config << line << std::endl;
        if (tmp->set_configuration_from_string(config.str())){
            this->assembly.reset();
            this->assembly = std::make_unique<std::variant<DrumAssemblyK, DrumAssembly>>(*tmp);
        }
        delete tmp;

    }else if(type == 2){

        DrumAssemblyK* tmp = new DrumAssemblyK();
        std::string line;
        std::stringstream config;

        //Type
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;

        //Drum 1
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;

        //Drum 2
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;

        //Drum 3
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;

        //Drum 4
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;

        //Reflector
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;
        std::getline(source, line);
        config << line << std::endl;

        //Offsets
        std::getline(source, line);
        config << line << std::endl;
        if (tmp->set_configuration_from_string(config.str())){
            this->assembly.reset();
            this->assembly = std::make_unique<std::variant<DrumAssemblyK, DrumAssembly>>(*tmp);
        }
        delete tmp;
    }else
        return false;

    //Plugboard
    std::getline(source, pboard);
    if(pboard.length() != 26)
        return false;
    source >> pboard;
    this->plugboard.reset();
    this->plugboard = std::make_unique<Plugboard>();
    for(int i = 0; i<26; i++){
        if ('A'+i != pboard[i])
            this->plugboard->insert_plug('A' + i, pboard[i]);
    }

    return true;
}

Plugboard *Machine::get_plugboard() {
    if (this->plugboard == nullptr)
        return nullptr;
    Plugboard* helper_ptr = new Plugboard(*this->plugboard);
    return helper_ptr;
}
