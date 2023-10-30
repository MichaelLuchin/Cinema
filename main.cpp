#include <fstream>
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

int main() {
  setlocale(LC_ALL, "Russian");
  srand(time(NULL));

  for (const movie v : get_movies("example.tsv")) {
    cout << v.title << endl;
    cout << v.show_date<< endl;
    cout << v.show_time<< endl;
    cout << v.snacks.size()<< endl;
    cout << endl;
  }

  return 0;
}
