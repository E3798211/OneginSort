#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <assert.h>

#define ERR_WHERE "In " << __FILE__ << ": " << __func__ << "(): " << ex.what()

const char POEM[] = "poem.txt";
const int SORRY   = -1;

using namespace std;

struct Line{
    int lengh = 0;
    char* line_beg = nullptr;
};


/// Reads file and returns all lines.
/**
    \param file_name Name of the file to read.
    \warning In case of mistake returns nullptr.
*/
char* FileRead(const char* file_name);

/// Gets line from poem.
char* GetPoemLine(char* line_beg);

/// Prints poem.
/**
    \warning Overoaded

    \param [in] lines_positions Pointers to the positions of lines.
    \param [in] label Your comment.
*/
void Print(char** lines_positions, char* label = "Array");
void Print(Line* lines_positions, char* label = "Array");

/// Counts amount of lines in poem.
/**
    \param [in] line_to_parse Poem written in one line.
*/
int LinesCount(char* line_to_parse);

/// Compares lines from the beginning.
int BegComp(const void* f_line, const void* s_line);

/// Parses line.
/**
    Returns array with pointers to beginnings of the lines,

    \param [in] line_to_parse Line to be broken into lines.
*/
char** Parse_v2(char* line_to_parse);

/// Parses line.
Line* Parse(char* line_to_parse);

//================================================================


int main(int argc, char* argv[])
{
    //Checking if User wants to choose another file
    const char* file_name = nullptr;
    if(argc > 1)
        file_name = argv[1];
    else
        file_name = POEM;

    //Reading from file
    char* poem_in_line = FileRead(file_name);
    //If nothing hive been read
    if(poem_in_line == nullptr)
        return SORRY;

    /*
    char** lines_positions = Parse_v2(poem_in_line);

    Print(lines_positions, "Before");
    qsort(lines_positions, LinesCount(poem_in_line), sizeof(char*), BegComp);
    Print(lines_positions, "After");
    */

    Line* lines_positions = Parse(poem_in_line);

    Print(lines_positions, "Before");

    //Free the resourses
    delete [] poem_in_line;
    poem_in_line = nullptr;
    //delete [] lines_positions;
    //lines_positions = nullptr;
}

//================================================================

char* FileRead(const char* file_name)
{
    FILE *input = fopen(file_name, "r");
    if(input == nullptr){
        cout << "File not found!" << endl;
        return nullptr;
    }

    if(fseek(input, 0, SEEK_END)){
        cout << "Can not set last position in " << file_name << endl;
        return nullptr;
    }

    int file_size = ftell(input);
    if(file_size == EOF){
        cout << "Can not get pointer's position in " << file_name << endl;
        return nullptr;
    }

    char* poem_in_line = nullptr;
    try{
        poem_in_line = new char [file_size];
    }catch(const bad_alloc& ex){
        cout << ERR_WHERE << ". Cannot allocate " << file_size << " bytes." << endl;
        return nullptr;
    }

    rewind(input);
    int n_chars = fread(poem_in_line, 1, file_size, input);

    fclose(input);
    return poem_in_line;
}

char* GetPoemLine(char* line_beg)
{
    //Exceptions
    assert(line_beg != nullptr);

    //Counting line's length
    int line_len = 0;
    while(*(line_beg + line_len) != '\n' &&
          *(line_beg + line_len) != '\0')
        line_len++;

    char* line = nullptr;
    try{
        line = new char [line_len + 1];
    }catch(const bad_alloc& ex){
        cout << ERR_WHERE << ". Cannot allocate " << line_len + 1 << " bytes." << endl;
        return nullptr;
    }

    //Creating line
    line_len = 0;
    while(*(line_beg + line_len) != '\n' &&
          *(line_beg + line_len) != '\0'){
        line[line_len] = *(line_beg + line_len);
        line_len++;
    }
    //Last character is '\0'
    line[line_len] = '\0';

    return line;
}

void Print(char** lines_positions, char* label)
{
    //Exceptions
    assert(lines_positions != nullptr);
    assert(label != nullptr);

    cout << "  !" << label << endl;
    int i = 0;
    while(lines_positions[i] != nullptr){
        cout << GetPoemLine(lines_positions[i]) << endl;
        i++;
    }
}

int LinesCount(char* line_to_parse)
{
    int n_lines = 0;
    int pos_in_line = 0;
    while(line_to_parse[pos_in_line] != '\0'){
        if(line_to_parse[pos_in_line] == '\n')
            n_lines++;
        pos_in_line++;
    }
    return n_lines;
}

int BegComp(const void* f_line, const void* s_line)
{
    char* line_1 = GetPoemLine(*(char**)f_line);
    char* line_2 = GetPoemLine(*(char**)s_line);

    if(strcmp(line_1, line_2) < 0)      return -1;
    if(strcmp(line_1, line_2) > 0)      return 1;
    return 0;
}

char** Parse_v2(char* line_to_parse)
{
    //Exceptions
    assert(line_to_parse != nullptr);

    int n_lines = LinesCount(line_to_parse);

    //Creating array with lines' coordinates
    char** lines_positions = nullptr;
    try{
        lines_positions = new char* [n_lines + 1];
    }catch(const bad_alloc& ex){
        cout << ERR_WHERE << ". Cannot allocate " << n_lines + 1 << " bytes." << endl;
        return nullptr;
    }

    //First line starts immidiately
    lines_positions[0] = line_to_parse;

    //Runnnig through the line_to_parse
    int pos_in_line = 0;
    int line_beg_num = 1;
    while(line_to_parse[pos_in_line] != '\0'){
        if(line_to_parse[pos_in_line] == '\n'){
            //Remembering position and going for the next line
            lines_positions[line_beg_num] = line_to_parse + pos_in_line + 1;
            line_beg_num++;
        }
        pos_in_line++;
    }
    //Array with positions of lines ends with (-1) (used as terminator)
    lines_positions[n_lines] = nullptr;

    return lines_positions;
}

Line* Parse(char* line_to_parse)
{
    //Exceptions
    assert(line_to_parse != nullptr);

    int n_lines = LinesCount(line_to_parse);

    //Creating array with lines' coordinates
    Line* lines_positions = nullptr;
    try{
        lines_positions = new Line [n_lines + 1];
    }catch(const bad_alloc& ex){
        cout << ERR_WHERE << ". Cannot allocate " << n_lines + 1 << " bytes." << endl;
        return nullptr;
    }

    //First line starts immidiately
    lines_positions[0].line_beg = line_to_parse;

    //Runnnig through the line_to_parse
    int pos_in_line = 0;
    int line_beg_num = 1;
    while(line_to_parse[pos_in_line] != '\0'){
        if(line_to_parse[pos_in_line] == '\n'){
            //Remembering position and going for the next line
            lines_positions[line_beg_num].line_beg = line_to_parse + pos_in_line + 1;
            lines_positions[line_beg_num].lengh = lines_positions[line_beg_num].line_beg -
                                                  lines_positions[line_beg_num - 1].line_beg;
            line_beg_num++;
        }
        pos_in_line++;
    }
    //First line didn't have length
    lines_positions[0].lengh = lines_positions[1].line_beg - lines_positions[0].line_beg - 1;
    //Array with positions of lines ends with (nullptr) (used as terminator)
    lines_positions[n_lines].line_beg = nullptr;

    return lines_positions;
}

void Print(Line* lines_positions, char* label)
{
    //Exceptions
    assert(lines_positions != nullptr);
    assert(label != nullptr);

    cout << "  !" << label << endl;
    int i = 0;
    while(lines_positions[i].line_beg != nullptr){
        cout << GetPoemLine(lines_positions[i].line_beg) << endl;
        cout << lines_positions[i].lengh << endl;
        i++;
    }
}

