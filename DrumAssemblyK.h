//

// Created by Mateusz on 27.12.2021.
//

#ifndef CPPPROJECT_DRUMASSEMBLYK_H
#define CPPPROJECT_DRUMASSEMBLYK_H

//class DrumAssembly;
#include "DrumAssembly.h"

//! Class holding information about extended Kriegsmarine enigma drum assembly (Kriegsenigma or Enigma Shark) with 4+1 configuration
/*!
 * It's main difference from basic Wehrmacht model is that of actually having 4 rotors except for the reversing drum
 * It is worth nothing though, that even if it has 4 rotors, the leftmost one is NOT propelled by third notch
 * And it remainst stationary during entire machine's cycle. This is partly because of this variant of the machinne
 * Required only some minor modifications to standard enigma, and therefore no additional mechanism of it's progression was added
 * This lack of major modification hovever has enforced redesiging both the reversing drum, and the drums used on fourth position
 * This redesigning is marked in they structure by setting the "narrow" flag during creation
 */
class DrumAssemblyK : public DrumAssembly {
    Drum* leftmost;
    uint8_t leftmost_offset;

    /*! Overriden old set_drums and function
     * This function is basically deprecated in this case, since it no longer allows to properly initialize assembly's structure
     * And could create some undefined behavior if used incorrectly, so it is overriden with empty function
     * Because of this, it is declared as private
     */
     bool set_drums(const Drum& r, const Drum& m, const Drum& l, const Drum& refl) override{return false;}
public:

    //! Default constructor - initializes all pointers with 0
    DrumAssemblyK();

    //! Standard constructor. Initializes assembly with referenced drums.
    /*!
     * This constructor actually creates copies of passed arguments, so that their invalidation
     * will not cause problems with later execution
     * @param r Reference to right (first and nineth) drum temlate
     * @param m Reference to middle (second and eight) drum template
     * @param l Reference to left (third and seventh) drum template
     * @param lm Reference to leftmost (fourth and sixth) drum template. It's narrow flag must be set
     * @param refl Reference to the far left (fifth) reflector drum. It's reflector flag and narrow flag must be set
     */
    DrumAssemblyK(const Drum& r, const Drum& m, const Drum& l, const Drum& lm, const Drum& refl);

    //! Copy constructor
    /*!
     * Assures that instantiating new object of the class with other one, actually results in proper memory allocation
     * @param assembly Reference to assembly that shall be used as template
     */
    DrumAssemblyK(const DrumAssemblyK& assembly);

    //! Destructor. Deallocates memory for all existing drums, since all of them are allocated within the class
    ~DrumAssemblyK();

    //! Safe setter for all drums
    /*!
     * It performs extra checks to ensure that correct drums were inserted at right positions
     * This function ensures, that if at least one drum setting fails, it will be left at it's initial state
     * @param r Reference to right (first and nineth) drum temlate
     * @param m Reference to middle (second and eight) drum template
     * @param l Reference to left (third and seventh) drum template
     * @param lm Reference to leftmost (fourth and sixth) drum template. It's narrow flag must be set
     * @param refl Reference to the far left (fifth) reflector drum. It's reflector flag and narrow flag must be set
     */
    [[nodiscard]]bool set_drums(const Drum& r, const Drum& m, const Drum& l, const Drum& lm, const Drum& refl);

    //! Safe setter for leftmost drum
    /*!
     * @param drum Reference to template drum
     * @return True if setting succesfull. False if drum cannot be set - possibly caused by reflector or narrow flag
     */
    [[nodiscard]]bool set_leftmost_drum(const Drum& drum);

    //! Safe setter for reflector drum
    /*!
     * @param drum Reference to template drum
     * @return True if setting succesfull. False if drum cannot be set - possibly caused by reflector or narrow flag
     */
    [[nodiscard]]bool set_reflector_drum(const Drum &drum) override;

    //! Function to configure current offset for each drum
    /*!
     * If offset outside of drum bounds is passed, it is treated as overflow and processed as so
     * Offset value is number visible in window corresponding to given drum on machine's top, decremented by 1
     * @param leftmost Offset for the left drum
     * @param left Offset for the left drum
     * @param middle Offset for the middle drum
     * @param right Offset for the right drum
     * @param reflector Offset for the reflector drum
     */
    void set_drums_offset(uint8_t leftmost, uint8_t left, uint8_t middle, uint8_t right, uint8_t reflector);

    //! Function to configure only the leftmost drum offset
    /*!
     * It is declared as separate, because this drum is only one not rotated by built-in mechanism and therefore only user's interaction can change it
     * If offset outside of drum bounds is passed, it is treated as overflow and processed as so
     * Offset value is number visible in window corresponding to given drum on machine's top, decremented by 1
     * @param offset Offset to witch set the leftmost drum
     */
    void set_leftmost_offset(uint8_t offset);

    //! Function to process letter by entire drum assembly - requires all of the drums to be set, and fails otherwise
    /*!
     * If any of the drums is not set, one can still use this function
     * In this case it will just return the passed letter without any change
     * @param in Character to be processed
     * @return Character after processing, or input character in case of failure
     */
    char process_letter(char in) override;

    //! This function returns tuple containing all the drums offsets right to left, including reflector drum and thin additional drum
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t, uint8_t> get_offset();


    //! Equal operator overload. Allows for assignment with deep copy behavior
    /*!
     * Similaringly to copy constructor, it ensures that newly created object will be a deep copy of template one
     * @param assembly Reference to template assembly
     * @return Value of new assembly
     */
    DrumAssemblyK& operator=(const DrumAssemblyK& assembly);
};


#endif //CPPPROJECT_DRUMASSEMBLYK_H
