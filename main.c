#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
Name: Padraig O Cosgora
Date 15/12/2019

Question:
You have been asked to provide a system (using the C programming language) for analysing
the results of a recent competition in the west of Ireland. The participants’ results will be
provided in random order as a text file.

Solution below:
Solution below specifically looks for a "data.txt" file with all bodybuilders information or creates a new file itself.

Functionalty: 
[*] user menu
[*] dynamically allocated array
[*] All 9 requested functions included and functional



*/

#define ADD_COMP 1    //define statements
#define VIEW_ALL 2
#define SORT_COMP_BY_NAME 3
#define SORT_COMP_BY_WEIGHT 4
#define FIND_DISQUALIFIED 5
#define FIND_BEST_OVERALL 6
#define QUIT 7
#define NAMESTRLEN 50
#define INITSIZE 3
#define MAXSIZE 100
#define MAX_LIFTS 3


typedef struct {
	int lifter_ID;
	char lifter_name[NAMESTRLEN + 1];
	char lifter_gender[NAMESTRLEN + 1];
	float lifter_weight;
	char weight_category[NAMESTRLEN + 1];
	float squat[MAX_LIFTS];
	float bench[MAX_LIFTS];
	float deadlift[MAX_LIFTS];
} Competitors, temp;

typedef struct {
	Competitors* items;
	int numlines;
} allitems_t;


allitems_t* initialize_arraystructs(void);
void check_ptr(void* ptr, const char* msg);
int display_menu();     //function prototypes
int load_data(FILE* filestream, allitems_t* allitems);
int add_new_competitor_result(allitems_t* allitems);
int view_all_competitor_results(allitems_t* allitems);
int sort_competitor_results_by_name(allitems_t* allitems);
int sort_competitor_results_by_category(allitems_t* allitems);
int find_disqualified_competitors(allitems_t* allitems);
int find_overall_best_lifter(allitems_t* allitems);
int save_data(allitems_t* allitems);


int main()
{
	int choice;     // main variables
	allitems_t* allitems;
	allitems = initialize_arraystructs();
	FILE* fp;
	fp = fopen("data.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "%s\n", "Error reading file!\n");
		printf("Unable to open the file '%s' for input\n", fp);
		printf("Created new empty file default.txt instead\n");
		fp = fopen("default.txt", "w+");
		//exit(EXIT_FAILURE);
	}
	load_data(fp, allitems);

	choice = display_menu();   // get user's first selection

	while (choice != QUIT)   //execute so long as choice is not equal to QUIT
	{
		switch (choice)
		{
		case ADD_COMP:
			add_new_competitor_result(allitems);
			break;
		case VIEW_ALL: view_all_competitor_results(allitems);
			break;
		case SORT_COMP_BY_NAME: sort_competitor_results_by_name(allitems);
			break;
		case SORT_COMP_BY_WEIGHT: sort_competitor_results_by_category(allitems);
			break;
		case FIND_DISQUALIFIED: find_disqualified_competitors(allitems);
			break;
		case FIND_BEST_OVERALL: find_overall_best_lifter(allitems);
			break;
		default:    printf("Oops! An invalid choice slipped through. ");
			printf("Please try again.\n");
		}
		choice = display_menu(); /* get user's subsequent selections */
	}
	save_data(allitems);
	printf("Exiting.. Bye bye!\n");
	return 0;
}

int display_menu(void)
{
	int option;
	printf("Powerlifting Analysis System\n\n");
	printf("1.\tAdd new competitor result\n");
	printf("2.\tView all competitor results\n");
	printf("3.\tSort competitor results by name\n");
	printf("4.\tSort competitor results by weight category\n");
	printf("5.\tFind disqualified competitors\n");
	printf("6.\tFind the overall best lifter\n");
	printf("7.\tExit\n\n");

	printf("Make your selection: ");

	while ((scanf(" %d", &option) != 1) /* non-numeric input */
		|| (option < 0)               /* number too small */
		|| (option > 8))              /* number too large */
	{
		fflush(stdin);                    /* clear bad data from buffer */
		printf("That selection isn't valid. Please try again.\n\n");
		printf("Your choice? ");
	}
	return option;
}

int load_data(FILE* filestream, allitems_t* allitems) {
	printf("Load data\n");
	int count = 0;
	char line[MAXSIZE];
	char* firstName, * secondName;
	char* gender, * weight, * class;
	char* c_squat[3], * c_deadlift[3], * c_bench[3];
	float squat[3], deadlift[3], bench[3];
	size_t numitems = INITSIZE;

	allitems->items = malloc(numitems * sizeof(Competitors));
	check_ptr(allitems->items, "Initial Allocation");

	while (fgets(line, MAXSIZE, filestream) != NULL) {
		firstName = strtok(line, " ");     /* First Token */
		//printf("\nfirstName: %s\n", firstName);
		secondName = strtok(NULL, " ");     /* Next Token */
		//printf("\nsecondName: %s\n", secondName);
		gender = strtok(NULL, " ");     /* Next Token */
		//printf("\ngender: %s\n", gender);
		weight = strtok(NULL, " ");     /* Next Token */
		//printf("\nweight: %s\n", weight);
		class = strtok(NULL, " ");     /* First Token */
		//printf("\nclass: %s\n", class);

		c_squat[0] = strtok(NULL, " ");     /* Next Token */
		//printf("\nc_squat[0]: %s\n", c_squat[0]);
		squat[0] = strtof(c_squat[0], NULL);

		c_squat[1] = strtok(NULL, " ");     /* Next Token */
		//printf("\nc_squat[1]: %s\n", c_squat[1]);
		squat[1] = strtof(c_squat[1], NULL);

		c_squat[2] = strtok(NULL, " ");     /* Next Token */
		//printf("\nc_squat[2]: %s\n", c_squat[2]);
		squat[2] = strtof(c_squat[2], NULL);

		c_bench[0] = strtok(NULL, " ");     /* First Token */
		//printf("\nc_bench[0]: %s\n", c_bench[0]);
		bench[0] = strtof(c_bench[0], NULL);

		c_bench[1] = strtok(NULL, " ");     /* Next Token */
		//printf("\nc_bench[1]: %s\n", c_bench[1]);
		bench[1] = strtof(c_bench[1], NULL);

		c_bench[2] = strtok(NULL, " ");     /* Next Token */
		//printf("\nc_bench[2]: %s\n", c_bench[2]);
		bench[2] = strtof(c_bench[2], NULL);

		c_deadlift[0] = strtok(NULL, " ");     /* Next Token */
		//printf("\nc_deadlift[0]: %s\n", c_deadlift[0]);
		deadlift[0] = strtof(c_deadlift[0], NULL);

		c_deadlift[1] = strtok(NULL, " ");     /* First Token */
		//printf("\nc_deadlift[1]: %s\n", c_deadlift[1]);
		deadlift[1] = strtof(c_deadlift[1], NULL);

		c_deadlift[2] = strtok(NULL, "\n");     /* Next Token */
		//printf("\nc_deadlift[2]: %s\n\n", c_deadlift[2]);
		deadlift[2] = strtof(c_deadlift[2], NULL);

		if (count == numitems) {
			numitems *= 2;
			allitems->items = realloc(allitems->items, numitems * sizeof(Competitors));
			check_ptr(allitems->items, "Reallocation");
		}

		char* destination[100];
		sprintf(destination, "%s %s", firstName, secondName);
		allitems->items[count].lifter_ID = count;
		strncpy(allitems->items[count].lifter_name, destination, 40);
		strncpy(allitems->items[count].weight_category, class, 40);
		strncpy(allitems->items[count].lifter_gender, gender, 40);
		allitems->items[count].lifter_weight = strtof(weight, NULL);
		allitems->items[count].squat[0] = squat[0];
		allitems->items[count].squat[1] = squat[1];
		allitems->items[count].squat[2] = squat[2];
		allitems->items[count].bench[0] = bench[0];
		allitems->items[count].bench[1] = bench[1];
		allitems->items[count].bench[2] = bench[2];
		allitems->items[count].deadlift[0] = deadlift[0];
		allitems->items[count].deadlift[1] = deadlift[1];
		allitems->items[count].deadlift[2] = deadlift[2];

		count++;
		allitems->numlines++;
	}
	return 0;
}

int add_new_competitor_result(allitems_t* allitems) {
	char gender[2], class[10], fName[50], sName[50];
	float squat[3], deadlift[3], bench[3], weight;
	printf("add_new_competitor_result\n");

	printf("\nPlease enter First name: ");
	scanf("%s", &fName);
	printf("\nPlease enter Second name: ");
	scanf("%s", &sName);

	printf("\nName: %s %s", fName, sName);
	printf("\nPlease enter gender(M/F): ");
	scanf("%s", gender);

	printf("\nPlease enter body weight: ");
	scanf("%f", &weight);
	if (strcmp(gender, "F") != 0) {
		printf("\nPlease enter male category(<74Kg, <83Kg, <93Kg, <105Kg, <120Kg, <120Kg+): ");
		scanf("%s", class);
	}
	else {
		printf("\nPlease enter female category(<47Kg, <52Kg, <57Kg, <63Kg, <72Kg, <84Kg, 84Kg+): ");
		scanf("%s", class);
	}
	printf("\nPlease enter squat attempts: (e.g 100 110 120): ");
	scanf("%f %f %f", &squat[0], &squat[1], &squat[2]);
	printf("\nPlease enter bench attempts: (e.g 100 110 120): ");
	scanf("%f %f %f", &bench[0], &bench[1], &bench[2]);
	printf("\nPlease enter deadlift attempts: (e.g 100 110 120): ");
	scanf("%f %f %f", &deadlift[0], &deadlift[1], &deadlift[2]);


	allitems->items = realloc(allitems->items, allitems->numlines * sizeof(Competitors));
	check_ptr(allitems->items, "Reallocation");

	char* destination[100];
	sprintf(destination, "%s %s", fName, sName);
	allitems->items[allitems->numlines].lifter_ID = allitems->numlines;
	strncpy(allitems->items[allitems->numlines].lifter_name, destination, 40);
	strncpy(allitems->items[allitems->numlines].weight_category, class, 40);
	strncpy(allitems->items[allitems->numlines].lifter_gender, gender, 40);
	allitems->items[allitems->numlines].lifter_weight = weight;
	allitems->items[allitems->numlines].squat[0] = squat[0];
	allitems->items[allitems->numlines].squat[1] = squat[1];
	allitems->items[allitems->numlines].squat[2] = squat[2];
	allitems->items[allitems->numlines].bench[0] = bench[0];
	allitems->items[allitems->numlines].bench[1] = bench[1];
	allitems->items[allitems->numlines].bench[2] = bench[2];
	allitems->items[allitems->numlines].deadlift[0] = deadlift[0];
	allitems->items[allitems->numlines].deadlift[1] = deadlift[1];
	allitems->items[allitems->numlines].deadlift[2] = deadlift[2];

	allitems->numlines++;

	return 1;
}
int view_all_competitor_results(allitems_t* allitems) {
	printf("view_all_competitor_results\n");
	int i;
	for (i = 0; i < allitems->numlines; i++) {
		//printf("numLines: %d\n", allitems->numlines);
		printf("ID : %d\n", allitems->items[i].lifter_ID);
		printf("Name : %s\n", allitems->items[i].lifter_name);
		printf("Gender : %s\n", allitems->items[i].lifter_gender);
		printf("Weight : %.02f\n", allitems->items[i].lifter_weight);
		printf("Category : %s\n", allitems->items[i].weight_category);
		printf("Squat[0] : %.0f\n", allitems->items[i].squat[0]);
		printf("Squat[1] : %.0f\n", allitems->items[i].squat[1]);
		printf("Squat[2] : %.0f\n", allitems->items[i].squat[2]);
		printf("Bench[0] : %.0f\n", allitems->items[i].bench[0]);
		printf("Bench[1] : %.0f\n", allitems->items[i].bench[1]);
		printf("Bench[2] : %.0f\n", allitems->items[i].bench[2]);
		printf("Deadlift[0] : %.0f\n", allitems->items[i].deadlift[0]);
		printf("Deadlift[1] : %.0f\n", allitems->items[i].deadlift[1]);
		printf("Deadlift[2] : %.0f\n\n", allitems->items[i].deadlift[2]);
	}
	return 2;
}
int sort_competitor_results_by_name(allitems_t* allitems) {
	printf("sort_competitor_results_by_name\n");
	int backdown;
	int i, j = allitems->numlines, s = 1;
	//char temp[100];
	Competitors temp;
	while (s) {
		s = 0;
		for (i = 1; i < allitems->numlines; i++) {
			for (j = i + 1; j < allitems->numlines; j++) {
				if (strcmp(&allitems->items[i].lifter_name[0], &allitems->items[j].lifter_name[0]) > 0) {
					//printf("compare true\n");
					//printf("Comparing %c to %c\n", allitems->items[i].lifter_name[0], allitems->items[j].lifter_name[0]);
					strcpy(temp.lifter_name, &allitems->items[i].lifter_name[0]);
					strcpy(temp.lifter_gender, &allitems->items[i].lifter_gender[0]);
					strcpy(temp.weight_category, &allitems->items[i].weight_category[0]);
					temp.lifter_weight = allitems->items[i].lifter_weight;
					temp.lifter_ID = allitems->items[i].lifter_ID;
					temp.squat[0] = allitems->items[i].squat[0];
					temp.squat[1] = allitems->items[i].squat[1];
					temp.squat[2] = allitems->items[i].squat[2];
					temp.bench[0] = allitems->items[i].bench[0];
					temp.bench[1] = allitems->items[i].bench[1];
					temp.bench[2] = allitems->items[i].bench[2];
					temp.deadlift[0] = allitems->items[i].deadlift[0];
					temp.deadlift[1] = allitems->items[i].deadlift[1];
					temp.deadlift[2] = allitems->items[i].deadlift[2];

					strcpy(&allitems->items[i].lifter_name[0], &allitems->items[j].lifter_name[0]);
					strcpy(&allitems->items[i].lifter_gender[0], &allitems->items[j].lifter_gender[0]);
					strcpy(&allitems->items[i].weight_category[0], &allitems->items[j].weight_category[0]);
					allitems->items[i].lifter_weight = allitems->items[j].lifter_weight;
					allitems->items[i].lifter_ID = allitems->items[j].lifter_ID;
					allitems->items[i].squat[0] = allitems->items[j].squat[0];
					allitems->items[i].squat[1] = allitems->items[j].squat[1];
					allitems->items[i].squat[2] = allitems->items[j].squat[2];
					allitems->items[i].bench[0] = allitems->items[j].bench[0];
					allitems->items[i].bench[1] = allitems->items[j].bench[1];
					allitems->items[i].bench[2] = allitems->items[j].bench[2];
					allitems->items[i].deadlift[0] = allitems->items[j].deadlift[0];
					allitems->items[i].deadlift[1] = allitems->items[j].deadlift[1];
					allitems->items[i].deadlift[2] = allitems->items[j].deadlift[2];

					strcpy(&allitems->items[j].lifter_name[0], temp.lifter_name);
					strcpy(&allitems->items[j].lifter_gender[0], temp.lifter_gender);
					strcpy(&allitems->items[j].weight_category[0], temp.weight_category);
					allitems->items[j].lifter_weight = temp.lifter_weight;
					allitems->items[j].lifter_ID = temp.lifter_ID;
					allitems->items[j].squat[0] = temp.squat[0];
					allitems->items[j].squat[1] = temp.squat[1];
					allitems->items[j].squat[2] = temp.squat[2];
					allitems->items[j].bench[0] = temp.bench[0];
					allitems->items[j].bench[1] = temp.bench[1];
					allitems->items[j].bench[2] = temp.bench[2];
					allitems->items[j].deadlift[0] = temp.deadlift[0];
					allitems->items[j].deadlift[1] = temp.deadlift[1];
					allitems->items[j].deadlift[2] = temp.deadlift[2];

					s = 1;
				}
			}
		}
	}
	FILE* fileptr;
	fileptr = fopen("sorted_names.txt", "w+");

	//    while(fileptr == NULL)
	//    {
	//        printf("Unable to open the file sorted_names.txt for output\n");
	//    }

	for (backdown = 0; backdown < allitems->numlines; backdown++) {
		printf("\n%d: %s\n", backdown, allitems->items[backdown].lifter_name);
		fprintf(fileptr, "%s %s %.02f %s %.02f %.02f %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
			allitems->items[backdown].lifter_name, allitems->items[backdown].lifter_gender, allitems->items[backdown].lifter_weight,
			allitems->items[backdown].weight_category, allitems->items[backdown].squat[0], allitems->items[backdown].squat[1],
			allitems->items[backdown].squat[3], allitems->items[backdown].bench[0], allitems->items[backdown].bench[1],
			allitems->items[backdown].bench[2], allitems->items[backdown].deadlift[0], allitems->items[backdown].deadlift[1],
			allitems->items[backdown].deadlift[2]);
		// name, gender, weight, category, squatsx3, benchx3, deadlift x3
	}
	fclose(fileptr); /* fclose closes file */
	printf("Sorted names stored in sorted_names.txt.\n");
	return 3;
}
int sort_competitor_results_by_category(allitems_t* allitems) {
	char user_entry[10];
	printf("sort_competitor_results_by_category\n");
	printf("\nPlease enter weight category\n"
		"(e.g for Females: 47, 52, 57, 63, 72, 84 or 84+)\n"
		"(e.g for Males: 74, 83, 93, 105, 120, 120+) : ");
	scanf("%s", &user_entry);
	int backdown;
	int results = 0;
	Competitors temp[200];
	int i, j = allitems->numlines, s = 1;
	for (i = 0; i < allitems->numlines; i++) {
		if (strcmp(&allitems->items[i].weight_category[0], user_entry) == 0) {
			printf("User found: %s\n", &allitems->items[i].lifter_name[0]);
			temp[results++] = allitems->items[i];
		}
	}
	float best_squat[200], best_bench[200], best_deadlift[200];
	float total_score[200], total_lifts[200], total_fails[200], total_passes[200];
	for (i = 0; i < results; i++) {
		total_fails[i] = 0;
		total_lifts[i] = 0;
		total_passes[i] = 0;
		total_score[i] = 0;
		best_bench[i] = 0;
		best_squat[i] = 0;
		best_deadlift[i] = 0;
		//printf("\n%d: %s\n",i, temp[i].weight_category);
		for (int k = 0; k < 3; ++k) {
			if (temp[i].squat[k] == 0) {
				total_passes[i] += 1;
			}
			else if (temp[i].squat[k] < 0) {
				total_fails[i] += 1;
			}
			else if (temp[i].squat[k] > temp[i + 1].squat[k]) {
				best_squat[i] = temp[i].squat[k];
				total_lifts[i] += 1;
			}
			else {
				best_squat[i] = temp[i + 1].squat[k];
				total_lifts[i] += 1;
			}
			if (temp[i].bench[k] == 0) {
				total_passes[i] += 1;
			}
			else if (temp[i].bench[k] < 0) {
				total_fails[i] += 1;
			}
			else if (temp[i].bench[k] > temp[i + 1].bench[k]) {
				best_bench[i] = temp[i].bench[k];
				total_lifts[i] += 1;
			}
			else {
				best_bench[i] = temp[i + 1].bench[k];
				total_lifts[i] += 1;
			}
			if (temp[i].deadlift[k] == 0) {
				total_passes[i] += 1;
			}
			else if (temp[i].deadlift[k] < 0) {
				total_fails[i] += 1;
			}
			else if (temp[i].deadlift[k] > temp[i + 1].deadlift[k]) {
				best_deadlift[i] = temp[i].deadlift[k];
				total_lifts[i] += 1;
			}
			else {
				best_deadlift[i] = temp[i + 1].deadlift[k];
				total_lifts[i] += 1;
			}
		}
		if (best_squat[i] > 0 && best_bench[i] > 0 && best_deadlift[i] > 0) {
			total_score[i] = best_squat[i] + best_bench[i] + best_deadlift[i];
		}
		else {
			total_score[i] = 0;
		}
	}
	printf("Results are as follows:\n");
	for (int i = 0; i < results; i++) {
		printf("\n%d:\nName: %s\nCategory: %s\nBest squat: %.02f\nBench bench: %.02f\nBest deadlift: %.02f\n"
			"Total fails: %.02f\nTotal lifts: %.02f\nTotal passes: %.02f\nTotal score: %.02f\n"
			, i + 1, temp[i].lifter_name, temp[i].weight_category, best_squat[i], best_bench[i],
			best_deadlift[i], total_fails[i], total_lifts[i], total_passes[i], total_score[i]);
	}
	return 4;
}
int find_disqualified_competitors(allitems_t* allitems) {
	printf("find_disqualified_competitors\n");
	int results = 0;
	Competitors temp[200];
	int i, j = allitems->numlines, s = 1;
	for (i = 0; i < j; i++) {
		temp[results++] = allitems->items[i];
		//printf("results: %d", results);
	}
	float best_squat[200], best_bench[200], best_deadlift[200];
	float total_score[200], total_lifts[200], total_fails[200], total_passes[200];
	for (i = 0; i < j; i++) {
		total_fails[i] = 0;
		total_lifts[i] = 0;
		total_passes[i] = 0;
		total_score[i] = 0;
		best_bench[i] = 0;
		best_squat[i] = 0;
		best_deadlift[i] = 0;
		//printf("\n%d: %s\n",i, temp[i].weight_category);
		for (int k = 0; k < 3; ++k) {
			if (temp[i].squat[k] == 0) {
				total_passes[i] += 1;
			}
			else if (temp[i].squat[k] < 0) {
				total_fails[i] += 1;
			}
			else if (temp[i].squat[k] > temp[i + 1].squat[k]) {
				best_squat[i] = temp[i].squat[k];
				total_lifts[i] += 1;
			}
			else {
				best_squat[i] = temp[i + 1].squat[k];
				total_lifts[i] += 1;
			}
			if (temp[i].bench[k] == 0) {
				total_passes[i] += 1;
			}
			else if (temp[i].bench[k] < 0) {
				total_fails[i] += 1;
			}
			else if (temp[i].bench[k] > temp[i + 1].bench[k]) {
				best_bench[i] = temp[i].bench[k];
				total_lifts[i] += 1;
			}
			else {
				best_bench[i] = temp[i + 1].bench[k];
				total_lifts[i] += 1;
			}
			if (temp[i].deadlift[k] == 0) {
				total_passes[i] += 1;
			}
			else if (temp[i].deadlift[k] < 0) {
				total_fails[i] += 1;
			}
			else if (temp[i].deadlift[k] > temp[i + 1].deadlift[k]) {
				best_deadlift[i] = temp[i].deadlift[k];
				total_lifts[i] += 1;
			}
			else {
				best_deadlift[i] = temp[i + 1].deadlift[k];
				total_lifts[i] += 1;
			}
		}
		if (best_squat[i] > 0 && best_bench[i] > 0 && best_deadlift[i] > 0) {
			total_score[i] = best_squat[i] + best_bench[i] + best_deadlift[i];
		}
		else {
			total_score[i] = 0;
		}
	}
	printf("\nDisqualified are as follows:\n");
	for (int i = 0; i < j; i++) {
		if (total_score[i] == 0) {
			printf("\n%d:\nName: %s\nCategory: %s\nBest squat: %.02f\nBench bench: %.02f\nBest deadlift: %.02f\n"
				"Total fails: %.02f\nTotal lifts: %.02f\nTotal passes: %.02f\nTotal score: %.02f\n"
				, i + 1, temp[i].lifter_name, temp[i].weight_category, best_squat[i], best_bench[i],
				best_deadlift[i], total_fails[i], total_lifts[i], total_passes[i], total_score[i]);
		}
	}

	return 5;
}
int find_overall_best_lifter(allitems_t* allitems) {
	printf("find_overall_best_lifter\n");
	// men
	double a = -216.0475144;
	double b = 16.2606339;
	double c = -0.002388645;
	double d = -0.00113732;
	double e = 7.01863 * exp(-6);
	double f = -1.291 * exp(-8);

	// female
	double a_f = 594.31747775582;
	double b_f = -27.23842536447;
	double c_f = 0.82112226871;
	double d_f = -0.00930733913;
	double e_f = 4.731582 * exp(-5);
	double f_f = -9.054 * exp(-8);
	int results = 0;
	Competitors temp[200];
	int i, j = allitems->numlines, s = 1;
	for (i = 0; i < j; i++) {
		temp[results++] = allitems->items[i];
		//printf("results: %d", results);
	}
	double total_wilks[200];
	float best_squat[200], best_bench[200], best_deadlift[200];
	float total_score[200], total_lifts[200], total_fails[200], total_passes[200];
	for (i = 0; i < j; i++) {
		total_wilks[i] = 0;
		total_fails[i] = 0;
		total_lifts[i] = 0;
		total_passes[i] = 0;
		total_score[i] = 0;
		best_bench[i] = 0;
		best_squat[i] = 0;
		best_deadlift[i] = 0;
		//printf("\n%d: %s\n",i, temp[i].weight_category);
		for (int k = 0; k < 3; ++k) {
			if (temp[i].squat[k] == 0) {
				total_passes[i] += 1;
			}
			else if (temp[i].squat[k] < 0) {
				total_fails[i] += 1;
			}
			else if (temp[i].squat[k] > temp[i + 1].squat[k]) {
				best_squat[i] = temp[i].squat[k];
				total_lifts[i] += 1;
			}
			else {
				best_squat[i] = temp[i + 1].squat[k];
				total_lifts[i] += 1;
			}
			if (temp[i].bench[k] == 0) {
				total_passes[i] += 1;
			}
			else if (temp[i].bench[k] < 0) {
				total_fails[i] += 1;
			}
			else if (temp[i].bench[k] > temp[i + 1].bench[k]) {
				best_bench[i] = temp[i].bench[k];
				total_lifts[i] += 1;
			}
			else {
				best_bench[i] = temp[i + 1].bench[k];
				total_lifts[i] += 1;
			}
			if (temp[i].deadlift[k] == 0) {
				total_passes[i] += 1;
			}
			else if (temp[i].deadlift[k] < 0) {
				total_fails[i] += 1;
			}
			else if (temp[i].deadlift[k] > temp[i + 1].deadlift[k]) {
				best_deadlift[i] = temp[i].deadlift[k];
				total_lifts[i] += 1;
			}
			else {
				best_deadlift[i] = temp[i + 1].deadlift[k];
				total_lifts[i] += 1;
			}
		}
		if (best_squat[i] > 0 && best_bench[i] > 0 && best_deadlift[i] > 0) {
			total_score[i] = best_squat[i] + best_bench[i] + best_deadlift[i];
		}
		else {
			total_score[i] = 0;
		}
	}

	printf("Results are as follows:\n");
	for (int i = 0; i < j; i++) {
		if (total_score[i] != 0) {
			double w = temp[i].lifter_weight;
			if (strcmp(temp[i].lifter_gender, "M") != 0) { // female
				printf("female: %s\n", temp[i].lifter_name);
				double first = pow(w, 2); double second = pow(w, 3); double third = pow(w, 4); double fourth = pow(w, 5);
				//                total_wilks[i] = ((total_score[i] * 500) / (a_f + (b_f * w) +
				//                        (c_f * (pow(w,2))) + (d_f * (pow(w,3))) + (e_f * (pow(w, 4))) + (f_f * (pow(w,5)))));
				total_wilks[i] = ((total_score[i] * 500) / (a_f + (b_f * w) +
					(c_f * first) + (d_f * second) + (e_f * third) + (f_f * fourth)));
			}
			else { // male
				printf("male: %s\n", temp[i].lifter_name);
				                total_wilks[i] = ((total_score[i] * 500) / (a + (b * w) +
				                                                                   (c * (pow(w,2))) + (d * (pow(w,3))) + (e * (pow(w, 4))) + (f * (pow(w,5)))));

			}
		}
		printf("\n%d:\nName: %s\nCategory: %s\nBest squat: %.02f\nBench bench: %.02f\nBest deadlift: %.02f\n"
			"Total fails: %.02f\nTotal lifts: %.02f\nTotal passes: %.02f\nTotal score: %.02f\nTotal Wilks Score: %.05lf\n"
			, i + 1, temp[i].lifter_name, temp[i].weight_category, best_squat[i], best_bench[i],
			best_deadlift[i], total_fails[i], total_lifts[i], total_passes[i], total_score[i], total_wilks[i]);
	}
	return 6;
}
int save_data(allitems_t* allitems) {
	printf("save_data\n");
	FILE* fileptr;
	fileptr = fopen("exit_and_save.txt", "w+");
	int backdown;
	int i, j = allitems->numlines, s = 1;
	//    while(fileptr == NULL)
	//    {
	//        printf("Unable to open the file sorted_names.txt for output\n");
	//    }

	for (backdown = 0; backdown < allitems->numlines; backdown++) {
		printf("\n%d: %s\n", backdown, allitems->items[backdown].lifter_name);
		fprintf(fileptr, "%s %s %.02f %s %.02f %.02f %.02f %.02f %.02f %.02f %.02f %.02f %.02f\n",
			allitems->items[backdown].lifter_name, allitems->items[backdown].lifter_gender, allitems->items[backdown].lifter_weight,
			allitems->items[backdown].weight_category, allitems->items[backdown].squat[0], allitems->items[backdown].squat[1],
			allitems->items[backdown].squat[3], allitems->items[backdown].bench[0], allitems->items[backdown].bench[1],
			allitems->items[backdown].bench[2], allitems->items[backdown].deadlift[0], allitems->items[backdown].deadlift[1],
			allitems->items[backdown].deadlift[2]);
		// name, gender, weight, category, squatsx3, benchx3, deadlift x3
	}
	fclose(fileptr); /* fclose closes file */
	printf("Sorted names stored in exit_and_save.txt.\n");
	return 7;
}

allitems_t* initialize_arraystructs(void) {
	allitems_t* allitems;

	allitems = malloc(sizeof(allitems_t));
	check_ptr(allitems, "Initial Allocation");

	allitems->items = NULL;
	allitems->numlines = 0;

	return allitems;
}

void check_ptr(void* ptr, const char* msg) {
	if (!ptr) {
		printf("Unexpected null pointer: %s\n", msg);
		exit(EXIT_FAILURE);
	}
}
