#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <iostream>
#include <math.h>

#define ANSI_RESET_ALL     "\x1b[0m"

#define COLOR_RED          "\x1b[31m"
#define COLOR_GREEN        "\x1b[32m"
#define COLOR_YELLOW       "\x1b[33m"
#define COLOR_MAGENTA      "\x1b[35m"

#define MAX_QUESTIONS 5
#define MAX_ANSWERS 8

typedef struct Answer {
	char answer[256];
	int isRight;
};
typedef struct Question {
	char name[256];
	Answer answers[MAX_ANSWERS];
};

Question questions[MAX_QUESTIONS] =
{
	{
		"Выберите все циклы языка Си",
		{
			{"for", 1},
			{"repeat", 0},
			{"foreach", 0},
			{"while", 1},
			{"do ... wile", 1},
			{"while ... do", 0}
		}
	},
	{
		"Выберите 32-битные типы данных",
		{
			{"short", 0},
			{"int", 1},
			{"char", 0},
			{"long", 1},
			{"float", 1},
			{"double", 0}
		}
	},
	{
		"Как называется операция: i++",
		{
			{"Преинкремент", 0},
			{"Постинкремен", 1},
			{"Предекремент", 0},
			{"Преинкремент", 0}
		}
	},
	{
		"Что будет выведено на экран в результате выполнения: \nint i = 1;\n--i ? printf(\"1\") : printf(\"0\");",
		{
			{"Error", 0},
			{"1", 0},
			{"0", 1},
			{"NaN", 0}
		}
	},
	{
		"Что будет выведено на экран в результате выполнения: \nint a = 3, b = 4, c = 5;\n--a += b = c += a -= ++++b;\nprintf(\"%d, %d, %d\", a, b, c);",
		{
			{"-3 1 1", 1},
			{"0 0 0", 0},
			{"5 4 3", 0},
			{"-1 2 2", 0}
		}
	}
};

int compareToStr(char* p1, char* p2)
{
	for (int i = 0; i < sizeof(p1); i++)
	{
		if (p1[i] == '\0' || p2[i] == '\0') break;
		if (p1[i] != p2[i]) return 0;
	}
	return 1;
}

int checkAnswers(int* userAns, Answer* answer)
{
	for (int i = 0; i < MAX_ANSWERS; i++)
	{
		if (userAns[i] != 0)
			if (answer[userAns[i] - 1].isRight == 0) return 0;

		if (answer[i].isRight)
		{
			int exist = 0;
			for (int j = 0; j < MAX_ANSWERS; j++)
				if (i + 1 == userAns[j])
				{
					exist = 1;
					break;
				}
			if (!exist) return 0;
		}
	}
	return 1;
}
// Функция для получения валидного ответа
int getAnswer(int* answers, int maxAns)
{
	long long int _answers;
	int isValid = 1;

	printf("Введите подряд номера ответов: \n");
	do
	{
		printf(">"); scanf("%lld", &_answers);
		while (getchar() != '\n');
		isValid = _answers < (int)pow(10, maxAns) ? 1 : 0;
		if (isValid)
		{
			for (int i = 0; i < maxAns + 1; i++)
			{
				int r = int(_answers % (int)pow(10, i + 1)) / pow(10, i);
				if (r < 1 or r > maxAns)
				{
					isValid = 0;
					break;
				}
				answers[i] = r;
				if (pow(10, i) - 1 < _answers && _answers < pow(10, i + 1))
					break;
			}
		}
	} while (!isValid);
	return 1;
}

int startTest(int isAdmin, int attempts) // тип пользователя и кол-во попыток
{
	printf(COLOR_MAGENTA "\t ТЕСТ НАЧАЛСЯ! УСПЕХОВ!" ANSI_RESET_ALL "\n");
	int rait = 0;
	for (int iq = 0; iq < MAX_QUESTIONS; iq++)
	{
		rait += attempts;
		int qRait = attempts, // кол-во баллов за вопрос
			nAns = 0; // кол-во ответов

		if (questions[iq].name[0] == '\0') break;
		printf(COLOR_YELLOW "\n\t%s" ANSI_RESET_ALL "\n", questions[iq].name);



		for (int ia = 0; ia < MAX_ANSWERS; ia++)
		{
			if (questions[iq].answers[ia].answer[0] == '\0') break;
			nAns = ia;
			if (isAdmin)
				if (questions[iq].answers[ia].isRight) printf(COLOR_GREEN);
				else printf(COLOR_RED);
			printf("%d) %s \n" ANSI_RESET_ALL, ia + 1, questions[iq].answers[ia].answer);
		} // Печать ответов и определение их кол-ва
		nAns++;

		for (int i = attempts; i != 0; i--)
		{
			int answers[MAX_ANSWERS] = { 0 };
			getAnswer(answers, nAns);

			if (checkAnswers(answers, questions[iq].answers))
			{
				printf("ВЕРНО!\n");
				break;
			}
			else
			{
				printf("НЕПРАВИЛЬНО! Осталось попыток: %d\n", i - 1);
				rait--;
			}
		}
	}

	return rait;
}

int authUser()
{
	const int ADMIN_PIN = 5812;
	int isAdmin = 0;

	char userEnter[8];

chooseUserType: printf(COLOR_YELLOW "\tВыберите режим" ANSI_RESET_ALL "\n[0] user\n[1] admin\n");
	printf(">"); scanf("%s", userEnter);	while (getchar() != '\n');


	if (compareToStr(userEnter, (char*)"0") or compareToStr(userEnter, (char*)"user")) isAdmin = 0;
	else if (compareToStr(userEnter, (char*)"1") or compareToStr(userEnter, (char*)"admin"))
	{
		printf("(Чтобы вернуться введите \"-1\")\n" COLOR_YELLOW "Введите пин-код администратора:" ANSI_RESET_ALL "\n");
		int pin = 0;
		do
		{
			printf(">"); scanf("%d", &pin); while (getchar() != '\n');
			if (pin == -1) goto chooseUserType;
		} while (pin != ADMIN_PIN);
		isAdmin = 1;
	}
	else goto chooseUserType;

	return isAdmin;
}

int main()
{
	setlocale(0, "Russian");

	int rait = startTest(authUser(), 3);
	printf(COLOR_MAGENTA "Набранный балл: %d" ANSI_RESET_ALL "\n", rait);
}