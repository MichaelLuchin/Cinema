#include <math.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum struct seat : unsigned char {
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

// struct snack {
//   string name;
//   int price;
//   int amount;
// };

// istream &operator>>(istream &is, snack &snack) {
//   is >> snack.name;
//   is >> snack.price;
//   is >> snack.amount;
//   return is;
// };

struct movie {
  string title;
  string show_time;
  int price;
  // vector<snack> snacks;
  vector<vector<seat>> hall;

  void generate_random_hall(int w, int h) {
    this->hall.resize(h);
    for (auto &r : this->hall) {
      r.resize(w);
      for (auto &v : r)
        v = seat(rand() % 3 + 1);
    }
  };
};

istream &operator>>(istream &is, movie &movie) {
  getline(is, movie.title, '\t');
  getline(is, movie.show_time, '\t');

  is >> movie.price;
  is.ignore(1, '\t');

  // чтение снэков
  // snack snck;
  // while (is.peek() != '\t' && is >> snck) {
  //   movie.snacks.push_back(snck);
  // }

  // movie.generate_random_hall(10, 10);

  // парсинг зала
  vector<seat> row;
  while (!is.eof()) {
    int count = 0;
    if (!(is >> count) || count < 1)
      count = 1;

    char letter;
    is >> letter;
    cout << count << letter;
    switch (letter) {
    case '_':
      row.resize(row.size() + count, seat::none);
      break;
    case 'x':
      row.resize(row.size() + count, seat::occupied);
      break;
    case 'r':
      row.resize(row.size() + count, seat::regular);
      break;
    case 'v':
      row.resize(row.size() + count, seat::vip);
      break;
    case 's':
      row.resize(row.size() + count, seat::sofa);
      break;
    case '?':
      row.reserve(row.size() + count);
      for (int i = 0; i < count; i++)
        row.push_back(seat(rand() % 2 + 1));
      break;
    }

    if (is.peek() == ' ') {
      movie.hall.push_back(row);
      row = vector<seat>();
    }
  }
  movie.hall.push_back(row);

  return is;
};

ostream &operator<<(ostream &os, const movie &movie) {
  int w = 0;
  for (const auto &row : movie.hall)
    w = log10(row.size()) > w ? log10(row.size()) : w;

  for (int row = 0; row < movie.hall.size(); row++) {
    cout << "| " << setw(w + 1) << row + 1 << " |";

    for (int col = 0; col < movie.hall[row].size(); col++) {
      switch (movie.hall[row][col]) {
      case seat::none:
        cout << "  ";
        for (int i = 0; i <= w; i++)
          cout << " ";
        break;

      case seat::occupied:
        cout << " ";
        for (int i = 0; i <= w; i++)
          cout << "#";
        cout << " ";
        break;

      case seat::regular:
        cout << " " << setw(w + 1) << setfill('0') << col + 1 << " ";
        break;

      case seat::vip:
        cout << "[" << setw(w + 1) << setfill('0') << col + 1 << "]";
        break;

      case seat::sofa:
        if (col == 0 || movie.hall[row][col - 1] != seat::sofa)
          cout << "(";
        else
          cout << " ";

        cout << setw(w + 1) << setfill('0') << col + 1;

        if (col == movie.hall[row].size() - 1 ||
            movie.hall[row][col + 1] != seat::sofa)
          cout << ")";
        else
          cout << " ";

        break;
      }
    }

    cout << setfill(' ');
    cout << endl;
  }

  cout << setw(0);

  return os;
}

vector<movie> get_movies(string filepath) {
  vector<movie> movies;

  ifstream file(filepath);
  if (!file.is_open())
    return movies;

  string line;

  // Игнорируем первую строчку
  getline(file, line);

  while (!file.eof() && getline(file, line).good()) {
    istringstream iss(line);
    movie movie;
    iss >> movie;
    movies.push_back(movie);
  }
  file.close();

  return movies;
}

void print_clerk() {
  // clang-format off
  cout <<
    "                                              :l'''''''''''''''l;" << endl <<
    "                  .,,'''''','.                :;  ...'.......  ;;" << endl <<
    "                .;:'       .',;'              :;  ',,;'.',,'.  :;" << endl <<
    "              .;:.   ..'''..  .;:.            cl.''''',,''','.'l;" << endl <<
    "             'c.  .,;,'..',;;'  .c'           oNNNNNNNNNNNNNNNNX:" << endl <<
    "             :;  .c,.       .:;. .c.          lWMMMMMMMMMMMMMMMN:" << endl <<
    "             :; .c, .c.   .:'.c;  ;:          o0kkkkkkkkkkkkkkO0:" << endl <<
    "             c, .c'           ::  .c'         :;               :;" << endl <<
    "            .c,  ::          .c'   ;:         :;               ;;" << endl <<
    "            .c'  .;c         c.    ,c.        ::...............c;" << endl <<
    "            .c.    .c'  .  'c.     'c.                           " << endl <<
    "            'c.     ::     :l.     'c.                           " << endl <<
    "            ,c. .',,,.     .',;;,. ,c.                           " << endl <<
    "            :d:,:d;           .ll,,dl.                           " << endl <<
    "         .;;;'. .c,           .c,  ,::;.                         " << endl <<
    "        .c;     .ll,,,,,,,,,,,:o,    .l'       ...,;,'....       " << endl <<
    "        ,c.     .c;...........'l,     :;     .coccc;;ccccl;      " << endl <<
    "       .c'      .c,           .c,     'c;.   .coccc;;ccccl;      " << endl <<
    "       ;:       .c,           .c,     .cx,   .coccc;;ccccl;      " << endl <<
    "....;l,.........,c;...........,oc........'c;...cxx::d;..:o:..... " << endl <<
    ":o,''',,,,,,,,,''''',,,,,,,,,',:;','',,''''',,',,;,,;,'',,,,'':o'" << endl <<
    ":o,''',,,,,,,,,''''',,,,,,,,,',:;','',,''''',,',,;,,;,'',,,,'':o'" << endl <<
    ":o,''',,,,,,,,,''''',,,,,,,,,',:;','',,''''',,',,;,,;,'',,,,'':o'" << endl <<
    "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccl," << endl;
  // clang-format on
}

struct row_and_col {
  int row;
  int col;
};

vector<row_and_col> find_open_seats(const vector<vector<seat>> &hall, int k) {
  vector<row_and_col> row;
  int near_free = 0;
  for (int i = 0; i < hall.size(); i++) {
    for (int j = 0; j < hall[i].size(); j++) {
      if (hall[i][j] == seat::occupied || hall[i][j] == seat::none)
        near_free = 0;
      else {
        near_free++;

        // TODO: проверять на отрезки дивана
        if (near_free >= k)
          row.push_back({i, j - k + 1});
      }
    }
    near_free = 0;
  }
  return row;
}

int calculate_price(const movie &movie, row_and_col rc, int visitors) {
  int sum = 0;

  if (rc.row < 0 || rc.row >= movie.hall.size())
    return -1;

  auto row = movie.hall[rc.row];
  if (rc.col + visitors - 1 >= row.size())
    return -1;

  for (int i = 0; i < visitors; i++) {
    switch (row[rc.col + i]) {
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

template <typename T> T ask(string msg) {
  T v;
  while (!(cout << msg) || !(cin >> v))
    ;
  return v;
}

bool prog(const vector<movie> &movies) {
  cout << "желаете посмотреть фильм?" << endl;
  cout << "доступные фильмы:" << endl;
  for (int i = 0; i < movies.size(); i++) {
    movie m = movies[i];
    cout << setw(log10(movies.size()) + 1) << i + 1 << " | " << m.show_time
         << " | " << m.price << "руб/место | " << m.title << endl;
  }
  cout << setw(0);

  int choice = 0;
  while (choice < 1 || choice > movies.size())
    choice = ask<int>("выберите фильм: ");

  cout << endl;
  cout << movies[choice];
  cout << endl;

  int visitors = 0;
  while (visitors < 1)
    visitors = ask<int>("введите кол-во человек: ");

  auto open_seats = find_open_seats(movies[choice].hall, visitors);
  if (open_seats.size() == 0) {
    cout << "мест нет, извините :(" << endl;
    return true;
  }

  cout << endl;
  cout << "доступные места для " << visitors << " человек: " << endl;
  for (int i = 0; i < open_seats.size() && i < 10; i++) {
    cout << "| " << i + 1 << " | "
         << "ряд " << open_seats[i].row + 1 << ", места "
         << open_seats[i].col + 1 << "-" << open_seats[i].col + visitors << " ("
         << calculate_price(movies[choice], open_seats[i], visitors) << "руб)"
         << endl;
  }
  if (open_seats.size() >= 10)
    cout << "и другие.." << endl;

  int rci = 0;
  while (rci < 1 || rci > open_seats.size())
    rci = ask<int>("выберите места: ");

  cout << "приятного просмотра!" << endl;
  return false;
}

int main() {
  setlocale(LC_ALL, "Russian");
  srand(time(NULL));

  print_clerk();

  vector<movie> movies = get_movies("movies.tsv");

  while (prog(movies))
    cout << endl;

  return 0;
}
