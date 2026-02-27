//C++17 +

#pragma once
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

namespace fl = std::filesystem;

class Admin {
private:
    std::string path;

    bool fileExists(const std::string& filename) {        //проверка на существование файла
        std::ifstream file(filename);
        return file.is_open();
    }
public:
    Admin() : path{ fl::current_path().string() += '\\'} {}
    Admin(std::string& path): path{ path } {}

    void RenameFile(std::string oldName, std::string newName) {
        if (rename(oldName.c_str(), newName.c_str()) != 0) std::cout << "Error" << std::endl;
        else std::cout << "Success" << std::endl;
    }

    void RemoveFile(std::string file) {
        if (remove(file.c_str()) != 0) std::cout << "Error" << std::endl;
        else std::cout << "Success" << std::endl;
    }

    void SearchDir(std::string path) {
        _finddata_t fileinfo;

        std::cout << std::endl;

        // Проверяем, заканчивается ли путь на маску. Если нет — добавляем \*
        std::string full_path = path;
        if (full_path.back() != '*' && full_path.find('*') == std::string::npos) {
            if (full_path.back() != '\\' && full_path.back() != '/') {
                full_path += "\\";
            }
            full_path += "*"; // Находит всё (и файлы, и папки)
        }
        intptr_t handle = _findfirst(full_path.c_str(), &fileinfo);

        if (handle == -1L) {
            std::cout << "Path: " << full_path << " | Not found." << std::endl;
            return;
        }

        int counter = 0;
        do {
            if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
                std::cout << fileinfo.name << (fileinfo.attrib & _A_SUBDIR ? " [DIR]" : "") << std::endl;
                counter++;
            }
        } while (_findnext(handle, &fileinfo) == 0);

        _findclose(handle);
        std::cout << "Total items: " << counter << std::endl;
    }

    void CreateDir(std::string Name) {
        if (_mkdir(Name.c_str()) == -1) std::cout << "Error" << std::endl;
        else std::cout << "Success" << std::endl;
    }
    void RemoveDir(std::string Name) {
        if (_rmdir(Name.c_str()) == -1) std::cout << "Error" << std::endl;
        else std::cout << "Success" << std::endl;
    }

    void create_file(std::string file_name) {
        if (!fileExists(file_name)) {
            std::ofstream MyFile(file_name);
            if (MyFile.is_open()) MyFile.close();
            else {
                std::cout << "Error file name" << std::endl;
                return;
            }
        }
    }

    void open_file(std::string file) {
        //std::fstream file_adm(file, std::ios::in | std::ios::out | std::ios::app);
        file = path += file;
        if (!fileExists(file)) {
            std::ofstream MyFile(file);
            if (MyFile.is_open()) MyFile.close();
            else {
                std::cout << "Error file name" << std::endl;
                return;
            }
        }

        int choose;
        std::string line;
        do {
            std::cout << "Choose action: " << std::endl;
            std::cout << "1. Read file\n2. Write file\n3. Append info\n4. Edit line\n0. Exit" << std::endl;
            std::cin >> choose;

            if (choose == 1) {                        //на switch жаловался почему то
                std::ifstream fileIn(file, std::ios::in);
                std::cout << std::endl;
                if (fileIn.is_open()) {
                    while (std::getline(fileIn, line)) {
                        std::cout << line << std::endl;
                    }
                    fileIn.close();
                }
                else std::cout << "Error" << std::endl;
                std::cout << std::endl;
            }
            else if (choose == 2) {
                if (file == "adm.txt") continue;
                std::ofstream fileOut(file);

                if (fileOut.is_open()) {
                    std::cin.ignore();
                    std::getline(std::cin, line);
                    fileOut << line << std::endl;
                    fileOut.close();
                }
                else std::cout << "Error" << std::endl;

            }
            else if (choose == 3) {
                if (file == "adm.txt") continue;

                std::ofstream fileApp(file, std::ios::app);

                if (fileApp.is_open()) {
                    std::cin.ignore();
                    std::getline(std::cin, line);
                    fileApp << line << std::endl;
                    fileApp.close();
                }
                else std::cout << "Error" << std::endl;
            }
            else if (choose == 4) {
                std::cout << std::endl;
                std::vector<std::string> lines;
                std::string currentLine;
                choose = 1; //как счетчик
                std::ifstream inFile(file);
                while (std::getline(inFile, currentLine)) {
                    std::cout << choose++ << "| ";
                    std::cout << currentLine << std::endl;
                    lines.push_back(currentLine);
                }
                std::cout << std::endl;
                inFile.close();
                std::cout << "Input index: ";
                std::cin >> choose;
                if (choose > 0 && choose <= lines.size()) {
                    std::cout << "New string: ";
                    std::cin.ignore();
                    std::getline(std::cin, line);

                    lines[choose - 1] = line;
                }
                else {
                    std::cout << "Invalid index!" << std::endl;
                }

                // Перезапись файлa
                std::ofstream outFile(file, std::ios::trunc);
                for (const auto& l : lines) {
                    outFile << l << "\n";
                }
                outFile.close();

            }
            else if (choose == 0) {}
            else std::cout << "Incorrect input!" << std::endl;
        } while (choose != 0);

    }

    void cd(const char* path) {
        if (strcmp(path,"..") == 0) {
            size_t last_slash = this->path.find_last_of("\\");
            this->path.erase(last_slash);
            return;
        }
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            this->path += path;
            this->path += '\\';
        }
        else std::cout << "Incorrect path" << std::endl;
    }
    void pwd() { std::cout << path << std::endl; }

    float average_in_the_category(std::string user_name, const char* category) {
        if (fl::exists(user_name.c_str())) {
            cd(user_name.c_str());
            cd("Tests.txt");
            int counter = 0, bals = 0; //кол-во пройденных тестов , сумма оценок
            
            std::ifstream test(path);
            std::string line;

            if (test.is_open()) {
                while (std::getline(test, line)) {
                    counter++;
                    if (strstr(line.c_str(), path.c_str()) != nullptr) {  //чтение оценки(реверс->чтение до пробела->реверс)
                        std::reverse(line.begin(), line.end());
                        size_t first_space = line.find(' ');
                        line = { line.begin(), line.begin() + first_space };
                        std::reverse(line.begin(), line.end());
                        bals += int(line.c_str());
                    }
                }
                test.close();
            }
            cd("..");
            cd("..");
            return (static_cast<float>(bals)/ static_cast<float>(counter));
        }
        else return -1;
            
    }
};

