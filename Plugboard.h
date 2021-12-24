//
// Created by Mateusz on 24.12.2021.
//

#ifndef CPPPROJECT_PLUGBOARD_H
#define CPPPROJECT_PLUGBOARD_H

#include <unordered_map>

//! Class holding information about plugboard module of the enigma machine
/*!
 * It's working principle is substituting any incoming and leaving character into another one
 * The way it is realized on real machine is by connecting two marked sockets on the front of the machine with special plugs
 * Hence the name "Plugboard" or "Plug switchboard"
 */
class Plugboard {
    std::unordered_map<char, char> substitutions;
public:
    //! There is no need for default constructor
    //! Empty map is interpreted as if no plugs were inserted

    //! Function that checks if substitution is possible and performs it if it is
    /*!
     *
     * @param in Character to be substituted on input
     * @param out Character to be substituted on output
     * @return True if substitution was successful, false otherwise
     */
    bool insert_plug(char in, char out);

    //! Function that disables existing substitution
    /*!
     *
     * @param target One of the letters of substitution pair
     * @return True if substitution was succesfully removed. False if none substitution with this letter exists
     */
    bool remove_plug(char target);

    //! Function that swaps character into corresponding one according to the plugboard current setting
    /*!
     *
     * @param in Letter to be swapped
     * @return Letter after swapping
     */
    char swap(char in);

};


#endif //CPPPROJECT_PLUGBOARD_H
