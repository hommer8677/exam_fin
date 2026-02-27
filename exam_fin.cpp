#define _CRT_SECURE_NO_WARNINGS
#include "Admin.h"

#include "Test_system.h"
int main()
{
    /*Admin a;
    int choose;
    std::string us;
    while (true) {
        std::cout << "1.cd \n2.pwd" << std::endl;
        std::cin >> choose;
        if (choose == 1) {
            std::cin.ignore();
            std::getline(std::cin, us);
            a.cd(us.c_str());
        }
        else a.pwd();

        std::cout << "322" << std::endl;;
        std::cout << "323";
    }*/
    Test_system s;
    s.launch_system();
}

