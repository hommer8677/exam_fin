#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <cstring>
#include <vector>

#include "User.h"
#include "Admin.h"

class Test_system {
	Admin admin;
	User current_user;

	bool DirExist(string path) {
		_finddata_t* fileinfo = new _finddata_t;
		intptr_t number = _findfirst(path.c_str(), fileinfo);
		intptr_t number_copy = number;
		while (number_copy != -1) {
			// . и .. это скрытые файлы VS. Если директория пуста, она всё равно
			// найдёт эти 2 файла.
			if (fileinfo->name == "." or fileinfo->name == "..") {
				number_copy = _findnext(number, fileinfo);
				continue;
			}
			_findclose(number);
			delete fileinfo;
			number_copy = -1;
			return true; // есть сработка, значит всё, возвращаемся.
			// Впринципе эта часть тоже не нужна. При сработке мы выходим из цикла.
			// А если файлов нет, то на этапе if в 54 строчке после .. 
			// number_copy = -1;
				/*number_copy = _findnext(number, fileinfo);*/
		}
		_findclose(number);
		delete fileinfo;
		return false;
		// если цикл не сработал, то значит директории нет.
	}

public:

	// Конструктора нет, тут он нам не нужен.
	// для начала нужны методы, чтобы зарегестрировать юзера.
	// Уникального юзера.
	void user_registration() {
		std::string login;
		std::cout << "Input login: ";
		std::cin.ignore();
		std::getline(std::cin, login);
		std:string path = { "Users\\" + login };

		if (mkdir(path.c_str()) != 0) {
			std::cout << "Error! User with tihs name already exist!";
			return;
		}
		else {
			// если цикл не сработал, то значит директории нет. А значит регистрируем юзера.
			std::string password;
			std::cout << "Input password: ";
			std::cin.ignore();
			std::getline(std::cin, password); // А пароли должны быть уникальны? Нас не просят об этом, так что не надо делать
			// лишнюю проверку на то, использовался ли этот пароль у кого либо.
			std::ofstream tests(path + "\\tests.txt", std::ios::out);
			std::ofstream password_stream(path + "\\password.txt", std::ios::out);
			tests.close();
			for (int i = 0; i < password.length(); i++) {
				password[i] = password[i] + 5;

			}
			password_stream << password;
			password_stream.close();
			current_user.setName(login);
			return;
		}
	}

	void user_login() {
		std::string login;
		std::cout << "Input login: ";
		std::cin.ignore();
		std::getline(std::cin, login);
		std:string path = { "Users\\" + login };
		if (DirExist(path)) {
			std::cout << "\nInput password (please, don't input special sybols): ";
			std::string real_password;
			std::string password;
			std::cin.ignore();
			std::getline(std::cin, password);
			std::ifstream file(path + "password.txt", std::ios::in);
			file >> real_password;
			for (int i = 0; i < real_password.length(); i++) {
				real_password[i] = real_password[i] - 5;

			}
			if (real_password == password) {
				current_user.setName(login);
				std::cout << "\nSucces!";
			}
		}
	}



	// Метод для самого запуска системы.
	void launch_system() {
		std::string user_input, line;
		do {
			std::cout << "------Main Menu------\n\n";

			/*std::cout << "Enter the number under which the function you need is located:\n"*/
			std::cout << "Which mode you need (input number of option):"
				// Тут я просто чутка пояснил за режимы.
				<< "\n1. User - passing the tests"
				<< "\n2. Administrator - users and tests management"
				<< "\n0. turn off the system ";

			std::cin.ignore();
			std::getline(std::cin, user_input);
			// Из за того, что это был командный проект, 
			// менюшку для юзера и всего юзера делал Игор Салукин,
			// а всего админа и вообще весь проект Аксонов Nван. (Игор списал у него)

			if (user_input == "0") {
				break; // Просто обрубаем лаун систем.
			}
			else if (user_input == "1") {
				// Запускаю режим юзера.
				do {
					system("cls"); // подчищаем консоль, хочу красивенько.
					if (current_user.get_username() == "") {
						std::cout << "1. Register\n2. Log in\n0. Return to main menu";
						std::cin.ignore();
						std::getline(std::cin, user_input);
						if (user_input == "1") {
							user_registration(); // Сами метод найдёте.
						}
						else if (user_input == "2") {
							user_login(); // Сами метод найдёте.
						}
						else if (user_input == "0") break;
						else {
							std::cout << "\nIncorrect input! Press any button to continue...";
							system("pause > NUL");
						}
					}
					// Юзер вошёл в аккаунт
					else {
						std::cout << "*****  " << current_user.get_username() << "  *****\n";
						std::cout << "\nEnter the number under which the function you need is located:\n"
							<< "\n1. Choose test and begin testing"
							<< "\n2. Check tests result"
							<< "\n3. Change login (username)"
							<< "\n4. Change password"
							<< "\n\n0. Log out\n";

						std::cin.ignore();
						std::getline(std::cin, user_input);

						if (user_input == "0") current_user.setName("");
						else if (user_input == "1") {
							current_user.start_test();
						}
						else if (user_input == "2") {
							current_user.get_tests_results();
						}
						else if (user_input == "3") {
							current_user.change_login();
						}
						else if (user_input == "4") {
							current_user.change_password();
						}
						else {
							std::cout << "\nIncorrect input! Press any button to continue...";
							system("pause > NUL");
						}
					}
				} while (true); // конец режимаю юзера


			}

			// Тут пиши меню для админа и блаблабла.
			else if (user_input == "2") {
				std::cin.ignore();
				std::getline(std::cin, user_input);
				code(user_input); \

				std::ifstream file("adm.txt");
				std::getline(file, line);
				if (user_input == line) {
					Admin a;
					int admin_choose;
					//std::string  adm_str, old_name;
					do {
						try {
							std::cout << "---------------------------" << std::endl;
							std::cout << "Choose action: " << std::endl;
							std::cout << "1.Rename file(dir)\n2.Remove file\n3.Search directory\n4.Create directory\n5.Remove directory\n6.Open file\n7.Create file" << std::endl;
							std::cout << "0.Exit" << std::endl;
							if (!(std::cin >> admin_choose)) throw std::invalid_argument("Invalid argument!");
							else if (admin_choose > 7 || admin_choose < 0) throw std::out_of_range("Out of range");
						}
						catch (std::exception& e) {
							std::cout << e.what() << std::endl;
							continue;
							//std::cin.clear();
						}
						switch (admin_choose) {
						case 0: break;
						case 1:
							std::cout << "Old file name: ";
							input(line);
							std::cout << "New file name: ";
							std::getline(std::cin, user_input);
							admin.RenameFile(line, user_input);
							break;
						case 2:
							std::cout << "File name: ";
							input(user_input);
							admin.RemoveFile(user_input);
							break;
						case 3:
							std::cout << "Dir name: ";
							input(user_input);
							admin.SearchDir(user_input);
							break;
						case 4:
							std::cout << "Dir name: ";
							input(user_input);
							admin.CreateDir(user_input);
							break;
						case 5:
							std::cout << "Dir name: ";
							input(user_input);
							admin.RemoveDir(user_input);
							break;
						case 6:
							std::cout << "File name: ";
							input(user_input);
							admin.open_file(user_input);
							break;
						case 7:
							std::cout << "File name: ";
							input(user_input);
							admin.create_file(user_input);
							break;
						default: std::cout << "Incorrect input";
						}
					} while (admin_choose != 0);
				}
				else std::cout << "No succes" << std::endl;
				file.close();
			}

			else {
				std::cout << "\nIncorrect input! Press any button to continue...";
				system("pause > NUL");
			}
			
		} while (true);
	}


};