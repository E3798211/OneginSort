#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstring>
#include <assert.h>

const char POEM[] = "poem.txt";
const int  SORRY  = -1;  // See below :)

using namespace std;

/// Reads file and returns all lines.
/**
    \param file_name Name of the file to read.
    \warning In case of mistake returns nullptr.
*/
char* FileRead(const char* file_name);

/// Parses line.
/**
    Returns array with positions of beginnings of lines.

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
void BeginSort(char* poem, int* lines_positions);

/// BinSearch.
/**
    \param [in, out] arr_sorted Sorted part of array.
    \param [in] lines_positions Posotions of beginnings of lines.
    \param [in] start_pos Start position.
    \param [in] end_pos Last position.
    \param [in] curr_line_value Value of current line.
*/
int  BinSearch(char* arr_sorted, int* lines_positions, int start_pos, int end_pos, char* curr_line_value);

/// Gets line from poem.
/**
    \param [in] poem Poem in line.
    \param [in] lines_positions Positions of beginnings of lines.
    \param [in] line_pos Number of line to get.
*/
char* GetPoemLine(char* poem, int* lines_positions, int line_pos);

/// Inserts element in array.
/**
    Places element between start_pos and end_pos.

    \param [in, out] arr_to_ins_in Array where new element inserts
    \param [in] start_pos First position of insert area.
    \param [in] end_pos Last position of insert area.
    \param [in] new_elem_pos New element's position.
    \param [in] new_elem_value Value of element to insert.
*/
void ElemInsert(int* arr_to_ins_in, int start_pos, int end_pos,
                int new_elem_pos, int new_elem_value);

/// Prints poem.
/**
    \param [in] poem Poem to print.
    \param [in] lines_positions Array with beginnings of lines.
    \param [in] label Extra information.
*/
void Printer(char* poem, int* lines_positions, char* label = "Array");

//================================================================

int main(int argc, char* argv[])
{
    //Checking if User wants to choose another file
    char* file_name = nullptr;
    if(argc > 1)
        file_name = argv[1];
    else
        file_name = POEM;  // Кстати, можно в этом случае со stdin читать.

    //Reading from file
    char* poem_in_line = FileRead(file_name);  // Советую в функцию подавать файл уже открытым, тогда можно будет пропихнуть stdin.
    //If nothing hive been read
    if(poem_in_line == nullptr)
        return SORRY;  // :)

    //Breaking line into pieces
    int* lines_positions = Parser(poem_in_line);

    //Array before  // Тот случай, когда комментарий не нужен :)
    Printer(poem_in_line, lines_positions, "Before");
    //Sort          // То же)
    BeginSort(poem_in_line, lines_positions);
    //Array after   // То же)
    Printer(poem_in_line, lines_positions, "After");

    // Free the resources (было Preserve - это "сохранять")
    delete [] poem_in_line;
    poem_in_line = nullptr;
    delete [] lines_positions;
    lines_positions = nullptr;
    
    return 0;
}

//================================================================

char* FileRead(const char* file_name)
{
    FILE *i_file = fopen(file_name, "r");  // А почему i_? Input? М.б. и назвать input?
    if(i_file == nullptr){
        cout << "File not found!" << endl;
        return nullptr;
    }

    fseek(i_file, 0, SEEK_END);     // Проверяй возвр. значение. Мало ли, вызов сбился
    int file_size = ftell(i_file);  // Он может быть и EOF в случае ошибки

    char* poem_in_line = nullptr;
    
    try{
        poem_in_line = new char [file_size];
    }
    
    catch(const bad_alloc& ex){
        cout << "In " << __FILE__ << ": " << __func__ << "(): " << ex.what() << ". Cannot allocate " << file_size << " bytes." << endl;
        //   \-------------это можно завернуть в дефайн-----------------------/
        return nullptr;
    }

    rewind(i_file);
    int n_chars = fread(poem_in_line, 1, file_size, i_file);

    fclose(i_file);
    return poem_in_line;
}

int* Parse(char* line_to_parse)
{
    //Exceptions
    assert(line_to_parse != nullptr);

    //Running through the line with the poem  -- это чтобы строчки посчитать? Сделай функцию
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
    }
    
    catch(bad_alloc){  // see above
        cout << "In main.cpp: Parser(): Bad allocation. Cannot allocate " << n_lines << " bytes." << endl;
        return nullptr;
    }

    //Firs line starts immidiately
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
    
    //Array with positions of lines ends with (-1) (used as Terminator. I'll be back! Yours, Arny Schwartz....)  :)
    lines_positions[n_lines] = -1;

    return lines_positions;
}

void BeginSort(char* poem, int* lines_positions)  // Сделай еще и через qsort (стд. ф-я). В одну строку будет вызов
{
    //Exceptions
    assert(poem != nullptr);
    assert(lines_positions != nullptr);

    //Sort (InsertBinSort)
    //Running through array with positions untill it ends
    int current_line_pos = 1;
    while(lines_positions[current_line_pos] != (-1)){
        //Getting line's value
        char* current_line_value = GetPoemLine(poem, lines_positions, current_line_pos);

        //Finding new position
        int new_line_pos = BinSearch(poem, lines_positions, 0, current_line_pos, current_line_value);
        //Inserting line
        ElemInsert(lines_positions, 0, current_line_pos, new_line_pos, lines_positions[current_line_pos]);

        //Going for next element
        current_line_pos++;
    }
}

int  BinSearch(char* arr_sorted, int* lines_positions, int start_pos, int end_pos, char* curr_line_value)
{
    //Exceptions
    assert(arr_sorted != nullptr);
    assert(lines_positions != nullptr);
    assert(curr_line_value != nullptr);
    assert(end_pos >= start_pos);
    assert(start_pos >= 0);
    assert(end_pos >= 0);

    while(start_pos != end_pos){
        
        assert (start_pos < end_pos); // THIS IS <s>SPARTA!!!</s> LOOP INVARIANT
            
        int middle_pos = (start_pos + end_pos)/2;
        char* middle_line = GetPoemLine(arr_sorted, lines_positions, middle_pos);

        if(strcmp(curr_line_value, middle_line) > 0)
            start_pos = middle_pos + 1;
        else
            end_pos = middle_pos;
    }
    return start_pos;
}

char* GetPoemLine(char* poem, int* lines_positions, int line_pos)  // Что-то выше уже подобное было
{
    //Exceptions
    assert(poem != nullptr);
    assert(lines_positions != nullptr);

    //Counting line's length
    int line_len = 0;
    while(poem[lines_positions[line_pos] + line_len] != '\n' &&
          poem[lines_positions[line_pos] + line_len] != '\0')
        line_len++;

    char* line = nullptr;
    
    try{
        line = new char [line_len + 1];
    }
    
    catch(bad_alloc){
        cout << "In main.cpp: GetPoemLine(): Bad allocation. Cannot allocate " << line_len << " bytes." << endl;
        return nullptr;
    }

    //Creating the line
    
    line_len = 0;
    while(poem[lines_positions[line_pos] + line_len] != '\n' &&
          poem[lines_positions[line_pos] + line_len] != '\0'){
        line[line_len] = poem[lines_positions[line_pos] + line_len];
        line_len++;
    }
    
    //Last character is '\0'
    line[line_len] = '\0';

    return line;
}

void ElemInsert(int* arr_to_ins_in, int start_pos, int end_pos,
                int new_elem_pos, int new_elem_value)
{
    //Exceptions
    assert(arr_to_ins_in != nullptr);
    assert(start_pos >= 0);
    assert(end_pos >= 0);
    assert(end_pos >= start_pos);
    assert(new_elem_pos >= start_pos);
    assert(new_elem_pos <=  end_pos);

    for(int i = end_pos; i > new_elem_pos; i--)
        arr_to_ins_in[i] = arr_to_ins_in[i - 1];
    arr_to_ins_in[new_elem_pos] = new_elem_value;
}

void Print(char* poem, int* lines_positions, char* label)
{
    //Exceptions
    assert(poem != nullptr);
    assert(lines_positions != nullptr);
    assert(label != nullptr);

    cout << "  !" << label << endl;
    int i = 0;
    while(lines_positions[i] != (-1)){
        cout << GetPoemLine(poem, lines_positions, i) << endl;
        i++;
    }
}
