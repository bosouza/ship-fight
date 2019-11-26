#ifndef CSV_READER_H_DEFINED
#define CSV_READER_H_DEFINED
#include <string>
#include <timer.h>

class csv_reader
{
private:
public:
    csv_reader(std::string);
    int get(unsigned int line, unsigned int column);
    int getXY(unsigned int x, unsigned int y);
    ~csv_reader();
};
#endif