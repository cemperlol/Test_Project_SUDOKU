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
        int** field; //�������� ����, ������� ����� ����� ������
        int** tempField; //���� ������, ������������ ��� ������������� � �������� ������������ �����, ����� ������ ����
        bool** boolField; //������ ����,  ����� �� ������� ��� ��������� ������

        SudokuField()
        {
            int k = 0;

            do
            {
                cout << "������� ������ ������ �������, �� �� ������ 2: ";
                cin >> size;
            } while (size < 2);

            do
            {
                cout << "�������� ��������� �������� ���� �� 0 �� 5: ";
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
                    field[j][i] = tempField[i][j]; //������ ���������� ���������, � ������� - ��������
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
                field[sector * (int)sqrt(size) + rowOne][j] = tempField[sector * (int)sqrt(size) + rowTwo][j]; //sector * (int)sqrt(size) + rowOne/rowTwo � ������ �������� �� ������ ���������� �������, ����� �������� ������ ������� ������� � ���������� ����� ������ ����� �������
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
                field[j][sector * (int)sqrt(size) + colOne] = tempField[j][sector * (int)sqrt(size) + colTwo]; //sector * (int)sqrt(size) + colOne/colTwo � ������ �������� �� ������ ���������� �������, ����� �������� ������ ������� ������� � ���������� ����� ������� ����� �������
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

                    field[rowSectorOne * (int)sqrt(size) + i][j] = tempField[rowSectorTwo * (int)sqrt(size) + i][j]; //rowSectorOne/rowSectorTwo * (int)sqrt(size) + i - ������ �������� �� ������ ���������� �� ������� � ��������� ����� ������ ���������� �� �����, ����� ��� ������ ������� �������� �� ��� ������ ������� �������
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

                    field[j][colSectorOne * (int)sqrt(size) + i] = tempField[j][colSectorTwo * (int)sqrt(size) + i]; //colSectorOne/colSectorTwo * (int)sqrt(size) + i - ������ �������� �� ������ ���������� �� ������� � ��������� ����� ������� ���������� �� �����, ����� ��� ������� ������� �������� �� ��� ������� ������� �������
                    field[j][colSectorTwo * (int)sqrt(size) + i] = tempField[j][colSectorOne * (int)sqrt(size) + i];
                }
            }
        }

        void getRandNum(int& firstNum, int& secondNum, const int k) //��������� ���� ������ ��������� ����� � ��������� �� 0 �� k
        {
            firstNum = rand() % k;
            do
            {
                secondNum = rand() % k;
            } while (secondNum == firstNum);

            //cout << "������ ����� = " << firstNum << "\t������ ����� = " << secondNum << endl;
        }

        void getRandNum(int& firstNum, int& secondNum, int& sector, const int k)//��������� ���� ��������� ����� ��� �� ������� �������� ����� �����. ��������� ���������� � ��������� �� 0 �� k
        {
            sector = rand() % k;
            firstNum = rand() % k;
            do
            {
                secondNum = rand() % k;
            } while (secondNum == firstNum);

            //cout << "������ ����� = " << sector << "\t������ ����� = " << firstNum << "\t������ ����� = " << secondNum << endl;
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

            removedCells = 0.2 * difficulty * 2 * pow(size, 2) / 3 - (rand() % size) / sqrt(size); // ������� �������� ���������� �����, ������� ����� �������
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
                } //���� ������ [cellOne][cellTwo] �� ���� �������, �� �� ����� �� �������, ����� �� �� ����������. ��� �������� ������������ ������� ������
            }
        }
    };

public:
    Sudoku()
    {
        show();

        if (cellPicker())
        {
            cout << "����������� � �������! �� ������� ������ ������ �������� " << currField.size << " �� " << currField.size << " �� ������ ��������� " << currField.difficulty << endl;
        }
        else
        {
            cout << "���, �� ���������!\n�� ����� ���� ���� ������ ��������� ���: " << endl;
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
    } //������������ ������ � ������ ��������� ��� ���� ����� ������

    bool cellPicker()
    {
        bool victory = true;
        unsigned short int row, col, value, mistakes;
        mistakes = 0;

        for (int i = 0; i < currField.removedCells; )
        {
            do
            {
                cout << "������� ����� ������ � �������, ����� ������� ������ ������, ��������� ����� �� 1 �� " << currField.size << endl;
                cout << "����� ������: ";
                cin >> row;
                cout << "����� �������: ";
                cin >> col;
                row--;
                col--;
            } while (currField.size < row || row < 0 || currField.size < col || col < 0 || currField.field[row][col] != 0);

            show(row, col);
            do
            {
                cout << "������� �������� ������(" << row << ", " << col << "), ��������� ����� �� 1 �� " << currField.size << " ��� ����������� 0 ��� ������ �����: ";
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
                cout << "��������� ���� ����� ��������� ��������, �������� ������ " << mistakes << " �� " << 5 - (currField.difficulty - 2) << " ���������!" << endl;

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
        cout << "����� ������ ����� ���� ������� 1, ����� ����� �� ���� ������� 0" << endl;
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