// ConsoleApplication1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <unordered_set>
#include <exception>
#include <unordered_map>
#include <stdio.h>

using namespace::std;

//const unsigned int lenX = 3;
int const lenX = 15;
float n = 1.03;


double random(double min, double max)
{
    return (double)(rand()) / RAND_MAX * (max - min) + max/2;
}

struct IntVector2 
{
    //bool X;
    //bool Y;
    int X;
    int Y;

    IntVector2(int x, int y)
        :X(x), Y(y)
    {
    }

    IntVector2(const IntVector2& other)
        :X(other.X), Y(other.Y)
    {
    }

    friend bool operator==(const IntVector2& a, const IntVector2& b) {
        return a.X == b.X && a.Y == b.Y;
    }
};

template<> struct std::hash<IntVector2> {
    std::size_t operator()(IntVector2 const& s) const noexcept
    {
        return s.X ^ s.Y;
    }
};

typedef std::unordered_map<IntVector2, int> Grid;

//берём значение по координатам
bool get(Grid& grid, int x, int y)
{
    auto it = grid.find(IntVector2(x, y));
    if (it == grid.end())
        return false;

    //возвращаем то, что положили
    return it->second;
}

//кладём значение по кординатам
bool put(Grid& grid, int x, int y, bool it)
{
    if (!it)
    {
        int r = 6;
    }
    grid.emplace(IntVector2(x, y), it);
    return 0;
}

//сравниваем 
bool random_decision(int k, int l, float Z, float n, float **T) //float &T[k][l]
{
    float r = random(0, 1);
    float psi = r * Z;
    float Z1 = pow((T[k - 1][l] + T[k + 1][l] + T[k][l - 1] + T[k][l + 1]), n);
    //float Z1 = pow(z1, n); 
    //float Z1 = pow((get(T, k - 1, l) + get(T, k + 1, l) + get(T, k, l - 1) + get(T, k, l + 1)), n);

    return Z1 > psi;
}


//генератор поля (работает неверно)
void generate_field(float n, Grid& varLig) //было int вместо Grid& (magic  @_@ )
{
    //создаю массив, в конце выполнения функции он удалится
    auto T = new float*[lenX];

    //записываю в потенциальное (температурное) поле нули
    for (int i = 0; i < lenX; i++)
    {
        T[i] = new float[lenX];
        for (int j = 0; j < lenX; j++)
        {
            T[i][j] = 0;
        }
    }

    //рассчитываю градиент поля
    for (int i = 1; i < lenX - 1; i++)
    {
        for (int j = 1; j < lenX - 1; j++)
        {
            T[i][j] = (i + 1) / ((float)(2) * (j + 1)) + pow(2, j);
            //T[i][j] = 0.25 * (T[i + 1][j] + T[i - 1][j] + T[i][j + 1] + T[i][j - 1]);
        }
    }

    for (int i = 2; i < lenX - 2; i++)
    {
        for (int j = 2; j < lenX - 2; j++)
        {
            float Z = (T[i - 1][j] + T[i + 1][j] + T[i][j - 1] + T[i][j + 1]);

            
            //__________________НАЧИНАЕТСЯ КОД С КОСЯКАМИ ДЛЯ ЗАДАНИЯ ПОЛЯ Lig_________________________________________________

            bool r_ip1 = random_decision(i + 1, j, Z, n, T) & (get(varLig, i, j) or get(varLig, i + 1, j - 1) or get(varLig, i + 1, j + 1) or get(varLig, i + 2, j));
            bool r_im1 = random_decision(i - 1, j, Z, n, T) & (get(varLig, i, j) or get(varLig, i - 1, j - 1) or get(varLig, i - 1, j + 1) or get(varLig, i - 2, j));
            bool r_jp1 = random_decision(i, j + 1, Z, n, T) & (get(varLig, i, j) or get(varLig, i - 1, j + 1) or get(varLig, i, j + 2) or get(varLig, i + 1, j + 1));

            if (r_ip1) {
                put(varLig, i + 1, j, true);
                /*}
                else {
                    put(varLig, i + 1, j, false);
                }*/
            }
            if (!r_ip1) {
                put(varLig, i + 1, j, false);
            }

            // определяю для x - 1
            if (r_im1) {
                //if (get(varLig, i, j) or get(varLig, i - 1, j - 1) or get(varLig, i - 1, j + 1) or get(varLig, i - 2, j)) {
                put(varLig, i - 1, j, true);
                /*}
                else {
                    put(varLig, i - 1, j, false);
                }*/
            }
            if (!r_im1) {
                put(varLig, i - 1, j, false);
            }

            // определяю для y + 1 # (get(Lig, k - 1, l] == 0) & (get(Lig, k + 1, l] == 0) |
            if (r_jp1) {
                put(varLig, i, j + 1, true);
                /*}
                else {
                    put(varLig, i, j + 1, false);
                }*/
            }
            if (!r_jp1) {
                put(varLig, i, j + 1, false);
            }
        }
    }
    

    //удаляю массив - очищаю память
    /*for (size_t i = 0; i <= lenX; i++)
    {
        delete T[i];
    }
    delete T;*/
}


int main()
{
    Grid Lig;

    //int nn = 4;
    //int r = round(random(3, 6));
    //int rand = round(random(1, 3));
    //int ri = round(random(0, 2));
    //int rj = round(random(0, 2));
    //for (int i = 0; i < nn; i++) {
    //    for (int j = 0; j < nn; j++) {
    //        int a = round(ri * (lenX - nn) + i);
    //        int b = round((lenX / r) * (r - rand));
    //        //(lenX / r) * (r - rand)
    //        //(lenX / r) * (r - rand)
    //        put(Lig, a, j, true);
    //            // Lig[ri * (lenX - i) + i, int(lenY / r) * (r - rand)] = 1
    //        put(Lig, i, rj * (lenX - nn) + j, true);
    //    }
    //}

    //int(lenX / 2)
    put(Lig, 0, 2, true);
    put(Lig, 1, 2, true);
    put(Lig, 2, 2, true);
    put(Lig, 3, 2, true);

    put(Lig, lenX - 2, 0, true);
    put(Lig, lenX - 2, 1, true);
    put(Lig, lenX - 2, 2, true);
    put(Lig, lenX - 2, 3, true);

    put(Lig, lenX, lenX - 2, true);
    put(Lig, lenX - 1, lenX - 2, true);
    put(Lig, lenX - 2, lenX - 2, true);
    put(Lig, lenX - 3, lenX - 2, true);

    int numIter = 25;
    for (int i = 0; i < numIter; i++) 
    {
        generate_field(n, Lig);
    }

    for (int i = 0; i < lenX; i++) {
        cout << " \n";
        for (int j = 0; j < lenX; j++) {
            if (get(Lig, i, j))
                cout << "1 ";
            else
                cout << "0 ";
        }
    }
}




// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
