#include<conio.h>
#include<ctype.h>
#include<dos.h>
#include<fstream.h>
#include<iostream.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define KEY_LEFT 75
#define KEY_RIGHT 77

const int bsize = 10;
const int b2 = bsize*bsize;
const char bs[6] = {1,2,4,79,252,254};
int head = 1;
int body = 0;
int speed = 1;

class Board {
		int board[b2];
		int noFood;
	public:
		Board() {
			noFood = 2;
			for (int i = 0; i < b2; i++)
				board[i] = 0;
			for (i = 0; i < noFood; i++) 
				putFood();
		}
		int isFood(int tile) {
			if (board[tile - 1]) {
				putFood();
				board[tile - 1] = 0;
				return 1;
			}
			return 0;
		}
		void putFood() {
			int t_rand;
			do {
				srand(time(NULL));
				t_rand = rand() % b2;
			} while (board[t_rand] == 1);
			board[t_rand] = 1;
		}
		void ShowBoard(int tiles[], int length) {
			int tempBoard[b2];
			for (int i = 0; i < b2; i++) {
				tempBoard[i] = board[i];
			}
			tempBoard[tiles[0]-1] = 2;
			for (i = 1; i < length; i++) {
				tempBoard[tiles[i] - 1] = 3;
			}
			clrscr(); //Clear Screen
			cout << "- - - - - - - - - - - - LENGTH: "<<length<<" PRESS ANY KEY TO PAuse";
			for (i = 0; i < bsize; i++) {
				cout << "\n| ";
				for (int j = 0; j < bsize; j++) {
					int pos = 10 * i + j;
					switch (tempBoard[pos]) {
						case 0:
							cout << "  ";
							break;
						case 1:
							cout << "* ";
							break;
						case 2:
							cout<<bs[head]<<' ';
							break;
						case 3:
							cout<<bs[body]<<' ';
							break;
					}
				}
				cout << "|";
			}
			cout << "\n- - - - - - - - - - - -";
		}
};

class Snake {
		int length;
		int tiles[b2];
		int alive;
		Board board;
		void pushTile(int headPos, int isFood) {
			int i = length;
			for (int j = 1; j < i; j++) {
				if (tiles[j] == headPos) {
					alive = 0;
					return;
				}

			}
			for (j = i; j > 0; j--) {
				tiles[j] = tiles[j - 1];
			}
			tiles[0] = headPos; 
			length++;
			if (isFood == 0) {
				tiles[i] = 0;				length--;
			}
			board.ShowBoard(tiles, length);
		}
	public:
		Snake() {
			length = 1;
			for (int i = 0; i < b2; i++) {
				tiles[i] = 0;
			}
			tiles[0] = 50;
			alive = 1;
		}

		int Move(char direction) {
			int headPos = tiles[0];
			switch (tolower(direction)) {
				case 'w': //Move UP
					if (headPos > bsize)
						headPos -= bsize;
					else
						headPos += bsize*(bsize-1); 
					break;
				case 's':  //Move DOWN
					if (headPos <= bsize*(bsize-1))
						headPos += bsize;
					else
						headPos -= bsize*(bsize-1); 
					break;
				case 'd': //Move Right
					if (headPos % bsize != 0)
						headPos += 1;
					else
						headPos -= bsize - 1; 
					break;
				case 'a': //Move Left
					if (headPos % bsize != 1)
						headPos -= 1;
					else
						headPos += bsize - 1;
					break;
				default:
					return 1;
			}
			pushTile(headPos, board.isFood(headPos));
			return alive;
		}

		void GameOver(){
			while(1){
				if(kbhit())
					break;
				clrscr();
				cout<<"GAME OVER";
				delay(250);
				board.ShowBoard(tiles,length);
				delay(250);
			}
		}

		int returnLength(){
		      return length;
		}
};
 

class HighScore{
	public:
		char name[4];
		int highscore;
	      void getdata(){
			fstream x;
			HighScore ob;
			x.open("highscores.dat", ios::in | ios::out | ios::binary);
			cout<<"NAME \t| SCORE";
			x.read((char*)&ob, sizeof(ob));
			while(!x.eof()){
				cout<<endl<<ob.name<<" \t| "<<ob.highscore;
				x.read((char*)&ob, sizeof(ob));
			}
			x.close();
	      }
	      int CheckIfHigh(int Score){
			int isHigh = 0;
			fstream x;
			HighScore ob;
			x.open("highscores.dat", ios::in | ios::binary);
			x.read((char*)&ob, sizeof(ob));
			while(!x.eof()){
				if(Score>=ob.highscore){
					break;
				}
				isHigh++;
				x.read((char*)&ob, sizeof(ob));
			}
			x.close();
			return isHigh;
	      }
	      void putdata(int pos, HighScore hs){
			int t = 0;
			fstream x, y;
			HighScore ob;
			x.open("highscores.dat", ios::in | ios::out | ios::binary);
			y.open("temphighscores.dat", ios::in | ios::out | ios::binary);
			x.read((char*)&ob, sizeof(ob));
			while((!x.eof() || t == pos) && t < 10){
				if(t!=pos){
					y.write((char*)&ob, sizeof(ob));
					x.read((char*)&ob, sizeof(ob));
				} else {
					y.write((char*)&hs, sizeof(ob));
				}
				t++;
			}
			x.close();
			y.close();
			remove("highscores.dat");
			rename("temphighscores.dat", "highscores.dat");

	      }

};

void displayHighscore(){
	HighScore temp;
	clrscr();
	temp.getdata();
	getch();
}

void highscore(int length){
	HighScore hs;
	int pos = hs.CheckIfHigh(length);
	if(pos < 10){
		cout<<"Enter name:";
		cin>>hs.name;
		hs.highscore = length;
		hs.putdata(pos, hs);
	}
	displayHighscore();
	getch();
}

void setLevel(){
    int c = 0;
    int selected = 1;
    while(selected){
	clrscr();
	cout<<"Level = "<<speed;
	cout<<"\nPRESS ARROW KEYS TO CHANGE LEVEL";
	cout<<"\nPRESS Q TO ENTER";
	c = 0;

	switch((c=getch())) {
		case KEY_LEFT:
		    if(speed > 0){
			speed--;
		    }
		    break;
		case KEY_RIGHT:
		    if(speed < 3){
			speed++;
		    }
		    break;
		case 81:
		case 113:
		    selected = 0;
		    break;
	}
   }
}

void setHead(){
    int c = 0;
    int selected = 1;
    while(selected){
	clrscr();
	cout<<"Head = "<<bs[head];
	cout<<"\nPRESS ARROW KEYS TO CHANGE HEAD";
	cout<<"\nPRESS Q TO ENTER";
	c = 0;

	switch((c=getch())) {
		case KEY_LEFT:
		    if(head > 0){
			head--;
		    }
		    break;
		case KEY_RIGHT:
		    if(head < 6){
			head++;
		    }
		    break;
		case 81:
		case 113:
		    selected = 0;
		    break;
	}
   }
}

void setBody(){
    int c = 0;
    int selected = 1;
    while(selected){
	clrscr();
	cout<<"Body = "<<bs[body];
	cout<<"\nPRESS ARROW KEYS TO CHANGE BODY";
	cout<<"\nPRESS Q TO ENTER";
	c = 0;

	switch((c=getch())) {
		case KEY_LEFT:
		    if(body > 1){
			body--;
		    }
		    break;
		case KEY_RIGHT:
		    if(body < 6){
			body++;
		    }
		    break;
		case 81:
		case 113:
		    selected = 0;
		    break;
	}
   }
}

void Game(){
	Snake mySnake;
	char t;
	int alive = 1;
	cout << "PRESS w,s,a,d TO START!";
	while (alive) {
		if (kbhit()) {
			t = getch();
		}
		alive = mySnake.Move(t);
		delay(250/speed);//Wait 0.25 Seconds;
	}
	clrscr();
	mySnake.GameOver();
	highscore(mySnake.returnLength());
}

void Settings(){
	int t;
	do{
		clrscr();
		cout<<"1 - Set Level\n2 - Set Head\n3 - Set Body\n4 - Back\n";
		cin>>t;
		switch(t){
			case 1:
				setLevel();
				break;
			case 2:
				setHead();
				break;
			case 3:
				setBody();
				break;
			case 4:
				break;
		}
	}while(t != 4);
}
 

void main(){
	int t;
	do{
		clrscr();
		cout<<"1 - New Game\n2 - Highscore\n3 - Settings\n4 - Quit\n";
		cin>>t;
		switch(t){
			case 1:
				Game();
				break;
			case 2:
				displayHighscore();
				break;
			case 3:
				Settings();
				break;
			case 4:
				break;
		}
	}while(t != 4);	
}
