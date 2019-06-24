#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

namespace Cache{
    /// Implements the three types of cache mapping
    class Cache{
        private:
            struct Word{
                int line;
                int block;
                int address;
                int content;
            };
            vector < vector<Word> > cache; //!< Will represent the cache.
            vector<int> frequency_register; //!< For the fully_associative mapping
            vector<vector<int>> input_register; //!< For the set_associative mapping
            vector<vector<int>> set_idx; //!< Lines of the cache that are into each set

            std::string file_name; //!< Receives the file name.
            int block_size; //!< Block size in number of words.
            int num_lines_cache; //!< Number of lines in the cache.
            int num_block_mp; //!< Number of blocks in the main memory.
            int mapping_type; //<! Mapping type.
            int num_sets; //!< Number of sets for the fully associative mapping.
            int replac_policy; //!< Replacement policy.

        
        public:

            /// Constructor
            Cache(int argc, char **argv);
            
            /// Destructor
            ~Cache(){/* Empty */};

            void directed_mapped(std::string command, int number1, int number2);

            void fully_associative(std::string command, int number1, int number2);

            void set_associative(std::string command, int number1, int number2);

            void randomic(std::string command, int number1, int number2, int block_, int set_);

            void FIFO(std::string command, int number1, int number2, int block_, int set_);

            void LFU(std::string command, int number1, int number2, int block_);
          
            void Read(std::string command, int number1, int number2);

            void Write(std::string command, int number1, int number2);

            void Show();
    };

}



#endif