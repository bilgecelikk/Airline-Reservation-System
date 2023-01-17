/*
Bilgenur Çelik

gcc -std=c99 -Wall -Werror AirlineRes.c -o AirlineRes
python -m calico.cli AirlineRes.yaml
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void print_current(char **seats, int rows, int seats_per_row, int *add_aisle_to,int aisles){
    char letters[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
    printf("  ");
    int k=0;
    for(int j=0; j<seats_per_row; j++){
        if(j == add_aisle_to[k]){
            printf("|  | ");
            k++;
        }
        printf("%c ", letters[j]);
    }
    printf("\n");
    // first row with letters is done.

    int aisle_there = 0;
    for(int i=0; i<rows; i++){
        printf("%d ", i+1);// <- (displayed row numbers(1,2,..))
        for(int j=0; j<seats_per_row; j++){
            for(int k=0; k<aisles; k++){
                aisle_there = add_aisle_to[k];
                if(aisle_there==j){
                    printf("|  | ");
                }
            }
            printf("%c ",seats[i][j]);
        }
        printf(" \n");
    }
}

// code handles aisles in dist() function:
//Because we use four operatons on i's and j's, it doesn't matter if our j's are out of scope of seats_per_row!
double dist(int i1,int j1, int i2,int j2,int *add_aisle_to,int aisles){ //distance between s1 and s2.
    for(int i=0; i<aisles; i++){
        if(j1 >= add_aisle_to[i]+i){
            j1++;
        }
        if(j2 >= add_aisle_to[i]+i){
            j2++;
        }
    }
    return abs(i1-i2)+ abs(j1-j2)/2.0;
}
double adjusted_dist(int i1,int j1, int i2,int j2,int *add_aisle_to, int aisles){
    if(dist(i1,j1,i2,j2,add_aisle_to, aisles)==0){
        return 0;
    }
    else{
        double addist=pow(100,(1/dist(i1,j1,i2,j2, add_aisle_to, aisles)));
        addist=100-addist;
        return addist;
    }
}
// score() finds the mark for only one seat:
double score(char **seats, int i1, int j1, int rows, int seats_per_row,int *add_aisle_to, int aisles){
    double mark = 0;
    for(int i=0;i<rows;i++){
        for(int j=0;j<seats_per_row;j++){
            if(seats[i][j]=='X'){
                mark+=adjusted_dist(i1,j1,i,j,add_aisle_to, aisles);
            }
        }
    }
    return mark;
}

//rezerv() can take one or more 'people' variable:
//And it prints the output via print_current():
void rezerv(int people, char **seats, int rows, int seats_per_row,int *add_aisle_to, int aisles){
    int count = 0;
    double summarks = 0;
    int largestindex_i = -1;
    int largestindex_j = -1;
    double max_summarks =- pow(10,9);

    for(int i=0;i<rows;i++){
        for(int j=0; j<seats_per_row-people+1; j++){

            count=0;
            summarks=0;
            for(int a=0; a<people; a++){
                if(seats[i][j+a]=='-'){
                    count++;
                }
            }

            if(count==people){
                for(int b=0; b<people; b++){
                    summarks+=score(seats, i, j+b, rows, seats_per_row, add_aisle_to, aisles);
                }

                if(summarks>max_summarks){
                    max_summarks=summarks;
                    largestindex_i=i;
                    largestindex_j=j;
                }
            }
        }
    }

    if(largestindex_i == -1){
        printf("\nNo available seats for the requested reservation!\n\n");
    }
    else{
        //time to change - to + ; after we're done + to X:
        char letters[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
        printf("Reserved seats: ");
        for(int i=0;i<people;i++){
            seats[largestindex_i][largestindex_j+i]='+';
            printf("%d%c ", largestindex_i+1, letters[largestindex_j+i]);
        }
        printf("\n");

        print_current(seats, rows, seats_per_row, add_aisle_to, aisles);
        for(int i=0;i<people;i++){
            seats[largestindex_i][largestindex_j+i]='X';
        }
    }
}

int main(){
    int rows=0, seats_per_row=0, aisles=0;
    bool statement = false;

    while(!statement){
        printf("Number of rows: ");
        scanf("%i",&rows );
        printf("Number of seats per row: ");
        scanf("%i",&seats_per_row );
        printf("Number of aisles: ");
        scanf("%i",&aisles );
        statement = (rows<100&&rows>0)&&(seats_per_row<21&&seats_per_row>0)&&(aisles<6&&aisles>0);
    }
    int add_aisle_to[aisles];

    for(int i=0; i<aisles; i++){
        printf("Add aisle %i after seat: ", i+1);
        scanf("%i", &add_aisle_to[i]);
    }//'aisles' is the number of aisles.

    //creating seats[][]:
    char *seats[rows];
    for(int i=0; i<rows; i++){
        seats[i]=(char*) malloc(sizeof(char)*seats_per_row);
    }
    //initializing:
    for(int i=0; i<rows; i++){
        for(int j=0; j<seats_per_row; j++){
            seats[i][j]='-';
        }
    }

    print_current(seats, rows, seats_per_row, add_aisle_to, aisles);

    //now everything is ready for user to enter some people variables:
    int people = 0;
    while(people != -1){
        printf("The number of people in the reservation\n (0: print current reservations, -1: exit): ");
        scanf("%d", &people);

        if(people==0){
            print_current(seats, rows, seats_per_row, add_aisle_to, aisles);
        }
        else if(people>0){
            rezerv(people, seats, rows, seats_per_row, add_aisle_to, aisles);
        }
    }

    //let malloc go:
    for(int i=0; i<rows; i++){
        free(seats[i]);
        seats[i]=NULL;
    }
    return EXIT_SUCCESS;
}
