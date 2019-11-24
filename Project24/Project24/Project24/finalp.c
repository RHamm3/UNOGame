#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define PASS 1
#define FAIL 0

//Ryan Hammond and Phil Sjoquist

//Struct
typedef struct cardS {
	char suit[9];
	int value;
	char action[15];
	struct cardS *pt;
	struct cardS *previous;
	struct cardS *next;
} card;

//randGen Function, used to find two random cards
int randGen(int count) {
	//Random number generator given in lab
	double frac;

    frac = (double)(rand()/((double)RAND_MAX+1));
	return floor(count*frac);
}

//deckCreation Function, assigns values to the memory spaces created in the addCards function
void deckCreation(card **headC) {
	char *suits[] = { "Blue", "Red", "Green", "Yellow" };
	char *actions[] = { "None", "Skip", "Reverse", "Draw 2", "Wild", "Draw 4" };
	int i = 0;
	int j = 0;
	card *c = *headC;

	//For each suit
	for (i = 0; i < 4; i++) {
		//For values 0-10, copy information into appropriate spots
		for (j = 0; j < 10; j++) {
			c->value = j;
			strcpy(c->suit, suits[i]);			
			strcpy(c->action, actions[0]);
			c = c->next;
		}
		//For special action cards, copy information into appropriate spots, with j moving along the actions string array
		for (j = 1; j <= 4; j++) {
			c->value = j + 9;
			strcpy(c->suit, suits[i]);			
			strcpy(c->action, actions[j]);
			c = c->next;
		}
		//Do it again, there are 2 of each card besides 0's, Wild's, and Draw 4's
		for (j = 1; j < 10; j++) {
			c->value = j;
			strcpy(c->suit, suits[i]);			
			strcpy(c->action, actions[0]);
			c = c->next;
		}
		for (j = 1; j <= 5; j++) {
			c->value = j + 9;
			if (c->value == 13) {
				j = 5;
				c->value = j + 9;
				strcpy(c->suit, suits[i]);
				strcpy(c->action, actions[j]);
			}
			else {
				strcpy(c->suit, suits[i]);
				strcpy(c->action, actions[j]);
			}
			c = c->next;
		}
	}
}

//removeCard Function, frees memory from a given list
void removeCard(card *c, card **headC, card **tailC) {

	//If the given card is the head, move the head, then remove the given card from the list and free memory
	if (c == *headC) {
		*headC = c->next;
	}
	else {
		c->previous->next = c->next;
	}
	//If the given card is the tail, move the tail, then remove the given card from the list and free memory
	if (c == *tailC) {
		*tailC = c->previous;
	}
	else {
		c->next->previous = c->previous;
	}
	free(c);
}

//Hand Addition Function, adds cards to a player hand
void pHandAddition(card **headC, card **p1head) {
	card *p = *p1head;
	p->value = (*headC)->value;
	strcpy(p->suit, (*headC)->suit);
	strcpy(p->action, (*headC)->action);
	//p = p->next;
}

//Puts values into the discard pile
void discardAddition(card **P, card **D) {
		card *d = *D;
		d->value = (*P)->value;
		strcpy(d->suit, (*P)->suit);
		strcpy(d->action, (*P)->action);
}

//Print Function, prints the discard pile
void printDeck(card **headC, int x) {
	int i = 0;
	int j = 0;
	card *c = *headC;
	
	//If x is less than 5, print every card in the discard
	if (x < 5) {
		for (i = 0; i < x; i++) {
			if (strcmp("None", c->action) == 0) {
				printf("%d %s\n", c->value, c->suit);
				c = c->next;
			}
			else if (strcmp("Wild", c->action) == 0) {
				printf("%s\n", c->action);
				c = c->next;
			}
			else if (strcmp("Draw 2", c->action) == 0) {
				printf("%s\n", c->action);
				c = c->next;
			}
			else if (strcmp("Draw 4", c->action) == 0) {
				printf("%s\n", c->action);
				c = c->next;
			}
			else {
				printf("%s %s\n", c->action, c->suit);
				c = c->next;
			}
		}
	}
	//Else x must be greater than 5, so only print the top 5 cards
	else {
		for (j = 0; j < (x - 5); j++) {
			c = c->next;
		}
		for (i = (x - 5); i < x; i++) {
			if (strcmp("None", c->action) == 0) {
				printf("%d %s\n", c->value, c->suit);
				c = c->next;
			}
			else if (strcmp("Wild", c->action) == 0) {
				printf("%s\n", c->action);
				c = c->next;
			}
			else if (strcmp("Draw 4", c->action) == 0) {
				printf("%s\n", c->action);
				c = c->next;
			}
			else {
				printf("%s %s\n", c->action, c->suit);
				c = c->next;
			}
		}
	}
}

//Print Player Hand
void printPlayerHand(card **p1head, int hSize) {
	int i = 0;
	card *p1 = *p1head;

	//Loop for hand size, printing cards in the hand
	for (i = 0; i < hSize; i++) {
		if (strcmp("None", p1->action) == 0) {
			printf("|%d %s|", p1->value, p1->suit);
			p1 = p1->next;
		}
		else if (strcmp("Wild", p1->action) == 0) {
			printf("|%s|", p1->action);
			p1 = p1->next;
		}
		else if (strcmp("Draw 4", p1->action) == 0) {
			printf("|%s|", p1->action);
			p1 = p1->next;
		}
		else {
			printf("|%s %s|", p1->action, p1->suit);
			p1 = p1->next;
		}
		//Spacing between cards
		printf("   ");
	}
	printf("\n\n");
}

//addCards Function, allocates memory for a single card
void addCards(card *s, card **head, card **tail) {

	card *t;
	t = (card*)malloc(sizeof(card));
	//If head is NULL, create a list
	if (*head == NULL) {
		t->previous = NULL;
		t->next = NULL;
		*head = t;
		*tail = t;
	}
	//If s is NULL, create at the end of the list
	else if (s == NULL) {
		(*tail)->next = t;
		t->previous = *tail;
		t->next = NULL;
		*tail = t;
	}
	//If the element after s is NULL, put in after s and make t->next NULL
	else if (s->next == NULL) {
		t->previous = s;
		t->next = NULL;
		s->next = t;
		*tail = t;
	}
	//Else add to the middle of the list
	else {
		t->next = s->next;
		t->previous = s;
		s->next = t;
		t->next->previous = t;
	}
}

//playability Function, checks to see if that card can be played
int playability(card *p, card *d) {
	//If the card is a wild or draw 4, it is always playable
	if (strcmp(p->action, "Wild") == 0) {
		return PASS;
	}
	if (strcmp(p->action, "Draw 4") == 0) {
		return PASS;
	}
	//If the value is the same, the card is playable
	if ((p->value - d->value) == 0) {
		return PASS;
	}
	//If the suits are the same, the card is playable
	else if (strcmp(p->suit, d->suit) == 0) {
		return PASS;
	}
	//If the actions are the same, the card is playable
	if (p->value > 9 && d->value > 9) {
		if (strcmp(p->action, d->action) == 0) {
			return PASS;
		}
	}
	//Otherwise the card is not playable
	return FAIL;
	
}

//colorChoice Function, for if a player plays a wild or draw 4 card
void colorChoice(card *c) {
	char ans[7] = "";
	//If the given card has Wild action, prompt for a color
	if (strcmp(c->action, "Wild") == 0) {
		printf("Choose a color. ");
		scanf("%s", ans);
		strcpy(c->suit, ans);
	}
	//If the given card has Draw 4 action, prompt for a color
	if (strcmp(c->action, "Draw 4") == 0) {
		printf("Choose a color. ");
		scanf("%s", ans);
		strcpy(c->suit, ans);
	}
}

//Shuffle Function, takes in head pointer and two random integers between 0 and 108
void shuffle(card *h, int x, int y) {
	//Initialize variables
	card* temp1 = h;
	card* temp2 = h;
	int i;
	char s[9] = "";
	char a[15] = "";
	
	//Send temp1 and temp2 to the randomly generated integer spots
	for (i = 0; i < x; i++) {
		temp1 = temp1->next;
	}
	for (i = 0; i < y; i++) {
		temp2 = temp2->next;
	}
	
	//Swap contents of temp1 and temp2
	int v = temp1->value;
	temp1->value = temp2->value;
	temp2->value = v;

	strcpy(s, temp1->suit);
	strcpy(temp1->suit, temp2->suit);
	strcpy(temp2->suit, s);

	strcpy(a, temp1->action);
	strcpy(temp1->action, temp2->action);
	strcpy(temp2->action, a);
	
}

//scoreC Function, calculates the score of each card in the losers hand
int scoreC(card *h, int size) {
	int score = 0;
	int i = 0;

	//While the list entered isn't NULL, loop to find value of each card
	while (h != NULL) {
		for (i = 0; i < size; i++) {
			if (h->value >= 0 && h->value <= 9) {
				score = score + h->value;
				h = h->next;
			}
			else if (h->value >= 10 && h->value <= 12) {
				score = score + 20;
				h = h->next;
			}
			else if (h->value >= 13) {
				score = score + 50;
				h = h->next;
			}
		}
	}
	return score;
}

//Scan Function, reading from the file and assigning info to proper part of structs
void scanInfo(card **p, FILE *data) {

	int i = 0;
	card *c = *p;

	for (i = 0; i < 108; i++) {
		int s = 0;

		fscanf(data, "%s", c->suit);
		fscanf(data, "%d", &c->value);

		//Scan the first string to the action
		fscanf(data, "%s", c->action);
		s = strlen(c->action);

		//If card is a draw 2 or draw 4, allow a space from first to last string
		if (c->value == 12 || c->value == 14) {
			c->action[s] = ' ';
			//Make s the element after the space
			s++;
			//Scan the last string to the same action string
			fscanf(data, "%s", &c->action[s]);
		}
		c = c->next;
	}
}

int main(void) {

	//Initialize variables
	int p1score = 0;
	int p2score = 0;
	int playerTurn = 1;

	while (p1score <= 500 && p2score <= 500) {

		system("cls");

		//Initialize variables
		card *headC = NULL;
		card *c = NULL;
		card *tailC = NULL;

		card *p1head = NULL;
		card *p1 = NULL;
		card *p1tail = NULL;

		card *p2head = NULL;
		card *p2 = NULL;
		card *p2tail = NULL;

		card *headD = NULL;
		card *d = NULL;
		card *tailD = NULL;

		FILE *data = NULL;

		int i = 0;
		int j = 0;
		int x = 0;
		int turnC = 1;
		int p1hand = 7;
		int p2hand = 7;
		int discardSize = 1;
		int shuffleAnswer = 0;

		//Alternating starting players
		if (playerTurn % 2 == 0) {
			turnC++;
		}
		srand((int)time(NULL));


		//Allocate memory for the deck
		for (i = 0; i < 108; i++) {
			addCards(tailC, &headC, &tailC);
		}

		printf("Press 1 to shuffle the deck or 2 to load the deck from a file.\n");
		scanf("%d", &shuffleAnswer);

		//Assign values to each card
		if (shuffleAnswer == 1) {
			deckCreation(&headC);
			//Shuffle
			for (i = 0; i < 10000; i++) {
				shuffle(headC, randGen(108), randGen(108));
			}
		}

		if (shuffleAnswer == 2) {
			data = fopen("deck.txt", "r");
				scanInfo(&headC, data);
		}

		//Allocate memory for player hands
		for (i = 0; i < 7; i++) {
			addCards(p1tail, &p1head, &p1tail);
			addCards(p2tail, &p2head, &p2tail);
		}

		p1 = p1head;
		p2 = p2head;

		//Deal out to player hands
		for (i = 0; i < 7; i++) {
			pHandAddition(&headC, &p1);
			removeCard(headC, &headC, &tailC);
			pHandAddition(&headC, &p2);
			removeCard(headC, &headC, &tailC);

			p1 = p1->next;
			p2 = p2->next;
		}

		//Allocate memory for the discard pile
		for (i = 0; i < 106; i++) {
			addCards(tailD, &headD, &tailD);
		}

		d = headD;
		discardAddition(&headC, &d);

		//If the first card on the discard is a draw 4, reshuffle the deck and pull a new card
		while ((d->value - 14) == 0) {
			for (i = 0; i < 10000; i++) {
				shuffle(headC, randGen(94), randGen(94));
			}
			d = headD;
			discardAddition(&headC, &d);
		}

		//Remove the pulled card from the deck
		c = headC->next;
		removeCard(headC, &headC, &tailC);
		headC = c;

		//If first card on discard is a skip or reverse, becomes the other players turn
		if (strcmp("Skip", d->action) == 0 || strcmp("Reverse", d->action) == 0) {
			turnC++;
		}

		//If first discard card is a draw 2, make the player lose a turn and draw 2 cards
		if (d->value - 12 == 0) {
			if (turnC == 1) {
				for (i = 0; i < 2; i++) {
					p1hand = p1hand + 1;
					addCards(p1tail, &p1head, &p1tail);
					pHandAddition(&headC, &p1tail);
					removeCard(headC, &headC, &tailC);
				}
			}
			else {
				for (i = 0; i < 2; i++) {
					p2hand = p2hand + 1;
					addCards(p2tail, &p2head, &p2tail);
					pHandAddition(&headC, &p2tail);
					removeCard(headC, &headC, &tailC);
				}
			}
			turnC++;
		}

		//If first discard card is a Wild, let the player choose the color
		if (strcmp("Wild", d->action) == 0) {
			colorChoice(d);
		}

		//While loop for each individual game, while neither player's hand is empty keep playing
		while (p1hand != 0 || p2hand != 0) {
			p1 = p1head;
			p2 = p2head;

			//Player 1's turn
			if (turnC % 2 != 0) {
				printf("Discard Pile:\n");
				printDeck(&headD, discardSize);

				//If statements for displaying current color
				if (strcmp(d->suit, "Blue") == 0) {
					printf("\n\nThe current color is Blue.\n");
				}
				if (strcmp(d->suit, "Yellow") == 0) {
					printf("\n\nThe current color is Yellow.\n");
				}
				if (strcmp(d->suit, "Green") == 0) {
					printf("\n\nThe current color is Green.\n");
				}
				if (strcmp(d->suit, "Red") == 0) {
					printf("\n\nThe current color is Red.\n");
				}

				//If statements for displaying if a player has uno
				if (p1hand == 1) {
					printf("\nPlayer 1 has UNO!\n");
				}
				if (p2hand == 1) {
					printf("\nPlayer 2 has UNO!\n");
				}

				//Print statements for player 1's hand and prompt for which card they want to play
				printf("\nPlayer 1 Hand: \n\n");
				printPlayerHand(&p1head, p1hand);
				printf("Enter a number to play that card. Enter 0 to draw. ");
				scanf("%d", &x);

				//Finding selected card
				for (i = 0; i < x - 1; i++) {
					p1 = p1->next;
				}

				//Check for if the card can be played, if not loop until they play a correct card
				while (playability(p1, d) == FAIL && x != 0) {
					p1 = p1head;
					printf("You cannot play that card. Enter in a valid number.\n");
					scanf("%d", &x);
					if (x == 0) {
						break;
					}
					for (i = 0; i < x - 1; i++) {
						p1 = p1->next;
					}
				}

				//If they enter 0, draw a card, otherwise play the card selected
				if (x == 0) {
					//If headC is NULL, move the discard pile back into the deck
					if (headC == NULL) {
						c = headC;
						for (j = 0; j < (discardSize - 5); j++) {
							discardAddition(&headD, &c);
							c = c->next;
						}
					}
					p1hand = p1hand + 1;
					addCards(p1tail, &p1head, &p1tail);
					pHandAddition(&headC, &p1tail);
					removeCard(headC, &headC, &tailC);
					turnC++;
				}
				else {
					colorChoice(p1);
					d = d->next;
					p1hand = p1hand - 1;
					discardSize++;
					discardAddition(&p1, &d);
					if (p1hand == 0) {
						printf("\nPlayer 1 Wins!\n\n");
						break;
					}

					//If conditions for if the card played was a special card
					if (strcmp(p1->action, "Skip") == 0 || strcmp(p1->action, "Reverse") == 0) {
						turnC++;
					}
					if (strcmp(p1->action, "Draw 2") == 0) {
						for (i = 0; i < 2; i++) {
							p2hand = p2hand + 1;
							addCards(p2tail, &p2head, &p2tail);
							pHandAddition(&headC, &p2tail);
							removeCard(headC, &headC, &tailC);
						}
						turnC++;
					}
					if (strcmp(p1->action, "Draw 4") == 0) {
						for (i = 0; i < 4; i++) {
							p2hand = p2hand + 1;
							addCards(p2tail, &p2head, &p2tail);
							pHandAddition(&headC, &p2tail);
							removeCard(headC, &headC, &tailC);
						}
						turnC++;
					}
					turnC++;
					removeCard(p1, &p1head, &p1tail);
				}
				system("cls");
			}

			//Player 2's turn
			if (turnC % 2 == 0) {
				printf("Discard Pile:\n");
				printDeck(&headD, discardSize);

				//If statements for displaying color
				if (strcmp(d->suit, "Blue") == 0) {
					printf("\n\nThe current color is Blue.\n");
				}
				if (strcmp(d->suit, "Yellow") == 0) {
					printf("\n\nThe current color is Yellow.\n");
				}
				if (strcmp(d->suit, "Green") == 0) {
					printf("\n\nThe current color is Green.\n");
				}
				if (strcmp(d->suit, "Red") == 0) {
					printf("\n\nThe current color is Red.\n");
				}

				//If statements for displaying if a player has uno
				if (p2hand == 1) {
					printf("\nPlayer 2 has UNO!\n");
				}
				if (p1hand == 1) {
					printf("\nPlayer 1 has UNO!\n");
				}

				//Print statements for player 2's hand and prompt for which card they want to play
				printf("\nPlayer 2 Hand: \n\n");
				printPlayerHand(&p2head, p2hand);
				printf("Enter a number to play that card. Enter 0 to draw. ");
				scanf("%d", &x);

				//Finding selected card
				for (i = 0; i < x - 1; i++) {
					p2 = p2->next;
				}

				//Check if the card can be played, if not loop until they play a correct card
				while (playability(p2, d) == FAIL && x != 0) {
					p2 = p2head;
					printf("You cannot play that card. Enter in a valid number.\n");
					scanf("%d", &x);
					if (x == 0) {
						break;
					}
					for (i = 0; i < x - 1; i++) {
						p2 = p2->next;
					}
				}

				//If they enter 0, draw a card, otherwise play the card selected
				if (x == 0) {
					//If headC is NULL, move the discard pile back into the deck
					if (headC == NULL) {
						c = headC;
						for (j = 0; j < (discardSize - 5); j++) {
							discardAddition(&headD, &c);
							c = c->next;
						}
					}
					p2hand = p2hand + 1;
					addCards(p2tail, &p2head, &p2tail);
					pHandAddition(&headC, &p2tail);
					removeCard(headC, &headC, &tailC);
					turnC++;
				}
				else {
					colorChoice(p2);
					d = d->next;
					p2hand = p2hand - 1;
					discardSize++;
					discardAddition(&p2, &d);
					//If player 2's hand is empty, they win
					if (p2hand == 0) {
						printf("\nPlayer 2 Wins!\n\n");
						break;
					}
					//If conditions for if the card played was a special card
					if (strcmp(p2->action, "Skip") == 0 || strcmp(p2->action, "Reverse") == 0) {
						turnC++;
					}
					if (strcmp(p2->action, "Draw 2") == 0) {
						for (i = 0; i < 2; i++) {
							p1hand = p1hand + 1;
							addCards(p1tail, &p1head, &p1tail);
							pHandAddition(&headC, &p1tail);
							removeCard(headC, &headC, &tailC);
						}
						turnC++;
					}
					if (strcmp(p2->action, "Draw 4") == 0) {
						for (i = 0; i < 4; i++) {
							p1hand = p1hand + 1;
							addCards(p1tail, &p1head, &p1tail);
							pHandAddition(&headC, &p1tail);
							removeCard(headC, &headC, &tailC);
						}
						turnC++;
					}
					turnC++;
					removeCard(p2, &p2head, &p2tail);
				}
				system("cls");
			}
		}
		playerTurn++;
		//If player 1's hand is empty, count player 2's hand score for player 1
		if (p1hand == 0) {
			p2 = p2head;
			p1score = p1score + scoreC(p2, p2hand);
		}
		//If player 2's hand is empty, count player 1's hand score for player 2
		else if (p2hand == 0) {
			p1 = p1head;
			p2score = p2score + scoreC(p1, p1hand);
		}
	}

	//Whoever gets to 500 points first wins the game
	if (p1score >= 500) {
		printf("Congratulations! Player 1 has beaten Player 2!\n");
	}
	else if (p2score >= 500) {
		printf("Congratulations! Player 2 has beaten Player 1!\n");
	}

	return 0;
}