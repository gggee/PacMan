#include <iostream>
#include <string>       // для строк std::string
#include <Windows.h>    // для Win10.h
#include <time.h>
#include <conio.h>      // для _getch(), _kbhit()
#include "Win10.h"
using namespace std;

//structure for coordinates of participants on the playing field
struct Coord {
	int row, col;
};
// Coord Hero;
// Coord Enemy[];

//playing field - an array of strings of type string
// game frame canvas (Canvas)
// Symbols: & - main player; @ - opponents
// # - wall; 1,2,3,4 - bonuses
// * - tablet/weapon
// B - door; ? - go to the next level
string Pole[]{
  "###################################",
  "#   #*##      @          @        #",
  "# # @  #@############## ######### #",
  "# ###### #   #*#3  @  # ##@##   # #",
  "#  @       # # ###### # #@4   #@# #",
  "###### # ### #      # # ##@##   # #",
  "##  @# # # # #### # #   ##### # # #",
  "#* #   # # @    # ##*######*#   # #",
  "######## # ####         #   # # #@#",
  "#  2#    #      ####### # # #   # #",
  "# ### #### ######       # #   #   #",
  "# @        #      ####### #########",
  "############ ######   #1# # @ #   #",
  "#   &               #   #   #@  #B#",
  "#################################?#"
};
// ru: высота и ширина игрового поля
// height and width of the playing field
int PoleHeight;
int PoleWidth;
// ru: Экранные координаты вывода поля на экран
//Screen coordinates for displaying the field on the screen
int PoleRow = 3;
int PoleCol = 8;
//
int LiveCount = 3; // ru: кол-во жизней, по умолчанию 3
//number of lives, default 3
int Score = 0; // ru: очки игрока 
// player points

// порототипы функций
// function prototypes
void StartGame();
Coord GetHero(char Sym = '&', int Index = 0);
void DrawPole();
void ShowGameInfo();


int main() {
	PoleHeight = sizeof(Pole) / sizeof(Pole[0]);
	PoleWidth = Pole[0].size(); // ru: ширина строки поля
	// field line width

// Main menu of the game

	StartGame(); // function to start a new game
	// ContinueGame(); // function to continue the game after a pause
	// Settings(); // settings
	// About(); // about authors

	cin.get(); cin.get();
} // main()

// Функция для получения массива координат участника игры
// Function for getting an array of coordinates of a game participant
Coord* GetHeroEx(char Sym, int& SizeCoord) {
	SizeCoord = 0;
	// ru: подсчет кол-ва для размера массива
	// count count for array size
	for (int i = 0; i < PoleHeight; i++) {
		for (int j = 0; j < PoleWidth; j++) {
			if (Pole[i][j] == Sym) { SizeCoord++; }
		}
	}
	// создать массив координат (массив структур типа Coord)
	// create an array of coordinates (an array of structures of type Coord)
	Coord* mas = new Coord[SizeCoord];
	int k = 0; // индекс для заполнения массива mas
	for (int i = 0; i < PoleHeight; i++) {
		for (int j = 0; j < PoleWidth; j++) {
			if (Pole[i][j] == Sym) {
				mas[k].row = i;
				mas[k].col = j;
				k++;
			}
		}
	}
	return mas;
} // GetHeroEx( char Sym, int &SizeCoord );

// Функция для получения координаты главного игрока
// Function to get the coordinates of the main player
Coord GetHero(char Sym, int Index) {
	Coord hero{ 0,0 }; //hero.row; hero.col;
	int count = 0;
	for (int i = 0; i < PoleHeight; i++) {
		for (int j = 0; j < PoleWidth; j++) {
			if (Pole[i][j] == Sym) {
				if (count == Index) {
					hero.row = i; // index of the line for the hero
					hero.col = j; // column index for hero
					return hero;
				}
				count++;
			}
		}
	}
	// player not found on the field
	cout << "Error: There is no player on the game map!" << endl;
	return hero;
} // Coord GetHero()

// Function to start a new game
void StartGame() {
	Coord Hero = GetHero(); // получить текущую позицию игрока // get the player's current position
	Coord HeroSpawn = Hero; // координата для появления главного героя игры
	// coordinate for the appearance of the main character of the game
	int key = 0;  // ru: для направления движения игрока
	// 0 - игрок стоит, не перемещается
	// for the direction of the player's movement
	// 0 - the player is standing, not moving
	bool isGameOver = false;
	LiveCount = 3;
	Score = 0;
	while (!isGameOver) {
		int dRow = 0, dCol = 0; // for player movement
		// 1) check for key presses
		if (_kbhit()) {
			key = _getch(); // получить код нажатой клавиши
		} // if(_kbhit())
		switch (key)
		{
		case _KEY::UP:
			if (Pole[Hero.row - 1][Hero.col] != '#') { dRow = -1; }
			break;
		case _KEY::DOWN:
			if (Pole[Hero.row + 1][Hero.col] != '#') { dRow = 1; }
			break;
		case _KEY::LEFT:
			if (Pole[Hero.row][Hero.col - 1] != '#') { dCol = -1; }
			break;
		case _KEY::RIGHT:
			if (Pole[Hero.row][Hero.col + 1] != '#') { dCol = 1; }
			break;
		case _KEY::SPACE: // pause
		case _KEY::ESC:   // exit
			break;
		} // switch()
		if (dRow != 0 || dCol != 0) {
			// движение по вертикали или горизонтали // move vertically or horizontally
			Pole[Hero.row][Hero.col] = ' ';
			Hero.row += dRow;
			Hero.col += dCol;
			// проверки на пересечение игрока с элементами карты
			// checks for the intersection of the player with the elements of the map
			if (Pole[Hero.row][Hero.col] == '@') {
				LiveCount--; // reduce number of lives
				Hero = HeroSpawn; // return the hero to the starting position
				if (LiveCount == 0) {
					isGameOver = true; // Game over
				}
				else {
					Pole[Hero.row][Hero.col] = '&';
				}
			}
			else if (Pole[Hero.row][Hero.col] == '1') {
				Score += 10; // увеличиваем очки
				Pole[Hero.row][Hero.col] = '&';
			}
			else if (Pole[Hero.row][Hero.col] == '2') {
				Score += 20;
				Pole[Hero.row][Hero.col] = '&';
			}
			else if (Pole[Hero.row][Hero.col] == '*') {
				Score += 10;
				LiveCount++;
				Pole[Hero.row][Hero.col] = '&';
			}
			else {
				Pole[Hero.row][Hero.col] = '&';
			}
		} // if (dRow != 0 || dCol != 0)

		// 3)movement of opponents
		for (int i = 0; i < 16; i++) {
			// get the coordinates of the i-th enemy
			Coord Enemy = GetHero('@', i);
			int masDir[4]{}; // array for possible directions
			int cntDir = 0; // number of possible directions
			if (Pole[Enemy.row - 1][Enemy.col] == ' ' ||
				Pole[Enemy.row - 1][Enemy.col] == '&') {
				masDir[cntDir++] = 1;
			}
			if (Pole[Enemy.row + 1][Enemy.col] == ' ' ||
				Pole[Enemy.row + 1][Enemy.col] == '&') {
				masDir[cntDir++] = 2;
			}
			if (Pole[Enemy.row][Enemy.col - 1] == ' ' ||
				Pole[Enemy.row][Enemy.col - 1] == '&') {
				masDir[cntDir++] = 3;
			}
			if (Pole[Enemy.row][Enemy.col + 1] == ' ' ||
				Pole[Enemy.row][Enemy.col + 1] == '&') {
				masDir[cntDir++] = 4;
			}
			// cntDir - кол-во возможных направлений бота
			// masDir - массив возможных направлений бота
			// выбрать случайное направление

			// cntDir - number of possible bot directions
			 // masDir - array of possible bot directions
			 // choose a random direction
			if (cntDir == 0) {
				// if the bot can't move, skip this one
				  // cycle step, go to the next bot
				continue;
			}
			int BotDir = masDir[rand() % cntDir];
			Pole[Enemy.row][Enemy.col] = ' ';
			switch (BotDir) {
			case 1: Enemy.row--; break;
			case 2: Enemy.row++; break;
			case 3: Enemy.col--; break;
			case 4: Enemy.col++; break;
			}
			Pole[Enemy.row][Enemy.col] = '@';
		}
		// 4) rescanning the field on the screen
		DrawPole();
		ShowGameInfo(); // output the number of lives, points
		Sleep(50); // delay at each step of the game loop
	} // while(true)
} // StartGame()

void ShowGameInfo() {
	system("chcp 1251 > nul");
	int ScoreRow = PoleRow + PoleHeight + 2;
	int ScoreCol = PoleCol;
	SetPos(ScoreRow, ScoreCol);
	cout << "Point: " << Score;

	int LiveRow = PoleRow + 1;
	int LiveCol = PoleCol + PoleWidth + 2;
	SetPos(LiveRow, LiveCol);
	cout << "Life: ";
	for (int i = 0; i < LiveCount; i++) {
		cout << char(32) << " ";
	}

	system("chcp 866 > nul");
	SetPos(LiveRow, LiveCol + 7);
	for (int i = 0; i < LiveCount; i++) {
		cout << char(3) << " ";
	}
}

// function of displaying the playing field on the screen
void DrawPole() {
	for (int i = 0; i < PoleHeight; i++) {
		SetPos(PoleRow + i, PoleCol);
		for (int j = 0; j < PoleWidth; j++) {
			if (Pole[i][j] == '#') { SetColor(COLOR::black, COLOR::light_green); cout << char(177); }
			else if (Pole[i][j] == '!') { SetColor(COLOR::black, COLOR::light_yellow); cout << char(178); }
			else if (Pole[i][j] == '1') { SetColor(COLOR::black, COLOR::light_purple); cout << '/'; }
			else if (Pole[i][j] == '@') { SetColor(COLOR::black, COLOR::light_aqua); cout << char(2); }
			else if (Pole[i][j] == 'B') { SetColor(COLOR::black, COLOR::bright_white); cout << char(1); }
			else if (Pole[i][j] == '*') { SetColor(COLOR::black, COLOR::light_red); cout << char(3); }
			else if (Pole[i][j] == '&') { SetColor(COLOR::black, COLOR::light_yellow); cout << char(2); }
			else if (Pole[i][j] == '2') { SetColor(COLOR::black, COLOR::light_purple); cout << '!'; }
			else if (Pole[i][j] == '3') { SetColor(COLOR::black, COLOR::light_purple); cout << char(24); }
			else if (Pole[i][j] == '4') { SetColor(COLOR::black, COLOR::light_purple); cout << '+'; }
			else { cout << Pole[i][j]; }
		}//for j
	}//for i
}//DrawPole
