#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <assert.h>
#include <cctype>

#define ERR_WHERE "In " << __FILE__ << ": " << __func__ << "(): " << ex.what()

const char POEM[] = "poem.txt";
const int SORRY   = -1;

using namespace std;

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

/// Compares lines from the end.
int EndComp(const void* f_line, const void* s_line);

/// Parses line.
/**
    \param [in] line_to_parse Whole poem.
    \param [in] n_lines Amount of lines in a poem
*/
Line* Parse(char* line_to_parse, int n_lines);

//================================================================


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");

    //Checking if User wants to choose another file
    const char* file_name = (argc > 1)? argv[1] : POEM;

    //Reading from file
    char* poem_in_line = FileRead(file_name);
    //If nothing hive been read
    if(poem_in_line == nullptr)
        return SORRY;

    int n_lines = LinesCount(poem_in_line);
    Line* lines_positions = Parse(poem_in_line, n_lines);

    Print(lines_positions, "Before");

    qsort(lines_positions, n_lines, sizeof(Line), BegComp);
    Print(lines_positions, "After BegComp");

    qsort(lines_positions, n_lines, sizeof(Line), EndComp);
    Print(lines_positions, "After EndComp");


    //Free the resourses
    delete [] poem_in_line;
    poem_in_line = nullptr;
    delete [] lines_positions;
    lines_positions = nullptr;
}

//================================================================

char* FileRead(const char* file_name)
{
    FILE *input = fopen(file_name, "r");
    if(input == nullptr){
        /*DEBUG*/ cout << "File not found!" << endl;
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
    Line* line_1 = (Line*)f_line;
    Line* line_2 = (Line*)s_line;

    return strcmp(line_1->beg, line_2->beg);
}

int EndComp(const void* f_line, const void* s_line)
{
    Line* line_1 = (Line*)f_line;
    Line* line_2 = (Line*)s_line;

    char* ptr_1 = line_1->beg + line_1->lengh - 1;
    char* ptr_2 = line_2->beg + line_2->lengh - 1;
    while(ptr_1 != line_1->beg && ptr_2 != line_2->beg){
        //Skipping punctuation
        if(ispunct(*ptr_1))     ptr_1--;
        if(ispunct(*ptr_2))     ptr_2--;

        if     ((*ptr_1 - *ptr_2) > 0)          return 1;
        else if((*ptr_1 - *ptr_2) < 0)          return -1;

        ptr_1--;
        ptr_2--;
    }
    //If we're here, lines are similar
    if     (line_1->lengh < line_2->lengh)      return 1;
    else if(line_1->lengh > line_2->lengh)      return -1;
    else                                        return 0;
}

Line* Parse(char* line_to_parse, int n_lines)
{
    //Exceptions
    assert(line_to_parse != nullptr);

    //int n_lines = LinesCount(line_to_parse);

    //Creating array with lines' coordinates
    Line* lines_positions = nullptr;
    try{
        lines_positions = new Line [n_lines + 1];
    }
    catch(const bad_alloc& ex){
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
            //Ending line
            line_to_parse[pos_in_line] = '\0';
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
        cout << lines_positions[i].beg << endl;
        i++;
    }
}


