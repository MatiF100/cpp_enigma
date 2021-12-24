//
// Created by Mateusz on 24.12.2021.
//

#ifndef CPPPROJECT_DRUMASSEMBLY_H
#define CPPPROJECT_DRUMASSEMBLY_H

#include "Drum.h"

//! Class holding information about standard 3+1 drum assembly
/*!
 * It's purpose is to add abstraction above individual drums
 * Making them more predictable and less prone to errors
 * It also keeps information about current offset of the drums, a.k.a their rotation
 */
class DrumAssembly {
    Drum* right;
    Drum* middle;
    Drum* left;
    Drum* reflector;
    uint8_t offset[3] = {0,0,0};

public:
    //! Default constructor - initializes all pointers with 0
    DrumAssembly();

    //! Standard constructor. Initializes assembly with referenced drums.
    /*!
     * This constructor actually creates copies of passed arguments, so that their invalidation
     * will not cause problems with later execution
     * @param r Reference to right (first and seventh) drum temlate
     * @param m Reference to middle (second and sixth) drum template
     * @param l Reference to left (third and fifth) drum template
     * @param refl Reference to the far left (fourth) reflector drum. It's reflector flag must be set
     */
    DrumAssembly(const Drum& r, const Drum& m, const Drum& l, const Drum& refl);

    //! Destructor. Deallocates memory for all existing drums, since all of them are allocated within the class
    ~DrumAssembly();

    //! Safe setter for all drums
    /*!
     * It performs extra checks to ensure that correct drums were inserted at right positions
     * This function ensures, that if at least one drum setting fails, it will be left at it's initial state
     * @param r Reference to right (first and seventh) drum temlate
     * @param m Reference to middle (second and sixth) drum template
     * @param l Reference to left (third and fifth) drum template
     * @param refl Reference to the far left (fourth) reflector drum. It's reflector flag must be set
     */
    bool set_drums(const Drum& r, const Drum& m, const Drum& l, const Drum& refl);

    //! Safe setter for left drum
    /*!
     * @param drum Reference to template drum
     * @return True if setting succesfull. False if drum cannot be set - possibly caused by reflector flag
     */
    bool set_left_drum(const Drum& drum);

    //! Safe setter for middle drum
    /*!
     * @param drum Reference to template drum
     * @return True if setting succesfull. False if drum cannot be set - possibly caused by reflector flag
     */
    bool set_middle_drum(const Drum& drum);

    //! Safe setter for right drum
    /*!
     * @param drum Reference to template drum
     * @return True if setting succesfull. False if drum cannot be set - possibly caused by reflector flag
     */
    bool set_right_drum(const Drum& drum);

    //! Safe setter for reflector drum
    /*!
     * @param drum Reference to template drum
     * @return True if setting succesfull. False if drum cannot be set - possibly caused by reflector flag
     */
    bool set_reflector_drum(const Drum& drum);

    //! Function to process letter by entire drum assembly - requires all of the drums to be set, and fails otherwise
    /*!
     * If any of the drums is not set, one can still use this function
     * In this case it will just return the passed letter without any change
     * @param in Character to be processed
     * @return Character after processing, or input character in case of failure
     */
    char process_letter(char in);

    //! Function to configure current offset for each drum
    /*!
     * If offset outside of drum bounds is passed, it is treated as overflow and processed as so
     * Offset value is number visible in window corresponding to given drum on machine's top, decremented by 1
     * @param left Offset for the left drum
     * @param middle Offset for the middle drum
     * @param right Offset for the right drum
     */
    void set_drums_offset(uint8_t left, uint8_t middle, uint8_t right);


    //! This function reconfigures drums, by advancing the right-most by one position, and the rest as necessary
    /*!
     * Rotation of all the drums but first depends on location of specific notch on the drum's ring
     * It's position can actually vary, because ring can be adjusted before inserting it into machine
     * @return True if rotation  was successful. False if could not get information about drum's notch location
     */
    bool rotate_drums();

};


#endif //CPPPROJECT_DRUMASSEMBLY_H
