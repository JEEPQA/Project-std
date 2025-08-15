#include <iostream>
#include <memory>

//
class Resume
{
protected: //保护成员
    virtual void SetPersonalInfo() {}
    virtual void SetEducation() {}
    virtual void SetWorkExp() {}
public:
    void FillResume()
    {
        SetPersonalInfo();
        SetEducation();
        SetWorkExp();
    }
};

class ResumeA : public Resume
{
protected:
    void SetPersonalInfo()
    {
        std::cout << "A's PersonalInfo" << std::endl;
    }
    void SetEducation()
    {
        std::cout << "A's Education" << std::endl;
    }
    void SetWorkExp()
    {
        std::cout << "A's Work Experience" << std::endl;
    }
};

class ResumeB : public Resume
{
protected:
    void SetPersonalInfo()
    {
        std::cout << "B's PersonalInfo" << std::endl;
    }
    void SetEducation()
    {
        std::cout << "B's Education" << std::endl;
    }
    void SetWorkExp()
    {
        std::cout << "B's Work Experience" << std::endl;
    }
};

int main()
{
    auto r1 = std::make_unique<ResumeA>();
    r1->FillResume();

    auto r2 = std::make_unique<ResumeB>();
    r2->FillResume();

    return 0;
}