//
// Created by Mateusz on 16.12.2021.
//

#ifndef CPPPROJECT_DRUM_H
#define CPPPROJECT_DRUM_H

#include <cstdint>
#include <string>
#include <vector>

#define DRUM_ALPHABET_LEN 26

//! Class holding information about single Drum configuration;
/*!
 * It holds basic information about drum inserted into machine.
 * It does not inlclude it's current rotation in the machine.
 */
class Drum {
    //! Contains vector of drum's output connections as their offset from (by default) latin alphabet
    std::vector<char> outputs;
    //! Contains offset of numbered ring from it's nominal position on the drum
    uint8_t ring_offset;
    //! Contains location of notch that allows the next drum to be rotated
    uint8_t notch_location;

public:
    //! Basic, no argument constructor. It only initializes default values as 0;
    Drum();

    //! FromStrings constructor; Takes all arguments as non-parsed strings
    /*!
     *
     * @param alphabet Coma separated string of alphabet characters, treated as output, with offset infered to their relative position to the latin alphabet
     * @param ro String value of ring offset
     * @param nl String value of notch location
     */
    Drum(const std::string& alphabet, const std::string& ro, const std::string& nl);

    //! FromStringAndValues constructor; Takes only offset and notch location as parsed integers
    /*!
     *
     * @param alph Coma separated string of alphabet characters, treated as output, with offset infered to their relative position to the latin alphabet
     * @param ro Value of ring offset
     * @param nl Value of notch location
     */
    Drum(const std::string& alphabet, uint8_t ro, uint8_t nl);

    //! FromValues constructor. All passed values should be of appropriate type
    /*!
     *
     * @param alphabet A reference to vector containing properly parsed alphabet. Use only for already sanitized inputs.
     * @param ro Value of ring offset
     * @param nl Value of notch location
     */
    Drum(std::vector<char>& alphabet, uint8_t ro, uint8_t nl);


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
};


#endif //CPPPROJECT_DRUM_H
