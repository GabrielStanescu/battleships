#include <ncurses.h>

#include <string.h>

#include <stdlib.h>

#include <time.h>

#include <unistd.h>

void logo() {
	mvwprintw(stdscr,6,0," ______   _______ __________________ _        _______  _______          _________ _______  _______ ");
	mvwprintw(stdscr,7,0,"(  ___ \\ (  ___  )\\__   __/\\__   __/( \\      (  ____ \\(  ____ \\|\\     /|\\__   __/(  ____ )(  ____ \\");
	mvwprintw(stdscr,8,0,"| (   ) )| (   ) |   ) (      ) (   | (      | (    \\/| (    \\/| )   ( |   ) (   | (    )|| (    \\/");
	mvwprintw(stdscr,9,0,"| (__/ / | (___) |   | |      | |   | |      | (__    | (_____ | (___) |   | |   | (____)|| (_____ ");
	mvwprintw(stdscr,10,0,"|  __ (  |  ___  |   | |      | |   | |      |  __)   (_____  )|  ___  |   | |   |  _____)(_____  )");
	mvwprintw(stdscr,11,0,"| (  \\ \\ | (   ) |   | |      | |   | |      | (            ) || (   ) |   | |   | (            ) |  ");
	mvwprintw(stdscr,12,0,"| )___) )| )   ( |   | |      | |   | (____/\\| (____/\\/\\____) || )   ( |___) (___| )      /\\____) |");
	mvwprintw(stdscr,13,0,"|/ \\___/ |/     \\|   )_(      )_(   (_______/(_______/\\_______)|/     \\|\\_______/|/       \\_______)");
	wrefresh(stdscr);	
}

void remove_logo() {
	mvwprintw(stdscr,6,40,"       	            ");
	mvwprintw(stdscr,7,40,"                     ");
	mvwprintw(stdscr,8,40,"                     ");
	mvwprintw(stdscr,9,40,"                     ");
	mvwprintw(stdscr,10,40,"                    ");
	mvwprintw(stdscr,11,40,"                    ");
	mvwprintw(stdscr,12,40,"                    ");
	mvwprintw(stdscr,13,40,"                    ");
}

void meniu(int * highlight, int resume_available) {
	//fereastra meniului
        WINDOW * menuwin = newwin(5, 101, 30, 0);
        keypad(menuwin, true);
        //box(menuwin, 0, 0);
	wborder(menuwin,32,32,0,0,32,32,32,32);
        refresh();
        wrefresh(menuwin);

        char meniu[3][12] = {
                "New Game",
                "Resume Game",
                "Quit"
        };
        int choice;

        while (1) {
                for (int i = 0; i < 3; i++) {
                        if (i == * highlight)
                                wattron(menuwin, A_REVERSE);
			if(meniu[i][0] == 'N')
	                        mvwprintw(menuwin, i + 1, 45, meniu[i]);
			if(meniu[i][0] == 'R')
	                        mvwprintw(menuwin, i + 1, 44, meniu[i]);
                        if(meniu[i][0] == 'Q')
	                        mvwprintw(menuwin, i + 1, 47, meniu[i]);
                        wattroff(menuwin, A_REVERSE);
                }
                choice = wgetch(menuwin);
                switch (choice) {
                case KEY_UP:
                        ( * highlight) --;
                        if ( * highlight == 1 && resume_available == 0)
                                ( * highlight) --;
                        break;
                case KEY_DOWN:
                        ( * highlight) ++;
                        if ( * highlight == 1 && resume_available == 0)
                                ( * highlight) ++;
                        break;
                default:
                        break;
                }
                if ( * highlight == 3)
                        *
                        highlight = 2;
                else if ( * highlight == -1)
                        *
                        highlight = 0;

                if (choice == 10) {
                        break;
                }
        }
}

//construieste hartile goale ale jucatorilor

void build_grids(WINDOW * playergrid, WINDOW * aigrid) {
        for (int i = 1; i <= 39; i++)
                for (int j = 2; j <= 19; j += 2) {
                        mvwprintw(playergrid, j, i, "-");
                        mvwprintw(aigrid, j, i, "-");
                }
        for (int i = 4; i <= 37; i = i + 4)
                for (int j = 1; j <= 19; j += 2) {
                        mvwprintw(playergrid, j, i, "|");
                        mvwprintw(aigrid, j, i, "|");
                }
}

// afiseaza pe grid pozitiile navelor playerului

void load_player_map(WINDOW * playergrid, int ** a, int x) {
        int k = 0;
        for (int i = 1; i <= 20; i += 2) {
                for (int j = 2; j < 39; j += 4) {
                        if (a[x - 1][k] != '0') {
                                mvwprintw(playergrid, i, j, "X");
                        } else
                                mvwprintw(playergrid, i, j, " ");
                        k++;
                }
        }
        wrefresh(playergrid);
}

//selecteaza harta dorita de jucator prin folosirea sagetilor

int map_choose(int argc, WINDOW * playergrid, int ** a) {
        int ok = 1, x = 1, choice;
        WINDOW * mapselect = newwin(2, 70, 27, 0);
        keypad(mapselect, true);
        wborder(mapselect, 32, 32, 32, 32, 32, 32, 32, 32);
        mvwprintw(mapselect, 0, 0, "Map range: [%d-%d]", 1, argc - 1);
        mvwprintw(mapselect, 1, 0, "Use your arrow keys to select a map: ");
        while (ok) {
                mvwprintw(mapselect, 1, 37, "%d", x);
                choice = wgetch(mapselect);
                switch (choice) {
                case KEY_RIGHT:
                        mvwprintw(mapselect, 1, 37, "%d", x++);
                        break;
                case KEY_LEFT:
                        mvwprintw(mapselect, 1, 37, "%d", x--);
                        break;
                }
                if (x < 1)
                        x = 1;
                if (x > argc - 1)
                        x = argc - 1;
                load_player_map(playergrid, a, x);
                wrefresh(mapselect);
                if (choice == 10)
                        return x - 1;
        }
	return 0;
}

//creeaza o fereastra cu cele 2 optiuni

void power_ups() {
        WINDOW * powerups = newwin(3, 70, 27, 0);
        mvwprintw(powerups, 0, 0, "POWER-UPS");
        mvwprintw(powerups, 1, 0, "Randomize map: R");
        mvwprintw(powerups, 2, 0, "Destroy in advance: D");
        wrefresh(powerups);
}

//verifica imprejurimile pe o distanta de 1 inclusiv diagonalele

int surroundings(int init, int ** a) {
        if (init / 10 + 1 <= 9)
                if (a[init / 10 + 1][init % 10] == 1)
                        return 0;
        if (init % 10 + 1 <= 9)
                if (a[init / 10][init % 10 + 1] == 1)
                        return 0;
        if (init / 10 - 1 >= 0)
                if (a[init / 10 - 1][init % 10] == 1)
                        return 0;
        if (init % 10 - 1 >= 0)
                if (a[init / 10][init % 10 - 1] == 1)
                        return 0;
        if (init / 10 + 1 <= 9 && init % 10 + 1 <= 9)
                if (a[init / 10 + 1][init % 10 + 1] == 1)
                        return 0;
        if (init / 10 - 1 >= 0 && init % 10 - 1 >= 0)
                if (a[init / 10 - 1][init % 10 - 1] == 1)
                        return 0;
        if (init / 10 + 1 <= 9 && init % 10 - 1 >= 0)
                if (a[init / 10 + 1][init % 10 - 1] == 1)
                        return 0;
        if (init / 10 - 1 >= 0 && init % 10 + 1 <= 9)
                if (a[init / 10 - 1][init % 10 + 1] == 1)
                        return 0;
        return 1;
}

//completeaza cu 2 spatiile de langa nave

void filler(int pos, int ** a) {
        if (pos / 10 + 1 <= 9)
                if (a[pos / 10 + 1][pos % 10] != 1)
                        a[pos / 10 + 1][pos % 10] = 2;
        if (pos % 10 + 1 <= 9)
                if (a[pos / 10][pos % 10 + 1] != 1)
                        a[pos / 10][pos % 10 + 1] = 2;
        if (pos / 10 - 1 >= 0)
                if (a[pos / 10 - 1][pos % 10] != 1)
                        a[pos / 10 - 1][pos % 10] = 2;
        if (pos % 10 - 1 >= 0)
                if (a[pos / 10][pos % 10 - 1] != 1)
                        a[pos / 10][pos % 10 - 1] = 2;
        if (pos / 10 + 1 <= 9 && pos % 10 + 1 <= 9)
                a[pos / 10 + 1][pos % 10 + 1] = 2;
        if (pos / 10 - 1 >= 0 && pos % 10 - 1 >= 0)
                a[pos / 10 - 1][pos % 10 - 1] = 2;
        if (pos / 10 + 1 <= 9 && pos % 10 - 1 >= 0)
                a[pos / 10 + 1][pos % 10 - 1] = 2;
        if (pos / 10 - 1 >= 0 && pos % 10 + 1 <= 9)
                a[pos / 10 - 1][pos % 10 + 1] = 2;
}

//urmatoarele 4 functii construiesc navele calculatorului pe directii diferite

int build_UP(int init, int length, int ** a) {
        if (init / 10 - length < 0)
                return 0;
        for (int i = 1; i <= length; i++) {
                if ((a[init / 10 - i][init % 10] == 1 || a[init / 10 - i][init % 10] == 2) && surroundings(init - i * 10, a) == 0)
                        return 0;
        }
        for (int i = 0; i <= length; i++) {
                filler(init - i * 10, a);
                a[init / 10 - i][init % 10] = 1;
        }
        return 1;
}

int build_DOWN(int init, int length, int ** a) {
        if (init / 10 + length > 9)
                return 0;
        for (int i = 1; i <= length; i++) {
                if ((a[init / 10 + i][init % 10] == 1 || a[init / 10 + i][init % 10] == 2) && surroundings(init + i * 10, a) == 0)
                        return 0;
        }
        for (int i = 0; i <= length; i++) {
                filler(init + i * 10, a);
                a[init / 10 + i][init % 10] = 1;
        }
        return 1;
}

int build_RIGHT(int init, int length, int ** a) {
        if (init % 10 + length > 9)
                return 0;
        for (int i = 1; i <= length; i++) {
                if ((a[init / 10][init % 10 + i] == 1 || a[init / 10][init % 10 + i] == 2) && surroundings(init + i, a) == 0)
                        return 0;
        }
        for (int i = 0; i <= length; i++) {
                filler(init + i, a);
                a[init / 10][init % 10 + i] = 1;
        }
        return 1;
}

int build_LEFT(int init, int length, int ** a) {
        if (init % 10 - length < 0)
                return 0;
        for (int i = 1; i <= length; i++) {
                if ((a[init / 10][init % 10 - i] == 1 || a[init / 10][init % 10 - i]) && surroundings(init - i, a) == 0)
                        return 0;
        }
        for (int i = 0; i <= length; i++) {
                filler(init - i, a);
                a[init / 10][init % 10 - i] = 1;
        }
        return 1;
}

//aranjeaza random un vector de lungime n

void array_shuffle(int * positions, int n) {
        int i;
        int aux;
        for (i = n - 1; i > 0; i--) {
                int j = rand() % (i + 1);

                aux = positions[j];
                positions[j] = positions[i];
                positions[i] = aux;
        }
}

//randomizeaza harta calculatorului

void ai_randomizer(WINDOW * aigrid, int ** a) {

        int nave_plasate = 0, nave[2][4], length, i = 0, loc = 0;
	int init, direction = 0;
        int * positions = (int * ) malloc(100 * sizeof(int));
        for (int j = 0; j <= 99; j++)
                positions[j] = j;
        array_shuffle(positions, 100);
	
	//stocheaza numarul de nave de lungimi diferite
        nave[0][0] = 1;
        nave[0][1] = 2;
        nave[0][2] = 3;
        nave[0][3] = 4;
        nave[1][3] = 1;
        nave[1][2] = 2;
        nave[1][1] = 3;
        nave[1][0] = 4;
        while (nave_plasate < 10 && loc < 99) {
                if (nave[0][i] == 0)
                        i++;
                length = nave[1][i] - 1;
                init = positions[loc];
                if (a[init / 10][init % 10] == 0 && surroundings(init, a) == 1) {
                        direction++;
                        if (direction > 3)
                                direction %= 4;
                        if (direction == 0) {
                                if (build_UP(init, length, a) == 1) {
                                        nave_plasate++;
                                        nave[0][i]--;
                                }
                        }
                        if (direction == 2) {
                                if (build_DOWN(init, length, a) == 1) {
                                        nave_plasate++;
                                        nave[0][i]--;
                                }
                        }
                        if (direction == 1) {
                                if (build_RIGHT(init, length, a) == 1) {
                                        nave_plasate++;
                                        nave[0][i]--;
                                }
                        }
                        if (direction == 3) {
                                if (build_LEFT(init, length, a) == 1) {
                                        nave_plasate++;
                                        nave[0][i]--;
                                }
                        }
                }
                loc++;
                wrefresh(aigrid);
        }
        free(positions);

}

//sterge urma precedenta lasata de "@"

void del_pos(int ypos, int xpos, int ** ai, WINDOW * aigrid) {
        if (ai[ypos / 2][xpos / 4] == 3)
                mvwprintw(aigrid, ypos, xpos, "O");
        else {
                if (ai[ypos / 2][xpos / 4] == 4)
                        mvwprintw(aigrid, ypos, xpos, "-");
                else
                        mvwprintw(aigrid, ypos, xpos, " ");
        }
}

//urmatoarele 4 functii definesc miscarile playerului pe harta ai-ului

void move_up(int * ypos) {
        ( * ypos) -= 2;
        if (( * ypos) < 1)
                ( * ypos) = 1;
}

void move_down(int * ypos) {
        ( * ypos) += 2;
        if (( * ypos) > 19)
                ( * ypos) = 19;
}

void move_left(int * xpos) {
        ( * xpos) -= 4;
        if (( * xpos) < 2)
                ( * xpos) = 2;
}

void move_right(int * xpos) {
        ( * xpos) += 4;
        if (( * xpos) > 38)
                ( * xpos) = 38;
}

void player_turn(int ** ai, int * game_state, WINDOW * aigrid, int * ai_health, int * hit, int * menu, int * dia, int * rm) {
        keypad(aigrid, true);
        int xpos = 2, ypos = 1, key, ok = 1, highlight = 0;
        while (ok) {
                mvwprintw(aigrid, ypos, xpos, "@");
                wrefresh(aigrid);
                key = wgetch(aigrid);
                switch (key) {
                case KEY_UP:
                        del_pos(ypos, xpos, ai, aigrid);
                        move_up( & ypos);
                        break;
                case KEY_DOWN:
                        del_pos(ypos, xpos, ai, aigrid);
                        move_down( & ypos);
                        break;
                case KEY_LEFT:
                        del_pos(ypos, xpos, ai, aigrid);
                        move_left( & xpos);
                        break;
                case KEY_RIGHT:
                        del_pos(ypos, xpos, ai, aigrid);
                        move_right( & xpos);
                        break;
                case 'q':
                        meniu( & highlight, 1);
                        if (highlight == 2)
                                *
                                menu = 2;
                        if (highlight == 0)
                                *
                                menu = 0;
                        if (highlight == 1)
                                *
                                menu = 1;
                        break;
                default:
                        break;
                }
                if ( * menu == 0 || * menu == 2)
                        break;
                if ( * menu == 1) {
                        WINDOW * menuwin = newwin(5, 101, 30, 0);
                        wborder(menuwin, 32, 32, 32, 32, 32, 32, 32, 32);
                        wrefresh(menuwin);
                }
		//momentul in care playerul alege o pozitie pe harta pc-ului
                if (key == 10 && ai[ypos / 2][xpos / 4] != 4) {
                        if (ai[ypos / 2][xpos / 4] == 1 || ai[ypos / 2][xpos / 4] == 3) {
                                mvwprintw(aigrid, ypos, xpos, "O");
                                ai[ypos / 2][xpos / 4] = 3;
				if(ai[ypos / 2][xpos / 4] == 1)
                                	( * ai_health) --;
                                wrefresh(aigrid);
                                * hit = 1;
                        } else {
                                mvwprintw(aigrid, ypos, xpos, "-");
                                ai[ypos / 2][xpos / 4] = 4;
                                wrefresh(aigrid);
                                * hit = 0;
                        }
                        ok = 0;
                }
		//playerul alege destroy in advance
                if (key == 100 && ( * dia) == 0) {
                        * dia = 1;
                        ok = 0;
                }
		//playerul alege randomize map
                if (key == 114 && ( * rm) == 0) {
                        * rm = 1;
                        ok = 0;
                }
        }
        if (( * ai_health) == 0)
                * game_state = 1;
}

void ai_turn(int ** a, int * game_state, WINDOW * playergrid, int * player_health, int map, int * hit, int val, int dia) {
	//se foloseste sleep doar daca nu este activ destroy in advance
        if (dia == 0 || dia == -1)
                sleep(3);
        if (a[map][val] == '1') {
                a[map][val] = '3';
                mvwprintw(playergrid, (val / 10) * 2 + 1, (val % 10) * 4 + 2, "O");
                ( * player_health) --;
                * hit = 1;
        } else {
                a[map][val] = '4';
                mvwprintw(playergrid, (val / 10) * 2 + 1, (val % 10) * 4 + 2, "-");
                * hit = 0;
        }
        wrefresh(playergrid);
        if (( * player_health) <= 0)
                *game_state = 2;
}

void dia_player_turn(int ** ai, int * game_state, WINDOW * aigrid, int * ai_health) {
        int pos = rand() % 100;
        while (ai[pos / 10][pos % 10] == 3 || ai[pos / 10][pos % 10] == 4) {
                pos = rand() % 100;
        }

        if (ai[pos / 10][pos % 10] == 1) {
                ai[pos / 10][pos % 10] = 3;
                ( * ai_health) --;
                mvwprintw(aigrid, (pos / 10) * 2 + 1, (pos % 10) * 4 + 2, "O");
                wrefresh(aigrid);
        } else {
                ai[pos / 10][pos % 10] = 4;
                mvwprintw(aigrid, (pos / 10) * 2 + 1, (pos % 10) * 4 + 2, "-");
                wrefresh(aigrid);
        }
}

//verifica daca exista nave de lungime length in jos sau dreapta

void check_DOWN(int ** a, int i, int map, int * length) {
        while (i / 10 + 1 <= 9) {
                i = i + 10;
                if (a[map][i] == '1') {
                        ( * length) ++;
                        a[map][i] = '0';
                } else {
                        if (a[map][i] == '0' || a[map][i] == '4')
                                break;
                }
        }
}

void check_RIGHT(int ** a, int i, int map, int * length) {
        while (i % 10 + 1 <= 9) {
                i++;
                if (a[map][i] == '1') {
                        ( * length) ++;
                        a[map][i] = '0';
                } else {
                        if (a[map][i] == '0' || a[map][i] == '4')
                                break;
                }
        }
}

//obtine lungimea navei

void check_proximities(int ** a, int i, int map, int * length) {
        check_DOWN(a, i, map, length);
        if ( * length == 1)
                check_RIGHT(a, i, map, length);
}

void player_randomizer(int ** a, WINDOW * playergrid, int map, int * new_ships) {
	//vector care contine pozitii random
        int * positions = (int * ) malloc(100 * sizeof(int));
        for (int j = 0; j <= 99; j++)
                positions[j] = j;

        array_shuffle(positions, 100);

	//matrice care inlocuieste temporar harta playerului
        int ** harta = (int ** ) malloc(sizeof(int * ) * (10));
        for (int j = 0; j < 10; j++)
                harta[j] = (int * ) malloc(sizeof(int) * 10);
        for (int j = 0; j < 10; j++) {
                for (int k = 0; k < 10; k++) {
                        harta[j][k] = 0;
                }
        }
        int nrnave = 0;
        for (int j = 0; j < 4; j++)
                nrnave += new_ships[j];
        int nave_plasate = 0, nr = 0, init, loc = 0, direction;
        while (nave_plasate < nrnave && loc < 99) {
                if (new_ships[nr] == 0)
                        nr++;
                init = positions[loc];
                direction = rand() % 4;
                if (harta[init / 10][init % 10] == 0 && surroundings(init, harta) == 1) {
                        if (direction == 0 && build_UP(init, nr, harta) == 1) {
                                nave_plasate++;
                                new_ships[nr]--;
                        }
                        if (direction == 2 && build_DOWN(init, nr, harta) == 1) {
                                nave_plasate++;
                                new_ships[nr]--;
                        }
                        if (direction == 1 && build_RIGHT(init, nr, harta) == 1) {
                                nave_plasate++;
                                new_ships[nr]--;
                        }
                        if (direction == 3 && build_LEFT(init, nr, harta) == 1) {
                                nave_plasate++;
                                new_ships[nr]--;
                        }
                }
                loc++;
        }
        wrefresh(playergrid);
        free(positions);
        int contor = 0;
	//transfera datele din matricea "harta" in matricea "a"
        for (int j = 0; j < 10; j++)
                for (int k = 0; k < 10; k++) {
                        if (harta[j][k] == 1)
                                a[map][contor] = 49;
                        else
                                a[map][contor] = 48;
                        contor++;
                }
	//afiseaza harta dupa "randomize map"
        load_player_map(playergrid, a, map + 1);
}

void randomize_map(int ** a, WINDOW * playergrid, int map, int * new_ships) {
        int i, length = 1;
        for (i = 0; i <= 99; i++) {
                if (a[map][i] == '1') {
                        length = 1;
                        check_proximities(a, i, map, & length);
                        a[map][i] = '0';
                        new_ships[length - 1]++;
                }
        }
        for (i = 0; i <= 99; i++)
                a[map][i] = '0';
        player_randomizer(a, playergrid, map, new_ships);
        wrefresh(playergrid);
}

void result(int game_state, int ai_health, int player_health) {
	if(ai_health==player_health)
		game_state = 3;
	int yMax, xMax;
        WINDOW * scorewin = newwin(10, 101, 9, 0);
        getmaxyx(scorewin, yMax, xMax);
	yMax=0;
	yMax=yMax+xMax;
	box(scorewin, 0, 0);
	wrefresh(scorewin);
	if(game_state == 3) {
		mvwprintw(scorewin, 3, xMax / 2 - 2, "REMIZA");
	}
	if(game_state == 2) {
		mvwprintw(scorewin, 1, xMax / 2 - 6, "COMPUTER WINS");
		mvwprintw(scorewin, 4, xMax / 2 - 12, "Bucati de nave doborate: %d", 20 - ai_health);
	}
	if(game_state == 1) {
		mvwprintw(scorewin, 1, xMax / 2 - 4, "PLAYER WINS");
		mvwprintw(scorewin, 4, xMax / 2 - 12, "Bucati de nave doborate: %d", 20 - player_health);
	}
	mvwprintw(scorewin, 7, xMax / 2 - 10, "Press any key to exit");
	wrefresh(scorewin);
	getch();
}

void new_game(int argc, int ** a) {
        int map, player_health = 19, ai_health = 20, hit = 0;
        //game_state= 0 -> new game
        //game_state= 1 -> playerul castiga
        //game_state= 2 -> calculatorul castiga
	int game_state = 0;	
	
        //creare matrice ce stocheaza harta calculatorului
        int ** ai = (int ** ) malloc(sizeof(int * ) * (10));
        for (int j = 0; j < 10; j++) {
                ai[j] = (int * ) malloc(sizeof(int) * 10);
        }

        for (int j = 0; j < 10; j++)
                for (int l = 0; l < 10; l++)
                        ai[j][l] = 0;

        wborder(stdscr, 32, 32, 32, 32, 32, 32, 32, 32);
        wrefresh(stdscr);

        WINDOW * playergrid = newwin(21, 41, 4, 0);
        box(playergrid, 0, 0);

        WINDOW * aigrid = newwin(21, 41, 4, 60);
        box(aigrid, 0, 0);

        build_grids(playergrid, aigrid);

        wrefresh(playergrid);
        wrefresh(aigrid);

        map = map_choose(argc, playergrid, a);
        power_ups();

        ai_randomizer(aigrid, ai);
	
	//pozitiile care vor fi ulterior alese de calculator la ghicire
        int * ai_chosen_pos = (int * ) malloc(100 * sizeof(int));
        for (int j = 0; j <= 99; j++)
                ai_chosen_pos[j] = j;
        array_shuffle(ai_chosen_pos, 100);
        //vector ce stocheaza noile nave dupa activarea "randomize map"
        int * new_ships = (int * ) malloc(4 * sizeof(int));
        for (int j = 0; j < 4; j++)
                new_ships[j] = 0;
        int pos = 0, val = ai_chosen_pos[pos], menu = -1, dia = 0, rm = 0;
	int dia_used = 0, rm_used = 0;
        while (game_state == 0 && menu != 2) {
                hit = 0;
                do {
			player_turn(ai, & game_state, aigrid, & ai_health, & hit, & menu, & dia, & rm);
                        if (dia == 1) {
                                dia_used = 1;
                                hit = 0;
                                for (int j = 1; j <= 10; j++) {
                                        ai_turn(a, & game_state, playergrid, & player_health, map, & hit, val, dia);
                                        dia_player_turn(ai, & game_state, aigrid, & ai_health);
                                        pos++;
                                        if (pos > 99)
                                                pos = 0;
                                        val = ai_chosen_pos[pos];
                                }
                                dia = -1;
                        }
                        if (rm == 1) {
                                rm_used = 1;
                                randomize_map(a, playergrid, map, new_ships);
                                rm = -1;
                        }
                } while (hit == 1 && menu == -1);
                if (menu == 2 || menu == 0)
                        break;
                hit = 0;
                if (dia_used == 0 && rm_used == 0) {
                        ai_turn(a, & game_state, playergrid, & player_health, map, & hit, val, dia);
                        pos++;
                        if (pos > 99)
                                pos--;
                        val = ai_chosen_pos[pos];
                }
                if (dia_used == 1)
                        dia_used = 0;
                if (rm_used == 1)
                        rm_used = 0;
                while (hit == 1) {
                        ai_turn(a, & game_state, playergrid, & player_health, map, & hit, val, dia);
                        pos++;
                        if (pos > 99)
                                pos--;
                        val = ai_chosen_pos[pos];
                }
        }
        free(ai_chosen_pos);
        for (int j = 0; j < 10; j++)
                free(ai[j]);
        free(ai);
        free(new_ships);
        if (menu == 0) {
                new_game(argc, a);
        }
	if(menu != 2) {
		result(game_state, ai_health, player_health);
	}
}

int main(int argc, char * argv[]) {
        srand(time(NULL));
        FILE * harti;
        int j = 0, k = 0, aux = 0;
        char v[100];
        int iv[100];

        if (argc == 1) {
                printf("[Eroare]: Nu s-au dat argumente de comanda.");
                return 1;
        }

        //creare matrice alocata dinamic care stocheaza hartile
        int ** a = (int ** ) malloc(sizeof(int * ) * (argc - 1));
        for (int i = 0; i < argc - 1; i++) {
                a[i] = (int * ) malloc(sizeof(int) * 100);
        }

	//citirea hartilor introduse ca input in terminal
        for (int i = 1; i < argc; ++i) {
                aux = 0;
                harti = fopen(argv[i], "r");
                if (!harti) {
                        printf("[Eroare]: Fisierul %s nu poate fi deschis.", argv[i]);
                        return 1;endwin();
                }
                for (int l = 0; l <= 9; l++) {
                        fgets(v, 1000, harti);
                        for (k = 0; k <= 9; k++) {
                                iv[k] = v[k];
                                a[j][aux] = iv[k];
                                aux++;
                        }
                }
                j++;
        }
        fclose(harti);

        initscr();

        noecho();
        cbreak();

        int highlight = 0;
	logo();
        meniu( & highlight, 0);
        if (highlight == 0) {
		remove_logo();
                new_game(argc, a);
	}
        endwin();
        for (int i = 0; i < argc - 1; i++)
                free(a[i]);
        free(a);
        return 1;
}
