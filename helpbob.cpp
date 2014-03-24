//
//  helpbob.cpp
//  helpbob
//
//  Created by Srikkanth Jagannathan on 2/23/14.
//  PROBLEM: www.spoj.com/problems/HELPBOB
//

#include <cstdio>

using namespace std;

struct State
{
    float discountedPricePercent[15];
    float pricePerArea;
    float price;
    float area;
};

int main()
{
    int m;

    State states[1<<15];
    
    while(true)
    {
        scanf("%d", &m);
        if( m == 0 )
            break;
        
        int totalStates = 1<<m;
        
        for( int i = 0; i < totalStates; i++ )
        {
            states[i].price = 0;
            states[i].area = 0;
            states[i].pricePerArea = 1000000;
            for( int j = 0; j < m; j++ )
                states[i].discountedPricePercent[j] = 1;
        }
        
        float discounts[15][15];
        int price[15];
        int area[15];
        
        for( int i = 0; i < m; i++ )
        {
            int numDiscounts;
            
            for( int j = 0; j < m; j++ )
                discounts[i][j] = 1;
            
            scanf("%d %d %d", &price[i], &area[i], &numDiscounts);
            
            for( int j = 0; j < numDiscounts; j++ )
            {
                int pizzaIndex;
                float discount;
                scanf("%d %f", &pizzaIndex, &discount);
                discounts[i][pizzaIndex - 1] = 1.0 - discount/100.0;
            }
        }
        
        float bestPricePerArea = 10000000;
        
        for( int currentState = 1; currentState < totalStates; currentState++ )
        {
            int possiblePermutations = 1;
            float minPrice = 0;
            float minArea = 0;
            float minPricePerArea = 10000000;
            int selectedIndex = -1;
            int selectedState = 0;
            int counter = 0;
            while( possiblePermutations <= currentState )
            {
                if( (currentState & possiblePermutations) != 0 )
                {
                    int possiblePrevState = currentState ^ possiblePermutations;
                    float newPrice, newArea;
                    newPrice = price[counter] * states[possiblePrevState].discountedPricePercent[counter];
                    newPrice += states[possiblePrevState].price;
                    newArea = (states[possiblePrevState].area + area[counter]);
                    float pricePerArea = newPrice/newArea;
                    if( pricePerArea < minPricePerArea )
                    {
                        minPricePerArea = pricePerArea;
                        minPrice = newPrice;
                        minArea = newArea;
                        selectedIndex = counter;
                        selectedState = possiblePrevState;
                    }
                }
                
                counter++;
                possiblePermutations = possiblePermutations<<1;
            }
            states[currentState].pricePerArea = minPricePerArea;
            states[currentState].price = minPrice;
            states[currentState].area = minArea;
            for( int i = 0; i < m; i++ )
            {
                states[currentState].discountedPricePercent[i] = states[selectedState].discountedPricePercent[i];
                states[currentState].discountedPricePercent[i] *= discounts[selectedIndex][i];
            }
            if( minPricePerArea < bestPricePerArea )
                bestPricePerArea = minPricePerArea;
        }
        
        printf("%1.4f\n", bestPricePerArea);
    }
}
