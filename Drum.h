//
// Created by Mateusz on 16.12.2021.
//

#ifndef CPPPROJECT_DRUM_H
#define CPPPROJECT_DRUM_H

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#define DRUM_ALPHABET_LEN 26

//! Class holding information about single Drum configuration;
/*!
 * It holds basic information about drum inserted into machine.
 * It does not inlclude it's current rotation in the machine.
 */
class Drum {
    //! Contains vector of drum's output connections as their offset from (by default) latin alphabet
    std::vector<char> outputs;
    //! Contains location of notch that allows the next drum to be rotated
    uint8_t notch_location;



public:
    //! Says if the drum is an reflector(true), or normal(false) drum
    bool reflector;

    //! Informs if the drum is of narrow kind. Required by Kriegsenigma
    bool narrow;

    //! Drum's name. Optional but helps identify currently used drum
    std::string name{};

    //! Contains offset of numbered ring from it's nominal position on the drum
    uint8_t ring_offset;

    //! Basic, no argument constructor. It only initializes default values as 0;
    Drum();

    //! Copy constructor.
    /*!
     * Creates exact copy of the drum, but ensures correct vector reallocation
     * @param templ Reference to object desired for copying
     */
    Drum(const Drum& templ);

    //! FromStrings constructor; Takes all arguments as non-parsed strings
    /*!
     *
     * @param alphabet Coma separated string of alphabet characters, treated as output, with offset infered to their relative position to the latin alphabet
     * @param ro String value of ring offset
     * @param nl String value of notch location
     */
    Drum(const std::string& alphabet, const std::string& ro, const std::string& nl, bool narrow = false);

    //! FromStringAndValues constructor; Takes only offset and notch location as parsed integers
    /*!
     *
     * @param alph Coma separated string of alphabet characters, treated as output, with offset infered to their relative position to the latin alphabet
     * @param ro Value of ring offset
     * @param nl Value of notch location
     */
    Drum(const std::string& alphabet, uint8_t ro, uint8_t nl, bool narrow = false);

    //! FromValues constructor. All passed values should be of appropriate type
    /*!
     *
     * @param alphabet A reference to vector containing properly parsed alphabet. Use only for already sanitized inputs.
     * @param ro Value of ring offset
     * @param nl Value of notch location
     */
    Drum(std::vector<char>& alphabet, uint8_t ro, uint8_t nl, bool narrow = false);

    //! FromFile constructor. It creates drum based on specially prepared file
    Drum(std::fstream* handle, uint8_t ro = 0);


    //! Destructor, obvious bevavior
    ~Drum();


    //! Function that processes letter according to the drum's setting, with electricity running in "forward" direction
    /*!
     *
     * @param c Character to be processed
     * @param offset Current offset of the drum from it's initial position
     * @return Processed character, being value corresponding to current drum configuration and rotation
     */
    char process_character_forward(char c, uint8_t offset);

    //! Function that processes letter according to the drum's setting, with electricity running in "backward" direction
    /*!
     *
     * @param c Character to be processed
     * @param offset Current offset of the drum from it's initial position
     * @return Processed character, being value corresponding to current drum configuration and rotation
     */
    char process_character_backward(char c, uint8_t offset);

    //! Function that returns real location of the notch that allows to rotate next drum in a row
    /*!
     * The real offset is sum of the position of the notch at the alphabetic ring and the offset of this ring from it's nominal position
     * @return Sum of notch postion and alphabetic ring offset
     */
    [[nodiscard]] uint8_t real_notch_location() const{
        return this->notch_location + this->ring_offset;
    }


    //!Function used to set ring's offset while preparing drum for usage
    /*!
     *
     * @param offset Offset to be set onto the ring
     */
    void set_ring_offset(uint8_t offset);

    //! Output stream operator overload. Used mostly for file operations
    friend std::ostream& operator<<(std::ostream& os, const Drum& drum);

    //! Input stream operator overload. Used mostly for loading configuration from file
    friend std::istream& operator>>(std::istream& is, Drum& drum);

};


#endif //CPPPROJECT_DRUM_H
