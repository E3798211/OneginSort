#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <assert.h>

#define ERR_WHERE "In " << __FILE__ << ": " << __func__ << "(): " << ex.what()

#if defined (_DEBUG)
    #define DEBUG
#else
    #define DEBUG  if (0) 
#endif

const char POEM[] = "poem.txt";
const int SORRY   = -1;
const int HAPPY   =  0;  // :)

using namespace std;  // нинада

struct Line{
    int lengh = 0;
    char* beg = nullptr;
};


/// Reads file and returns all lines.
/**
    \param file_name Name of the file to read.
    \warning In case of mistake returns nullptr.
*/
char* FileRead(const char* file_name);

/// Gets line from poem.
/**
    \param [in] line_beg Pointer to the beginning of the line.
*/
char* GetPoemLine(char* line_beg);

/// Prints poem.
/**
    \warning Overoaded

    \param [in] lines_positions Pointers to the positions of lines.
    \param [in] label Your comment.
*/
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
    \param [in] line_to_parse Whole poem.
*/
Line* Parse(char* line_to_parse);

//================================================================

int main(int argc, const char* argv[])
{
    //Checking if User wants to choose another file
    const char* file_name = (argc > 1)? argv[1] : POEM;

    //Reading from file
    char* poem_in_line = FileRead(file_name);
    //If nothing hive been read
    if(poem_in_line == nullptr)
        return SORRY;

    Line* lines_positions = Parse(poem_in_line);

    Print(lines_positions, "Before");
    
    qsort(lines_positions, LinesCount(poem_in_line), sizeof(Line), BegComp);
    Print(lines_positions, "After BegComp");

    qsort(lines_positions, LinesCount(poem_in_line), sizeof(Line), EndComp);
    Print(lines_positions, "After EndComp");

    //Free the resources
    
    delete [] poem_in_line;
    poem_in_line = nullptr;
    delete [] lines_positions;
    lines_positions = nullptr;
    
    return HAPPY;  // :)
}

//================================================================

char* FileRead(const char* file_name)
{
    FILE *input = fopen(file_name, "r");
    if(input == nullptr){
        DEBUG cout << "File not found!" << endl;
        return nullptr;
    }

    if(fseek(input, 0, SEEK_END)){
        DEBUG cout << "Can not set last position in " << file_name << endl;
        return nullptr;
    }

    int file_size = ftell(input);
    if(file_size == EOF){
        DEBUG cout << "Can not get pointer's position in " << file_name << endl;
        return nullptr;
    }

    char* poem_in_line = nullptr;
    
    try{
        poem_in_line = new char [file_size];
    }
    catch(const bad_alloc& ex){
        cout << ERR_WHERE << ". Cannot allocate " << file_size << " bytes." << endl;
        return nullptr;
    }

    rewind(input);
    int n_chars = fread(poem_in_line, 1, file_size, input);

    fclose(input);
    return poem_in_line;
}

char* GetPoemLine(char* line_beg)  // Зачем это вообще? Она еще копию строки создает, и эта память потом не освобождается (см. ниже)
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
    }catch(const bad_alloc& ex){  // не надо так слеплять try-catch. See example above
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

int LinesCount(const char* line_to_parse)
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
    Line* line_1 = (Line*)f_line;
    Line* line_2 = (Line*)s_line;

    return strcmp(GetPoemLine(line_1->beg), GetPoemLine(line_2->beg));  // line_1->beg/end должно быть достаточно. Зачем эта GetPoemLine?
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
    lines_positions[0].beg = line_to_parse;

    //Runnnig through the line_to_parse
    int pos_in_line = 0;
    int line_beg_num = 1;
    while(line_to_parse[pos_in_line] != '\0'){
        if(line_to_parse[pos_in_line] == '\n'){
            //Remembering position and going for the next line
            lines_positions[line_beg_num].beg = line_to_parse + pos_in_line + 1;
            lines_positions[line_beg_num - 1].lengh = lines_positions[line_beg_num].beg -
                                                      lines_positions[line_beg_num - 1].beg - 1;
            line_beg_num++;
        }
        pos_in_line++;
    }
    //Last one has wrong lengh
    lines_positions[n_lines - 1].lengh = lines_positions[n_lines].beg - lines_positions[n_lines - 1].beg - 1;
    //Array with positions of lines ends with (nullptr) (used as terminator)
    lines_positions[n_lines].beg = nullptr;

    return lines_positions;
}

void Print(Line* lines_positions, char* label)
{
    //Exceptions
    assert(lines_positions != nullptr);
    assert(label != nullptr);

    cout << "  !" << label << endl;
    int i = 0;
    while(lines_positions[i].beg != nullptr){
        cout << GetPoemLine(lines_positions[i].beg) << endl;
        i++;
    }
}

