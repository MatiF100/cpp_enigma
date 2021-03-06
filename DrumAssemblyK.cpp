//
// Created by Mateusz on 27.12.2021.
//

#include "DrumAssemblyK.h"
#include "sstream"

//Helper function - not exported
DrumAssemblyK::DrumAssemblyK() : DrumAssembly(){
    this->leftmost = nullptr;
    this->leftmost_offset = 0;
}

DrumAssemblyK::DrumAssemblyK(const Drum &r, const Drum &m, const Drum &l, const Drum &lm, const Drum &refl)
: DrumAssembly(r, m, l, refl)
{
    this->leftmost = new Drum(lm);
    this->leftmost_offset = 0;
}

DrumAssemblyK::~DrumAssemblyK() {
    delete this->leftmost;
}

bool DrumAssemblyK::set_drums(const Drum &r, const Drum &m, const Drum &l, const Drum &lm, const Drum &refl){

    //Saving current state as backup
    Drum* c_leftmost = this->leftmost;
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

    if(!this->set_leftmost_drum(lm)){

        delete this->leftmost;
        delete this->left;
        delete this->right;
        delete this->middle;

        this->leftmost = c_leftmost;
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
    delete c_leftmost;

    return true;
}

bool DrumAssemblyK::set_leftmost_drum(const Drum &drum) {
    if(!drum.reflector && drum.narrow) {
        this->leftmost = new Drum(drum);
        return true;
    }
    return false;
}

bool DrumAssemblyK::set_reflector_drum(const Drum &drum) {
    //Only change here is check for narrowness of the drum. It is necessary though
    if(drum.reflector && drum.narrow) {
        this->reflector = new Drum(drum);
        return true;
    }
    return false;
}

void DrumAssemblyK::set_drums_offset(uint8_t leftmost, uint8_t left, uint8_t middle, uint8_t right, uint8_t reflector) {
    this->leftmost_offset = leftmost;
    DrumAssembly::set_drums_offset(left, middle, right, reflector);
}

void DrumAssemblyK::set_leftmost_offset(uint8_t offset) {
    this->leftmost_offset = offset % 26;
}

char DrumAssemblyK::process_letter(char in) {
    if (this->reflector == nullptr || this->left == nullptr || this->right == nullptr || this->middle == nullptr || this->leftmost == nullptr || !DrumAssembly::is_alphabetic(in))
        return in;

    //In real machine, characters are processed AFTER the drums perform rotation
    this->rotate_drums();
    return this->right->process_character_backward(
            this->middle->process_character_backward(
                    this->left->process_character_backward(
                            this->leftmost->process_character_backward(
                                this->reflector->process_character_forward( //Forward or backward doesn't make difference with reflector, but offset needs to be calculated accordingly or it will not behave as real device
                                    this->leftmost->process_character_forward(
                                            this->left->process_character_forward(
                                                    this->middle->process_character_forward(
                                                            this->right->process_character_forward(in, this->offset[0]), this->offset[1]), this->offset[2]),
                                                            this->leftmost_offset), this->refl_offset), this->leftmost_offset),
                                                            this->offset[2]), this->offset[1]), this->offset[0]);
}

std::tuple<uint8_t, uint8_t, uint8_t, uint8_t, uint8_t> DrumAssemblyK::get_offsets() {
    auto [r, m, l, _lm, ref] =  DrumAssembly::get_offsets();
    return {r, m, l, this->leftmost_offset, ref};
}

DrumAssemblyK::DrumAssemblyK(const DrumAssemblyK &assembly) : DrumAssembly(assembly) {

    this->reflector = nullptr;
    this->right = nullptr;
    this->middle = nullptr;
    this->left = nullptr;
    this->leftmost = nullptr;

    this->offset[0] = assembly.offset[0];
    this->offset[1] = assembly.offset[1];
    this->offset[2] = assembly.offset[2];

    this->refl_offset = assembly.refl_offset;
    this->leftmost_offset = assembly.leftmost_offset;

    if (assembly.reflector != nullptr)
        this->reflector = new Drum(*assembly.reflector);
    if (assembly.right != nullptr)
        this->right = new Drum(*assembly.right);
    if (assembly.middle != nullptr)
        this->middle = new Drum(*assembly.middle);
    if (assembly.left != nullptr)
        this->left = new Drum(*assembly.left);
    if (assembly.leftmost != nullptr)
        this->leftmost = new Drum(*assembly.leftmost);
}


DrumAssemblyK& DrumAssemblyK::operator=(const DrumAssemblyK& assembly){

    this->reflector = nullptr;
    this->right = nullptr;
    this->middle = nullptr;
    this->left = nullptr;
    this->leftmost = nullptr;

    this->offset[0] = assembly.offset[0];
    this->offset[1] = assembly.offset[1];
    this->offset[2] = assembly.offset[2];

    this->refl_offset = assembly.refl_offset;
    this->leftmost_offset = assembly.leftmost_offset;

    if (assembly.reflector != nullptr)
        this->reflector = new Drum(*assembly.reflector);
    if (assembly.right != nullptr)
        this->right = new Drum(*assembly.right);
    if (assembly.middle != nullptr)
        this->middle = new Drum(*assembly.middle);
    if (assembly.left != nullptr)
        this->left = new Drum(*assembly.left);
    if (assembly.leftmost != nullptr)
        this->leftmost = new Drum(*assembly.leftmost);

    return *this;
}

std::string DrumAssemblyK::get_configuration() {
    std::stringstream config;
    config << "2" << std::endl << *this->right << *this->middle << *this->left << *this->leftmost << *this->reflector;
    config << (unsigned int)this->offset[0] << " " << (unsigned int)this->offset[1] << " " << (unsigned int)this->offset[2] << " " <<  (unsigned int)this->leftmost_offset << " " << (unsigned int)this->refl_offset << std::endl;
    return config.str();
}

bool DrumAssemblyK::set_configuration_from_string(std::string cfg) {

    std::stringstream config(cfg);
    int type;
    config >> type;
    //This means we are trying to load wrong configuration
    if (type != 2)
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

    if (this->leftmost == nullptr)
        this->leftmost = new Drum();
    config >> *this->leftmost;

    if (this->reflector == nullptr)
        this->reflector = new Drum();
    config >> *this->reflector;

    unsigned int r,m,l, lm,ref;
    config >> r >> m >> l >> lm >> ref;
    this->offset[0] = r;
    this->offset[1] = m;
    this->offset[2] = l;
    this->leftmost_offset = lm;
    this->refl_offset = ref;

    return true;

}


