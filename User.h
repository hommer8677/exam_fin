#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <string>
#include <cstring>
#include <vector>

using namespace std;
class User
{

	std::string username;
protected:
	 

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
	// Функция для проверки на существование файла.
	bool fileExists(const std::string& filename) {
		std::ifstream file(filename);
		return file.good(); // возвращает true, если файл открылся успешно
	}

	// Функция принимает номер строки и новую строку. Индексация
	// строк идёт от 1 так как мой папик так попросил, и так почеловечнее
	void ChangeFString(int index, string ustr, std::string file) {
		std::cout << std::endl;
		std::vector<std::string> lines;
		std::string currentLine;
		std::ifstream inFile(file);
		while (std::getline(inFile, currentLine)) {
			std::cout << currentLine << std::endl;
			lines.push_back(currentLine);
		}
		inFile.close();
		if (index > 0 && index <= lines.size()) {

			lines[index - 1] = ustr;
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

public:
	User(string usern) {
		username = usern;
		// При создании юзера я создаю директорию.
	}
	User() {
		username = ""; // Хотел сделать None, однако найдётся же дурачьё с ником None
	}
	void setName(string name) {
		username = name;
	}
	

	void start_test() {
		string user_input = "";
		std::cout << "Choose test category:\n";
		string full_path = "Tests\\*";
		_finddata_t* fileinfo = new _finddata_t;
		intptr_t number = _findfirst(full_path.c_str(), fileinfo); // fileinfo - первый в списке.
		// number - номер группы найденных файлов.

		intptr_t number_copy = number;
		while (number_copy != -1) {
			if ((strcmp(fileinfo->name, ".") == 0 or strcmp(fileinfo->name, "..")==0)) {
				number_copy = _findnext(number, fileinfo);
				continue;
			}
			std::cout << fileinfo->name << "\n";
			number_copy = _findnext(number, fileinfo);
			
		}
		std::cin >> user_input; // Принимаем, какую категорию хочет выбрать юзер.
		
		full_path = { full_path.begin(), full_path.end() - 1 };
		full_path = { full_path + user_input + "\\*.*"};
		// Сейчас путь равен Tests\(category)\
		
		if (DirExist(full_path)) {
			string file_name_for_user = "";
			std::cout << "\n";
			_finddata_t* fileinfo = new _finddata_t;
			intptr_t number = _findfirst(full_path.c_str(), fileinfo);
			intptr_t number_copy = number;
			while (number_copy != -1) {
				// . и .. это скрытые файлы VS. Если директория пуста, она всё равно
				// найдёт эти 2 файла.
				if ((strcmp(fileinfo->name, ".") == 0 or strcmp(fileinfo->name, "..") == 0)) {
					number_copy = _findnext(number, fileinfo);
					continue;
				}
				file_name_for_user = { fileinfo->name };
				file_name_for_user = { file_name_for_user.begin(), file_name_for_user.end() - 4 };

				std::cout << file_name_for_user << "\n";
				number_copy = _findnext(number, fileinfo);
			}
			std::cout << "\nChoose test: ";
			_findclose(number);
			delete fileinfo;
			std::cin >> user_input; // Принимаю ввод названия теста.
			full_path = { full_path.begin(), full_path.end() - 3 };
			full_path = { full_path + user_input + ".txt" };
			// Полный путь к файлу готов. Но ещё надо проверить на существование.
			if (fileExists(full_path)) {
				// Теперь надо проверить, проходил ли юзер этот тест.
				std::ifstream test("Users\\" + username + "\\tests.txt", std::ios::in);
				string test_info = "";
				
				int answers = 0;
				int score = 0;
				int counter = 0; // Это нужно для того, чтобы я знал,
				//в какой строчке я меняю информацию.
				bool TestWasBefore = false;
				while (std::getline(test, test_info)) {
					counter++;
					if (strstr(test_info.c_str(), full_path.c_str()) != nullptr) {
						size_t pos = test_info.find(' ');
						std::string twoNums = { test_info.substr(pos + 1) };
						pos = twoNums.find(' ');
						std::string temp = twoNums.substr(0,pos);
						answers = std::stoi(temp);
						temp = twoNums.substr(pos + 1);
						score = std::stoi(temp);
						TestWasBefore = true;
						
					}
				}
				test.close();
				
				if (TestWasBefore) {
					// индексация от 0 так что если тест пройден, то дано
					// 11 ответов.
					if (answers != 12) {
						std::cout << "\nYou started this test before.\n"
							<< "Your score " << score << " / 12\n"
							<< "Amount of answers : " << answers << " / 12";
						std::cout << "\nif you want continue test, write 1\n"
							<< "if you want reset and take the test again, write 2\n "
							<< "if you do not want to take the test, write 0:\n";
					}
					else if (answers == 12) {
						std::cout << "\nYou passed this test before with score " << score << " / 12\n";
						std::cout << "if you want reset and take the test again, write 2\n "
							<< "if you do not want to take the test, write 0:\n";
					}
					std::cin >> user_input;


					if (user_input == "1") {
						// ну тут я ничего не делаю.
					}
					else if (user_input == "2") {
						answers = 0;
						score = 0;
					}
					else {
						return;
					}
				}

				

				std::ifstream testing(full_path, std::ios::in);
				// Начинаем с вопроса, на котором был пользователь.
				std::string question = "";
				std::string answer = "";
			// Я тут подумал - всё равно мы за один цикл можем считать
			// сразу 2 строчки. Так что цикл на 12 элементов.
				std::cout << "\n----------------------------"
					<< "\nIf you want abort testing, write 000\n\n";
				for (int i = 0; i < 12; i++) {
					if (i < answers) {
						std::getline(testing, question);
						std::getline(testing, answer);
						continue;
					}
					std::getline(testing, question);
					std::cout << question << "\nYour answer: ";
					std::cin >> user_input;
					std::getline(testing, answer);
					if (answer == user_input) {
						score++;
					}
					if (user_input == "000") {
						break;
					}
					answers++; 
					

				}
				string fstring = { full_path + " " + to_string(answers) + " " + to_string(score) };
				string routeTOUser = { "Users\\" + username + "\\tests.txt" };
				if (TestWasBefore) {
					ChangeFString(counter, fstring, routeTOUser);
				}

				else{
					std::ofstream file(routeTOUser, std::ios::app);
					file << fstring;
					file.close();
				}

			}
			else {
				std::cout << "\nFile doesn't exist! Press any button to continue...\n";
				system("pause > NUL");
				return;
			}

		}
		else {
			std::cout << "\nCategory doesn't exist! Press any button to continue...\n";
			system("pause > NUL");
			return;
		}
	}

	// Тут я просматриваю балл за все пройденные тесты.
	void get_tests_results() {
		string routeToUser = { "Users\\" + username + "\\tests.txt" };
		std::ifstream file(routeToUser, std::ios::in);

		std::string full_string = "";
		std::string test_name = "";
		std::string answers = "";
		std::string score = "";

		std::cout << "Tests results:\n\n";
		short counter = 0;
		while (std::getline(file, full_string)) {
			size_t pos = full_string.find(' ');
			// я знаю, что название Users + ещё знак это 6 символов. Значит я могу чуток облегчить поиск подстроки с названием теста.
			test_name = full_string.substr(6, pos);
			// не уверен, почему все эти преобразования строки отказывались
			// лезть в 1 строчку, но да ладно.
			test_name = test_name.substr(test_name.find('\\')+1);
			test_name = test_name.substr(0, test_name.find('.'));

			std::string twoNums = { full_string.substr(pos + 1) };
			pos = twoNums.find(' ');
			std::string temp = twoNums.substr(0, pos);
			answers = temp;
			temp = twoNums.substr(pos + 1);
			score = temp;

			std::cout << test_name << ":\nAnswers " << answers << " / 12 and score " << score << " / 12\n";
		}
	}

	void change_password() {
		std::string password;
		std::cout << "Input new password (please, don't input special sybols): ";

		std::getline(std::cin, password);
		for (int i = 0; i < password.length(); i++) {
			password[i] = password[i] + 5;
		}
		std::ofstream file("Users\\" + username + "\\password.txt", std::ios::in);

		file << password;
	}

	void change_login() {

		std::string user_input;
		std::cout << "Input login (without spaces):  ";
		std::getline(std::cin, user_input);
		for (size_t i = 0; i < user_input.length(); i++)
			if (user_input[i] == ' ') user_input[i] = '_';

		if (rename(username.c_str(), user_input.c_str()) != 0) {
			std::cout << "\Error! Press any button to continue...";
			system("pause > NUL");
		}
		else std::cout << "Success" << std::endl;

		username = user_input;
	}

	std::string get_username() const {
		return username;
	}
};
