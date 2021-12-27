//
// Created by Mateusz on 27.12.2021.
//

#include "DrumAssemblyK.h"

DrumAssemblyK::DrumAssemblyK() {

}

DrumAssemblyK::DrumAssemblyK(const Drum &r, const Drum &m, const Drum &l, const Drum &lm, const Drum &refl) {

}

DrumAssemblyK::~DrumAssemblyK() {

}

bool DrumAssemblyK::set_drums(const Drum &r, const Drum &m, const Drum &l, const Drum &lm, const Drum &refl) {
    return false;
}

bool DrumAssemblyK::set_leftmost_drum(const Drum &drum) {
    return false;
}

bool DrumAssemblyK::set_reflector_drum(const Drum &drum) {
    return DrumAssembly::set_reflector_drum(drum);
}

void DrumAssemblyK::set_drums_offset(uint8_t leftmost, uint8_t left, uint8_t middle, uint8_t right, uint8_t reflector) {

}

void DrumAssemblyK::set_leftmost_offset(uint8_t offset) {

}

char DrumAssemblyK::process_letter(char in) {
    return DrumAssembly::process_letter(in);
}
