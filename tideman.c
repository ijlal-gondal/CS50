#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_cycle(int end, int start);
int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //looks for candidates in the candidate list by comparing to voter input and updates the ranks
    for (int i = 0; i < candidate_count ; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i  ;
            return true;
        }
    }
    // TODO
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i; j < candidate_count - 1 ; j++)
        {
            preferences[ranks[i]][ranks[j + 1]]++;

        }
    }
    // printf("%i", preferences[0][1]);
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // printf("%i", preferences[0][1]);
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i; j < candidate_count - 1; j++)
        {
            // printf("%i", preferences[i][j+1]);
            // printf("%i", preferences[j+1][i]);
            if ((preferences[i][j + 1]) > (preferences[j + 1][i]))
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j + 1;
                pair_count++;
                // printf("%i", pair_count);
            }
            else if ((preferences[i][j + 1]) < (preferences[j + 1][i]))
            {
                pairs[pair_count].winner = j + 1;
                pairs[pair_count].loser = i;
                pair_count++;
                // printf("%i", pair_count);
            }

        }
    }
    // TODO
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair tmp[0];
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            if ((preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]) < 
                (preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner]))
            {
                tmp[0].winner = pairs[i].winner;
                tmp[0].loser = pairs[i].loser;
                pairs[i].winner = pairs[j].winner;
                pairs[i].loser = pairs[j].loser;
                pairs[j].winner = tmp[0].winner;
                pairs[j].loser = tmp[0].loser;
            }
        }
    }
    // TODO
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        // for(int j=i+1; j<pair_count; j++)
        // {
        if (!check_cycle(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        // }
    }
// if (pairs[pair_count-1].loser != pairs[0].winner)
// {
// locked[pairs[pair_count-1].winner][pairs[pair_count-1].loser] = true;
// }
// for (int j = 0; j <candidate_count-1; j++)
// {
//     for (int k=j; k<candidate_count-1; k++)
//     {
//       locked[k][j]

//     }
// }

    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int counter = 0;
        for (int j = 0; j < candidate_count; j++)
        {

            if (!locked[j][i])
            {
                counter++;
               
            }
            if (counter == candidate_count)
            {
                printf("%s\n", candidates[i]);
            }
        }
    }
    // TODO
    return;
}

bool check_cycle(int end, int start)
{
    if (end == start)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[end][i])
        {
            if (check_cycle(i, start))
            {
                return true;
            }
        }
    }

    return false;
}