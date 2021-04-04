#include <iostream>
#include <vector>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>

using  namespace std;

class Vector
{
public:
    float x;
    float y;

    Vector(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    Vector()
    {
        this->x = 0;
        this->y = 0;
    }

    static Vector Add(Vector vector1, Vector vector2)
    {
        return Vector(vector1.x + vector2.x, vector1.y + vector2.y);
    }
    static Vector Mult(Vector vector1, float number)
    {
        return Vector(vector1.x * number, vector1.y * number);
    }
};

class GameObject
{
public:
    virtual void DrawSelf(HANDLE handle, int w, int h) { }
};

class Border : public GameObject
{
private:
    string borderLine;
public:
    char sprite;
    int height;
    int width;
    Border() {}
    Border(int width, int height, char sprite)
    {
        this->sprite = sprite;
        this->width = width;
        this->height = height;
        for (int i = 0; i <= this->width; i++)
        {
            borderLine += sprite;
        }
    }

    void DrawSelf(HANDLE handle, int w, int h)
    {
        COORD cursorPos;
        cursorPos.X = 0;
        cursorPos.Y = height;

        SetConsoleCursorPosition(handle, cursorPos);
        cout << borderLine;
        cursorPos.X = width;
        for (int i = 0; i < height; i++)
        {
            cursorPos.Y = i;
            SetConsoleCursorPosition(handle, cursorPos);
            cout << sprite;
        }
    }
};

class Pipe : public GameObject
{
private:
    string sprite1 = "| |";
    string sprite2 = "|_|";
    string sprite3 = " _ ";
    string spriteClear = "   ";
    Vector oldPosition;
public:
    int heightCrotch = 3;
    Vector position;
    float y1 = 0.0f;
    float y2 = 0.0f;

    void Move(Vector pos)
    {
        oldPosition = position;
        position = Vector::Add(position, pos);
    }

    void DrawSelf(HANDLE handle, int width, int height)
    {
        COORD pos;
        if (oldPosition.x < width - 2)
        {
            pos.X = oldPosition.x;
            pos.Y = oldPosition.y;
            SetConsoleCursorPosition(handle, pos);

            for (int i = 0; i < oldPosition.y + 1; i++)
            {
                pos.Y = i;
                SetConsoleCursorPosition(handle, pos);
                cout << spriteClear;
            }

            pos.Y = oldPosition.y + heightCrotch;
            SetConsoleCursorPosition(handle, pos);
            cout << spriteClear;

            for (int i = oldPosition.y + heightCrotch; i < height; i++)
            {
                pos.Y = i;
                SetConsoleCursorPosition(handle, pos);
                cout << spriteClear;
            }
        }

        if (position.x < width - 2)
        {
            //рисовка
            pos.X = position.x;
            pos.Y = position.y;
            SetConsoleCursorPosition(handle, pos);
            for (int i = 0; i < position.y; i++)
            {
                pos.Y = i;
                SetConsoleCursorPosition(handle, pos);
                cout << sprite1;
            }
            pos.Y = position.y;
            y1 = pos.Y;
            SetConsoleCursorPosition(handle, pos);
            cout << sprite2;

            pos.Y = position.y + heightCrotch;
            y2 = pos.Y;
            SetConsoleCursorPosition(handle, pos);
            cout << sprite3;

            for (int i = position.y + heightCrotch + 1; i < height; i++)
            {
                pos.Y = i;
                SetConsoleCursorPosition(handle, pos);
                cout << sprite1;
            }
        }
    }
};

class PipeManager
{
private:
    Border border;
public:
    static const int CountPipes = 10;
    Pipe pipes[CountPipes];
    float speedMove;

    PipeManager() {}
    PipeManager(float speed, int heightCrotch, Border border)
    {
        srand(time(NULL));
        this->border = border;
        speedMove = speed;
        for (int i = 0; i < CountPipes; i++)
        {
            pipes[i] = Pipe();
            pipes[i].position = Vector(60 + i * 20, rand() % 10 + 1);
            pipes[i].heightCrotch = heightCrotch;
        }
    }

    void MovePipes()
    {
        Vector vector = { -1,0 };
        for (int i = 0; i < CountPipes; i++)
        {
            if (pipes[i].position.x > 0)
            {
                pipes[i].Move(Vector::Mult(vector, speedMove));
            }
            else
            {
                pipes[i].position = Vector(CountPipes * 20, rand() % 10 + 1);
            }
        }
    }
};

class Bird : public GameObject
{
private:
    Vector oldPosition;
public:

    string sprite;
    Vector position;
    Vector velocity;

    Bird()
    {
        position = Vector();
        velocity = Vector();
        sprite = "-@>";
    }

    void AddForce(Vector force)
    {
        velocity = Vector::Add(velocity, force);
    }

    void ForceApply()
    {
        oldPosition = position;
        position = Vector::Add(position, velocity);
    }

    void DrawSelf(HANDLE handle, int w, int h)
    {
        if ((position.y > 0) && (position.y < h - 1))
        {
            COORD pos;

            pos.X = oldPosition.x - 1;
            pos.Y = oldPosition.y;
            SetConsoleCursorPosition(handle, pos);
            cout << "   ";

            pos.X = position.x - 1;
            pos.Y = position.y;
            SetConsoleCursorPosition(handle, pos);
            cout << sprite;
        }
    }
};

class Game
{
private:
    Border borderGame;
public:

    vector <GameObject*> gameObjects;

    Game(Border border)
    {
        borderGame = border;
    }

    void Update(HANDLE handle)
    {
        for (int i = 0; i < gameObjects.size(); i++)
        {
            gameObjects[i]->DrawSelf(handle, borderGame.width, borderGame.height);
        }

        Sleep(60);
    }
};

void DrawDisplay(HANDLE handle, string typeDisplay, int score)
{
    SetConsoleTextAttribute(handle, 4);
    COORD pos = { 40, 5 };
    if (typeDisplay == "GAME")
    {
        pos.X = 45;
        pos.Y = 2;
        SetConsoleCursorPosition(handle, pos);
        cout << "YOUR SCORE = " << score;
    }
    else
    {
        string ItemText[3] = { typeDisplay, "PLAY", "HELP" };
        COORD pos = { 40, 5 };

        for (int i = 0; i < 6; i++)
        {
            pos.X = 40;
            pos.Y += 1;
            SetConsoleCursorPosition(handle, pos);
            cout << "|-----------------------------------|";
            if (i < 3)
            {
                pos.X = 55;
                pos.Y += 1;
                SetConsoleCursorPosition(handle, pos);
                if (i > 0)
                {
                    cout << i << ". " << ItemText[i];
                }
                else
                {
                    cout << ItemText[i];
                }
            }
            if (typeDisplay == "GAMEOVER" && i == 4)
            {
                pos.X = 45;
                pos.Y += 1;
                SetConsoleCursorPosition(handle, pos);
                cout << "YOUR SCORE = " << score;
            }

        }
    }
    SetConsoleTextAttribute(handle, 2);
}

enum Keys
{
    Space = 32,
    Esc = 27,
    Zero = 48,
    One = 49,
    Two = 50
};

int main()
{
    Bird bird;
    Border border = { 100, 25, '*' };
    Vector forceJump = { 0, -1 };
    Vector gravity = { 0, 0.1 };
    Game game = { border };
    PipeManager pipeManager;
    string typeDisplay = "MENU";
    bool IsGame = false;
    int score = 0;
    int countInit = 0;
    bird.position = { 5, 1 };

    for (int i = 0; i < pipeManager.CountPipes; i++)
    {
        game.gameObjects.push_back(&pipeManager.pipes[i]);
    }
    game.gameObjects.push_back(&bird);
    HANDLE hStdout;
    COORD pos;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    SetConsoleTitle(L"Игра Владислава Романюка ");
    SetConsoleTextAttribute(hStdout, 2);

    do
    {
        if (_kbhit())
        {
            switch (_getch())
            {
            case Space:
                bird.velocity.y = 0;
                bird.AddForce(forceJump);
                break;

            case Esc:
                system("cls");
                return 0;

            case One:
                system("cls");
                score = 0;
                IsGame = true;
                typeDisplay = "GAME";
                if (countInit < 1)
                {
                    bird.position = { 5, 1 };
                    pipeManager = PipeManager(1, 5, border);
                    countInit++;
                }
                break;

            case Two:
                break;
            }
        }
        DrawDisplay(hStdout, typeDisplay, score);
        if (IsGame)
        {
            pipeManager.MovePipes();
        
            for (int i = 0; i < pipeManager.CountPipes; i++)
            {
                if (bird.position.x == pipeManager.pipes[i].position.x
                    && (bird.position.y < pipeManager.pipes[i].y1 ||
                        bird.position.y > pipeManager.pipes[i].y2))
                {
                    system("cls");
                    IsGame = false;
                    typeDisplay = "GAMEOVER";
                    countInit = 0;
                }

                if (bird.position.x == pipeManager.pipes[i].position.x
                    && (bird.position.y > pipeManager.pipes[i].y1 ||
                        bird.position.y < pipeManager.pipes[i].y2))
                {
                    if (IsGame)
                    {
                        score++;
                    }
                }
            }
            bird.AddForce(gravity);
            border.DrawSelf(hStdout, border.width, border.height);
            game.Update(hStdout);
            bird.ForceApply();
        }
    } while (true);
    system("pause");
}