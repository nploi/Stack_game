#include "game.h"

void Game::runLR() {
    int top = 15, topTemp = 15;
    int size = SIZE;
    int color = indexColor;
    color++;
    while (true) {
        bool odd = arrBlock.size() % 2 == 0;
        Block block(size, colors[color]);

        while (!stop && odd) {
            goRight(block, top);
            goLeft(block, top);
        }
        while (!stop && !odd) {
            goLeft(block, top);
            goRight(block, top);
        }

        if(stop) {
            block.destroy();
            gameOver = insertBlock(block);
            if(gameOver) {
                arrBlock.push_back(block);
                printBlocks(topTemp);
                return;
            }

            if(top == 7) {
                topTemp--;
            } else {
                top--;
                topTemp = top;
            }
           printBlocks(topTemp++);
            color--;
            if(color < 0) {
                color = 14;
            }

            stop = false;
            size = arrBlock[arrBlock.size() - 1].getSize();
        }
    }
}

void Game::getInput() {

    do {
        char c;
        util::getchar(c);
        if(gameOver) {
            return;
        }
        switch (c) {
            case 32:
                stop = true;
                break;
//            case 'e':
//                return;
        }
		util::sleep(10);

    } while (true);
}

void Game::init() {
    indexColor = util::random(0, 12);
    Block defaultBlock(SIZE, colors[indexColor]);
    defaultBlock.move(15, 15);
    insertBlock(defaultBlock);
    printBlocks(15);
    stop = false;
    speed = 35;
    gameOver = false;
}

void Game::printBlocks(int top) {
	util::clear();
	for (int i = arrBlock.size() - 1; i >= 0; --i) {
        arrBlock[i].move(arrBlock[i].getX(), ++top);
        arrBlock[i].display();
        if(!gameOver && top > 15) {
            break;
        }
    }

    util::gotoxy(0, top + 1);
    //cout << Color::color(colors[indexColor]) ;
    for (int i = 0; i < SIZE_OF_TABLE; i++) {
		cout << BLOCK;
	}
    util::gotoxy(59, 7);
    cout << "[PRESS SPACE]\n";

#ifdef _WIN32
	util::gotoxy(55, 8);
	cout << "|==================|\n";
	util::gotoxy(55, 9);
	cout << "|YOUR SCORE:       |\n";
	util::gotoxy(55, 10);
	cout << "|==================|\n";
#else
	util::gotoxy(55, 8);
	cout << "╔══════════════════╗\n";
	util::gotoxy(55, 9);
	cout << "║YOUR SCORE:       ║\n";
	util::gotoxy(55, 10);
	cout << "╚══════════════════╝\n";
#endif // _WIN32

    util::gotoxy(70, 9);
    cout << Color::color(Color::Code::FG_WHITE);
    cout << ((gameOver) ? arrBlock.size() - 2 : arrBlock.size() - 1);
    if(gameOver) {
            util::gotoxy(55, 11);
        cout << "Any key to exit." << endl;
    }
}

void Game::start() {
    init();
    thread thread1(&runLR);
    thread thread2(&getInput);
    thread1.join();
    thread2.join();
}

bool Game::removeExcess(Block &block) {
    int size = arrBlock.size();

    if(size == 0) {
        return false;
    }

    int start1 = block.getX();
    int end1 = start1 + block.getSize();
    int start2 = arrBlock[size - 1].getX();
    int end2 = start2 + arrBlock[size - 1].getSize();

    if (end1 <= start2) {
        return true;
    }

    if (start1 >= end2) {
        return true;
    }

    int num = start2 - start1;
    if(num > 0) {
        block.removeLeft(num);
    }

    num = end1 - end2;
    if(num > 0) {
        block.removeRight(num);
    }
    return false;
}

bool Game::insertBlock(Block &block) {
    bool result = removeExcess(block);
    if(!result) {
        arrBlock.push_back(block);
    }
    return result;
}

void Game::goRight(Block &block, int top) {
    // Move block to right
    for (int i = 0; i + block.getSize() <= SIZE_OF_TABLE && !stop; ++i) {
		util::sleep(50);
        block.move(i, top);
        block.display();
        util::remove(i, top);
    }
}

void Game::goLeft(Block &block, int top) {
    // Move block to left
    for (int i = SIZE_OF_TABLE - block.getSize(); i >= 0 && !stop; --i) {
        util::sleep(50);
        block.move(i, top);
        block.display();
		util::remove(i + block.getSize() - 1, top);
    }
}
