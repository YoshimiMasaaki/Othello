
#include    <stdio.h>


#define BORD_SIZE       8
#define DATA_NOTHING    0
#define DATA_WHITE      1
#define DATA_BLACK     -1

#define NUMBER_MOVES    4

static int evalPos[BORD_SIZE][BORD_SIZE] = {
    { 10, 2, 9, 8, 8, 9, 2, 10 },
    {  2, 1, 7, 3, 3, 7, 1,  2 },
    {  9, 7, 7, 5, 5, 7, 7,  9 },
    {  8, 3, 5, 0, 0, 5, 3,  8 },
    {  8, 3, 5, 0, 0, 5, 3,  8 },
    {  9, 7, 7, 5, 5, 7, 7,  9 },
    {  2, 1, 7, 3, 3, 7, 1,  2 },
    { 10, 2, 9, 8, 8, 9, 2, 10 }
};

void init_Bord(char cbBord[BORD_SIZE][BORD_SIZE])
{
    int i;
    int j;
    
    for ( i=0; i<BORD_SIZE; i++ ) {
        for ( j=0; j<BORD_SIZE; j++ ) {
            cbBord[i][j] = DATA_NOTHING;
        }
    }
    cbBord[3][4] = DATA_BLACK;
    cbBord[4][3] = DATA_BLACK;
    cbBord[3][3] = DATA_WHITE;
    cbBord[4][4] = DATA_WHITE;
}

void disp_Bord(char cbBord[BORD_SIZE][BORD_SIZE])
{
    int i;
    int j;
	char *pNum[8] = { "‚P", "‚Q", "‚R", "‚S", "‚T", "‚U", "‚V", "‚W" };

    
    for ( i=0; i<BORD_SIZE; i++ ) {
        if ( i == 0 ) {
            printf("@%s%s%s%s%s%s%s%s\n", pNum[0], pNum[1], pNum[2], pNum[3], pNum[4], pNum[5], pNum[6], pNum[7]);
        }
		printf("%s", pNum[i]);
		for ( j=0; j<BORD_SIZE; j++ ) {
            if ( cbBord[i][j] == DATA_NOTHING ) {
                printf("@");
            }
            if ( cbBord[i][j] == DATA_BLACK ) {
				printf("›");
			}
            if ( cbBord[i][j] == DATA_WHITE) {
				printf("œ");
            }
        }
		printf("\n");
    }
}

int count_Bord(char cData, char cbBord[BORD_SIZE][BORD_SIZE])
{
    int i;
    int j;
    int cnt = 0;

    for ( i=0; i<BORD_SIZE; i++ ) {
		for ( j=0; j<BORD_SIZE; j++ ) {
            if ( cbBord[i][j] == cData ) {
                cnt++;
            }
        }
    }
    return cnt;
}

void change_Bord(int x, int y, char cData, char cbBord[BORD_SIZE][BORD_SIZE])
{
    int i;
    int j;
    int n;
    int nStepX;
    int nStepY;

    if ( cbBord[y][x] != DATA_NOTHING ) {
        return;
    }

    for ( nStepY=-1; nStepY<=1; nStepY++ ) {
	    i = y + nStepY;
        if ( (i < 0) || (i >= BORD_SIZE) ) continue;
		for ( nStepX=-1; nStepX<=1; nStepX++ ) {
		    j = x + nStepX;
            if ( (j < 0) || (j >= BORD_SIZE) ) continue;

		    if ( (j == x) && (i == y) ) continue;

		    n = 0;
            while(cbBord[i+n*nStepY][j+n*nStepX] == -cData) {
                n++;
                if ( (i+n*nStepY < 0) || (i+n*nStepY >= BORD_SIZE) ) break;
                if ( (j+n*nStepX < 0) || (j+n*nStepX >= BORD_SIZE) ) break;
                
                if ( cbBord[i+n*nStepY][j+n*nStepX] == cData ) {
                    while( n >= 0 ) {
                        n--;
                        cbBord[i+n*nStepY][j+n*nStepX] = cData;
                    }
                    break;
                }
            }
        }
    }
}

int check_position(int nDeep, char cData, char cbBord[BORD_SIZE][BORD_SIZE], int *outPosX, int *outPosY)
{
    int x;
    int y;
    int nBefor;
    int nAfter;
    int evalTotal = 0;
    int evalCheck = 0;
    char cbBordBack[BORD_SIZE][BORD_SIZE];

	memcpy(cbBordBack, cbBord, BORD_SIZE * BORD_SIZE);
    for ( y=0; y<BORD_SIZE; y++ ) {
		for ( x=0; x<BORD_SIZE; x++ ) {
            nBefor = count_Bord(cData, cbBord);
            change_Bord(x, y, cData, cbBord);
            nAfter = count_Bord(cData, cbBord);
		    if ( nAfter > nBefor ) {
		        if ( nDeep < NUMBER_MOVES ) {
		            evalCheck = check_position(nDeep+1, -cData, cbBord, outPosX, outPosY);
		            if ( nDeep == 0 ) {
    		            if ( evalTotal < evalCheck + evalPos[y][x] ) {
    		                evalTotal = evalCheck + evalPos[y][x];
    		                *outPosX = x;
    		                *outPosY = y;
    		            }
		            } else {
    		            if ( (nDeep%2) == 0 ) {
    		                evalTotal += (evalCheck + evalPos[y][x]);
    		            } else {
       		                evalTotal += (evalCheck + 10 - evalPos[y][x]);
    		            }
		            }
		        } else {
    		        evalTotal += evalPos[y][x];
		        }
		    }
            memcpy(cbBord, cbBordBack, BORD_SIZE * BORD_SIZE);
		}
    }
    return evalTotal;
}

void computer(char cData, char cbBord[BORD_SIZE][BORD_SIZE])
{
    int outPosX = -1;
    int outPosY = -1;
    
    if ( check_position(0, cData, cbBord, &outPosX, &outPosY) > 0 ) {
		printf("Computer position is (%d %d)\n", outPosX+1, outPosY+1);
		change_Bord(outPosX, outPosY, cData, cbBord);
    }
    return;
}

void input_position(char cData, char cbBord[BORD_SIZE][BORD_SIZE])
{
    int x;
    int y;
    int nBefor;
    int nAfter;
    int outPosX;
    int outPosY;
    char strIn[256];

	if (check_position(NUMBER_MOVES, cData, cbBord, &outPosX, &outPosY) <= 0) {
		return;
	}
	do {
        printf("Input position (xy) or Pass : ");
    	scanf("%s", &strIn);
        
        if ( (strIn[0] == 'P') || (strIn[0] == 'p') ) {
            if ( check_position(2, cData, cbBord, &outPosX, &outPosY) > 0 ) {
                printf("Can't pass !\n");
                continue;
            }
            return;
        }

        x = strIn[0] - '0';
        y = strIn[1] - '0';
        if ( (x >= 1) && (x <= BORD_SIZE) &&
             (y >= 1) && (y <= BORD_SIZE) ) {
            nBefor = count_Bord(cData, cbBord);
            change_Bord(x-1, y-1, cData, cbBord);
            nAfter = count_Bord(cData, cbBord);
            if ( nAfter > nBefor ) break;
        }

        printf("Bad position (%d %d) !\n", x, y);
    }while(1);
}


main()
{
	int nBeforM;
	int nAfterM;
	int nBeforC;
	int nAfterC;
	char man_post;
	char game_post = DATA_BLACK;
	char strIn[256];
    char cbBord[BORD_SIZE][BORD_SIZE];
    
	printf("This is Othello Game. First or After? : ");
	scanf("%s", &strIn);
	if (strIn[0] == 'F' || strIn[0] == 'f') {
		man_post = DATA_BLACK;
	} else
	if (strIn[0] == 'A' || strIn[0] == 'a') {
		man_post = DATA_WHITE;
	}
	else {
		printf("Sorry. exit game");
	}

    init_Bord(cbBord);
	do {
		disp_Bord(cbBord);
		if (man_post == game_post) {
            nBeforM = count_Bord(game_post, cbBord);
            input_position(man_post, cbBord);
            nAfterM = count_Bord(game_post, cbBord);
		}
		else {
            nBeforC = count_Bord(game_post, cbBord);
            computer(-man_post, cbBord);
            nAfterC = count_Bord(game_post, cbBord);
		}
	    game_post = -game_post;
	} while ((nAfterM > nBeforM) || (nAfterC > nBeforC));
    
    nAfterM = count_Bord(man_post, cbBord);
    nAfterC = count_Bord(-man_post, cbBord);
    if ( nAfterC < nAfterM ) {
    	printf("Game Over. Winner is Human!! ( %d : %d )\n", nAfterM, nAfterC);
    } else
    if ( nAfterC > nAfterM ) {
    	printf("Game Over. Winner is Computer!! ( %d : %d )\n", nAfterC, nAfterM);
    } else {
    	printf("Game Over. Draw!!\n");
    }
}

