#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    bool found = false;
    int i = 0;
    for (i; i < candidate_count; i++)
    {
        if (strcasecmp(candidates[i].name, name) == 0)
        {
            // If the candidate's name is valid (i.e., was found in the array of candidate's names),
            // then change 'found' to true and break the 'for' loop
            found = true;
            break;
        }
    }
    if (found)
    {
        // Update the two dimentional array of preferences with the index of the candidate
        preferences[voter][rank] = i;
        return true;
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    int candidate_index = 0;
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            candidate_index = preferences[i][j];
            // If the candidate haven't been eliminated, add more one vote to him/her
            if (!candidates[candidate_index].eliminated)
            {
                candidates[candidate_index].votes += 1;
                // If the most preferred candidate is a valid one, we need to 'break' in order to not count for the others
                // possible candidates
                break;
            }
            // Else, the foor loop will check for the next voter's preference, i.e., will increase the 'j' by one
            // and also check if that candidate has been eliminated.
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int half = voter_count / 2;
    // Check for, each candidate, if the number of votes he/she has is higher than the half of voters (total votes)
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > half)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    // If we passed through all candidates and none of them won, return false
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int smallest = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        // If the first candidate, let's assume that his/her votes are the minimal one
        // (having that, we can compare to others candidate's votes numbers)
        if (i == 0)
        {
            smallest = candidates[i].votes;
        }
        else
        {
            // If the candidate haven't been eliminated and his/her votes are less than the 'min',
            // then update 'min' to that number of votes
            if (!candidates[i].eliminated && candidates[i].votes < smallest)
            {
                smallest = candidates[i].votes;
            }
        }
    }
    return smallest;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    bool tied = false;
    for (int i = 0; i < candidate_count; i++)
    {
        // If all of the non-eliminated candidates has the number of votes equal to the minimal 'min' number of votes,
        // we return true for each occurrency, i.e, all of them
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            tied = true;
        }
        // But, if the candidate haven't been eliminated and his/her number of votes are different from the 'min' number of votes,
        // so there's not a tie, and we should return false to 'tied' and break the loop, since the scenario of 'not tie' appeared
        if (!candidates[i].eliminated && candidates[i].votes != min)
        {
            tied = false;
            break;
        }
        // One remain case would be if the candidate have been eliminated, but this corner case doesn't matter
    }
    return tied;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // If the candidate haven't been eliminated yet and unfortunately he/she has the minimal number of votes,
        // we should eliminate him/her
        if (!candidates[i].eliminated && candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}