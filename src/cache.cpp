#include "../include/cache.h"

Cache::Cache::Cache(int argc, char **argv)
    : file_name("config.txt"){
    if(argc == 1) return;
    std::string param(argv[1]);
    file_name = param;

    std::ifstream file;
    file.open(file_name);
    if(!file){
        std::cout << "\033[31m >>> Could not open the file.\n \033[0;0m";
        std::quick_exit(1);
    }
    int line; // will receive each line of the file
    file >> line;
    block_size = line;
    file >> line;
    num_lines_cache = line;
    file >> line;
    num_block_mp = line;
    file >> line;
    mapping_type = line;
    file >> line;
    num_sets = line;
    file >> line;
    replac_policy = line;

    cache.resize(num_lines_cache);
    for(int i = 0; i < num_lines_cache; i++){
        cache[i].resize(block_size);
    }

    frequency_register.resize(num_lines_cache);
    for(int i = 0; i < num_lines_cache; i++){
        frequency_register[i] = 0;
    }

    input_register.resize(num_sets);
    for(int i = 0; i < num_sets; i++){
        input_register[i].resize(num_lines_cache/num_sets);
    }

    if(mapping_type == 1 || mapping_type == 2)
        num_sets = 1;

    set_idx.resize(num_sets);
    for(int i = 0; i < num_sets; i++){
        set_idx[i].resize(num_lines_cache/num_sets);
    }
    int lines_division = num_lines_cache/num_sets;
    int aux = 0;
    int set_ = 0;
    for(int i = 0; i < num_lines_cache; i++){
        if(aux == lines_division){
            aux = 0;
            set_++;
        }
        set_idx[set_][aux] = i;
        aux++;
    }

    int count = 0;
    for(int i = 0; i < num_lines_cache; i++){
        for(int j = 0; j < block_size; j++){
            cache[i][j].line = count;
            cache[i][j].block = 0;
            cache[i][j].address = num_block_mp+1;
            cache[i][j].content = 0;
        }
        count++;
    }
    std::cout << "\033[1;35m       ---> Cache Simulator <--- \033[0m\n";
}

void Cache::Cache::directed_mapped(std::string command, int number1, int number2){
    int block_ = (int)(number1/block_size); // discovering block
    int line_ = (block_ % num_lines_cache); // discovering which line number1 will be in
    if(command == "Read"){
        int block_replaced = cache[line_][0].block;
        cache[line_][0].block = block_;
        cache[line_][0].address = number1;
        cache[line_][0].content = 0;
        for(int i = 1; i < block_size; i++){
            cache[line_][i].block = block_;
            cache[line_][i].address = 0;
            cache[line_][i].content = 0;
        }
        std::cout << "\033[1;34m" << "Read " << number1 << " -> MISS -> alocado na linha " << line_ << " -> bloco " << block_replaced << " substituído\n";
    }

    else if(command == "Write"){
        bool aux = 0;
        for(int i = 0; i < block_size; i++){
            if(cache[i][0].block == block_){
                cache[i][0].address = number1;
                cache[i][0].content = number2;
                for(int j = 1; j < block_size; j++){
                    cache[i][j].block = block_;
                    cache[i][j].address = 0;
                    cache[i][j].content = 0;
                }
                std::cout << "\033[1;34m" << "Write " << number1 << " -> HIT linha " << line_ << " -> novo valor do endereço " << number1 << "=" << number2;
                aux = 1;
                break;
            }
        }
        if(aux == 0){
            cache[line_][0].block = block_;
            cache[line_][0].address = number1;
            cache[line_][0].content = number2;
            for(int i = 1; i < block_size; i++){
                cache[line_][i].block = block_;
                cache[line_][i].address = 0;
                cache[line_][i].content = 0;
            }
            frequency_register[line_] += 1;
            std::cout << "\033[1;34m" << "Write " << number1 << " -> MISS -> alocado na linha " << line_ << " -> novo valor do endereço " << number1 << "=" << number2;
        }
    }
}

void Cache::Cache::fully_associative(std::string command, int number1, int number2){
    int block_ = (int)(number1/block_size); // discovering block
    int set_ = (block_ % num_sets); // discovering which line number1 will be in if replac_policy is 2
    if(command == "Read"){
        int aux = 0;
        for(int i = 0; i < num_lines_cache; i++){
            if(cache[i][0].block == block_){
                cache[i][0].address = number1;
                cache[i][0].content = 0;
                for(int j = 1; j < block_size; j++){
                    cache[i][j].block = block_;
                    cache[i][j].address = 0;
                    cache[i][j].content = 0;
                }
                std::cout << "\033[1;34m" << "Read " << number1 << " -> HIT linha " << i;
                aux = 1;
                frequency_register[i] += 1;
                break;
            }
        }
        if(aux == 0){
            for(int i = 0; i < num_lines_cache; i++){
                if(cache[i][0].address == 17){
                    std::cout << "\033[1;34m" << "Read " << number1 << " -> MISS -> alocado na linha " << i << " -> bloco " << cache[i][0].block << " substituído\n";
                    cache[i][0].block = block_;
                    cache[i][0].address = number1;
                    cache[i][0].content = 0;
                    for(int j = 1; j < block_size; j++){
                        cache[i][j].block = block_;
                        cache[i][j].address = 0;
                        cache[i][j].content = 0;
                    }
                    aux = 1;
                    frequency_register[i] += 1;

                    // input_register[set_][i] = (num_lines_cache/num_sets)-1;
                    // for(int j = 0; j < num_lines_cache/num_sets; j++){
                    //     if(j != i){
                    //         input_register[set_][j] -= 1;
                    //     }
                    // }

                    break;
                }
            }
            if(aux == 0){
                if(replac_policy == 1){
                    Cache::randomic(command, number1, number2, block_, 0);
                }
                else if(replac_policy == 2){
                    Cache::FIFO(command, number1, number2, block_, set_);
                }
                else if(replac_policy == 3){
                    Cache::LFU(command, number1, number2, block_);
                }
            }
        }
    }

    else if(command == "Write"){
        bool aux = 0;
        for(int i = 0; i < num_lines_cache; i++){
            if(cache[i][0].block == block_){
                cache[i][0].address = number1;
                cache[i][0].content = number2;
                for(int j = 1; j < block_size; j++){
                    cache[i][j].block = block_;
                    cache[i][j].address = 0;
                    cache[i][j].content = 0;
                }
                std::cout << "\033[1;34m" << "Write " << number1 << " -> HIT linha " << i << " -> novo valor do endereço " << number1 << "=" << number2;
                aux = 1;
                frequency_register[i] += 1;
                break;
            }      
        }

        if(aux == 0){
            for(int i = 0; i < num_lines_cache; i++){
                if(cache[i][0].address == 17){
                    std::cout << "\033[1;34m" << "Write " << number1 << " -> MISS -> alocado na linha " << i << " -> novo valor do endereço " << number1 << "=" << number2;
                    cache[i][0].block = block_;
                    cache[i][0].address = number1;
                    cache[i][0].content = number2;
                    for(int j = 1; j < block_size; j++){
                        cache[i][j].block = block_;
                        cache[i][j].address = 0;
                        cache[i][j].content = 0;
                    }
                    aux = 1;
                    frequency_register[i] += 1;

                    // input_register[set_][i] = (num_lines_cache/num_sets)-1;
                    // for(int j = 0; j < num_lines_cache/num_sets; j++){
                    //     if(j != i){
                    //         input_register[set_][j] -= 1;
                    //     }
                    // }

                    break;
                }
            }
            if(aux == 0){
                if(replac_policy == 1){
                    Cache::randomic(command, number1, number2, block_, 0);
                }
                else if(replac_policy == 2){
                    Cache::FIFO(command, number1, number2, block_, set_);
                }
                else if(replac_policy == 3){
                    Cache::LFU(command, number1, number2, block_);
                }
            }
        }
    }
}

void Cache::Cache::set_associative(std::string command, int number1, int number2){
    int block_ = (int)(number1/block_size); // discovering block
    int set_ = (block_ % num_sets); // discovering which line number1 will be in if replac_policy is 2
    
    if(command == "Read"){
        int aux = 0;
        for(int i = 0; i < num_lines_cache; i++){
            if(cache[i][0].block == block_){
                cache[i][0].address = number1;
                cache[i][0].content = 0;
                for(int j = 1; j < block_size; j++){
                    cache[i][j].block = block_;
                    cache[i][j].address = 0;
                    cache[i][j].content = 0;
                }
                std::cout << "\033[1;34m" << "Read " << number1 << " -> HIT linha " << i;
                aux = 1;
                frequency_register[i] += 1;
                break;   
            }
        }
        if(aux == 0){
            for(int i = 0; i < num_lines_cache; i++){
                bool var = 0;
                for(int a = 0; a < num_lines_cache/num_sets; a++){
                    if(set_idx[set_][a] == i)
                        var = 1;
                }
                if(var == 0)
                    continue;
                if(cache[i][0].address == 17){
                    std::cout << "\033[1;34m" << "Read " << number1 << " -> MISS -> alocado na linha " << i << " -> bloco " << cache[i][0].block << " substituído\n";
                    cache[i][0].block = block_;
                    cache[i][0].address = number1;
                    cache[i][0].content = 0;
                    for(int j = 1; j < block_size; j++){
                        cache[i][j].block = block_;
                        cache[i][j].address = 0;
                        cache[i][j].content = 0;
                    }
                    aux = 1;
                    frequency_register[i] += 1;

                    int column;
                    for(int k = 0; k < num_lines_cache/num_sets; k++){
                        if(set_idx[set_][k] == i){
                            column = k;
                        }
                    }
                    
                    input_register[set_][column] = (num_lines_cache/num_sets)-1;
                    for(int j = 0; j < num_lines_cache/num_sets; j++){
                        if(j != column && input_register[set_][j]-1 >= 0){
                            input_register[set_][j] -= 1;
                        }
                    }

                    break;
                }
            }
            if(aux == 0){
                if(replac_policy == 1){
                    Cache::randomic(command, number1, number2, block_, set_);
                }
                else if(replac_policy == 2){
                    Cache::FIFO(command, number1, number2, block_, set_);
                }
                else if(replac_policy == 3){
                    Cache::LFU(command, number1, number2, block_);
                }
            }
        }
    }

    else if(command == "Write"){
        bool aux = 0;
        for(int i = 0; i < num_lines_cache; i++){
            if(cache[i][0].block == block_){
                cache[i][0].address = number1;
                cache[i][0].content = number2;
                for(int j = 1; j < block_size; j++){
                    cache[i][j].block = block_;
                    cache[i][j].address = 0;
                }
                std::cout << "\033[1;34m" << "Write " << number1 << " -> HIT linha " << i << " -> novo valor do endereço " << number1 << "=" << number2;
                aux = 1;
                frequency_register[i] += 1;
                break;
            }
        }

        if(aux == 0){
            for(int i = 0; i < num_lines_cache; i++){
                bool var = 0;
                for(int a = 0; a < num_lines_cache/num_sets; a++){
                    if(set_idx[set_][a] == i)
                        var = 1;
                }
                if(var == 0)
                    continue;
                if(cache[i][0].address == 17){
                    std::cout << "\033[1;34m" << "Write " << number1 << " -> MISS -> alocado na linha " << i << " -> novo valor do endereço " << number1 << "=" << number2;
                    cache[i][0].block = block_;
                    cache[i][0].address = number1;
                    cache[i][0].content = number2;
                    for(int j = 1; j < block_size; j++){
                        cache[i][j].block = block_;
                        cache[i][j].address = 0;
                        cache[i][j].content = 0;
                    }
                    aux = 1;
                    frequency_register[i] += 1;

                    int column;
                    for(int k = 0; k < num_lines_cache/num_sets; k++){
                        if(set_idx[set_][k] == i){
                            column = k;
                        }
                    }

                    input_register[set_][column] = (num_lines_cache/num_sets)-1;
                    for(int j = 0; j < num_lines_cache/num_sets; j++){
                        if(j != column){
                            input_register[set_][j] -= 1;
                        }
                    }

                    break;
                }
            }
            if(aux == 0){
                if(replac_policy == 1){
                    Cache::randomic(command, number1, number2, block_, set_);
                }
                else if(replac_policy == 2){
                    Cache::FIFO(command, number1, number2, block_, set_);
                }
                else if(replac_policy == 3){
                    Cache::LFU(command, number1, number2, block_);
                }
            }
        }
    }

}

void Cache::Cache::randomic(std::string command, int number1, int number2, int block_, int set_){
    if(mapping_type != 3){
        int i = rand()%num_lines_cache;
        if(command == "Read"){
            std::cout << "\033[1;34m" << "Read " << number1 << " -> MISS -> alocado na linha " << i << " -> bloco " << cache[i][0].block << " substituído\n";
            cache[i][0].block = block_;
            cache[i][0].address = number1;
            cache[i][0].content = 0;
            for(int j = 1; j < block_size; j++){
                cache[i][j].block = block_;
                cache[i][j].address = 0;
                cache[i][j].content = 0;
            }
        }
        else if(command == "Write"){
            std::cout << "\033[1;34m" << "Write " << number1 << " -> MISS -> alocado na linha " << i << " -> novo valor do endereço " << number1 << "=" << number2;
            cache[i][0].block = block_;
            cache[i][0].address = number1;
            cache[i][0].content = number2;
            for(int j = 1; j < block_size; j++){
                cache[i][j].block = block_;
                cache[i][j].address = 0;
                cache[i][j].content = 0;
            }
        }
    }
    else{
        int prev = rand()%num_lines_cache/num_sets;
        int i = input_register[set_][prev];
        if(command == "Read"){
            std::cout << "\033[1;34m" << "Read " << number1 << " -> MISS -> alocado na linha " << i << " -> bloco " << cache[i][0].block << " substituído\n";
            cache[i][0].block = block_;
            cache[i][0].address = number1;
            cache[i][0].content = 0;
            for(int j = 1; j < block_size; j++){
                cache[i][j].block = block_;
                cache[i][j].address = 0;
                cache[i][j].content = 0;
            }
        }
        else if(command == "Write"){
            std::cout << "\033[1;34m" << "Write " << number1 << " -> MISS -> alocado na linha " << i << " -> novo valor do endereço " << number1 << "=" << number2;
            cache[i][0].block = block_;
            cache[i][0].address = number1;
            cache[i][0].content = number2;
            for(int j = 1; j < block_size; j++){
                cache[i][j].block = block_;
                cache[i][j].address = 0;
                cache[i][j].content = 0;
            }
        }
    }
}

void Cache::Cache::FIFO(std::string command, int number1, int number2, int block_, int set_){    
    if(command == "Read"){
        int line;
        for(int i = 0; i < num_lines_cache/num_sets; i++){
            if(input_register[set_][i] == 0){
                line = i;
            }
        }
        std::cout << "\033[1;34m" << "Read " << number1 << " -> MISS -> alocado na linha " << line << " -> bloco " << cache[line][0].block << " substituído\n";
        cache[line][0].block = block_;
        cache[line][0].address = number1;
        cache[line][0].content = 0;
        for(int j = 1; j < block_size; j++){
            cache[line][j].block = block_;
            cache[line][j].address = 0;
            cache[line][j].content = 0;
        }

        int column;
        for(int k = 0; k < num_lines_cache/num_sets; k++){
            if(set_idx[set_][k] == line){
                column = k;
            }
        }

        input_register[set_][column] = (num_lines_cache/num_sets)-1;
        for(int i = 0; i < num_lines_cache/num_sets; i++){
            if(i != column){
                input_register[set_][i] -= 1;
            }
        }
    }

    else if(command == "Write"){
        int line;
        for(int i = 0; i < num_lines_cache/num_sets; i++){
            if(input_register[set_][i] == 0){
                line = i;
            }
        }
        std::cout << "\033[1;34m" << "Write " << number1 << " -> MISS -> alocado na linha " << line << " -> novo valor do endereço " << number1 << "=" << number2;
        cache[line][0].block = block_;
        cache[line][0].address = number1;
        cache[line][0].content = number2;
        for(int j = 1; j < block_size; j++){
            cache[line][j].block = block_;
            cache[line][j].address = 0;
            cache[line][j].content = 0;
        }

        int column;
        for(int k = 0; k < num_lines_cache/num_sets; k++){
            if(set_idx[set_][k] == line){
                column = k;
            }
        }

        input_register[set_][column] = (num_lines_cache/num_sets)-1;
        for(int i = 0; i < num_lines_cache/num_sets; i++){
            if(i != column){
                input_register[set_][i] -= 1;
            }
        }
    }
}

void Cache::Cache::LFU(std::string command, int number1, int number2, int block_){
    if(command == "Read"){
        int smallest = 1000000;
        int line;
        for(int i = 0; i < num_lines_cache; i++){
            if(frequency_register[i] < smallest){
                smallest = frequency_register[i];
                line = i;
            }
        }
        frequency_register[line] = 1;
        std::cout << "\033[1;34m" << "Read " << number1 << " -> MISS -> alocado na linha " << line << " -> bloco " << cache[line][0].block << " substituído\n";
        cache[line][0].block = block_;
        cache[line][0].address = number1;
        cache[line][0].content = 0;
        for(int j = 1; j < block_size; j++){
            cache[line][j].block = block_;
            cache[line][j].address = 0;
            cache[line][j].content = 0;
        }
    }

    else if(command == "Write"){
        int smallest = 1000000;
        int line;
        for(int i = 0; i < num_lines_cache; i++){
            if(frequency_register[i] < smallest){
                smallest = frequency_register[i];
                line = i;
            }
        }
        frequency_register[line] = 1;
        std::cout << "\033[1;34m" << "Write " << number1 << " -> MISS -> alocado na linha " << line << " -> novo valor do endereço " << number1 << "=" << number2;
        cache[line][0].block = block_;
        cache[line][0].address = number1;
        cache[line][0].content = number2;
        for(int j = 1; j < block_size; j++){
            cache[line][j].block = block_;
            cache[line][j].address = 0;
            cache[line][j].content = 0;
        }
    }
}

void Cache::Cache::Read(std::string command, int number1, int number2){
    /*
     First checks if gets hit. Otherwise, see which mapping was choose, 
     and if is empty, see what kind of substitution is
    */
    int block_ = (int)(number1/block_size);
    bool aux = 0;
    for(int i = 0; i < num_lines_cache; i++){
        if(cache[i][0].block == block_){
            cache[i][0].address = number1;
            cache[i][0].content = 0;
            for(int j = 1; j < block_size; j++){
                cache[i][j].block = block_;
                cache[i][j].address = 0;
                cache[i][j].content = 0;
            }
            std::cout << "\033[1;34m" << "Read " << number1 << " -> HIT linha " << i;
            aux = 1;
            frequency_register[i] += 1;
            break;
        }
    }
    if(aux == 0){
        if(mapping_type == 1){
            Cache::directed_mapped(command, number1, number2);
        }
        else if(mapping_type == 2){
            Cache::fully_associative(command, number1, number2);
        }
        else if(mapping_type == 3){
            Cache::set_associative(command, number1, number2);
        }
    }
}

void Cache::Cache::Write(std::string command, int number1, int number2){
    if(mapping_type == 1){
        Cache::directed_mapped(command, number1, number2);
    }
    else if(mapping_type == 2){
        Cache::fully_associative(command, number1, number2);
    }
    else if(mapping_type == 3){
        Cache::set_associative(command, number1, number2);
    }
}

void Cache::Cache::Show(){
    std::cout << "\033[1;34m" << "Show ->" << "\033[0m\n";
    if(mapping_type != 3){
        for(int i = 0; i < num_lines_cache; i++){
            for(int j = 0; j < block_size; j++){
                std::cout << cache[i][j].line << " - " << cache[i][j].block << " - " << cache[i][j].address << " - " << cache[i][j].content;
                std::cout << std::endl;
            }
        }
    }
    else{
        for(int i = 0; i < num_sets; i++){
            std::cout << "\033[1;34mSET " << i << ":\033[0m\n";
            for(int j = 0; j < num_lines_cache/num_sets; j++){
                for(int k = 0; k < block_size; k++){
                    std::cout << cache[set_idx[i][j]][k].line << " - " << cache[set_idx[i][j]][k].block << " - " << cache[set_idx[i][j]][k].address << " - " << cache[set_idx[i][j]][k].content;
                    std::cout << std::endl;
                }
            }
        }
    }
}