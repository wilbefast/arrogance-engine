/*
Arrogance Engine: a simple SDL/OpenGL game engine for Desktop and Android.
Copyright (C) 2012 William James Dyce

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "file.hpp"

#include "SDL.h"                // for SDL_RWops

#include "../assert.hpp"
#include "../warn.hpp"

using namespace io;

int io::read_text(const char* source_file, char** destination)
{
    // Open the file
    SDL_RWops* file;
    file = SDL_RWFromFile(source_file, "r");
    ASSERT(file, "Opening text file using SDL_RWops");

    // Find the length of the file
    int file_length = SDL_RWseek(file, 0, SEEK_END);
    (*destination) = new char[file_length];

    // Reset seek to beginning of file and read text
    SDL_RWseek(file, 0, SEEK_SET);
    int n_blocks = SDL_RWread(file, (*destination), BLOCK_SIZE, MAX_BLOCKS);
    SDL_RWclose(file);
    // Make sure the operation was successful
    ASSERT(n_blocks >= 0, "Reading blocks of data from SDL_RWops");
    WARN_IF(n_blocks == MAX_BLOCKS, "Reading data from SDL_RWops",
                                    "Buffer full so may be too small for data");

    // Success!
    return EXIT_SUCCESS;
}

int io::read_xml(const char* file_name, TiXmlDocument* doc)
{
    #if BUFFER_XML
        // Open with SDL_RWops, read contents to a buffer
        char* file_contents = NULL;
        ASSERT(io::read_text(file_name, &file_contents) == EXIT_SUCCESS,
                "Reading XML file to buffer");

        // Pass the buffer to the TinyXML document
        ASSERT_AUX(doc->Parse(file_contents), "Parsing XML buffer",
                   doc->ErrorDesc());
        // Delete the buffer
        delete file_contents;
    #else
        // Open normally, without an intermediate buffer
        doc->SetValue(file_name);
        ASSERT_AUX(doc->LoadFile(), "Loading XML file directly",
                    doc->ErrorDesc());
    #endif

    // All clear
    return EXIT_SUCCESS;
}
