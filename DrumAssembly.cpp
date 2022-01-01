//
// Created by Mateusz on 24.12.2021.
//

#include "DrumAssembly.h"
#include "string"
#include "sstream"
#include "iostream"



DrumAssembly::DrumAssembly(const Drum &r, const Drum &m, const Drum &l, const Drum &refl) {
    this->reflector = new Drum(refl);
    this->right = new Drum(r);
    this->middle = new Drum(m);
    this->left = new Drum(l);
}

DrumAssembly::DrumAssembly() {
    this->reflector = nullptr;
    this->right = nullptr;
    this->middle = nullptr;
    this->left = nullptr;
}

DrumAssembly::~DrumAssembly() {
    delete this->reflector;
    delete this->left;
    delete this->right;
    delete this->middle;

}

bool DrumAssembly::set_drums(const Drum &r, const Drum &m, const Drum &l, const Drum &refl) {
    //Saving current state as backup
    Drum* c_left = this->left;
    Drum* c_right = this->right;
    Drum* c_mid = this->middle;
    Drum* c_refl = this->reflector;

    //If setting fails we need to restore original drum's settigns
    //We also need to ensure there are no memory leaks
    if(!this->set_left_drum(l)) {
        return false;
    }
    if (!this->set_right_drum(r)) {

        delete this->left;

        this->left = c_left;
        return false;
    }
    if (!this->set_middle_drum(m)) {

        delete this->left;
        delete this->right;

        this->left = c_left;
        this->right = c_right;
        return false;
    }
    if (!this->set_reflector_drum(refl)) {

        delete this->left;
        delete this->right;
        delete this->middle;

        this->left = c_left;
        this->right = c_right;
        this->middle = c_mid;
        return false;
    }
    //Removing old drums, as there were new copies generated
    delete c_refl;
    delete c_mid;
    delete c_right;
    delete c_left;

    return true;
}



bool DrumAssembly::set_left_drum(const Drum &drum) {
    if(!drum.reflector && !drum.narrow) {
        this->left = new Drum(drum);
        return true;
    }
    return false;
}

bool DrumAssembly::set_middle_drum(const Drum &drum) {
    if(!drum.reflector && !drum.narrow) {
        this->middle = new Drum(drum);
        return true;
    }
    return false;
}

bool DrumAssembly::set_right_drum(const Drum &drum) {
    if(!drum.reflector && !drum.narrow) {
        this->right = new Drum(drum);
        return true;
    }
    return false;
}

bool DrumAssembly::set_reflector_drum(const Drum &drum) {
    if(drum.reflector && !drum.narrow) {
        this->reflector = new Drum(drum);
        return true;
    }
    return false;
}

char DrumAssembly::process_letter(char in) {
    if (this->reflector == nullptr || this->left == nullptr || this->right == nullptr || this->middle == nullptr || !DrumAssembly::is_alphabetic(in))
        return in;

    //In real machine, characters are processed AFTER the drums perform rotation
    this->rotate_drums();
    return this->right->process_character_backward(
            this->middle->process_character_backward(
                this->left->process_character_backward(
                    this->reflector->process_character_forward( //Forward or backward doesn't make difference with reflector, but offset needs to be calculated accordingly or it will not behave as real device
                        this->left->process_character_forward(
                            this->middle->process_character_forward(
                                this->right->process_character_forward(in, this->offset[0]), this->offset[1]), this->offset[2]), this->refl_offset), this->offset[2]), this->offset[1]), this->offset[0]);
}

void DrumAssembly::set_drums_offset(uint8_t left, uint8_t middle, uint8_t right, uint8_t reflector) {
    this->offset[0] = right % 26;
    this->offset[1] = middle % 26;
    this->offset[2] = left % 26;
    this->refl_offset = reflector % 26;
}

bool DrumAssembly::rotate_drums() {
    if(this->left == nullptr || this->middle == nullptr || this->right == nullptr)
        return false;
    uint8_t right_notch = this->right->real_notch_location();
    uint8_t middle_notch = this->middle->real_notch_location();
    //leftmost notch is not needed, as there are no more drums for it to rotate

    if (this->offset[1] == middle_notch) {
        ++this->offset[1] %= 26; //Double step simulation
        ++this->offset[2] %= 26;
    }

    if(this->offset[0] == right_notch) {
        ++this->offset[1] %= 26;
    }

    ++this->offset[0] %= 26;

    return true;
}

std::tuple<uint8_t, uint8_t, uint8_t, uint8_t, uint8_t> DrumAssembly::get_offsets() {
    return {this->offset[0], this->offset[1], this->offset[2],0, this->refl_offset};
}

bool DrumAssembly::is_alphabetic(char in) {
    if (in >= 'A' && in <= 'Z' || in >= 'a' && in <= 'z')
        return true;
    return false;

}

DrumAssembly::DrumAssembly(const DrumAssembly &assembly) {

    this->reflector = nullptr;
    this->right = nullptr;
    this->middle = nullptr;
    this->left = nullptr;

    this->offset[0] = assembly.offset[0];
    this->offset[1] = assembly.offset[1];
    this->offset[2] = assembly.offset[2];

    this->refl_offset = assembly.refl_offset;

    if (assembly.reflector != nullptr)
        this->reflector = new Drum(*assembly.reflector);
    if (assembly.right != nullptr)
        this->right = new Drum(*assembly.right);
    if (assembly.middle != nullptr)
        this->middle = new Drum(*assembly.middle);
    if (assembly.left != nullptr)
        this->left = new Drum(*assembly.left);
}

DrumAssembly& DrumAssembly::operator=(const DrumAssembly &assembly) {
    this->reflector = nullptr;
    this->right = nullptr;
    this->middle = nullptr;
    this->left = nullptr;

    this->offset[0] = assembly.offset[0];
    this->offset[1] = assembly.offset[1];
    this->offset[2] = assembly.offset[2];

    this->refl_offset = assembly.refl_offset;

    if (assembly.reflector != nullptr)
        this->reflector = new Drum(*assembly.reflector);
    if (assembly.right != nullptr)
        this->right = new Drum(*assembly.right);
    if (assembly.middle != nullptr)
        this->middle = new Drum(*assembly.middle);
    if (assembly.left != nullptr)
        this->left = new Drum(*assembly.left);

    return *this;
}

std::string DrumAssembly::get_configuration() {
    std::stringstream config;
    config << "1" << *this->right << *this->middle << *this->left << *this->reflector << std::endl;
    config << (unsigned int)this->offset[0] << " " << (unsigned int)this->offset[1] << " " << (unsigned int)this->offset[2] << " " << (unsigned int)this->refl_offset << std::endl;
    return config.str();
}

bool DrumAssembly::set_configuration_from_string(std::string cfg) {
    std::stringstream config(cfg);
    int type;
    config >> type;
    //This means we are trying to load wrong configuration
    if (type != 1)
        return false;
    if (this->right == nullptr)
        this->right = new Drum();
    config >> *this->right;

    if (this->middle == nullptr)
        this->middle = new Drum();
    config >> *this->middle;

    if (this->left == nullptr)
        this->left = new Drum();
    config >> *this->left;

    if (this->reflector == nullptr)
        this->reflector = new Drum();
    config >> *this->reflector;
    unsigned int r,m,l,ref;
    config >> r >> m >> l >> ref;
    this->offset[0] = r;
    this->offset[1] = m;
    this->offset[2] = l;
    this->refl_offset = ref;
    return true;
}

