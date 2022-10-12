#include <cs50.h>
#include <string.h>
#include <stdio.h>

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
bool check_cycle(int head, int tail);
void print_winner(void);

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
    bool found = false;
    int i = 0;
    // Go through candidate array name to find the candidate's name given
    for (i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            // If found, update 'found' to true and break the loop
            found = true;
            break;
        }
    }
    // If found, update the 'ranks' array and return true
    if (found)
    {
        ranks[rank] = i;
        return true;
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Iterate through the whole ranks array (with the size of 'candidate_count')
    for (int i = 0; i < candidate_count; i++)
    {
        // At each rank (the rank[0] would be the most preferred one), see who is preferred over who
        // Of course, the rank[0] is preferred over the rank[1], rank[2], and so forth
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Update preferences array as explained in the walkthrough video
            preferences[ranks[i]][ranks[j]] ++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Iterates over the 2D array of preferences
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // If there's more voters that prefers 'i' to 'j' than 'j' to 'i', so 'i' is the winner and 'j' is the loser
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                // Update the numbers of pairs
                pair_count ++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Initialize our temporary variable to do the swap operation
    pair tmp;
    // Sort the pairs array with the Bubble Sort, but inverted, i.e., the smallest numbers bubble away to the end
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count - 1; j++)
        {
            // The strength of a pair is measured by the number of people that prefer one candidate over the other
            // That way, we can see the number of preference of the [j] pair and compare to the [j + 1] pair
            // (Recall that we are implementing a "inverted Bubble Sort", so to speak)
            if ((preferences[pairs[j].winner][pairs[j].loser]) < (preferences[pairs[j + 1].winner][pairs[j + 1].loser]))
            {
                // Swap the pairs
                tmp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = tmp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Loops through over all pairs
    for (int i = 0; i < pair_count; i++)
    {
        // Check if that pair creates a cycle
        if (!check_cycle(pairs[i].winner, pairs[i].loser))
        {
            // If not, lock that pair
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

bool check_cycle(int head, int tail)
{
    // Our stop point: if the head ends up being the tail (as a snake biting itself), we have a cycle
    /*
        In our bellow example, i -> j means that i wins over j
        Say we have the following: a -> b -> c -> a
        See that 'a' is the same as the head (i.e., the first candidate that wins over b) and tail (the candidate that 'c' beats) at the same time
    */ // [https://gist.github.com/nicknapoli82/6c5a1706489e70342e9a0a635ae738c9]
    if (head == tail)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        // Checks if the loser of this pair wins over other candidate, i.e., if there's a lock 'loser' -> 'other candidate'
        if (locked[tail][i])
        {
            // Checks recursively if that 'other candidate' is the 'head', i.e., the 'other candidate' is our tail so if he/she is our head as well, then we got a cycle going
            if (check_cycle(head, i))
            {
                return true;
            }
        }
    }
    // If we passed over all the candidates and there wasn't a cycle, return false, i.e., we didn't found a cycle
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // Initialize our local variables
    bool no_defeats = false;
    int index_winner = 0;
    // Loops througth all the 2D 'locked' array
    for (int j = 0; j < candidate_count; j++)
    {
        for (int i = 0; i < candidate_count; i++)
        {
            // Checks a candidate's column in our 'locked' matrix
            // (See that 'j' represents the candidate's column and 'i' the candidate1's row)
            // If all of the candidate's column is set to 'false', this means that anyone is 'pointing' to him/her, i.e., winning against him/her
            if (locked[i][j] == false)
            {
                // "Remember" that he/she has no defeats
                no_defeats = true;
            }
            else
            {
                // If there's somebody pointing to that candidate, this means that he/she is not our winner, so break the inner loop
                no_defeats = false;
                break;
            }
        }
        // If we passed through all over the candidate's column and he/she has no defeats, we found our winner!
        if (no_defeats)
        {
            // Update the 'index_winner' to the candidate's column index
            index_winner = j;
            // Break the external loop, as we don't need to loop over other candidate
            break;
        }
    }
    // Print the winner
    printf("%s\n", candidates[index_winner]);
    return;
}