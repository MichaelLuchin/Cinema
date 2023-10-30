#include <cmath>
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

struct snack {
  string name;
  int price;
  int amount;
};

istream &operator>>(istream &is, snack &snack) {
  is >> snack.name;
  is >> snack.price;
  is >> snack.amount;
  return is;
};

struct movie {
  string title;
  string show_date;
  string show_time;
  int price_coef;
  vector<snack> snacks;
  vector<vector<seat>> hall;

  void generate_random_hall(int w, int h) {
    this->hall.resize(h);
    for (auto &r : this->hall) {
      r.resize(w);
      for (auto &v : r)
        v = seat(rand() % 2 + 1);
    }
  };
};

istream &operator>>(istream &is, movie &movie) {
  getline(is, movie.title, '\t');
  // for (string part; (is >> part) && is.peek() != '\t';)
  //   movie.title += part + (is.peek() != '\t' ? " " : "");

  is >> movie.show_date;
  is >> movie.show_time;
  is >> movie.price_coef;

  is.ignore(1, '\t');

  snack snck;
  while (is.peek() != '\t' && is >> snck) {
    movie.snacks.push_back(snck);
  }

  movie.generate_random_hall(10, 10);

  return is;
};

ostream &operator<<(ostream &os, const movie &movie) {
  int j = 0;
  int rows = movie.hall.size();

  for (int i = 0; i < rows; ++i) {
    cout << " || ";
    if (rows >= 9 && i < 9)
      cout << " ";
    cout << 1 + i << ": ";
    int q = 1;

    for (const auto v : movie.hall[i]) {
      switch (v) {
      case seat::none:
        os << "  ";
        for (int i = 0; i < to_string(q).length(); ++i)
          os << " ";
        break;
      case seat::occupied:
        os << "[";
        for (int i = 0; i < to_string(q).length(); ++i)
          os << "#";
        os << "]";
        break;
      case seat::regular:
        os << "[" << q << "]";
        break;
      case seat::vip:
        os << "{" << q << "}";
        break;
      case seat::sofa:
        os << "(" << q << ")";
        break;
      }
      ++q;
    }
    cout << "  ||\n";
  }

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

vector<row_and_col> find_open_seats(vector<vector<seat>> hall, int k) {
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

template <typename T> T ask(string msg) {
  T v;
  while (!(cout << msg) || !(cin >> v))
    ;
  return v;
}

int main() {
  setlocale(LC_ALL, "Russian");
  srand(time(NULL));

  vector<movie> movies = get_movies("movies.tsv");

  print_clerk();

  cout << "доступные фильмы:" << endl;
  cout << setw(log10(movies.size()));
  for (int i = 0; i < movies.size(); i++) {
    movie m = movies[i];
    cout << i + 1 << ": ";
    cout << m.show_date << " " << m.show_time << " : " << m.title;
    cout << endl;
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
  
  cout << endl;
  cout << "доступные места для " << visitors << " человек: \n";
  for (int i = 0; i < open_seats.size() && i < 10; i++) {
    cout << "ряд " << open_seats[i].row + 1 << ", места ";
    cout << open_seats[i].col + 1 << "-" << open_seats[i].col + 1 + visitors;
    cout << endl;
  }
  if (open_seats.size() >= 10)
    cout << "и другие..\n";

  return 0;
}
