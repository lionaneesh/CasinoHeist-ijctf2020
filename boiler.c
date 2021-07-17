/* Code for a Slot Machine Controller*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Bets {
 unsigned long val;
} BET;

typedef struct User {
 char *name;
 int profitable;
 unsigned long cash;
} USER;

int idx_jour;
#define BASE_CASH 20

void add_bet(BET *bets, int idx, BET *cur) {
	bets[idx].val = cur->val;
}

void print_bets(BET *bets) {
	int i;
	for (i = 0; i <= idx_jour; i++) {
		printf("Bet #%d: Value %ld\n", i, bets[i].val);
	}
}

int main(void)
{

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  BET bets_journal[100];
  int bet=5;
  int numO;
  int numT;
  int numTh;
  int yesNo;
  USER user;
  char user_name[100];
  int mult_match = 2;
  int mult_jack = 10;
  BET cur; 
  srand(time(NULL));
  printf("Welcome to the Slot Machines!\nEnter your name:\n");
  fgets(user_name, 100, stdin);
  int counter = 0;
  int winamt = 0;
  user.name = user_name;
  user.profitable = 0;
  char bet_val[21];
  user.cash = BASE_CASH;

  printf("Welcome %s-\n\tYou start with $%ld cash.\n", user.name, user.cash);
  printf("\tMatch 2 numbers to win %dx the bet amount. Match 3 numbers to win %dx amount.\n", mult_match, mult_jack);
  printf("\tMin bet: $5");

  printf("\n");
	do
	{
		if (idx_jour < 0) {
			idx_jour = 0; 
		}

		printf("Current Balance=$%ld\n", user.cash);
		printf("Enter bet value: ");
		scanf("%20s", bet_val);
		cur.val = atol(bet_val);
		numO=1 + rand() % 10;
		numT=1 + rand() % 10;
		numTh=1 + rand() % 10;
		if (user.cash < cur.val) {
			printf("Not enough funds.\n");
			idx_jour -= 1; // bug 1 to allow changing bets_joutnal 1 below required to control the stack after leak. 
			continue;
		}
		if (strchr(bet_val, 'x') != NULL) {
			printf("Cashing out $%ld!\n", user.cash);
			break;
		}
		if (strchr(bet_val, 'v') != NULL) {
			print_bets(bets_journal);
			continue;
		}
	
		if (cur.val < 5) {
			printf("Please bet atleast $5\n");
			idx_jour -= 1;
			continue;
		}
		
		if (user.profitable && (strchr(bet_val, 'm') != NULL)) {
			printf("Margins applied on this bet. You get 2x margin.\n");
			cur.val = cur.val * 2;
		}
	
		printf("	Spin #%2d: ", counter);
		printf("%d %d %d 	", numO, numT, numTh);
		user.cash -= cur.val;
		if ((numO==numT) || (numT==numTh) || (numTh==numO)) {
			winamt = cur.val * mult_match;
			user.cash += winamt;
			printf("You win $%d!\n", winamt);
		}
		else if((numO==numT)&&(numT==numTh)&&(numTh==numO))
		{
			winamt = cur.val * mult_jack;
			user.cash += winamt;
			printf("Jackpot! You win $%d!\n", winamt);
		}
		else {
			printf("You lose!\n");
		}
		add_bet(bets_journal, idx_jour, &cur);
		printf("bets_journal %p, idx: %d\n", bets_journal, idx_jour);
		idx_jour += 1;
		if (user.cash > BASE_CASH) {
			user.profitable = 1;
			printf("\nCongrats! As you are our profitable customer we have enabled Margin bets for you. Append m with betvalue to have margins.\n");
		}

	} while (user.cash > 0);
	return 0;
}
