#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <list>
#include <utility>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int hit, miss, seq, f, capacity, heap_size;
pair<int, pair<int, int>> *harr; // key, <count, seq>
list<int> *table;    // key


void swap(pair<int, pair<int, int>> *x, pair<int, pair<int, int>> *y){
    pair<int, pair<int, int>> temp = *x;
    *x = *y;
    *y = temp;
}

int parent(int i){
    return (i-1)/2; 
}

void insertKey(pair<int, pair<int, int>> k){
    heap_size++;
    int i = heap_size - 1;
    harr[i] = k;
    while (i != 0 && harr[parent(i)].second > harr[i].second){
       swap(&harr[i], &harr[parent(i)]);
       i = parent(i);
    }
}

void MinHeapify(int i){
    int l = 2*i + 1;
    int r = 2*i + 2;
    int smallest = i;
    if(l < heap_size && harr[l].second < harr[i].second)
        smallest = l;
    if(r < heap_size && harr[r].second < harr[smallest].second)
        smallest = r;
    if(smallest != i){
        swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}

pair<int, pair<int, int>> extractMin(){
    if (heap_size == 1){
        heap_size--;
        return harr[0];
    }
    pair<int, pair<int, int>> root = harr[0];
    harr[0] = harr[heap_size-1];
    heap_size--;
    MinHeapify(0);
    return root;
}

void hashupdate(int key){
    int index = key % f;
    list<int>::iterator i;
    for(i = table[index].begin(); i != table[index].end(); i++)
        if(*i == key)
            break;
    if(i != table[index].end()){
        hit++;
        int j;
        for(int j = 0; j < heap_size; j++)
            if(harr[j].first == key){
                harr[j].second.first++;
                harr[j].second.second = seq++;
                MinHeapify(j);
                break;
            }
    }
    else{
        miss++;
        if(heap_size == f){
            pair<int, pair<int, int>> p = harr[0];
            extractMin();
            list<int>::iterator it;
            int dindex = p.first % f;
            for(it = table[dindex].begin(); it != table[dindex].end(); it++)
                if(*it == p.first){
                    table[dindex].erase(it);
                    break;
                }
            table[index].push_back(key);
            insertKey(make_pair(key, make_pair(1, seq++)));
        }
        else{
            table[index].push_back(key);
            insertKey(make_pair(key, make_pair(1, seq++)));
        }
    }
}

void lfu(){
    struct timeval start, end;
    gettimeofday(&start, 0);
    cout << "LFU policy:\nFrame\tHit\t\tMiss\t\tPage fault ratio\n";
    for(f = 64; f <= 512; f *= 2){
        string page;
        fstream ifs;
        table = new list<int>[f];
        harr = new pair<int, pair<int, int>>[f];
        hit = miss = seq = heap_size = 0;
        ifs.open("zipf.txt");
        while(ifs >> page)
            hashupdate(stoi(page));
        cout << f << "\t" << hit << "\t\t" << miss << "\t\t" << (double)miss/(miss + hit) << '\n';
        delete []table;
        delete []harr;
        ifs.close();
    }
    gettimeofday(&end, 0);
    float sec = end.tv_sec - start.tv_sec, usec = end.tv_usec - start.tv_usec;
    cout << "Total elapsed time " << sec + usec / 1000000 << " sec\n\n";
}

int main(){
    lfu();
    return 0;
}