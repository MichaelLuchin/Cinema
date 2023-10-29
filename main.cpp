#include <iostream>
#include <string>
#include <regex>

//0-FREE PLACE

int isHave(bool**, int, int, int);

bool** HallMtxCreate(int, int);

bool** HallMtxCreate(int, int*);



int main()
{
    int n, m;
    std::cin >> n >> m;
    bool** arr = new bool* [n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = new bool[m];
        for (int j = 0; j < m; j++)
        {
            std::cin >> arr[i][j];
        }
    }
    int need;
    std::cin >> need;
    std::cout << isHave(arr, n, m, need);
    for (int i = 0; i < n; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
    return 0;
}

int isHave(bool** arr, int n, int m, int need)
{
    int max = 0, seriya = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (!arr[i][j]) seriya++;
            else
            {
                max = max < seriya ? seriya : max;
                seriya = 0;
            }
        }
        max = max < seriya ? seriya : max;
        if (max >= need) return i + 1;
        seriya = 0;
    }

    return 0;
}


bool** HallMtxCreate(int n, int m)
{
    bool** arr = new bool* [n];
    for (int i = 0; i < n; i++)
    {
        arr[i] = new bool[m];
        for (int j = 0; j < m; j++)
        {
            arr[i][j] = 0;
        }
    }
    return arr;
}

bool** HallMtxCreate(int rows_count, int* in_row_place)
{
    bool** hall = new bool* [rows_count];
    for (int i = 0; i < rows_count; i++)
    {
        hall[i] = new bool[in_row_place[i]];
    }

    //Инициализация 

    for (int i = 0; i < rows_count; i++)
    {
        for (int j = 0; j < in_row_place[i]; j++)
        {
            hall[i][j] = 0;
        }
    }
    return hall;
}




//string error_text = "|ERROR.Incorect input|\n";
//regex regular("[1-9]|[1-9][0-9]{1,8}");
//regex regular_repeat("[Y]|[N]");
//
//bool YNCheck(string user_data)
//{
//	if (regex_match(user_data.c_str(), regular_repeat))
//		return 1; //Возвращает функции значение "Истина"
//	else
//		return 0; //Возвращает функции значение "Ложь"
//}
//
//bool IntCheck(string x) //Проверка введённого размера
//{
//	regex regular("[1-9]|[1-9][0-9]{1,8}"); //Регулярное выражение
//	if (regex_match(x.c_str(), regular))
//		return 1;
//	else
//		return 0;
//}
//
//int ElMax(int in_row_place[], int rows_count)
//{
//	int max = in_row_place[0];
//	for (int i = 0; i < rows_count; i++)
//	{
//		if (in_row_place[i] > max)
//			max = in_row_place[i];
//	}
//	return max;
//}
//
//int main()
//{
//	string user_continue;
//	do //Условие повтора программы
//	{
//
//		string temp;
//
//
//		cout << "Your hall's format is standart?[Y/N]: ";
//		getline(cin, temp);
//		while (!YNCheck(temp))
//		{
//			cout << error_text;
//			cout << "Your hall's format is standart?[Y/N]: ";
//			getline(cin, temp);
//		}
//
//		//Если зал квадратный
//		if (temp == "Y")
//		{
//
//
//			cout << "Enter rows count: ";
//			getline(cin, temp);
//			while (!IntCheck(temp))
//			{
//				cout << error_text;
//				cout << "Enter rows count: ";
//				getline(cin, temp);
//			}
//			int rows_count = stoi(temp);
//
//
//
//			cout << "Enter place in rows count: ";
//			getline(cin, temp);
//			while (!IntCheck(temp))
//			{
//				cout << error_text;
//				cout << "Enter place in rows count: ";
//				getline(cin, temp);
//			}
//			int place_count = stoi(temp);
//
//			string** hall = new string * [rows_count];
//			for (int i = 0; i < rows_count; i++)
//				hall[i] = new string[place_count];
//
//			//Инициализация и вывод элементов отсюда
//
//			for (int i = 0; i < rows_count; i++)
//			{
//				cout << i + 1 << "| ";
//				for (int j = 0; j < place_count; j++)
//				{
//					hall[i][j] = to_string(j + 1);
//					cout << "[" << hall[i][j] << "]";
//				}
//				cout << endl;
//			}
//			//До сюда
//
//			cout << "Enter books count: ";
//			getline(cin, temp);
//			while (!IntCheck(temp))
//			{
//				cout << error_text;
//				cout << "Enter row's num: ";
//				getline(cin, temp);
//			}
//			int book_repeats = stoi(temp);
//
//			for (int k = 0; k < book_repeats; k++)
//			{
//				cout << "Enter row's num: ";
//				getline(cin, temp);
//				while (!IntCheck(temp))
//				{
//					cout << error_text;
//					cout << "Enter row's num: ";
//					getline(cin, temp);
//				}
//				int x = stoi(temp);
//
//				cout << "Enter side's num: ";
//				getline(cin, temp);
//				while (!IntCheck(temp))
//				{
//					cout << error_text;
//					cout << "Enter side's num: ";
//					getline(cin, temp);
//				}
//				int y = stoi(temp);
//				hall[x - 1][y - 1] = "X";
//
//				for (int i = 0; i < rows_count; i++)
//				{
//					cout << i + 1 << "| ";
//					for (int j = 0; j < place_count; j++)
//					{
//						cout << "[" << hall[i][j] << "]";
//					}
//					cout << endl;
//				}
//			}
//
//		}
//		else //Если зал нестандартного размера
//		{
//			cout << "Enter rows count: ";
//			getline(cin, temp);
//			while (!IntCheck(temp))
//			{
//				cout << error_text;
//				cout << "Enter rows count: ";
//				getline(cin, temp);
//			}
//			int rows_count = stoi(temp);
//
//			string** hall = new string * [rows_count];
//			int* in_row_place = new int[rows_count];
//
//			for (int i = 0; i < rows_count; i++)
//			{
//				cout << "Enter place count in " << i + 1 << "th row:";
//				getline(cin, temp);
//				while (!IntCheck(temp))
//				{
//					cout << error_text;
//					cout << "Enter rows count: ";
//					getline(cin, temp);
//				}
//				in_row_place[i] = stoi(temp);
//			}
//
//			for (int i = 0; i < rows_count; i++)
//			{
//				int current_place_count = stoi(temp);
//				hall[i] = new string[in_row_place[i]];
//			}
//
//			//Инициализация и вывод элементов отсюда
//
//			for (int i = 0; i < rows_count; i++)
//			{
//				cout << i + 1 << "| ";
//				// Центровка рядов отсюда
//				for (int k = 0; k < (ElMax(in_row_place, rows_count) - in_row_place[i]) * 3 / 2; k++)
//					cout << " ";
//				//До сюда
//				for (int j = 0; j < in_row_place[i]; j++)
//				{
//					hall[i][j] = to_string(j + 1);
//					cout << "[" << hall[i][j] << "]";
//				}
//				cout << endl;
//			}
//			//До сюда
//
//			cout << "Enter books count: ";
//			getline(cin, temp);
//			while (!IntCheck(temp))
//			{
//				cout << error_text;
//				cout << "Enter row's num: ";
//				getline(cin, temp);
//			}
//			int book_repeats = stoi(temp);
//
//			for (int k = 0; k < book_repeats; k++)
//			{
//				cout << "Enter row's num: ";
//				getline(cin, temp);
//				while (!IntCheck(temp))
//				{
//					cout << error_text;
//					cout << "Enter row's num: ";
//					getline(cin, temp);
//				}
//				int x = stoi(temp);
//
//				cout << "Enter side's num: ";
//				getline(cin, temp);
//				while (!IntCheck(temp))
//				{
//					cout << error_text;
//					cout << "Enter side's num: ";
//					getline(cin, temp);
//				}
//				int y = stoi(temp);
//				hall[x - 1][y - 1] = "X";
//
//				for (int i = 0; i < rows_count; i++)
//				{
//					cout << i + 1 << "| ";
//					// Центровка рядов отсюда
//					for (int k = 0; k < (ElMax(in_row_place, rows_count) - in_row_place[i]) * 3 / 2; k++)
//						cout << " ";
//					//До сюда
//					for (int j = 0; j < in_row_place[i]; j++)
//					{
//						cout << "[" << hall[i][j] << "]";
//					}
//					cout << endl;
//				}
//			}
//		}
//
//		cout << "\nPrefer to continue working?[Y/N]:"; //Запрос повтора программы
//		getline(cin, user_continue); //Получить строку целиком--std::getline(откуда,куда записать)
//		while (!YNCheck(user_continue)) //Цикл пока не будут получены корректные данные, проверка на соответствие регулярному выражению
//		{
//			cout << error_text;
//			cout << "Prefer to continue working?[Y/N]:";
//			getline(cin, user_continue); //Получить строку целиком--std::getline(откуда,куда записать)
//		}
//
//	} while (user_continue == "Y");
//	return 0;
//}

