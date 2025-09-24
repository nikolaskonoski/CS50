#include <cs50.h>
#include <stdio.h>
#include <string.h>

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
} candidate;

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
    // Percorre todos candidatos para encontrar uma correspondência
    for (int i = 0; i < candidate_count; i++)
    {
        // Verifica se o input fornecido é o nome de um candidato
        if (strcmp(candidates[i].name, name) == 0) // CORRIGIDO
        {
            // Se sim, atualiza a matrix de preferências com o indice do candidato
            preferences[voter][rank] = i;
            return true; // Sucesso na hora de gravar o voto
        }
    }
    // Invalidez se nenhum nome for encontrado
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // Percorre cada eleitor
    for (int i = 0; i < voter_count; i++)
    {
        // Percorre as preferências de cada eleitor (classificações)
        for (int j = 0; j < candidate_count; j++)
        {
            // Obtèm o indice do candidato para a preferência atual
            int candidate_index = preferences[i][j];

            // Verifica se este candidato não foi eliminado
            if (!candidates[candidate_index].eliminated) // CORRIGIDO
            {
                // Se não foi eliminado, este é o voto atual dele, incrementando a contagem de votos
                candidates[candidate_index].votes++; // CORRIGIDO

                // Sai do loop interno, pois já foi encontrada a escolha deste eleitos para esta
                // rodada
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // Calcule os votos necessários para obter a maioria
    int majority = voter_count / 2;

    // Percorre todos os candidatos
    for (int i = 0; i < candidate_count; i++) // CORRIGIDO
    {
        // Verifica se o candidato tem mais da metade dos votos
        if (candidates[i].votes > majority)
        {
            // Se sim, imprima o nome dele e retorne verdadeiro
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    // Se nenhum candidato tiver a maioria, retorne falso.
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // Inicializa min_votes com um valor alto
    int min_votes = voter_count;

    // Percorre todos os candidatos
    for (int i = 0; i < candidate_count; i++)
    {
        // Verifica se o candidato não foi eliminado e tem menos votos que o mínimo atual
        if (!candidates[i].eliminated && candidates[i].votes < min_votes)
        {
            // Atualiza o número mínimo de votos
            min_votes = candidates[i].votes;
        }
    }
    return min_votes;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Percorre todos os candidatos
    for (int i = 0; i < candidate_count; i++)
    {
        // Verifica se um candidato que ainda está na disputa NÃO tem o número mínimo de votos
        if (!candidates[i].eliminated && candidates[i].votes != min)
        {
            // Se tal candidato existir, não é um empate
            return false;
        }
    }
    // Se todos os candidatos restantes tiverem o número mínimo de votos, é um empate
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // Percorre todos os candidatos
    for (int i = 0; i < candidate_count; i++)
    {
        // Se um candidato tiver o número mínimo de votos, elimine-o
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
