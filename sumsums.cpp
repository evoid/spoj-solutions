//
//  sumsums.cpp
//  sumsums
//
//  Created by Srikkanth Jagannathan on 3/4/14.
//  PROBLEM: www.spoj.com/problems/SUMSUMS
//

#include <cstdio>

using namespace std;

int main()
{
    unsigned int n,t;
    scanf("%d%d", &n, &t);
    
    if( n == 0 )
        return -1;
    
    const int MAX_SUM = 98765431;
    
    long long sum = 1;
    unsigned int countTo = t;
    unsigned int counter = 1;
    unsigned long long nPower = n - 1;
    
    if( n > 1 )
    {
        while( counter <= countTo )
        {
            if( (counter & countTo) != 0 )
            {
                sum = ((sum * nPower) % MAX_SUM);
            }
            nPower = ((nPower * nPower) % MAX_SUM);
            counter = (counter<<1);
        }
    }
    else
    {
        sum = 1;
    }
    
    int cowN[50000];
    int diffFrom1[50000];
    long long sumDiffs = 0;
    long long sum2 = 0;
    for( int i = 0; i < n; i++ )
    {
        scanf("%d", &cowN[i]);
        diffFrom1[i] = cowN[0] - cowN[i];
        sumDiffs += diffFrom1[i];
        sum2 = ((sum2 + cowN[i]) % MAX_SUM);
    }
    
    sum = ((sum2 * sum) % MAX_SUM);
    //printf("%lld\n", sum);
    
    for( long long i = 0; i <= n; i++ )
    {
        long long possibleSum = (sum + MAX_SUM * i);
        if( t % 2 == 0 )
        {
            if( possibleSum + sumDiffs < 0 )
                continue;
            if( (possibleSum + sumDiffs) % n == 0 )
            {
                sum = possibleSum;
                break;
            }
            else if((possibleSum + (sumDiffs % MAX_SUM)) % n == 0)
            {
                sum = possibleSum;
                sumDiffs = sumDiffs % MAX_SUM;
                break;
            }
        }
        else
        {
            if( possibleSum - sumDiffs < 0 )
                continue;
            if( (possibleSum - sumDiffs) % n == 0 )
            {
                sum = possibleSum;
                break;
            }
            else if((possibleSum - (sumDiffs % MAX_SUM)) % n == 0)
            {
                sum = possibleSum;
                sumDiffs = sumDiffs % MAX_SUM;
                break;
            }
        }
    }
    
    //printf("%lld %lld\n", sum, sumDiffs);
    
    if( t % 2 == 0 )
    {
        cowN[0] = (((sum + sumDiffs)/n) % MAX_SUM);
    }
    else
    {
        cowN[0] = (((sum - sumDiffs)/n) % MAX_SUM);
    }
    
    for( int i = 0; i < n; i++ )
    {
        int out;
        if( t % 2 == 0 )
        {
            if( cowN[0] - diffFrom1[i] > 0 )
                out = ((cowN[0] - diffFrom1[i]) % MAX_SUM);
            else
                out = (cowN[0] - diffFrom1[i] + MAX_SUM);
        }
        else
        {
            if( cowN[0] + diffFrom1[i] > 0 )
                out = ((cowN[0] + diffFrom1[i]) % MAX_SUM);
            else
                out = (cowN[0] + diffFrom1[i] + MAX_SUM);
        }
        printf("%d\n", out);
    }
}
