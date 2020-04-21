#include <iostream>
#include <string>
#include <ctime>
#include <time.h>
#include <fstream>
#include <sstream>
#include<vector>
#include<Windows.h>
#include <SFML\Graphics.hpp>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

using namespace std;
using namespace sf;
char tableValues[15][15];
char tempTableValues[15][15];

Sprite keysLayout[100];
Vector2f oldPosition[100];
Vector2f newPosition[100];
int RackCompleteCheck = 0;
int positionsUsed[100];
int positionIndex = 1;
int firstMove = 1;
int positionCounter;
Vector2f undoPositions[7];
char Alp[100] = {};
#define No_Of_Alphabets 26 
const int CASE = 'a';

int premiumWords(int z)
{
	int dletter[] = { 3, 11, 45, 165, 59, 179, 52, 36, 38, 108, 92, 122, 172, 186, 188, 116, 102, 132, 96, 98, 126, 128 };
	int tletter[] = { 20, 24, 76, 136, 200, 204, 88, 148, 80, 84, 140, 144 };
	int dword[] = { 16, 32, 48, 64, 28, 42, 56, 70, 112, 196, 182, 168, 154, 208, 192, 176, 160 };
	int tword[] = { 0, 7, 14, 105, 119, 210, 217, 224 };

	for (int i = 0; i < 22; i++)
	{
		if (z == dletter[i])
			return 1;
	}
	for (int i = 0; i < 12; i++)
	{
		if (z == tletter[i])
			return 2;
	}
	for (int i = 0; i < 17; i++)
	{
		if (z == dword[i])
			return 3;
	}
	for (int i = 0; i < 8; i++)
	{
		if (z == tword[i])
			return 4;
	}
	return 0;
		
	}

struct TriesTheNode
{
	TriesTheNode* parentNode = NULL;
	TriesTheNode* childNode[No_Of_Alphabets];
	bool isEndOfWord = 0;
	int index = 0;
	TriesTheNode()
	{
		for (int i = 0; i < No_Of_Alphabets; i++)
			childNode[i] = { 0 };
	}
};

class Tries
{
public:
	void Insertion(TriesTheNode* ThreeTree, char* iWord)
	{
		TriesTheNode* TrieNode = ThreeTree;
		while (*iWord != '\0')
		{
			if (TrieNode->childNode[*iWord - CASE] == NULL)
			{
				TrieNode->childNode[*iWord - CASE] = new TriesTheNode();
				TrieNode->childNode[*iWord - CASE]->parentNode = TrieNode;
			}
			TrieNode = TrieNode->childNode[*iWord - CASE];
			iWord++;
		}
		++TrieNode->index;
	}

	TriesTheNode* search(TriesTheNode* ThreeTree, char* iWord)
	{
		while (*iWord != '\0')
		{
			{
				if (ThreeTree->childNode[*iWord - CASE] != NULL)
				{
					ThreeTree = ThreeTree->childNode[*iWord - CASE];
					++iWord;
				}
				else
					return NULL;
			}
		}
		return (ThreeTree->index != 0) ? ThreeTree : NULL;

	}
};

void alphabets()
{
	char CharOfTile[27] = { ' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
	int FreqOfTile[27] = { 2, 9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1 };
	
	int ind = 0;
	for (int i = 0; i < 27; i++)
	{
		for (int j = 0; j < FreqOfTile[i]; j++)
		{
			Alp[ind++] = CharOfTile[i];
		}
	}
}

void loadImages(Texture *keys, Texture &table, Sprite &tableLayout, Texture &background, Sprite &backgroundlayout, Texture &DoneButton, Sprite &DoneButtonLayout1, Sprite &DoneButtonLayout2, Texture &ScoreTile, Sprite &ScoreTileLayout1, Sprite &ScoreTileLayout2, Texture &Title, Sprite &TitleLayout, Font &text, Text &text1, Text &text2, Text &text3, Text &text4, Text &text5,Texture &finalSheet, Sprite &finalSheetLayout, Texture &pass, Sprite &passLayout)
{
	text.loadFromFile("arial.ttf");
	text1.setFont(text);
	text2.setFont(text);
	text3.setFont(text);
	text4.setFont(text);
	text5.setFont(text);

	table.loadFromFile("Board.jpg");
	tableLayout.setTexture(table);
	background.loadFromFile("Background.jpg");
	backgroundlayout.setTexture(background);
	DoneButton.loadFromFile("Done.jpg");
	DoneButtonLayout1.setTexture(DoneButton);
	DoneButtonLayout2.setTexture(DoneButton);
	ScoreTile.loadFromFile("ScoreTile.jpg");
	ScoreTileLayout1.setTexture(ScoreTile);
	ScoreTileLayout2.setTexture(ScoreTile);
	Title.loadFromFile("Title.jpg");
	TitleLayout.setTexture(Title);
	finalSheet.loadFromFile("FinalSheet.jpg");
	finalSheetLayout.setTexture(finalSheet);
	pass.loadFromFile("passTurn.jpg");
	passLayout.setTexture(pass);

	if (!keys[0].loadFromFile("0.jpg"))
		cout << "0 error";
	if (!keys[1].loadFromFile("0.jpg"))
		cout << "0 error";
	for (int i = 2; i < 11; i++)
		if (!keys[i].loadFromFile("A.jpg"))
			cout << "A error";
	if (!keys[11].loadFromFile("B.jpg"))
		cout << "B error";
	if (!keys[12].loadFromFile("B.jpg"))
		cout << "B error";
	if (!keys[13].loadFromFile("C.jpg"))
		cout << "C error";
	if (!keys[14].loadFromFile("C.jpg"))
		cout << "C error";
	for (int i = 15; i < 19; i++)
		if (!keys[i].loadFromFile("D.jpg"))
			cout << "D error";
	for (int i = 19; i < 31; i++)
		if (!keys[i].loadFromFile("E.jpg"))
			cout << "E error";
	if (!keys[31].loadFromFile("F.jpg"))
		cout << "F error";
	if (!keys[32].loadFromFile("F.jpg"))
		cout << "F error";
	for (int i = 33; i < 36; i++)
		if (!keys[i].loadFromFile("G.jpg"))
			cout << "G error";
	if (!keys[36].loadFromFile("H.jpg"))
		cout << "H error";
	if (!keys[37].loadFromFile("H.jpg"))
		cout << "H error";
	for (int i = 38; i < 47; i++)
		if (!keys[i].loadFromFile("I.jpg"))
			cout << "I error";
	if (!keys[47].loadFromFile("J.jpg"))
		cout << "J error";
	if (!keys[48].loadFromFile("K.jpg"))
		cout << "K error";
	for (int i = 49; i < 53; i++)
		if (!keys[i].loadFromFile("L.jpg"))
			cout << "L error";
	if (!keys[53].loadFromFile("M.jpg"))
		cout << "M error";
	if (!keys[54].loadFromFile("M.jpg"))
		cout << "M error";
	for (int i = 55; i < 61; i++)
		if (!keys[i].loadFromFile("N.jpg"))
			cout << "N error";
	for (int i = 61; i < 69; i++)
		if (!keys[i].loadFromFile("O.jpg"))
			cout << "O error";
	if (!keys[69].loadFromFile("P.jpg"))
		cout << "P error";
	if (!keys[70].loadFromFile("P.jpg"))
		cout << "P error";
	if (!keys[71].loadFromFile("Q.jpg"))
		cout << "Q error";
	for (int i = 72; i < 78; i++)
		if (!keys[i].loadFromFile("R.jpg"))
			cout << "R error";
	for (int i = 78; i < 82; i++)
		if (!keys[i].loadFromFile("S.jpg"))
			cout << "S error";
	for (int i = 82; i < 88; i++)
		if (!keys[i].loadFromFile("T.jpg"))
			cout << "T error";
	for (int i = 88; i < 92; i++)
		if (!keys[i].loadFromFile("U.jpg"))
			cout << "U error";
	if (!keys[92].loadFromFile("V.jpg"))
		cout << "V error";
	if (!keys[93].loadFromFile("V.jpg"))
		cout << "V error";
	if (!keys[94].loadFromFile("W.jpg"))
		cout << "W error";
	if (!keys[95].loadFromFile("W.jpg"))
		cout << "W error";
	if (!keys[96].loadFromFile("X.jpg"))
		cout << "X error";
	if (!keys[97].loadFromFile("Y.jpg"))
		cout << "Y error";
	if (!keys[98].loadFromFile("Y.jpg"))
		cout << "Y error";
	if (!keys[99].loadFromFile("Z.jpg"))
		cout << "Z error";
	for (int i = 0; i < 100; i++)
	{
		keysLayout[i].setTexture(keys[i], true);
	}
}

void tempArray(Vector2f *oldPosition, Vector2f *newPosition, int &pos)
{
	if (newPosition[pos].x > 37 && newPosition[pos].x < 629 && newPosition[pos].y > 3 && newPosition[pos].y < 630)
	{

		for (int i = 0; i < 15; i++)
		{
			for (int j = 1; j < 16; j++)
			{

				int X = (36 * j) + 2.5 + (4 * (j - 1));
				int Y = (36 * i) + 4 + (4 * i);
				if (X == newPosition[pos].x  && Y == newPosition[pos].y)
				{
					tempTableValues[i][j - 1] = Alp[pos];
				}
			}
		}
	}

}

void movement(Event &event, Vector2f *oldPosition, int &pos, Vector2f *newPosition, int &firstPlayer)
{
	int temp1 = 0;
	int temp2 = 37;
	if (firstPlayer == 2)
	{
		temp1 = 634;
		temp2 = 670;
	}

	if (event.type == Event::MouseButtonPressed)
	{
		if (event.key.code == Mouse::Left)
		{
			for (int i = 0; i < 100; i++)
			{
				Vector2f X = keysLayout[i].Transformable::getPosition();
				if (temp1 <= event.mouseButton.x && temp2 > event.mouseButton.x)
				{
					if (X.x <= event.mouseButton.x && (X.x + 37) > event.mouseButton.x && X.y <= event.mouseButton.y && (X.y + 37) > event.mouseButton.y)
					{
						oldPosition[i] = keysLayout[i].getPosition();
						pos = i;

						break;
					}
				}
			}

			if (((0 <= event.mouseButton.x && 50 > event.mouseButton.x) || (620 <= event.mouseButton.x && 670 > event.mouseButton.x)) && 600 <= event.mouseButton.y && 650 > event.mouseButton.y)
			{
				if (firstPlayer == 1)
				{
					RackCompleteCheck = 1;
				}
				else
				{
					RackCompleteCheck = 2;
				}
			}
			if (0 <= event.mouseButton.x && 20 > event.mouseButton.x && 580 <= event.mouseButton.y && 600 > event.mouseButton.y)
			{
				if (firstPlayer == 1)
				{
					firstPlayer = 2;
				}
				else
				{
					firstPlayer = 1;
				}
			}
		}
	}
	if (event.type == Event::MouseButtonReleased)
	{
		for (int i = 0; i < 100; i++)
		{
			if (keysLayout[i].Transformable::getPosition() == Vector2f(318, 284))
			{
				firstMove = 2;
				break;
			}
			else
				firstMove = 1;
		}
		for (int i = 0; i < 15; i++)
		{
			for (int j = 1; j < 16; j++)
			{
				int a = (36 * j) + 2 + (4 * (j - 1));
				int b = (36 * i) + 4 + (4 * i);
				if ((a <= event.mouseButton.x && event.mouseButton.x < (a + 36)) && (b <= event.mouseButton.y && event.mouseButton.y < (b + 40)) && (-1<pos))
				{
					if (firstMove == 1)
					{
						if (i == 7 && j == 8)
						{
							newPosition[pos].x = a;
							newPosition[pos].y = b;
							keysLayout[pos].setPosition(newPosition[pos]);
							tempArray(oldPosition, newPosition, pos);
							pos = -1;
							firstMove = 2;
						}
						
					}
					else
					{
						int c = (36 * j) + 2 + (4 * (j - 1));
						int d = (36 * i) + 4 + (4 * i);
						int check = 0;
						for (int i = 0; i < 100; i++)
						{
							if (keysLayout[i].Transformable::getPosition() == Vector2f(c, d))
								check = 1;
						}
						if (check == 0)
						{
							for (int z = 0; z < 100; z++)
							{
								if (keysLayout[z].Transformable::getPosition() == Vector2f(c - 40, d) || keysLayout[z].Transformable::getPosition() == Vector2f(c + 40, d) || keysLayout[z].Transformable::getPosition() == Vector2f(c, d - 40) || keysLayout[z].Transformable::getPosition() == Vector2f(c, d + 40))// c amd d are x y pixel coordinates to be moved tile
								{
									newPosition[pos].x = a;
									newPosition[pos].y = b;
									keysLayout[pos].setPosition(newPosition[pos]);
									tempArray(oldPosition, newPosition, pos);

									pos = 0;
									break;
								}
							}
						}
					}
					break;
				}
			}
		}
	}
}

class graphicTile
{
private:
	int xCoordinate;
	int yCoordinate;
public:
	graphicTile()
	{
	}
	void setX(int x)
	{
		xCoordinate = x;
	}
	void setY(int y)
	{
		yCoordinate = y;
	}
	int getX()
	{
		return xCoordinate;
	}
	int getY()
	{
		return yCoordinate;
	}

};

class Tile
{
private:
	char letter;
	int value;

public:
	graphicTile OneTile;
	int serial;
	Tile()
	{
	}
	Tile(int v, char l, int x, int y, int s)
	{
		letter = l;
		value = v;
		OneTile.setX(x);
		OneTile.setY(y);
		serial = s;
	}
	void setPosition(int x, int y)
	{
		OneTile.setX(x);
		OneTile.setY(y);
	}

	char getLetter()
	{
		return letter;
	}

	int getValue()
	{
		return value;
	}

};

class BagOfTiles
{
private:
	int NoOfTiles;
public:
	Tile* AllTiles[100];
	BagOfTiles()
	{
		char CharOfTile[27] = { '0', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
		int FreqOfTile[27] = { 2, 9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1 };
		int ValueOfTile[27] = { 0, 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10 };


		int index = 0;
		for (int i = 0; i < 27; i++)
		{
			for (int j = 0; j < FreqOfTile[i]; j++)
			{
				AllTiles[index++] = new Tile(ValueOfTile[i], CharOfTile[i], -35, -35, index);
			}
		}
		NoOfTiles = 100;
	}


	int getNoOfTiles()
	{
		return NoOfTiles;
	}

	Tile* getTile(int &a, int index)
	{

		if (NoOfTiles != 0)
		{
			srand((unsigned)time(0));

			int x = rand() % 100;
		//	if (positionIndex != 1)
		//	{
				for (int i = 0; i < positionIndex - 1; i++)
				{
					while(x == positionsUsed[i]  || x == 0 || x == 1)
					{
						x = rand() % 100;
						i = 0;
					}
				}
	//		}
			a = x;
			Tile* temp = AllTiles[x]; //making a temp tile equal to a random tile
			positionsUsed[positionIndex - 1] = x; // random no in global check array
			positionIndex++;
			NoOfTiles--;
			return temp;
		}
		else
			return NULL;
	}

	void showAllBag()
	{
		cout << "Tiles in Bag" << endl;
		for (int i = 0; i < NoOfTiles; i++)
		{
			cout << i << " " << AllTiles[i]->getLetter() << endl;
		}
	}
};

class RackOfTiles
{
private:
public:
	int x[7];
	Tile* PlayerTile[7];

	RackOfTiles()
	{
	}

	void completeRack1(BagOfTiles &bag)
	{
		for (int i = 0; i < 7; i++)
		{
			if (keysLayout[x[i]].Transformable::getPosition().x != 0)
			{
				PlayerTile[i] = bag.getTile(x[i], i);
				PlayerTile[i]->setPosition(0, 39 + (i * 40));
				keysLayout[x[i]].setPosition(0, 39 + (i * 40));
				oldPosition[x[i]].x = 0;
				oldPosition[x[i]].y = 39 + (i * 40);
			}
		}
	}

	void completeRack2(BagOfTiles &bag)
	{
		for (int i = 0; i < 7; i++)
		{
			if (keysLayout[x[i]].Transformable::getPosition().x != 635)
			{
				PlayerTile[i] = bag.getTile(x[i], i);
				PlayerTile[i]->setPosition(635, 39 + (i * 40));
				keysLayout[x[i]].setPosition(635, 39 + (i * 40));
				oldPosition[x[i]].x = 635;
				oldPosition[x[i]].y = 39 + (i * 40);
			}
		}
	}

	void makeRack1(BagOfTiles &bag)
	{
		for (int i = 0; i < 7; i++)
		{
			PlayerTile[i] = bag.getTile(x[i], i);
			PlayerTile[i]->setPosition(0, 39 + (i * 40));
			keysLayout[x[i]].setPosition(0, 39 + (i * 40));
			oldPosition[x[i]].x = 0;
			oldPosition[x[i]].y = 39 + (i * 40);
		}
	}

	void makeRack2(BagOfTiles &bag)
	{
		for (int i = 0; i < 7; i++)
		{
			PlayerTile[i] = bag.getTile(x[i], i);
			PlayerTile[i]->setPosition(635, 39 + (i * 40));
			keysLayout[x[i]].setPosition(635, 39 + (i * 40));
			oldPosition[x[i]].x = 635;
			oldPosition[x[i]].y = 39 + (i * 40);
		}
	}

	~RackOfTiles()
	{
		for (int i = 0; i < 7; i++)
			delete PlayerTile[i];
	}

	void showAllRack()
	{
		cout << "Player Tiles";
		for (int i = 0; i < 7; i++)
		{
			cout << PlayerTile[i]->getLetter() << endl;
		}
	}

};

class Player
{
private:
	int LocationIndex = 0;;
public:
	string wordyword;
	int playerScore = 0;;
	RackOfTiles Rack;
	Vector2f wordLocation[15];

	Player()
	{
		playerScore = 0;
	//	LocationIndex = 0;
	}

	void IntPlayerRack(BagOfTiles &Bag, int x)
	{
		if (x == 1)
			Rack.makeRack1(Bag);
		if (x == 2)
			Rack.makeRack2(Bag);
	}

	void recordingMovement(Vector2f *oldPositions)
	{
		for (int i = 0; i < 7; i++)
		{
			undoPositions[i].x = oldPositions[Rack.x[i]].x;
			undoPositions[i].y = oldPositions[Rack.x[i]].y;
		}
	}

	bool formingWord()
	{
		Vector2f word[15];
		int index = 0;
		wordyword = "";
		LocationIndex = 0;
		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				if (tempTableValues[i][j] != ' ')
				{
					word[index].x = i;
					word[index].y = j;
					index++;
				}
			}
		}
		int tempx = 0;
			for (int i = 0; i < index; i++)
			{
				int y = word[0].x;
				int x = word[0].y;
				if ((word[i].x == word[index - 1].x && index!=1) || tableValues[y][x-1] != ' ' || tableValues[y][x+1] != ' ')
				{
					tempx = 1;
				}
				else
				{
					tempx = 2;
					break;
				}
			}
		int tempy = 0;
			for (int i = 0; i < index; i++)
			{
				int y = word[0].x;
				int x = word[0].y;
				if ((word[i].y == word[index - 1].y && index!=1) || tableValues[y-1][x] != ' ' || tableValues[y+1][x] != ' ')
				{
					tempy = 1;
				}
				else
				{
					tempy = 2;
					break;
				}
			}
		if ((tempx == 2 && tempy == 1) )
		{
			int y = word[0].y;
			int j = word[0].x;
			int a = j - 1;
			int b = word[index - 1].x + 1;
			int z = 0;
			if (tableValues[a][y] != ' ' && a > -1)
			{
				int temp = a - 1;
				while (tableValues[temp][y] != ' ' && temp > -1)
				{
					temp--;
				}
				temp++;
				while (temp < a)
				{
					wordyword += tableValues[temp][y];
					wordLocation[LocationIndex].x = temp;
					wordLocation[LocationIndex].y = y;
					LocationIndex++;
					temp++;
				}
				wordyword += tableValues[a][y];
				wordLocation[LocationIndex].x = a;
				wordLocation[LocationIndex].y = y;
				LocationIndex++;
			}

			while (b > j)
			{
				if (tempTableValues[j][y] != ' ')
				{
					wordyword += tempTableValues[j][y];
					wordLocation[LocationIndex].x = j;
					wordLocation[LocationIndex].y = y;
					j++;
					LocationIndex++;
				}
				else
				{
					wordyword += tableValues[j][y];
					wordLocation[LocationIndex].x = j;
					wordLocation[LocationIndex].y = y;
					j++;
					LocationIndex++;
				}
			}
			if (tableValues[b][y] != ' ' && index != 15)
			{
				int temp = b + 1;
				while (tableValues[temp][y] != ' ' && temp > -1)
				{
					temp++;
				}
				temp--;
				while (temp > b)
				{
					wordyword += tableValues[temp][y];
					wordLocation[LocationIndex].x = temp;
					wordLocation[LocationIndex].y = y;
					LocationIndex++;
					temp--;
				}
				wordyword += tableValues[b][y];
				wordLocation[LocationIndex].x = b;
				wordLocation[LocationIndex].y = y;
				LocationIndex++;
			}
		}
		if ((tempx == 1 && tempy == 2))
		{
			int x = word[0].x;
			int i = word[0].y;
			int a = word[0].y - 1;
			int b = word[index-1].y + 1;
			if (tableValues[x][a] != ' ' && a > -1)
			{
				int temp = a - 1;
				while (tableValues[x][temp] != ' ' && temp > -1)
				{
					temp--;
				}
				temp++;
				while (temp < a)
				{
					wordyword += tableValues[x][temp];
					wordLocation[LocationIndex].x = x;
					wordLocation[LocationIndex].y = temp;
					LocationIndex++;
					temp++;
				}
				wordyword += tableValues[x][a];
				wordLocation[LocationIndex].x = x;
				wordLocation[LocationIndex].y = a;
				LocationIndex++;
			}

			while (b > i)
			{
				if (tempTableValues[x][i] != ' ')
				{
					wordyword += tempTableValues[x][i];
					wordLocation[LocationIndex].x = x;
					wordLocation[LocationIndex].y = i;
					i++;
					LocationIndex++;
				}
				else
				{
					wordyword += tableValues[x][i];
					wordLocation[LocationIndex].x = x;
					wordLocation[LocationIndex].y = i;
					i++;
					LocationIndex++;
				}
			}
			if (tableValues[x][b] != ' ' && index != 15)
			{
				int temp = b + 1;
				while (tableValues[x][temp] != ' ' && temp > -1)
				{
					temp++;
				}
				temp--;
				while (temp > a)
				{
					wordyword += tableValues[x][temp];
					wordLocation[LocationIndex].x = x;
					wordLocation[LocationIndex].y = temp;
					LocationIndex++;
					temp--;
				}
				wordyword += tableValues[x][b];
				wordLocation[LocationIndex].x = x;
				wordLocation[LocationIndex].y = b;
				LocationIndex++;
			}
		}
		if (tempx == 2 && tempy == 2)
		{
			wordyword = "";
		}
		string passedWord;
		for (int i = 0; i < wordyword.length(); i++)
		{
			if (wordyword[i] == ' ')
				break;
			passedWord += wordyword[i];
		}
		wordyword = passedWord;
		if (validatingWord(wordyword))
		{
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					if (tableValues[i][j] == ' ')
						tableValues[i][j] = tempTableValues[i][j];
					tempTableValues[i][j] = ' ';
				}
			}
			passedWord = "";
			score();
			return true;
		}
		else
		{
			for (int i = 0; i < 7; i++)
			{
				if (keysLayout[Rack.x[i]].Transformable::getPosition() != undoPositions[i])
				{
					keysLayout[Rack.x[i]].setPosition(undoPositions[i]);
					newPosition[Rack.x[i]].x = 0;
					newPosition[Rack.x[i]].y = 0;
				}
			}
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					tempTableValues[i][j] = ' ';
				}
			}
			RackCompleteCheck = 0;
			passedWord = "";
			return false;
		}
	}

	bool validatingWord(string input)
	{
		vector<char> iWord;
		TriesTheNode* ThreeTree = new TriesTheNode();
		ifstream file;
		file.open("20000 words.txt");
		string data;

		Tries word;
		while (file)
		{
			file >> data;
			char* tempWord = (char*)data.c_str();
			word.Insertion(ThreeTree, tempWord);
		}
		for (int i = 0; i < input.length(); i++)
		{
			if (input[i] < 97)
			{
				input[i] += 32;
			}
		}
		char *x = &input[0];
		if (word.search(ThreeTree, x))
		{
			return true;
		}
		else
			return false;
	}

	void score()
	{
		BagOfTiles x;
		int pValue[10];
		int pos = 0;
		for (int i = 0; i < wordyword.length(); i++)
		{
			int k = 0;;
			pos = 0;
			for (int j = 0; j < 15; j++)
			{	
				for (k = 0; k < 15; k++)
				{
					if (wordLocation[i].x == j && wordLocation[i].y == k)
						break;
					pos++;
				}
				if (wordLocation[i].x == j && wordLocation[i].y == k)
					break;
			}
			pValue[i] = premiumWords(pos);
		}
		int Multipier = 1;
		int tempScore = 0;
		for (int z = 0; z < wordyword.length(); z++)
		{
			for (int i = 2; i < 100; i++)
			{
				char character = wordyword[z];
				if (character == x.AllTiles[i]->getLetter())
				{
					if (pValue[z] == 0)
					{
						tempScore += x.AllTiles[i]->getValue();
					}
					if (pValue[z] == 1)
					{
						tempScore += (x.AllTiles[i]->getValue() * 2);
					}
					if (pValue[z] == 2)
					{
						tempScore += (x.AllTiles[i]->getValue() * 3);
					}
					if (pValue[z] == 3)
					{
						tempScore += x.AllTiles[i]->getValue();
						Multipier *= 2;
					}
					if (pValue[z] == 4)
					{
						tempScore += x.AllTiles[i]->getValue();
						Multipier *= 3;
					}
					break;
				}
			}
		}
		tempScore *= Multipier;
		playerScore += tempScore;
		for (int i = 0; i < 15; i++)
		{
			wordLocation[i].x = NULL;
			wordLocation[i].y = NULL;
		}
		cout << playerScore << endl;
	}

};

void game()
{
	alphabets();
	RenderWindow window(VideoMode(670, 650), "Scrabble", Style::Default); //let the shit begin
	Texture background;
	Sprite backgroundlayout;

	Texture DoneButton;
	Sprite DoneButtonLayout1, DoneButtonLayout2;

	Texture ScoreTile;
	Sprite ScoreTileLayout1, ScoreTileLayout2;

	Texture Title;
	Sprite TitleLayout;

	Texture table; //def not needed
	Sprite tableLayout; // not needed after 7 lines.. give or take

	Texture pass;
	Sprite passLayout;

	Texture keys[100]; //not needed after next fntn

	Texture finalSheet;
	Sprite finalSheetLayout;

	Font text;
	Text text1, text2, text3, text4, text5;

	CircleShape shape1(10), shape2(10);

	loadImages(keys, table, tableLayout, background, backgroundlayout, DoneButton, DoneButtonLayout1, DoneButtonLayout2, ScoreTile, ScoreTileLayout1, ScoreTileLayout2, Title, TitleLayout, text, text1, text2, text3, text4,text5, finalSheet, finalSheetLayout, pass, passLayout); //loading every shit

	BagOfTiles Bag; //bag of tiles

	Player P1, P2; //two players
	P1.IntPlayerRack(Bag, 1); //making initial rack for 1
	P2.IntPlayerRack(Bag, 2); //making initial rack for 2
	//deciding wth is gonna play first
	int firstPlayer;
	if (P1.Rack.PlayerTile[0]->getLetter() <= P2.Rack.PlayerTile[0]->getLetter())
	{
		firstPlayer = 1;
		shape1.setFillColor(Color::Green);
		shape2.setFillColor(Color::Red);
	}
	if (P1.Rack.PlayerTile[0]->getLetter() > P2.Rack.PlayerTile[0]->getLetter())
	{
		firstPlayer = 2;
		shape1.setFillColor(Color::Red);
		shape2.setFillColor(Color::Green);
	}

	text1.setString("Player 1:");
	text1.setFillColor(Color::White);
	text1.setStyle(Text::Bold | Text::Underlined);
	text1.setCharacterSize(15);
	text2.setFillColor(Color::White);
	text2.setStyle(Text::Bold);
	text2.setCharacterSize(15);
	text3.setString("Player 2:");
	text3.setFillColor(Color::White);
	text3.setStyle(Text::Bold | Text::Underlined);
	text3.setCharacterSize(15);
	text4.setFillColor(Color::White);
	text4.setStyle(Text::Bold);
	text4.setCharacterSize(15);
	text5.setFillColor(Color::White);
	text5.setStyle(Text::Bold);

	tableLayout.setPosition(Vector2f(35, 0)); //setting scrabble board
	backgroundlayout.setPosition(Vector2f(0, 0)); //setting background
	DoneButtonLayout1.setPosition(Vector2f(0, 600));
	DoneButtonLayout2.setPosition(620, 600);
	ScoreTileLayout1.setPosition(50, 600);
	ScoreTileLayout2.setPosition(520, 600);
	TitleLayout.setPosition(200, 607);
	text1.setPosition(55, 600);
	text2.setPosition(100, 620);
	text3.setPosition(525, 600);
	text4.setPosition(570, 620);
	text5.setPosition(170,215);
	shape1.setPosition(10, 10);
	shape2.setPosition(640, 10);
	finalSheetLayout.setPosition(135,175);
	passLayout.setPosition(0,580);
	//	Score

	for (int i = 0; i < 15; i++) //makes my table of alphabets
	{
		for (int j = 0; j < 15; j++)
		{
			tempTableValues[i][j] = ' ';
			tableValues[i][j] = ' ';
		}
	}

	while (window.isOpen())
	{
		Event event; //everything responsible for
		Vector2i mouseCoordInt;
		//new pos of tile vector
		int pos; //psoition of tile

		//shit done during game
		while (window.pollEvent(event))
		{
			string score1, score2;
			stringstream convert1, convert2;
			convert1 << P1.playerScore;
			convert2 << P2.playerScore;
			score1 = convert1.str();
			score2 = convert2.str();
			text2.setString(score1);
			text4.setString(score2);

			if (firstPlayer == 1)
			{
				shape1.setFillColor(Color::Green);
				shape2.setFillColor(Color::Red);
			}
			if (firstPlayer == 2)
			{
				shape1.setFillColor(Color::Red);
				shape2.setFillColor(Color::Green);
			}

			if (event.type == Event::Closed)
			{
				window.draw(backgroundlayout);
				window.draw(finalSheetLayout);
				text1.setCharacterSize(30);
				text2.setCharacterSize(40);
				text3.setCharacterSize(30);
				text4.setCharacterSize(40);
				text5.setCharacterSize(50);
				text1.setPosition(180,335);
				text2.setPosition(230,380);
				text3.setPosition(365,335);
				text4.setPosition(415,380);
				if (P1.playerScore > P2.playerScore)
				{
					text5.setString("Player 1 Wins!");
				}
				if (P2.playerScore > P1.playerScore)
				{
					text5.setString("Player 2 Wins!");
				}
				if (P1.playerScore == P2.playerScore)
				{
					text5.setString("Its A Tie!");
					text5.setPosition(235,215);
				}
				window.draw(text1);
				window.draw(text2);
				window.draw(text3);
				window.draw(text4);
				window.draw(text5);
				window.display();
				Sleep(5000);

				window.close();
			}

			if (event.type == Event::MouseButtonPressed || event.type == Event::MouseButtonReleased)
			{
				movement(event, oldPosition, pos, newPosition, firstPlayer);
				if (RackCompleteCheck == 1)
				{
					P1.recordingMovement(oldPosition);
					if (P1.formingWord())
					{
						P1.Rack.completeRack1(Bag);
						firstPlayer = 2;
						RackCompleteCheck = 0;
					}
				}
				if (RackCompleteCheck == 2)
				{
					P2.recordingMovement(oldPosition);
					if (P2.formingWord())
					{
						P2.Rack.completeRack2(Bag);
						firstPlayer = 1;
						RackCompleteCheck = 0;
					}
				}
			}
		}

		window.clear();
		//draws on screen after shit
		window.draw(backgroundlayout);
		window.draw(tableLayout);
		window.draw(DoneButtonLayout1);
		window.draw(DoneButtonLayout2);
		window.draw(ScoreTileLayout1);
		window.draw(ScoreTileLayout2);
		window.draw(TitleLayout);
		window.draw(text1);
		window.draw(text2);
		window.draw(text3);
		window.draw(text4);
		window.draw(shape1);
		window.draw(shape2);
		window.draw(passLayout);
		for (int i = 2; i < 100; i++)
		{
			if (keysLayout[i].Transformable::getPosition().y != 0)
				window.draw(keysLayout[i]);
		}
		//displays shit

		window.display();
	}
}

void main()
{
	//start
	game();
	//end
	//bye
}