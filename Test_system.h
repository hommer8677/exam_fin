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
		do {


		} while (true);
	}


};