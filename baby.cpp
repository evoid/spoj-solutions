//
//  baby.cpp
//  baby
//
//  Created by Srikkanth Jagannathan on 2/25/14.
//  PROBLEM: www.spoj.com/problems/BABY
//

#include <algorithm>
#include <cstdio>
#include <cstdlib>

using namespace std;

struct State
{
    int columnsFilled;
    int distance;
};

struct Distances
{
    int distance;
    int originalPosition;
};

bool lessThan( Distances i, Distances j )
{
    return i.distance < j.distance;
}

int main()
{
    while(true)
    {
        int n;
        scanf("%d", &n);
        
        if( n== 0 )
            break;
        
        int givenConfiguration[16];
        int desiredConfiguration[16];
        const int TOTAL_CONFIGURATIONS = 1<<n;
        State states[TOTAL_CONFIGURATIONS];
        Distances distanceToRow[16][16];
        
        for( int i = 0; i < n; i++ )
            scanf("%d", &givenConfiguration[i]);
        for( int i = 0; i < n; i++ )
            scanf("%d", &desiredConfiguration[i]);
        
        for( int i = 0; i < n; i++ )
        {
            for( int j = 0; j < n; j++ )
            {
                int i1 = i + 1, j1 = j + 1;
                distanceToRow[i][j].distance = abs(j1 - i1) + abs(desiredConfiguration[j] - givenConfiguration[i]);
                distanceToRow[i][j].originalPosition = j;
            }
            sort( distanceToRow[i], (distanceToRow[i] + n), lessThan );
        }
        
        int minDistance = 10000;
        
        states[0].columnsFilled = 0;
        states[0].distance = 0;
        
        for( int currentState = 1; currentState < TOTAL_CONFIGURATIONS; currentState++ )
        {
            int possiblePermuts = 1;
            int minDistanceThisState = 10000;
            int selectedPermut = -1;
            int selectedColumn = -1;
            int selectedIndex = -1;
            int counter = 0;
            while( possiblePermuts <= currentState )
            {
                if( (currentState & possiblePermuts) != 0 )
                {
                    int possiblePreviousState = currentState ^ possiblePermuts;
                    int preferredColumn = 0;
                    while( ((1<<distanceToRow[counter][preferredColumn].originalPosition) & states[possiblePreviousState].columnsFilled) != 0 )
                    {
                        preferredColumn++;
                    }
                
                    int distance = states[possiblePreviousState].distance + distanceToRow[counter][preferredColumn].distance;
                    if( distance < minDistanceThisState )
                    {
                        minDistanceThisState = distance;
                        selectedPermut = possiblePermuts;
                        selectedColumn = preferredColumn;
                        selectedIndex = counter;
                    }
                }
                counter++;
                possiblePermuts = possiblePermuts<<1;
            }
            
            states[currentState].distance = minDistanceThisState;
            states[currentState].columnsFilled = (states[currentState ^ selectedPermut].columnsFilled | (1<<distanceToRow[selectedIndex][selectedColumn].originalPosition));
            
            
            minDistance = minDistanceThisState;
        }
    
        printf("%d\n", minDistance);
    }
}
