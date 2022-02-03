#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;

class Sudoku
{
    class SudokuField
    {
    public:
        unsigned short int difficulty;
        unsigned short int size;
        int removedCells;
        int** field; //Основное поле, которое будет видно игроку
        int** tempField; //Поле судоку, используется при перемешивании и проверки правильности ввода, после начала игры
        bool** boolField; //Булево поле,  чтобы не удалять уже удаленные клетки

        SudokuField()
        {
            int k = 0;

            do
            {
                cout << "Введите размер одного сектора, но не меньше 2: ";
                cin >> size;
            } while (size < 2);

            do
            {
                cout << "Выберите сложность игрового поля от 0 до 5: ";
                cin >> difficulty;
            } while (difficulty > 5);

            this->size = pow(size, 2);
            field = new int* [this->size];
            tempField = new int* [this->size];
            boolField = new bool* [this->size];
            for (int i = 0; i < this->size; i++)
            {
                field[i] = new int[this->size];
                tempField[i] = new int[this->size];
                boolField[i] = new bool[this->size];
            }

            for (int i = 0; i < this->size; i++)
            {
                for (int j = 0; j < this->size; j++)
                {
                    boolField[i][j] = false;

                    if (i == 0)
                    {
                        field[i][j] = j + 1;
                    }
                    else
                    {
                        field[i][j] = field[i - 1][(j + (int)sqrt(this->size) + k) % this->size];
                    }
                }

                if (i % (int)sqrt(this->size) == (int)sqrt(this->size) - 1)
                {
                    k++;
                }
                else
                {
                    k = 0;
                }
            }

            fieldGeterator();
        }

        ~SudokuField()
        {
            delete[]field;
            delete[]tempField;
            delete[]boolField;
        }

    private:
        void copyField()
        {
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    tempField[i][j] = field[i][j];
                }
            }
        }

        void transposing()
        {
            copyField();

            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    field[j][i] = tempField[i][j]; //строки становятся столбцами, а столбцы - строками
                }
            }
        }

        void swapRows()
        {
            int rowOne, rowTwo, sector;

            copyField();

            getRandNum(rowOne, rowTwo, sector, (int)sqrt(size));
            for (int j = 0; j < size; j++)
            {
                field[sector * (int)sqrt(size) + rowOne][j] = tempField[sector * (int)sqrt(size) + rowTwo][j]; //sector * (int)sqrt(size) + rowOne/rowTwo – сектор умножаем на корень квадратный размера, чтобы получить начало нужного сектора и прибавляем номер строки этого сектора
                field[sector * (int)sqrt(size) + rowTwo][j] = tempField[sector * (int)sqrt(size) + rowOne][j];
            }
        }

        void swapCols()
        {
            int colOne, colTwo, sector;

            copyField();

            getRandNum(colOne, colTwo, sector, (int)sqrt(size));
            for (int j = 0; j < size; j++)
            {
                field[j][sector * (int)sqrt(size) + colOne] = tempField[j][sector * (int)sqrt(size) + colTwo]; //sector * (int)sqrt(size) + colOne/colTwo – сектор умножаем на корень квадратный размера, чтобы получить начало нужного сектора и прибавляем номер столбца этого сектора
                field[j][sector * (int)sqrt(size) + colTwo] = tempField[j][sector * (int)sqrt(size) + colOne];
            }
        }

        void swapRowSectors()
        {
            int rowSectorOne, rowSectorTwo;

            copyField();

            getRandNum(rowSectorOne, rowSectorTwo, (int)sqrt(size));
            for (int i = 0; i < (int)sqrt(size); i++)
            {
                for (int j = 0; j < size; j++)
                {

                    field[rowSectorOne * (int)sqrt(size) + i][j] = tempField[rowSectorTwo * (int)sqrt(size) + i][j]; //rowSectorOne/rowSectorTwo * (int)sqrt(size) + i - сектор умножаем на корень квадратный из размера и прибаляем номер строки итерируясь по циклу, чтобы все строки сектора поменять на все строки другого сектора
                    field[rowSectorTwo * (int)sqrt(size) + i][j] = tempField[rowSectorOne * (int)sqrt(size) + i][j];
                }
            }
        }

        void swapColSectors()
        {
            int colSectorOne, colSectorTwo;

            copyField();

            getRandNum(colSectorOne, colSectorTwo, (int)sqrt(size));
            for (int i = 0; i < (int)sqrt(size); i++)
            {
                for (int j = 0; j < size; j++)
                {

                    field[j][colSectorOne * (int)sqrt(size) + i] = tempField[j][colSectorTwo * (int)sqrt(size) + i]; //colSectorOne/colSectorTwo * (int)sqrt(size) + i - сектор умножаем на корень квадратный из размера и прибаляем номер столбца итерируясь по циклу, чтобы все столбцы сектора поменять на все столбцы другого сектора
                    field[j][colSectorTwo * (int)sqrt(size) + i] = tempField[j][colSectorOne * (int)sqrt(size) + i];
                }
            }
        }

        void getRandNum(int& firstNum, int& secondNum, const int k) //Генератор двух разных случайных чисел в диапозоне от 0 до k
        {
            firstNum = rand() % k;
            do
            {
                secondNum = rand() % k;
            } while (secondNum == firstNum);

            //cout << "Первое число = " << firstNum << "\tВторое число = " << secondNum << endl;
        }

        void getRandNum(int& firstNum, int& secondNum, int& sector, const int k)//генератор трех случайных чисел два из которых различны между собой. Генерация происходит в диапозоне от 0 до k
        {
            sector = rand() % k;
            firstNum = rand() % k;
            do
            {
                secondNum = rand() % k;
            } while (secondNum == firstNum);

            //cout << "Первое число = " << sector << "\tВторое число = " << firstNum << "\tТретье число = " << secondNum << endl;
        }

        void fieldGeterator()
        {
            int index = 0;
            int swapNumber, randSwapper;

            swapNumber = 1 + rand() % (int)pow(size, 3);
            do
            {
                randSwapper = rand() % 5;
                switch (randSwapper)
                {
                case 0:
                {
                    swapRows();
                }break;

                case 1:
                {
                    swapCols();
                }break;

                case 2:
                {
                    swapRowSectors();
                }break;

                case 3:
                {
                    swapColSectors();
                }

                case 4:
                {
                    transposing();
                }break;
                }

                index++;
            } while (index < swapNumber);

            copyField();
            cellRemover();
        }

        void cellRemover()
        {
            int cellOne, cellTwo, index;

            removedCells = 0.2 * difficulty * 2 * pow(size, 2) / 3 - (rand() % size) / sqrt(size); // формула рассчета количества ячеек, которые будут удалены
            index = 0;

            while (index < removedCells)
            {
                getRandNum(cellOne, cellTwo, size);

                if (boolField[cellOne][cellTwo] == false)
                {
                    boolField[cellOne][cellTwo] = true;
                    field[cellOne][cellTwo] = 0;
                    index++;
                }
                else
                {
                    continue;
                } //Если ячейка [cellOne][cellTwo] не была удалена, то мы можем ее удалить, иначе мы ее пропускаем. Для проверки используется булевый массив
            }
        }
    };

public:
    Sudoku()
    {
        show();

        if (cellPicker())
        {
            cout << "Поздравляем с победой! Вы успешно решили судоку размером " << currField.size << " на " << currField.size << " на уровне сложности " << currField.difficulty << endl;
        }
        else
        {
            cout << "Увы, Вы проиграли!\nНа самом деле поле судоку выглядело так: " << endl;
            show(currField.difficulty);
        }

    }

private:
    unsigned short int fieldSize;
    int fieldDiff;
    SudokuField currField;

    void show()
    {
        for (int i = 0; i < currField.size; i++)
        {
            for (int j = 0; j < currField.size; j++)
            {
                if (currField.field[i][j] == 0)
                {
                    cout << "* ";
                }
                else
                {
                    cout << currField.field[i][j] << ' ';
                }

                if (j % (int)sqrt(currField.size) == (int)sqrt(currField.size) - 1)
                {
                    cout << '\t';
                }
            }

            if (i % (int)sqrt(currField.size) == (int)sqrt(currField.size) - 1)
            {
                cout << "\n\n\n";
            }
            else
            {
                cout << '\n';
            }
        }
    }

    void show(const int row, const int col)
    {
        for (int i = 0; i < currField.size; i++)
        {
            for (int j = 0; j < currField.size; j++)
            {
                if (currField.field[i][j] == 0)
                {
                    if (i == row && j == col)
                    {
                        cout << "+ ";
                    }
                    else
                    {
                        cout << "* ";
                    }
                }
                else
                {
                    cout << currField.field[i][j] << ' ';
                }

                if (j % (int)sqrt(currField.size) == (int)sqrt(currField.size) - 1)
                {
                    cout << '\t';
                }
            }

            if (i % (int)sqrt(currField.size) == (int)sqrt(currField.size) - 1)
            {
                cout << "\n\n\n";
            }
            else
            {
                cout << '\n';
            }
        }
    }

    void show(const int mistakes)
    {
        for (int i = 0; i < currField.size; i++)
        {
            for (int j = 0; j < currField.size; j++)
            {
                cout << currField.tempField[i][j] << ' ';

                if (j % (int)sqrt(currField.size) == (int)sqrt(currField.size) - 1)
                {
                    cout << '\t';
                }
            }

            if (i % (int)sqrt(currField.size) == (int)sqrt(currField.size) - 1)
            {
                cout << "\n\n\n";
            }
            else
            {
                cout << '\n';
            }
        }
    } //отображается только в случае поражения или если игрок сдался

    bool cellPicker()
    {
        bool victory = true;
        unsigned short int row, col, value, mistakes;
        mistakes = 0;

        for (int i = 0; i < currField.removedCells; )
        {
            do
            {
                cout << "Введите номер строки и столбца, чтобы выбрать пустую ячейку, используя цифры от 1 до " << currField.size << endl;
                cout << "Номер строки: ";
                cin >> row;
                cout << "Номер столбца: ";
                cin >> col;
                row--;
                col--;
            } while (currField.size < row || row < 0 || currField.size < col || col < 0 || currField.field[row][col] != 0);

            show(row, col);
            do
            {
                cout << "Введите значение ячейки(" << row << ", " << col << "), используя цифры от 1 до " << currField.size << " или используйте 0 для отмены выбор: ";
                cin >> value;
            } while (currField.size < value);

            if (value == 0)
            {
                continue;
            }
            else if (value == currField.tempField[row][col])
            {
                currField.field[row][col] = value;
                i++;
                show();
            }
            else
            {
                mistakes++;
                cout << "Введенное Вами число оказалось неверным, допущено ошибок " << mistakes << " из " << 5 - (currField.difficulty - 2) << " возможных!" << endl;

                if (mistakes == 5 - (currField.difficulty - 2))
                {
                    victory = false;
                    break;
                }
                else
                {
                    show();
                    continue;
                }
            }
        }

        return victory;
    }
};

int main()
{
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    bool startGame = true;

    while (startGame)
    {
        cout << "Чтобы начать новую игру введите 1, чтобы выйти из игры введите 0" << endl;
        cin >> startGame;
        if (startGame)
        {
            Sudoku GameOn;
        }
        else
        {
            break;
        }
    }

    return 0;
}