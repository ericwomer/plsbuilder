#ifndef PRIMER_H
#define PRIMER_H

#include <string>
#include <vector>

namespace rake {

class base
{
public:
    base() {};
	virtual int main(void) = 0;
    virtual int main(int argv, char *argc[]) = 0;
    virtual int main(std::vector<std::string> &params) = 0;
	virtual int size(void) { return sizeof(this); };
	virtual std::string name(void) = 0;
    virtual void help(void) = 0;
    virtual void setAppName(std::string name) = 0;
private:
    std::string app_name;
    std::vector<std::string> app_destription;
};

} // rake
#endif // PRIMER_H
