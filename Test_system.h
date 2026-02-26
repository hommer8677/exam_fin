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
			return;
		}
	}



	// Метод для самого запуска системы.
	void launch_system() {
		do {


		} while (true);
	}


};