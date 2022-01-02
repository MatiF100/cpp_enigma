//
// Created by Mateusz on 28.12.2021.
//

#ifndef CPPPROJECT_MACHINE_H
#define CPPPROJECT_MACHINE_H

#include "DrumAssembly.h"
#include "DrumAssemblyK.h"
#include "Plugboard.h"

#include <memory>
#include <variant>
#include <string>


//! Class holding information about an enigma machine's current inner configuration
/*!
 * This class holds pointers to both, a drum assembly which performs cyclical substitution
 * And the plugboard that performs extra substitutions on entry and exit of a character
 * Most of the standard drums in first 3 positions (so excluding the 4th drum from Kriegsmarine configuration) perform cyclical rotations during machine's operation
 * Reversing drum can technically be rotated too, but this was rarely (if ever) used in army versions of the machine
 * This was though an option that civil versions of enigma (lacking one drum and having reversing drum's wheel exposed) offered
 */
class Machine {
    std::unique_ptr<std::variant<DrumAssemblyK, DrumAssembly>> assembly;
    std::unique_ptr<Plugboard> plugboard;
    std::string buffer;
public:
    //!Default constructor. Will create empty machine, without any modules attatched
    Machine();

    //! Basic constructor (Wehrmaht). It will create machine with ready drum assembly
    /*!
     * This constructor will initialize machine with Wehrenigma settings
     * @param assem Reference to drum's assembly that should be inserted into machine
     * @param pboard Pointer to the plugboard object
     */
    Machine(DrumAssembly& assem, Plugboard* pboard = nullptr);

    //! Basic constructor (Kriegsmarine). It will create machine with ready drum assembly
    /*!
     * This constructor will initialize machine with Wehrenigma settings
     * @param assem Reference to drum's assembly that should be inserted into machine
     * @param pboard Pointer to the plugboard object
     */
    Machine(DrumAssemblyK& assem, Plugboard* pboard = nullptr);

    //! Sets enigma machine to use Kriegsmarine configuration
    /*!
     * It requires an Wehrmacht version of the drum assembly, which uses 4 rotors (3 of which are rotating while operation) and reversing drum
     * @param assembly Reference to template, Kriegsmarine drum assembly
     */
    void set_kriegsenigma(DrumAssemblyK& assembly);

    //! Sets enigma machine to use Wehrmacht configuration
    /*!
     * It requires an Wehrmacht version of the drum assembly, which uses 3 rotors and reversing drum
     * @param assembly  Reference to template, Wehrmacht drum assembly
     */
    void set_wehrenigma(DrumAssembly& assembly);

    //! Resets current drum assembly, and switches machine into non-operational/service state
    void detatch_drums();

    //! Attatches plugboard module to the machine
    /*!
     * This function attatches plugboard module, which allows to use additional letter substitutions
     * It creates a copy of the object, leaving the original plugboard untouched
     * @param pboard Reference to plugboard object
     */
    void attatch_plugboard(const Plugboard& pboard);

    //! Detatches plugboard module from the machine and deallocates memory assigned to it
    void detatch_plugboard();

    //! Abstract function that processes given message accordingly to the current machine's state
    /*!
     * This function will go trough entire given string, and will return new string containing the modified message
     * It will not alter the original message
     * @param msg String containing the message to be processed
     * @return string containing processed message
     */
    std::string process_message(const std::string& msg);

    //! Abstract function to configure drum's  setting no matter the configuration
    /*!
     * This function check which configuration is currently used, and initializes drum assembly's module accordingly
     * @param reflector Offset for the reflector drum (always present)
     * @param right Offset for the right drum (always present)
     * @param middle Offset for the middle drum (always present)
     * @param left Offset for the left drum - last one in case of wehrenigma (always present)
     * @param leftmost Offset for the leftmost drum (Kriegsenigma only)
     */
    void set_drum_offsets(uint8_t reflector = 0, uint8_t right = 0, uint8_t middle = 0, uint8_t left = 0, uint8_t leftmost = 0);

    //!Abstract function to get current offset of drums
    /*!
     * This function will always return 5 element tuple with following offsets: right, middle, left, leftmost, reflector.
     * In case of drum's non-existance, the value will be 511
     * @return Tuple containing offsets of corresponding drums
     */
    std::tuple<uint8_t , uint8_t , uint8_t , uint8_t , uint8_t > get_offsets();

    //! Function returning variant of the enigma machine currently used
    /*!
     * Return codes:
     * 0 - none or unknown variant
     * 1 - Wehrmacht variant
     * 2 - Kriegsmarine variant
     * @return One of above codes
     */
    [[nodiscard]] uint8_t get_variant() const;

    //! Function returning current plugboard setting
    /*!
     *
     * @return Pointer to copy of currently attached plugboard
     */
    Plugboard* get_plugboard();

    //!Output stream operator overload. Used for message processing
    friend std::ostream& operator<<(std::ostream& os, const Machine& machine);

    //! Input stream operator overload. Used for message processing
    friend std::istream& operator>>(std::istream& is, Machine& machine);

    //! Function saving current configuration to file, with proprietary (and very simple) format
    /*!
     * This function will save current state of plugboard, selected drums and their current offsets
     * @param filename name of the file for the config to be saved into
     * @return True if operation was successful
     */
    [[nodiscard]] bool save_config(std::string &filename);

    [[nodiscard]] bool load_config(std::string &filename);
};


#endif //CPPPROJECT_MACHINE_H
