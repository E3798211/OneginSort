#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <assert.h>

#define POEM "poem.txt"

using namespace std;

/// Reads file and returns all lines.
/**
    \param file_name Name of the file to read.
    \warning In case of mistake returns nullptr.
*/
char* FileRead(const char* file_name);

/// Parses line.
/** Returns array with positions of beginnings of lines.

    \param line_to_parse Line to be broken into lines.
    \warning Array ends with (-1) as terminator.
*/
int* Parser(char* line_to_parse);

/// Sorts array based on the beginnings of lines.
/**
    Returns sorted array with lines' beginnigs' coordinates.

    \param poem Whole poem in one line.
    \param lines_positions Array with beginnings of lines.
*/
int* BeginSort(char* poem, int* lines_positions);

int main(int argc, char* argv[])
{
    //Checking if User wants to choose another file
    char* file_name = nullptr;
    if(argc > 1)
        file_name = argv[1];
    else
        file_name = POEM;

    //Reading from file
    char* poem_in_line = FileRead(file_name);
    //If nothing hive been read
    if(poem_in_line == nullptr)
        return -1;

    //Breaking line into pieces
    int* lines_positions = Parser(poem_in_line);

    //Sort
    int* lines_sorted = BeginSort(poem_in_line, lines_positions);
    /*
    int counter = 0;
    while(lines_positions[counter] != -1){
        cout << lines_positions[counter] << "\t" << poem_in_line[lines_positions[counter]] << endl;
        counter++;
    }
    */
}

//================================================================

char* FileRead(const char* file_name)
{
    FILE *i_file = fopen(file_name, "r");
    if(i_file == nullptr){
        cout << "File not found!" << endl;
        return nullptr;
    }

    fseek(i_file, 0, SEEK_END);
    int file_size = ftell(i_file);

    char* poem_in_line = nullptr;
    try{
        poem_in_line = new char [file_size];
    }catch(bad_alloc){
        cout << "In main.cpp: FileRead(): Bad allocation. Cannot allocate " << file_size << " bytes." << endl;
        return nullptr;
    }

    rewind(i_file);
    int n_chars = fread(poem_in_line, 1, file_size, i_file);

    fclose(i_file);
    return poem_in_line;
}

int* Parser(char* line_to_parse)
{
    //Exceptions
    assert(line_to_parse != nullptr);

    //Running through the line with the poem
    int n_lines = 0;
    int pos_in_line = 0;
    while(line_to_parse[pos_in_line] != '\0'){
        if(line_to_parse[pos_in_line] == '\n')
            n_lines++;
        pos_in_line++;
    }

    //Creating array with lines' coordinates
    int* lines_positions = nullptr;
    try{
        lines_positions = new int [n_lines + 1];
    }catch(bad_alloc){
        cout << "In main.cpp: Parser(): Bad allocation. Cannot allocate " << n_lines << " bytes." << endl;
        return nullptr;
    }

    //Firs line starts immidiatety
    lines_positions[0] = 0;

    //Runnnig through the line again
    pos_in_line = 0;
    int line_beg_num = 1;
    while(line_to_parse[pos_in_line] != '\0'){
        if(line_to_parse[pos_in_line] == '\n'){
            //Remembering position and going for the next line
            lines_positions[line_beg_num] = pos_in_line + 1;
            line_beg_num++;
        }
        pos_in_line++;
    }
    //Array with positions of lines ends with (-1) (used as terminator)
    lines_positions[n_lines] = -1;

    return lines_positions;
}

int* BeginSort(char* poem, int* lines_positions)
{
    //Exceptions
    assert(poem != nullptr);
    assert(lines_positions != nullptr);

    //Sort
}
