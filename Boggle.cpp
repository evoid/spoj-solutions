/*

PROBLEM:        Given a dictionary and a boggle board, find all the words that can be formed. For information on boggle, visit:
                http://en.wikipedia.org/wiki/Boggle

ASSUMPTION:     I've assumed that the length of the longest word in the dictionary does not exceed 100.

Algorithm:      Store the dictionary as a tree. Use the Boggle board as an adjacency list and run a breadth first search.
                Products of prime numbers are used to check if a particular letter at a position on the board has already been
                used in that word(essentially they're used as flags to mark nodes as visited, instead of using one boolean for each 
                node).
                This program can handle a dictionary of over 200,000 words and a boggle board of size 100x100 in less than two seconds.
                Sample input file has been provided.

*/

#include<cstdio>
#include<vector>
#include<map>
#include<cmath>
#include<cstring>
#include<queue>
#include<windows.h>

using namespace std;

//Stores a node in the tree.
struct node
{
       char ch;
       multimap<char, node*> next;
       bool word;
       bool found;
};

//Stores the position of a character on the boggle board.
struct rc_pair
{
       int r,c;
       rc_pair(int r1, int c1)
       {
            r = r1;
            c = c1;
       }
};

//Stores a node on the search tree while running BFS.
struct bfs_node
{
       int r, c, len, ctr;
       char word[100];
       int visited[100];
       node *cur;
};

node *root;
multimap<char, rc_pair> pos_in_board;
vector<int> primes;
char **board;
int rows, cols;
int num_primes = -1, npsqr1, npsqr2;
int dirc[8][2] = {{1, 1}, {1, 0}, {1, -1}, {0 , -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}};

void bfs()
{
     queue<bfs_node> q;
     bfs_node temp;
     temp.len = -1;
     temp.ctr = 0;
     temp.visited[0] = 1;
     temp.cur = root;
     temp.r = temp.c = -1;
     q.push(temp);
     int lev = 0;
     while(q.size())
     {
              lev++;
              //printf("\n%d\n",lev);
              bfs_node cur = q.front();
              q.pop();
              bfs_node t1 = cur;
              multimap<char, node*>::iterator it;
              node *track = cur.cur;
              t1.len++;
              int temp1 = cur.r, temp2 = cur.c;
              for(it = track->next.begin(); it != track->next.end(); it++)
              {
                      bool add = false;
                      //printf("%c", (*it).second->ch);
                      cur.word[t1.len] = (*it).second->ch;
                      cur.len = t1.len;
                      cur.cur = (*it).second;
                      cur.ctr = t1.ctr;
                      if(t1.r == -1)
                      {
                               pair<multimap<char, rc_pair>::iterator, multimap<char, rc_pair>::iterator> pos = pos_in_board.equal_range((*it).first);
                               multimap<char, rc_pair>::iterator it1;
                               for(it1 = pos.first; it1 != pos.second; it1++)
                               {
                                       cur.r = (*it1).second.r;
                                       cur.c = (*it1).second.c;
                                       int tr = (cur.r % npsqr1), tc = (cur.c % npsqr2);
                                       int prm = primes[(tr * npsqr2) + tc];
                                       cur.visited[t1.ctr] = prm;
                                       q.push(cur);
                                       add = true;
                               }
                      }
                      else
                      {
                               for(int idx = 0; idx < 8; idx++)
                               if((temp1 - dirc[idx][0]) >= 0 && (temp2 - dirc[idx][1]) >= 0 && (temp1 - dirc[idx][0]) < rows && (temp2 - dirc[idx][1]) < cols)
                                       if(board[temp1 - dirc[idx][0]][temp2 - dirc[idx][1]] == (*it).first)
                                       {
                                                      bool bfl = false;
                                                      int tr = (temp1 - dirc[idx][0]) % npsqr1, tc = (temp2 - dirc[idx][1]) % npsqr2;
                                                      int prm = primes[(tr * npsqr2) + tc];
                                                      for(int idx1 = 0; idx1 <= t1.ctr; idx1++)
                                                              if(t1.visited[idx1] % prm == 0)
                                                              {
                                                                     bfl = true;
                                                                     break;
                                                              }
                                                      if(bfl)
                                                             continue;
                                                      int prod = t1.visited[t1.ctr] * prm;
                                                      if(prod < 0)
                                                      {
                                                              cur.ctr = t1.ctr+1;
                                                              prod = prm;
                                                      }
                                                      cur.visited[cur.ctr] = prod;
                                                      cur.r = temp1 - dirc[idx][0];
                                                      cur.c = temp2 - dirc[idx][1];
                                                      q.push(cur);
                                                      add = true;
                                       }
                      }
                      if(add)
                      {
                             if(cur.cur->word)
                             {
                                   //printf("sdfaf\n");
                                   if(!cur.cur->found)
                                   {
                                        cur.word[t1.len + 1] = '\0';
                                        printf("%s\n", cur.word);
                                        cur.cur->found = true;
                                   }
                             }
                             //q.push(cur);
                      }
              }
     }
}

//Add a word from the dictionary to the tree.
void add_word(char word[100])
{
     int len = strlen(word);
     word[0] = tolower(word[0]);
     node *cur;
     multimap<char, node*>::iterator it = root->next.find(word[0]);
     if(it == root->next.end())
     {
           cur = new node;
           cur->ch = word[0];
           cur->word = false;
           cur->found = false;
           root->next.insert(pair<char, node*>(cur->ch, cur));
     }
     else
           cur = (*it).second;
     //printf("%d\n", len);
     for(int i = 1; i < len; i++)
     {
             it = cur->next.find(word[i]);
             //printf("%c\n", word[i]);
             if(it == cur->next.end())
             {
                   node *temp = new node;
                   temp->ch = word[i];
                   if(i != len-1)
                   temp->word = false;
                   else
                   temp->word = true;
                   temp->found = false;
                   cur->next.insert(pair<char, node*>(temp->ch, temp));
                   cur = temp;
             }
             else
                   cur = (*it).second;      
     }
     cur->word = true;
}

void free_mem()
{
     node *cur = root;
     multimap<char, node *>::iterator it;
     queue<node*> q;
     q.push(root);
     while(q.size())
     {
                    cur = q.front();
                    q.pop();
                    for(it = cur->next.begin(); it != cur->next.end(); it++)
                           q.push((*it).second);
                    delete cur;
     }
}
                           
//Generate prime numbers to check if a particular letter(at a particular position on the board) has already been used in this word.
void prime(int num)
{
     int ctr = 1, cur = 3;
     primes.push_back(2);
     while(1)
     {
             int till = (int)sqrt((float)cur), i;
             bool fl = false;
             for(i = 0; primes[i] <= till && i < ctr; i++)
             {
                     if(cur % primes[i] == 0)
                     {
                            fl = true;
                            break;
                     }
             }
             if(!fl)
             {
                  primes.push_back(cur);
                  ctr++;
             }
             cur++;
             if(ctr == num)
                    break;
     }
}

int main()
{
    SYSTEMTIME st1,st2;
    GetSystemTime(&st1);
    char word[100];
    
    root = new node;
    root->word = false;
    root->ch = '\\';
    root->found = false;
    
    scanf("%d%d",&rows, &cols);
    
    board = new char *[rows];
    for(int i = 0; i < rows; i++)
    {
            board[i] = new char[cols];
            scanf("%s", board[i]);
            for(int j = 0; j < cols; j++)
                    pos_in_board.insert(pair<char, rc_pair>(board[i][j], rc_pair(i, j)));
    }
    //system("pause");
    
    while(1)
    {
           if(scanf("%s", word) == EOF)
               break;
           //printf("%d\n",strlen(word));
           int temp = strlen(word);
           if(temp > num_primes)
               num_primes = temp;
           add_word(word);
           //system("pause");
    }
    
    //system("pause");
    
    num_primes *= num_primes;
    int rc = rows * cols;
    if(num_primes < rc)
    {
        npsqr1 = npsqr2 = (int)sqrt(num_primes);
    }
    else
    {
        num_primes = rc;
        npsqr1 = rows;
        npsqr2 = cols;
    }
    
    printf("%d %d %d\n", num_primes, rows, cols);
    prime(num_primes);
    bfs();
    free_mem();
    GetSystemTime(&st2);
    float secs1 = (float)st1.wMinute * 60 + (float)st1.wSecond + (float)st1.wMilliseconds/1000;
    float secs2 = (float)st2.wMinute * 60 + (float)st2.wSecond + (float)st2.wMilliseconds/1000;
    printf("\n%f seconds\n", secs2 - secs1);
    //system("pause");
}
