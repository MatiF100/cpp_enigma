//
// Created by Mateusz on 24.12.2021.
//

#ifndef CPPPROJECT_DRUMASSEMBLY_H
#define CPPPROJECT_DRUMASSEMBLY_H

#include "Drum.h"
#include "DrumAssemblyK.h"
#include <tuple>

//! Class holding information about standard 3+1 drum assembly
/*!
 * It's purpose is to add abstraction above individual drums
 * Making them more predictable and less prone to errors
 * It also keeps information about current offset of the drums, a.k.a their rotation
 * In this version of drum assembly, you cannot fit any drum marked with "narrow" flag
 */
class DrumAssembly {
    Drum* right;
    Drum* middle;
    Drum* left;
    Drum* reflector;
    uint8_t offset[3] = {0,0,0};
    uint8_t refl_offset = 0;

    //Friend functions are declared, since they require some redesign, which in turn requires access to some private fields of this class
    friend DrumAssemblyK::set_drums(const Drum &r, const Drum &m, const Drum &l, const Drum &refl);
    friend DrumAssemblyK::set_reflector_drum(const Drum &drum);
    char DrumAssemblyK::process_letter(char in);

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
    [[nodiscard]]virtual bool set_drums(const Drum& r, const Drum& m, const Drum& l, const Drum& refl);

    //! Safe setter for left drum
    /*!
     * @param drum Reference to template drum
     * @return True if setting succesfull. False if drum cannot be set - possibly caused by reflector or narrow flag
     */
    [[nodiscard]]bool set_left_drum(const Drum& drum);

    //! Safe setter for middle drum
    /*!
     * @param drum Reference to template drum
     * @return True if setting succesfull. False if drum cannot be set - possibly caused by reflector or narrow flag
     */
    [[nodiscard]]bool set_middle_drum(const Drum& drum);

    //! Safe setter for right drum
    /*!
     * @param drum Reference to template drum
     * @return True if setting succesfull. False if drum cannot be set - possibly caused by reflector or narrow flag
     */
    [[nodiscard]]bool set_right_drum(const Drum& drum);

    //! Safe setter for reflector drum
    /*!
     * @param drum Reference to template drum
     * @return True if setting succesfull. False if drum cannot be set - possibly caused by reflector or narrow flag
     */
    [[nodiscard]] virtual bool set_reflector_drum(const Drum& drum);

    //! Function to process letter by entire drum assembly - requires all of the drums to be set, and fails otherwise
    /*!
     * If any of the drums is not set, one can still use this function
     * In this case it will just return the passed letter without any change
     * @param in Character to be processed
     * @return Character after processing, or input character in case of failure
     */
    virtual char process_letter(char in);

    //! Function to configure current offset for each drum
    /*!
     * If offset outside of drum bounds is passed, it is treated as overflow and processed as so
     * Offset value is number visible in window corresponding to given drum on machine's top, decremented by 1
     * @param left Offset for the left drum
     * @param middle Offset for the middle drum
     * @param right Offset for the right drum
     * @param reflector Offset for the reflector drum
     */
    void set_drums_offset(uint8_t left, uint8_t middle, uint8_t right, uint8_t reflector);


    //! This function reconfigures drums, by advancing the right-most by one position, and the rest as necessary
    /*!
     * Rotation of all the drums but first depends on location of specific notch on the drum's ring
     * It's position can actually vary, because ring can be adjusted before inserting it into machine
     * It will move ONLY drums at positions 1-3, (right, middle and left) ignoring anything that follows
     * This property is important when it comes to Kriegsenigma
     * @return True if rotation  was successful. False if could not get information about drum's notch location
     */
    bool rotate_drums();

    //! This function returns tuple containing all the drums offsets right to left, including reflector drum
    std::tuple<uint8_t , uint8_t , uint8_t , uint8_t > get_offsets();

};


#endif //CPPPROJECT_DRUMASSEMBLY_H
