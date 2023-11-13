#include <fstream> // библиотека, включающая набор классов, методов и функций, которые предоставляют интерфейс для чтения/записи данных из/в файл
#include <iomanip> // библиотека, которая используется для управления выводом программы
#include <iostream> // библиотека для работы с потоками ввода вывода
#include <math.h> // библиотека для выполнения математических операций
#include <sstream> // библиотека с классами, функциями и переменными для организации работы со строками, через интерфейс потоков
#include <string> // библиотека с методами и переменными для организации работы со строками
#include <vector> //библиотека для работы с векторами
#include <thread> // библиотека для создания потоков и управления потоками
#include <chrono> //библиотека, предоставляющая набор типов и функций для работы со временем
#include <windows.h> //библиотека, в которой объявляются функции, предоставляющие интерфейс доступа к Windows API

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;

using std::endl;
using std::string;
using std::ifstream;

enum struct seat : unsigned char //перечисление, определяющее тип мест в кинозале
{
  // нет места (дырка)
  none,
  // занято
  occupied,
  // обычное место
  regular,
  // вип место дороже
  vip,
  // диванчик стоит как вип место, но если
  // они стоят подряд, то их можно купить
  // только все разом
  sofa,
};

struct movie //структура, содержащая информацию о фильме
{
    string title; //название фильма
    string show_time; //время показа
    
    int price; //цена билета
    
    vector<vector<seat>> hall; //матрица мест

    void generate_random_hall(int w, int h)
    {
        this->hall.resize(h);
        
        for (auto& r : this->hall)
        {
            r.resize(w);
            
            for (auto& v : r)
            {
                v = seat(rand() % 3 + 1);
            }
        }
    }
};

istream &operator>>(istream &is, movie &movie) //перегрузка оператора ввода для класса movie
{
    getline(is, movie.title, '\t'); //считывает название фильма из файла
    getline(is, movie.show_time, '\t'); //считывает время показа из файла

    is >> movie.price; //считывает цену из файла
    is.ignore(1, '\t');

    vector<seat> row; //создание вектора (ряда) сидений
  
    while (!is.eof()) //цикл выполняется до конца строки
    {
        int count = 0;
        
        if (!(is >> count) || count < 1) 
        {
            is.clear();
            count = 1;
        }

        char letter;
        is >> letter; //считывание символа из потока ввода
        
        switch (letter) //добавляет к ряду соотвествующее количество определённых мест
        {
            default: //если не один из следующих case не выполняется, то производится считывание символа
                is.get();
                break;
            
            case '_': //_ - none в seat
                row.resize(row.size() + count, seat::none);
                break;
            
            case 'x': //x - occupied в seat
                row.resize(row.size() + count, seat::occupied);
                break;
            
            case 'r': //r - regular в seat
                row.resize(row.size() + count, seat::regular);
                break;
            
            case 'v': //v - vip в seat
                row.resize(row.size() + count, seat::vip);
                break;
            
            case 's': //s - sofa в seat
                row.resize(row.size() + count, seat::sofa);
                break;
            
            case '?':
                row.reserve(row.size() + count);
                
                for (int i = 0; i < count; i++)
                {
                    row.push_back(seat(rand() % 2 + 1));
                }
                
                break;
        }

        if (is.peek() == ' ') //если символ - пробел
        {
          movie.hall.push_back(row); //полученный вектор добавляется в матрицу мест
          row = vector<seat>(); //вектор сидений очищается
        }
    }
    movie.hall.push_back(row); //последний полученный ряд добавляется в матрицу мест

    return is;
}

ostream &operator<<(ostream &os, const movie &movie) //перегрузка оператора вывода для класса movie
{
    int w = 0;
    
    for (const auto& row : movie.hall) //цикл проходится по каждму ряду зала и обновляет значение w, если текущая строка имеет большую длину.
    {
        w = log10(row.size()) > w ? log10(row.size()) : w;
    }

    for (int row = 0; row < movie.hall.size(); row++) //цикл проходится по каждму ряду зала
    {
        os << "| " << setw(w + 1) << setfill('0') << row + 1 << " |";

        for (int col = 0; col < movie.hall[row].size(); col++) //цикл проходится по каждому месту в ряде
        {
            switch (movie.hall[row][col]) //в зависимости от типа места в ряде производится его оформленный вывод в консоли
            {
                case seat::none:
                    os << "  ";
                    
                    for (int i = 0; i <= w; i++)
                    {
                        os << " ";
                    }
                    
                    break;

                case seat::occupied:
                    os << " ";
                    
                    for (int i = 0; i <= w; i++)
                    {
                        os << "#";
                    }
                    
                    os << " ";
                    break;

                case seat::regular:
                    os << " " << setw(w + 1) << setfill('0') << col + 1 << " ";
                    break;

                case seat::vip:
                    os << "[" << setw(w + 1) << setfill('0') << col + 1 << "]";
                    break;

                case seat::sofa:
                    if (col == 0 || movie.hall[row][col - 1] != seat::sofa) //проверяется, является ли текущее место диваном или предыдущее не является диваном
                    {
                        os << "(";
                    }
                    
                    else
                    {
                        os << " ";
                    }

                    os << setw(w + 1) << setfill('0') << col + 1;

                    if (col == movie.hall[row].size() - 1 || movie.hall[row][col + 1] != seat::sofa) //проверяется, является ли текущее место последним или следующее не является диваном
                    {
                        os << ")";
                    }
                    
                    else
                    {
                        os << " ";
                    }
                    
                    break;
            }
        }
        os << "\n";
    }
    return os;
}

vector<movie> get_movies(string filepath) 
{
    vector<movie> movies; //создание вектора фильмов

    ifstream file(filepath); //открытие файла соответствующего пути
    
    if (!file.is_open())
    {
        return movies;
    }

    string line;

    getline(file, line); // Игнорируем первую строчку

    while (!file.eof() && getline(file, line).good()) //считываем каждую строку файла
    {
        istringstream iss(line); //создаётся объект, которому передаётся строка. значения из строки считываются как из потока
        movie movie;
        iss >> movie; //в movie считываются значения из istringstream с помощью перегруженного оператора
        movies.push_back(movie); //полученный объект добавляется в вектор movies
    }
    
    file.close(); //закрытие файла

    return movies;
}

string ReadFileIntoString(const string& path) // принимает путь к файлу в виде строки
{
    ifstream input_file(path); // чтение файла
    return string(std::istreambuf_iterator<char>(input_file), std::istreambuf_iterator<char>()); // преобразование файла в строку (создаются итераторы входа и выхода)
}

void SetCursorPosition(int x, int y) // устанавливает курсор на соответствующие координаты
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // создается константная переменная, которая содержит дескриптор стандартного потока вывода
    std::cout.flush(); // поток cout сбрасывается
    COORD coord = { (SHORT)x, (SHORT)y }; // определяется структура, которая представляет координаты в консоли, и ее поля заполняются текущими координатами курсора
    SetConsoleCursorPosition(hOut, coord); // устанавливает курсор в указанную позицию с помощью дескриптора и структуры
}

void Draw(int frame)
{
    SetCursorPosition(0, 0); // курсор устанавливается на нулевые координаты
    string m = ReadFileIntoString("C:/texts1/frame" + std::to_string(frame) + ".txt"); // читает файл с параметром frame в виде строки
    std::cout << m; // вывод строки
}

void Roll()
{
    for (int i = 0; i < 2; i++)
    {
        auto start = std::chrono::steady_clock::now(); // определяет текущее время
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start); // рассчитывает прошедшее время с момента старта программы и до текущего момента
        
        while (elapsed.count() < 2120) // прошло ли 2120 млс
        {
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start); // повторный рассчёт
            Draw(elapsed.count() / 40);
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // пауза в 10 млс
        }
    }
}

void clear() //очищение консоли
{
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void print_clerk()
{
  cout <<
    "                                              :l'''''''''''''''l;" << "\n" <<
    "                  .,,'''''','.                :;  ...'.......  ;;" << "\n" <<
    "                .;:'       .',;'              :;  ',,;'.',,'.  :;" << "\n" <<
    "              .;:.   ..'''..  .;:.            cl.''''',,''','.'l;" << "\n" <<
    "             'c.  .,;,'..',;;'  .c'           oNNNNNNNNNNNNNNNNX:" << "\n" <<
    "             :;  .c,.       .:;. .c.          lWMMMMMMMMMMMMMMMN:" << "\n" <<
    "             :; .c, .c.   .:'.c;  ;:          o0kkkkkkkkkkkkkkO0:" << "\n" <<
    "             c, .c'           ::  .c'         :;               :;" << "\n" <<
    "            .c,  ::          .c'   ;:         :;               ;;" << "\n" <<
    "            .c'  .;c         c.    ,c.        ::...............c;" << "\n" <<
    "            .c.    .c'  .  'c.     'c.                           " << "\n" <<
    "            'c.     ::     :l.     'c.                           " << "\n" <<
    "            ,c. .',,,.     .',;;,. ,c.                           " << "\n" <<
    "            :d:,:d;           .ll,,dl.                           " << "\n" <<
    "         .;;;'. .c,           .c,  ,::;.                         " << "\n" <<
    "        .c;     .ll,,,,,,,,,,,:o,    .l'       ...,;,'....       " << "\n" <<
    "        ,c.     .c;...........'l,     :;     .coccc;;ccccl;      " << "\n" <<
    "       .c'      .c,           .c,     'c;.   .coccc;;ccccl;      " << "\n" <<
    "       ;:       .c,           .c,     .cx,   .coccc;;ccccl;      " << "\n" <<
    "....;l,.........,c;...........,oc........'c;...cxx::d;..:o:..... " << "\n" <<
    ":o,''',,,,,,,,,''''',,,,,,,,,',:;','',,''''',,',,;,,;,'',,,,'':o'" << "\n" <<
    ":o,''',,,,,,,,,''''',,,,,,,,,',:;','',,''''',,',,;,,;,'',,,,'':o'" << "\n" <<
    ":o,''',,,,,,,,,''''',,,,,,,,,',:;','',,''''',,',,;,,;,'',,,,'':o'" << "\n" <<
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccl," << "\n";
}

struct row_and_col //структура строк и столбцов
{
    int row;
    int col;
};

vector<row_and_col> find_open_seats(const vector<vector<seat>> &hall, int k) //поиск незанятых мест
{
    vector<row_and_col> row;
    int near_free = 0;
  
    for (int i = 0; i < hall.size(); i++) //циклы пробегаются по всем местам зала
    {
        for (int j = 0; j < hall[i].size(); j++) 
        {
            if (hall[i][j] == seat::occupied || hall[i][j] == seat::none) //если место занято или мест нет
            {
                near_free = 0;
            }
      
            else 
            {
                near_free++;
                if (near_free >= k)
                {
                    row.push_back({ i, j - k + 1 });
                }
            }
        }
        near_free = 0;
    }
    return row;
}

void claim_seats(vector<vector<seat>> &hall, row_and_col rc, int visitors) //занимает места для данного количества посетителей
{
    for (int i = 0; i < visitors; i++)
    {
        hall[rc.row][rc.col + i] = seat::occupied;
    }
}

int calculate_price(const movie &movie, row_and_col rc, int visitors) 
{
    int sum = 0;

    if (rc.row < 0 || rc.row >= movie.hall.size()) //проверка выхода за границы ряда
    {
        return -1;
    }

    auto row = movie.hall[rc.row];
  
    if (rc.col + visitors - 1 >= row.size()) //проверка выхода за границы колонки
    {
        return -1;
    }

    for (int i = 0; i < visitors; i++) //для каждого посетителя проверяется тип места, и общая цена увеличивается в зависимости от типа
    {
        switch (row[rc.col + i]) 
        {
            case seat::none:
            case seat::occupied:
                return -1;
            case seat::regular:
                sum += movie.price;
                break;
            case seat::vip:
            case seat::sofa:
                sum += (movie.price * 1.35);
                break;
        }
    }
    return sum;
}

template <typename T> T ask(string msg) 
{
    T v; //объявление переменной типа T (любого типа)
    
    while (!(cout << msg) || !(cin >> v)) //получение корректного ввода пользователя
    {
        cin.clear();
        cin.ignore(LLONG_MAX, '\n');
    }
    return v;
}

bool prog(vector<movie> &movies) 
{
    print_clerk();
  
    cout << "желаете посмотреть фильм?" << "\n";
    cout << "доступные фильмы:" << "\n";
  
    for (int i = 0; i < movies.size(); i++) // вывод доступных фильмов, времени показа и цены за место
    {
        movie m = movies[i];
        cout << setw(log10(movies.size()) + 1) << i + 1 << " | " << m.show_time << " | " << m.price << "руб/место | " << m.title << "\n";
    }
    
    cout << setw(0);

    int choice = 0;
    
    while (choice < 1 || choice > movies.size()) // выбор фильма пользователем
    {
        choice = ask<int>("введите номер фильма: ");
    }

    choice--;

    cout << "\n";
    cout << movies[choice];
    cout << "легенда: # - занято, [...] - вип место, (...) - диванчики\n";

    int visitors = 0;
  
    while (visitors < 1) //выбор кол-ва посетителей
    {
        visitors = ask<int>("введите кол-во человек: ");
    }

    auto open_seats = find_open_seats(movies[choice].hall, visitors); //поиск свобоных мест для данного кол-ва посетителей
    
    if (open_seats.size() == 0) //если не удалось найти свободных мест
    {
        cout << "мест нет, извините :(" << "\n";
        return true;
    }

    cout << "\n";
    cout << "доступные места для " << visitors << " человек: " << "\n";
  
    for (int i = 0; i < open_seats.size() && i < 15; i++) //вывод вариантов свободных мест
    {
        cout << "| " << setw(log10(open_seats.size()) + 1) << setfill('0') << i + 1 << " | "
             << "ряд " << open_seats[i].row + 1 << ", места "
             << open_seats[i].col + 1 << "-" << open_seats[i].col + visitors << " ("
             << calculate_price(movies[choice], open_seats[i], visitors) << "руб)" << "\n";
    }
  
    if (open_seats.size() >= 15)
    {
        cout << "и другие.." << "\n";
    }

    int rci = 0;
    
    while (rci < 1 || rci > open_seats.size()) //выбор вариантов рассадки пользователем
    {
        rci = ask<int>("выберите номер желаемой рассадки: ");
    }
    
    rci--;

    claim_seats(movies[choice].hall, open_seats[rci], visitors);

    cout << "\nзабронировали вам места.\nприятного просмотра!\n";
  
    sleep_for(seconds(5)); //пауза в 5 секунд

    clear();

    Roll();

    clear();
  
    return true;
}

int main() 
{
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    HWND console = GetConsoleWindow();
    MoveWindow(console, 0, 0, 850, 750, TRUE); //изменение размера окна консоли

    vector<movie> movies = get_movies("movies.tsv");

    while (prog(movies))
    {
        cout << "\n";
    }
    return 0;
}
